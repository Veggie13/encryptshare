#pragma once

#include <string>


class FileSystemFile;
class FileSystemFolder;
class IFileSystemElementFactory;

class AFileSystemElement
{
public:
    AFileSystemElement(
        const std::string& fullName,
        IFileSystemElementFactory* factory );
    virtual ~AFileSystemElement() { }

    bool operator ==( const AFileSystemElement& other ) const;

    AFileSystemElement* Parent() const;
    std::string ShortName() const;
    std::string FullName() const;

    virtual unsigned long Size() const = 0;
    virtual FileSystemFile* AsFile() = 0;
    virtual FileSystemFolder* AsFolder() = 0;

protected:
    IFileSystemElementFactory* _factory;

private:
    std::string _fullPath;

};
