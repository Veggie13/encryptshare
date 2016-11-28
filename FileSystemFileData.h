#pragma once


class FileSystemFileData
{
public:
    FileSystemFileData( char* data, unsigned long size );
    FileSystemFileData( const FileSystemFileData& other );
    ~FileSystemFileData();

    const char* Data() const { return _data; }
    unsigned long Size() const { return _size; }

private:
    char* _data;
    unsigned long _size;

};
