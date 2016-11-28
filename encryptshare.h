#pragma once

#include <list>
#include <string>

#include "dlldef.h"


class DLL_EXPORT EncryptShare
{
public:
    void Encrypt(
        const char* buffer,
        unsigned long bufSize,
        const std::list<std::string>& passwords,
        unsigned long offset,
        char** outputs,
        unsigned outputCount,
        bool createOutputBuffers = false );

    void Decrypt(
        const char* const* keyBuffers,
        unsigned long bufSize,
        unsigned keyCount,
        const std::list<std::string>& passwords,
        unsigned long offset,
        char** output,
        bool createOutputBuffer = false );

};
