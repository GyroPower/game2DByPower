project "shaders"
    kind "None"
    files {"./*"}

    removefiles {"**.vcxproj", "**.vcxproj.filters"}
    targetdir ("../Binaries/"..OutputDir.."/%{prj.name}")