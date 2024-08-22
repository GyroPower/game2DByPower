project "resources"
    kind "SharedLib"
    files {"textures/*"}

    removefiles {"**.vcxproj", "**.vcxproj.filters"}

    targetdir ("../Binaries/"..OutputDir.."/%{prj.name}")