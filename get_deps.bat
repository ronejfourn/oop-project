@echo off
echo getting deps
if not exist "extern" mkdir extern
pushd extern
if not exist "stb" mkdir stb
pushd stb
if exist "stb_image.h" goto SkipSTB
echo downloading stb_image
call curl "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h" -o stb_image.h
echo done
:SkipSTB
popd

if exist "SDL2-2.0.20/" goto SkipSDL
echo downloading SDL
call curl "https://www.libsdl.org/release/SDL2-devel-2.0.20-VC.zip" -o SDL2.zip
call tar -xf SDL2.zip
del SDL2.zip
pushd SDL2-2.0.20
ren include SDL2
echo done
popd
:SkipSDL
popd
echo done
