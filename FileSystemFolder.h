#pragma once

#include <list>

#include "AFileSystemElement.h"


class FileSystemFolder : public AFileSystemElement
{
public:
    FileSystemFolder(
        const std::string& fullName,
        const std::list<std::string>& childNames,
        IFileSystemElementFactory* factory );

    std::list<std::string> ChildShortNames() const;
    AFileSystemElement* Child( const std::string& shortName ) const;

    virtual unsigned long Size() const;
    virtual FileSystemFile* AsFile() { return NULL; }
    virtual FileSystemFolder* AsFolder() { return this; }

private:
    std::list<std::string> _childNames;

};
