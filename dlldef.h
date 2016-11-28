#pragma once

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#elif TEST_DLL
    #define DLL_EXPORT
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

