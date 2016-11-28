#include <string.h>

#include "FileSystemFileData.h"


FileSystemFileData::FileSystemFileData(char* data, unsigned long size)
:   _data(data),
    _size(size)
{ }

FileSystemFileData::FileSystemFileData(const FileSystemFileData& other)
:   _size(other._size)
{
    _data = new char[_size];
    memcpy( _data, other._data, _size );
}

FileSystemFileData::~FileSystemFileData()
{
    delete[] _data;
}
