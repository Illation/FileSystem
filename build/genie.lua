solution "FileSystem"
    configurations {
        "Debug",
        "Release"
    }

    platforms {
        "x32",
        "x64",
        "Native" -- for targets where bitness is not specified
    }

    language "C++"

    location "../"
    objdir "../build/"

PROJECT_DIR = "../"
SOURCE_DIR = path.join(PROJECT_DIR, "src/")


project "FileSystem"
    kind "ConsoleApp"

    configuration "Debug"
        targetdir "../bin/debug/"
        objdir "obj/debug"
        defines { "_DEBUG" }
        flags { "Symbols" }
    configuration "Release"
        targetdir "../bin/release/"
        objdir "obj/debug"
	flags {"OptimizeSpeed", "No64BitChecks"}

    configuration "vs*"
        flags { "Unicode", "Winmain"}--, "-std=c++14"}
        defines { "WIN32", "PLATFORM_Win" }
    configuration { "linux", "gmake"}
--	flags {"-pedantic"}--, "-std=c++14"}
	defines { "PLATFORM_Linux", "__linux__" }

    configuration {}

    defines { "_CONSOLE" }

    files {
        path.join(SOURCE_DIR, "*.cpp"),
        path.join(SOURCE_DIR, "*.h"),
        path.join(path.join(SOURCE_DIR, "FileSystem/"), "*.cpp"),
        path.join(path.join(SOURCE_DIR, "FileSystem/"), "*.h"),
        path.join(path.join(SOURCE_DIR, "FileSystem/"), "*.hpp"),
        path.join(path.join(SOURCE_DIR, "FileSystem/Facade/"), "*.cpp"),
        path.join(path.join(SOURCE_DIR, "FileSystem/Facade/"), "*.h"),
        path.join(path.join(SOURCE_DIR, "FileSystem/Facade/"), "*.hpp"),
        path.join(path.join(SOURCE_DIR, "FileSystem/Directory/"), "*.cpp"),
        path.join(path.join(SOURCE_DIR, "FileSystem/Directory/"), "*.h"),
        path.join(path.join(SOURCE_DIR, "FileSystem/Directory/"), "*.hpp")
    }

    excludes {
        path.join(path.join(SOURCE_DIR, "FileSystem/"), "FileSystem.cpp"),
        path.join(path.join(SOURCE_DIR, "FileSystem/"), "FileSystem.h"),
        path.join(path.join(SOURCE_DIR, "FileSystem/"), "FileSystem.hpp"),

    }

    -- optional. This is purely cosmetically.
    vpaths {
    }
        

