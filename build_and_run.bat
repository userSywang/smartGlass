@echo off
setlocal enabledelayedexpansion

cmake -S . -B build
if errorlevel 1 exit /b 1

cmake --build build
if errorlevel 1 exit /b 1

set "BIN=build\bin\smartGlass.exe"
if not exist "%BIN%" (
  echo Binary not found: "%BIN%"
  exit /b 1
)

"%BIN%"
