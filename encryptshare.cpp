#include <string.h>

#include "encryptshare.h"

using namespace std;


void Encrypt(
    char* buffer,
    unsigned long bufLength,
    const char* key,
    unsigned long keyLength,
    unsigned long keyOffset )
{
    for ( unsigned bi = 0, ki = keyOffset;
          bi < bufLength;
          bi++, ++ki %= keyLength )
    {
        buffer[bi] ^= key[ki];
    }
}

/*
template <typename T>
void WriteValue( FILE* pFile, const T& val )
{
    const char* bc = (const char*)(&val);
    for ( int i = 0; i < sizeof(T); i++ )
    {
        fwrite( bc, sizeof(T), 1, pFile );
    }
}

const unsigned long BUF_SIZE = 65536;
unsigned long ReadMax( FILE* pFile, char* buffer, long sizeLeft, long maxsize )
{
    long size = ( sizeLeft > maxsize ) ? maxsize : sizeLeft;
    return fread( buffer, sizeof(char), size, pFile );
}
*/
void RandomizeBuffer( char* buffer, unsigned long bufLength )
{
    for ( unsigned long i = 0; i < bufLength; i++ )
    {
        buffer[i] = rand() & 0xff;
    }
}

void EncryptShare::Encrypt(
    const char* buffer,
    unsigned long bufSize,
    const list<string>& passwords,
    unsigned long offset,
    char** outputs,
    unsigned outputCount,
    bool createOutputBuffers )
{
    if ( outputCount < 1 )
        return;

    if ( createOutputBuffers )
        outputs[0] = new char[bufSize];
    if ( buffer != outputs[0] )
        memcpy( outputs[0], buffer, bufSize );

    for ( unsigned i = 1; i < outputCount; i++ )
    {
        if ( createOutputBuffers )
            outputs[i] = new char[bufSize];
        RandomizeBuffer( outputs[i], bufSize );
        ::Encrypt( outputs[0], bufSize, outputs[i], bufSize, offset );
    }

    for ( list<string>::const_iterator it = passwords.begin();
          it != passwords.end();
          it++ )
    {
        ::Encrypt( outputs[0], bufSize, (*it).c_str(), (*it).length(), offset );
    }
}

void EncryptShare::Decrypt(
    const char* const* keyBuffers,
    unsigned long bufSize,
    unsigned keyCount,
    const list<string>& passwords,
    unsigned long offset,
    char** output,
    bool createOutputBuffer )
{
    if ( keyCount < 1 )
    {
        *output = NULL;
        return;
    }

    if ( createOutputBuffer )
        *output = new char[bufSize];
    if ( *output != keyBuffers[0] )
        memcpy( *output, keyBuffers[0], bufSize );

    for ( unsigned i = 1; i < keyCount; i++ )
    {
        ::Encrypt( *output, bufSize, keyBuffers[i], bufSize, offset );
    }

    for ( list<string>::const_iterator it = passwords.begin();
          it != passwords.end();
          it++ )
    {
        ::Encrypt( *output, bufSize, (*it).c_str(), (*it).length(), offset );
    }
}

/*
bool EncryptShare::WriteShares(
    const string& password1,
    const string& password2,
    const string& outfile1,
    const string& outfile2 )
{
    unsigned long long totalsize = 0;

    list<long> sizes;
    for ( list<string>::iterator it = _filenames.begin();
          it != _filenames.end();
          it++ )
    {
        FILE* pFile = fopen ( (*it).c_str(), "rb" );
        if ( pFile == NULL )
            return false;

        fseek ( pFile, 0, SEEK_END );
        long size = ftell( pFile );
        fclose( pFile );

        if ( size == -1L )
            return false;

        sizes.push_back( size );
        totalsize += size;
    }

    FILE* pOut1 = fopen( outfile1.c_str(), "wb" );
    FILE* pOut2 = fopen( outfile2.c_str(), "wb" );

    long written = 0;
    char buffer[BUF_SIZE], rbuffer[BUF_SIZE];
    list<string>::iterator it = _filenames.begin();
    list<long>::iterator sIt = sizes.begin();
    for ( ; it != _filenames.end(); it++, sIt++ )
    {
        long size = *sIt;
        *(long*)buffer = size;
        char* pBuf = buffer + sizeof(long);
        long extra = sizeof(long);

        memcpy( pBuf, (*it).c_str(), (*it).length() + 1 );
        pBuf += (*it).length() + 1;
        extra += (*it).length() + 1;

        FILE* pFile = fopen( (*it).c_str(), "rb" );
        long maxsize = BUF_SIZE - (pBuf - buffer);
        while ( size > 0 )
        {
            long read = ReadMax(pFile, pBuf, size, maxsize);
            Encrypt( buffer, BUF_SIZE, password1.c_str(), password1.length(), written % password1.length() );
            Encrypt( buffer, BUF_SIZE, password2.c_str(), password2.length(), written % password2.length() );
            RandomizeBuffer( rbuffer, BUF_SIZE );
            Encrypt( buffer, BUF_SIZE, rbuffer, BUF_SIZE, 0 );

            written += read + extra;
            size -= read;
            fwrite( buffer,
                    sizeof(unsigned char),
                    read + extra,
                    pOut1 );
            fwrite( rbuffer,
                    sizeof(unsigned char),
                    read + extra,
                    pOut2 );
            fflush( pOut1 );
            fflush( pOut2 );

            pBuf = buffer;
            maxsize = BUF_SIZE;
            extra = 0;
        }
        fclose( pFile );
    }

    fclose( pOut1 );
    fclose( pOut2 );

    return true;
}
*/
