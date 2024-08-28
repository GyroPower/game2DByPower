workspace "Game2DByPower"
    architecture "x64"
    configurations {"Debug","Release","Dist"}
    startproject "Game-App"

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Game-External-Core"
    include "Game-External-Libraries/Build-Game-External.lua"
    include "Game-Core/Build-Game-Core.lua"
group "Game-GUI"
    include "Game-GUI/Build-Game-GUI.lua"
group "resources"
    include "resources/Build-Game-resources.lua"
    include "shaders/Build-Game-shaders.lua"
group ""
    include "Game-App/Build-Game-App.lua"