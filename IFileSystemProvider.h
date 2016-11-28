#pragma once


class AFileSystemElement;
class FileSystemFileData;

class IFileSystemProvider
{
public:
    IFileSystemProvider() { }
    virtual ~IFileSystemProvider() { }

    virtual AFileSystemElement* Root() = 0;
    virtual FileSystemFileData FileData( AFileSystemElement* el ) = 0;

};
