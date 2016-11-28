#include "IFileSystemElementFactory.h"

#include "AFileSystemElement.h"


AFileSystemElement::AFileSystemElement(
    const std::string& fullName,
    IFileSystemElementFactory* factory )
:   _factory(factory),
    _fullPath(fullName)
{ }

bool AFileSystemElement::operator ==( const AFileSystemElement& other ) const
{
    return _fullPath == other._fullPath;
}

AFileSystemElement*
AFileSystemElement::Parent() const
{
    std::string subPath = _fullPath.substr(0, _fullPath.find_last_of('/'));
    return _factory->Create( subPath );
}

std::string AFileSystemElement::ShortName() const
{
    return _fullPath.substr(_fullPath.find_last_of('/'));
}

std::string AFileSystemElement::FullName() const
{
    return _fullPath;
}
