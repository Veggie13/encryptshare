#pragma once

//
// BASED ON Example of how to create a custom streambuf.
// By perfectly.insane
//

#include <fstream>
#include <iostream>
#include <list>
#include <streambuf>
#include <vector>


class EncryptShare_streambuf : public std::basic_streambuf<char>
{
public:
    // The size of the input and output buffers.
    static const size_t BUFF_SIZE = 1024;

    typedef std::basic_streambuf<char> base_type;

    typedef base_type::traits_type traits_type;
    typedef base_type::char_type charT;

    typedef traits_type::int_type int_type;
    typedef traits_type::pos_type pos_type;
    typedef traits_type::off_type off_type;


    template <class iosT>
    static EncryptShare_streambuf* Create(
        std::vector<iosT*>& keys,
        std::list<std::string>& passwords )
    {
        std::vector<base_type*> key_bufs( keys.size() );
        for ( unsigned i = 0; i < keys.size(); i++ )
        {
            key_bufs[i] = keys[i]->rdbuf();
        }
        return new EncryptShare_streambuf( key_bufs, passwords );
    }

    explicit EncryptShare_streambuf(
        std::vector<base_type*>& key_bufs,
        std::list<std::string>& passwords );

    ~EncryptShare_streambuf();

protected:
    // This is called when there are too many characters in the buffer (thus, a write needs to be performed).
    virtual int_type overflow(int_type c);
    // This is called when the buffer needs to be flushed.
    virtual int_type sync();

    // This is called when there are no more characters left in the buffer (reads a character).
    virtual int_type underflow();

private:
    // These are the streambuf objects that we are to read/write from/to.
    std::vector<base_type*> _key_bufs;
    // Encryption passwords
    std::list<std::string> _passwords;
    // Input buffer
    charT* in_buf_;
    charT** _in_bufs;
    // Output buffers
    charT* out_buf_;
    charT** _out_bufs;
    // Maintain the offset
    unsigned long _out_offset;
    unsigned long _in_offset;
};

template <typename streamT>
class EncryptShareStream : public streamT
{
public:
    typedef streamT base_type;

    explicit EncryptShareStream(
        std::vector<base_type*>& key_bufs,
        std::list<std::string>& passwords )
    : base_type( EncryptShare_streambuf::Create<base_type>(key_bufs, passwords) )
    { }

};

typedef EncryptShareStream<std::istream> Encrypt_istream;
typedef EncryptShareStream<std::ostream> Encrypt_ostream;
typedef EncryptShareStream<std::iostream> Encrypt_iostream;
