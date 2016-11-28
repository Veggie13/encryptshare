#pragma once

#include <string>


class AFileSystemElement;

class IFileSystemElementFactory
{
public:
    IFileSystemElementFactory() { }
    virtual ~IFileSystemElementFactory() { }

    virtual AFileSystemElement* Create( const std::string& fullPath ) = 0;

};
