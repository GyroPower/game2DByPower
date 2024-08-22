project "Game-App"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++14"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files {"**.h", "**.cpp"}

    includedirs {"./", "../Game-External-Libraries/include",
    "../Game-External-Libraries/include/glad/include",
                "../Game-Core", "../Game-UI"}

    links {"Game-Core","Game-External-Libraries","Game-UI"}

    targetdir ("../Binaries/"..OutputDir.."/%{prj.name}")
    objdir ("../Binaries/Intermediates/"..OutputDir.."/%{prj.name}")

    
    filter "system:windows"
        systemversion "latest"
        defines {"WINDOWS"}

        filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"
 
    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"
 
    filter "configurations:Dist"
        defines { "DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"

