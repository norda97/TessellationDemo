-- premake5.lua


workspace "TessellationDemo" -- Name of sln file
    location "TessellationDemo" -- Folder where to put it
    configurations { "Debug", "Release" }
    platforms { "x64" }

    --Set architecture
    filter { "platforms:x64" }
        system "Windows"
        architecture "x64"

    filter { }

project "TessellationDemo" -- Name of project
    kind "ConsoleApp" -- Uses the console
    language "C++"
    location "%{prj.name}" -- location of vcxproj file
    targetdir "build/bin/%{cfg.buildcfg}/%{cfg.architecture}" -- .exe files is in bin/(debug or release)/(x86 or x64)/
    --location of source files to include. Here we include All files ending with .h and .cpp
    --in the folder Minimal Example even files in subfolders.
    files { "%{prj.name}/inc/**.h", "%{prj.name}/src/**.cpp" } 

    --Include directories
    includedirs {
        "%{prj.name}/inc",
        "./external/glm/",
        "./external/glfw-3/include",
        "./external/glew-2.2/include"
    }

    --libraries and links
        --links (the library files)
        links {
            "glew32",
            "opengl32",
            "glfw3"
        }

        --for x64 use these
        filter "architecture:x64"
            libdirs { 
                "./external/glfw-3/lib",
                "./external/glew-2.2/lib/Release/x64"
            }
    
    --Debug and Release configurations
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter { }