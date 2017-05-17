@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64     

REM hello -----------------------------------------------------
REM Build - make an OpenCL program
REM 
REM -----------------------------------------------------------
echo --------------------------------------------------------------
echo Building hello 
echo --------------------------------------------------------------
set compilerflags=/Fo.\bin\ /Od /Zi /EHsc 
set linkerflags=/OUT:bin\gpu.exe
set amd_incldues="C:\Program Files (x86)\AMD APP SDK\3.0\include"
set amd_libs="C:\Program Files (x86)\AMD APP SDK\3.0\lib\x86_64"

cl.exe %compilerflags% source/gpu.cpp /I%amd_incldues% /DYNAMICBASE OpenCL.lib /link /LIBPATH:%amd_libs% %linkerflags%