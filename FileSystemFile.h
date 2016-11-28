#pragma once

#include "AFileSystemElement.h"


class FileSystemFile : public AFileSystemElement
{
public:
    FileSystemFile(
        const std::string& fullName,
        unsigned long size,
        IFileSystemElementFactory* factory );

    virtual unsigned long Size() const { return _size; }
    virtual FileSystemFile* AsFile() { return this; }
    virtual FileSystemFolder* AsFolder() { return NULL; }

private:
    unsigned long _size;

};
