project "Game-UI"
    kind "StaticLib"
    language "C++"
    cppdialect "C++14"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files {"UI/**.h","UI/**.cpp"}

    includedirs {"./","../Game-External-Libraries/include"}

    links {"Game-External-Libraries"}

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
        
