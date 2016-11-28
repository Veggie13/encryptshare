#include "FileSystemFile.h"


FileSystemFile::FileSystemFile(
    const std::string& fullName,
    unsigned long size,
    IFileSystemElementFactory* factory )
:   AFileSystemElement(fullName, factory),
    _size(size)
{ }
