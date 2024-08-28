project "Game-GUI"
    kind "StaticLib"
    language "C++"
    cppdialect "C++14"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files {"GUI/**.h","GUI/**.cpp"}

    includedirs {"./","../Game-External-Libraries/include","../Game-Core",
    "../Game-External-Libraries/include/glad/include"}

    links {"Game-External-Libraries","Game-Core"}

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
        
