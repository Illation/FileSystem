#pragma once

#include "FileHandle.h"
#include "FileAccessFlags.h"
#include "FileAccessMode.h"
#include <string>

#if defined(PLATFORM_Linux)
#include "FileBaseLinuxReferences.h"
#elif defined(PLATFORM_Win)
#include "FileBaseWinReferences.h"
#endif

class FILE_BASE {
public:
    static FILE_HANDLE Open( const char * pathName, FILE_ACCESS_FLAGS accessFlags, FILE_ACCESS_MODE accessMode );

    static bool Close( FILE_HANDLE handle );

    static bool ReadFile( FILE_HANDLE handle, std::string & content );

    static bool WriteFile( FILE_HANDLE handle, const std::string & content );

private:
#if defined(PLATFORM_Linux)
    #include "FileBaseLinuxMembers.h"
#elif defined(PLATFORM_Win)

#endif
};
