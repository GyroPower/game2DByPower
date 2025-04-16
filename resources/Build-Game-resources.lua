project "resources"
    kind "None"
    files {"textures/*"}

    removefiles {"**.vcxproj", "**.vcxproj.filters"}

    targetdir ("../Binaries/"..OutputDir.."/%{prj.name}")