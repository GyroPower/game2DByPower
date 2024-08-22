@echo off 

pushd .. 
Vendor\Binaries\Premake\Windows\premake5.exe --file=Build.lua vs2017
popd
pause
pushd .. 
start Game2DByPower.sln