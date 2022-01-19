@echo off
if not exist "extern\SDL2-2.0.20\lib\x64\SDL2.dll" call get_deps.bat
if not exist "bin\Release\" goto skipRelease
if not exist "bin\Release\SDL2.dll" copy extern\SDL2-2.0.20\lib\x64\SDL2.dll bin\Release\
:skipRelease
if not exist "bin\Debug\" goto skipDebug
if not exist "bin\Debug\SDL2.dll" copy extern\SDL2-2.0.20\lib\x64\SDL2.dll bin\Debug\
:skipDebug
