@echo off
mkdir build32
mkdir build64
cmake -G "Visual Studio 17 2022" -A Win32 -S . -B "build32"
cmake -G "Visual Studio 17 2022" -A x64 -S . -B "build64"
cmake --build build32 --config Release
cmake --build build64 --config Release
cmake --build build32 --config Debug
cmake --build build64 --config Debug
mkdir Library
mkdir Library\Win32
mkdir Library\Win64
mkdir Library\Win32\Debug
xcopy /Y build32\Debug\*.* Library\Win32\Debug
mkdir Library\Win32\Release
xcopy /Y build32\Release\*.* Library\Win32\Release
mkdir Library\Win64\Debug
xcopy /Y build64\Debug\*.* Library\Win64\Debug
mkdir Library\Win64\Release
xcopy /Y build64\Release\*.* Library\Win64\Release
rmdir /S /Q build32
rmdir /S /Q build64