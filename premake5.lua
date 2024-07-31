workspace "nester"
    location "build"
    configurations { "Debug", "Release" }


project "nester-lib"
    kind "StaticLib"
    toolset "clang"
    language "C++"
    cppdialect "C++17"
    location "build"
    targetdir "build/%{cfg.buildcfg}"
    flags { "FatalCompileWarnings", "FatalLinkWarnings" }

    files { "src/**.cpp" }
    removefiles { "src/main.cpp" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"


project "nester-bin"
    kind "ConsoleApp"
    toolset "clang"
    language "C++"
    cppdialect "C++17"
    location "build"
    targetdir "build/%{cfg.buildcfg}"
    flags { "FatalCompileWarnings", "FatalLinkWarnings" }

    files { "src/main.cpp" }
    links { "nester-lib" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"


project "nester-tests"
    kind "ConsoleApp"
    toolset "clang"
    language "C++"
    cppdialect "C++17"
    location "build"
    targetdir "build/%{cfg.buildcfg}"

    files { "test/**.cpp" }
    links { "nester-lib", "gtest", "pthread" }

    defines { "DEBUG" }
    symbols "On"
