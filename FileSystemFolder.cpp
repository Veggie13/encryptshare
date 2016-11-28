#include "IFileSystemElementFactory.h"

#include "FileSystemFolder.h"


FileSystemFolder::FileSystemFolder(
    const std::string& fullName,
    const std::list<std::string>& childNames,
    IFileSystemElementFactory* factory )
:   AFileSystemElement(fullName, factory),
    _childNames(childNames)
{ }

std::list<std::string> FileSystemFolder::ChildShortNames() const
{
    return _childNames;
}

AFileSystemElement* FileSystemFolder::Child( const std::string& shortName ) const
{
    std::string childName = FullName() + "/" + shortName;
    return _factory->Create(childName);
}

unsigned long FileSystemFolder::Size() const
{
    unsigned long total = 0;
    for ( std::list<std::string>::const_iterator it = _childNames.begin();
          it != _childNames.end();
          it++ )
    {
        AFileSystemElement* child = Child(*it);
        total += child->Size();
        delete child;
    }

    return total;
}
