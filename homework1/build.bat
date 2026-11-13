@echo off
setlocal

set "VSDIR=D:\Visual Studio\2"
set "OPENCV_DIR=D:\opencv\build"

call "%VSDIR%\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1

if not exist "%~dp0build" mkdir "%~dp0build"
if not exist "%~dp0output" mkdir "%~dp0output"

for %%f in (ex1_drawing ex2_histogram ex3_channels ex4_camera) do (
    cl /nologo /utf-8 /EHsc /std:c++17 /MD /O2 /W3 ^
      /Fo"%~dp0build\\" /Fd"%~dp0build\vc140.pdb" ^
      "%~dp0src\%%f.cpp" /I"%OPENCV_DIR%\include" ^
      /link /LIBPATH:"%OPENCV_DIR%\x64\vc16\lib" opencv_world4120.lib ^
      /OUT:"%~dp0build\%%f.exe"
    if errorlevel 1 exit /b 1
)

copy /y "%OPENCV_DIR%\x64\vc16\bin\opencv_world4120.dll" "%~dp0build\" >nul
echo Build complete.
