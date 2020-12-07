
project "TessellationDemo" 
    kind "ConsoleApp" 
    language "C++"
    location "%{prj.name}" 
    targetdir "build/bin/%{cfg.buildcfg}/%{cfg.architecture}"
    files { "%{prj.name}/inc/**.h", "%{prj.name}/src/**.cpp", "./external/imgui/**.hpp", "./external/imgui/**.cpp" } 

    includedirs {
        "%{prj.name}/inc",
        "./external/glm/",
        "./external/glfw-3/include",
        "./external/imgui",
        "./external/glew-2.2/include"
    }

        links {
            "glew32",
            "opengl32",
            "glfw3"
        }


        filter "architecture:x64"
            libdirs { 
                "./external/glfw-3/lib",
                "./external/glew-2.2/lib/Release/x64"
            }
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter { }