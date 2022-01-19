@echo off
where clang >nul 2>nul
if %ERRORLEVEL% neq 0 goto SkipCLANG

setlocal

set SRC=src/*.cpp
set CFG=Release

set CLANGFlags=-O2 -gcodeview

if "%1" neq "debug" goto DoneConfig
echo debug configuration active
set CFG=Debug
set CLANGFlags=-g -gcodeview
:DoneConfig

call get_deps.bat

set OUT=OOPProject.exe
set EXT_INC="extern/"
set SDL_INC="extern/SDL2-2.0.20/"
set SDL_LIB="extern/SDL2-2.0.20/lib/x64"

if not exist "bin" mkdir bin
if not exist bin\%CFG% mkdir bin\%CFG%
call copy_dll.bat
echo building with clang
call clang -I%SDL_INC% -I%EXT_INC% -L%SDL_LIB% %CLANGFlags% %SRC% -o bin\%CFG%\%OUT% -lSDL2main -lSDL2 -lShell32 -Xlinker -subsystem:windows
echo done

if "%1" neq "run" goto Finished
echo running
pushd bin\%CFG%
call %OUT%
popd

:SkipCLANG
echo clang not found, please setup clang.
:Finished
