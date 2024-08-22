project "Game-External-Libraries"
    kind "StaticLib"
    language "C++"
    cppdialect "C++14"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files {"**.h","**.c","**.cpp","**.hpp"}

    includedirs {"include","include/ImGui","include/glad/include"}
    libdirs {"lib"}

    links {"freetype","opengl32","glfw3"}

    targetdir ("../Binaries/"..OutputDir.."/%{prj.name}")
    objdir ("../Binaries/Intermediates/"..OutputDir.."/%{prj.name}")

    

    filter "system:windows"
    systemversion "latest"
    defines {}

    filter "configurations:Debug"
        defines {"DEBUG"}
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines {"RELEASE"}
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        defines {"DIST"}
        runtime "Release"
        optimize "On"
        symbols "Off"  
