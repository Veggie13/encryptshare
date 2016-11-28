#include "encryptshare.h"

#include "EncryptShare_iostream.h"

using namespace std;


// You can use any method that you want, but here, we'll just take in raw streambufs as
// slave I/O objects.
EncryptShare_streambuf::EncryptShare_streambuf(
    vector<base_type*>& key_bufs,
    list<string>& passwords )
:   _key_bufs(key_bufs),
    _passwords(passwords),
    in_buf_( new charT[key_bufs.size() * BUFF_SIZE] ),
    _in_bufs( new charT*[key_bufs.size()] ),
    out_buf_( new charT[key_bufs.size() * BUFF_SIZE] ),
    _out_bufs( new charT*[key_bufs.size()] ),
    _out_offset(0),
    _in_offset(0)
{
    // Initialize the output buffer pointers.
    for ( unsigned i = 0; i < _key_bufs.size(); i++ )
    {
        _in_bufs[i] = in_buf_ + (i * BUFF_SIZE);
        _out_bufs[i] = out_buf_ + (i * BUFF_SIZE);
    }

    // Initialize get pointer.  This should be zero so that underflow is called upon first read.
    this->setg(0, 0, 0);

    // Initialize the put pointer.  Overflow won't get called until this buffer is filled up,
    // so we need to use valid pointers.
    this->setp(out_buf_, out_buf_ + BUFF_SIZE - 1);
}

// It's a good idea to release any resources when done using them.
EncryptShare_streambuf::~EncryptShare_streambuf()
{
    delete [] in_buf_;
    delete [] out_buf_;
    delete [] _in_bufs;
    delete [] _out_bufs;
}

// This function is called when the output buffer is filled.
// In this function, the buffer should be written to wherever it should
// be written to (in this case, the streambuf object that this is controlling).
EncryptShare_streambuf::int_type
EncryptShare_streambuf::overflow(EncryptShare_streambuf::int_type c)
{
    charT* ibegin = this->out_buf_;
    charT* iend = this->pptr();

    // Reset the put pointers to indicate that the buffer is free
    // (at least it will be at the end of this function).
    setp(out_buf_, out_buf_ + BUFF_SIZE + 1);

    // Compute the write length.
    int_type ilen = iend - ibegin;

    EncryptShare enc;
    enc.Encrypt( out_buf_, ilen, _passwords, _out_offset, _out_bufs, _key_bufs.size() );
    _out_offset += ilen;

    // If this is the end, add an eof character to the buffer.
    // This is why the pointers passed to setp are off by 1
    // (to reserve room for this).
    if(!traits_type::eq_int_type(c, traits_type::eof())) {
        *iend++ = traits_type::to_char_type(c);
        ilen++;
    }

    // Write it using the sputn method of the streambuf
    // objects in our control.
    for ( unsigned i = 0; i < _key_bufs.size(); i++ )
        _key_bufs[i]->sputn(_out_bufs[i], (streamsize) ilen);

    return traits_type::not_eof(c);
}

// This is called to flush the buffer.
// This is called when we're done with the file stream (or when .flush() is called).
EncryptShare_streambuf::int_type
EncryptShare_streambuf::sync()
{
    return traits_type::eq_int_type(this->overflow(traits_type::eof()),
                                    traits_type::eof()) ? -1 : 0;
}

// Fill the input buffer.  This reads from the streambuf and
// decrypts the contents.
EncryptShare_streambuf::int_type
EncryptShare_streambuf::underflow()
{
    // Count enough bytes to fill the buffer.
    streamsize minlen = BUFF_SIZE;
    for ( unsigned i = 0; i < _key_bufs.size(); i++ )
    {
        streamsize len = _key_bufs[i]->sgetn( _in_bufs[i], minlen );
        if ( len < minlen ) minlen = len; // if these buffers aren't the same size, we're in trouble.
    }
    // If nothing was read, then the end is here.
    if(minlen == 0)
        return traits_type::eof();

    // Since the input buffer content is now valid (or is new)
    // the get pointer should be initialized (or reset).
    setg(in_buf_, in_buf_, in_buf_ + minlen);

    // Decrypt the buffer.
    EncryptShare enc;
    enc.Decrypt( _in_bufs, minlen, _key_bufs.size(), _passwords, _in_offset, &in_buf_ );
    _in_offset += minlen;

    // Return the first character.
    return traits_type::not_eof(in_buf_[0]);
}
