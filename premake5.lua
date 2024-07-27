workspace "nester"
    location "build"
    configurations { "Debug", "Release" }

project "nester"
    kind "ConsoleApp"
    toolset "clang"
    language "C++"
    cppdialect "C++17"
    location "build"
    targetdir "build/%{cfg.buildcfg}"
    flags { "FatalCompileWarnings", "FatalLinkWarnings" }

    files { "**.hpp", "**.cpp" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
