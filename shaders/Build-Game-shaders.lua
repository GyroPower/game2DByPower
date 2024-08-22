project "shaders"
    kind "SharedLib"
    files {"./*"}

    removefiles {"**.vcxproj", "**.vcxproj.filters"}
    targetdir ("../Binaries/"..OutputDir.."/%{prj.name}")