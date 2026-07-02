@echo off
setlocal

cmake -S . -B build\release -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 exit /b 1

cmake --build build\release --parallel
if errorlevel 1 exit /b 1

echo Release binary: build\release\bin\smartGlass.exe
