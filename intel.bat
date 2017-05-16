@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64     

set compilerflags=/Fo.\bin\ /Od /Zi /EHsc 
set linkerflags=/OUT:bin\gpu.exe
set Intel_include="C:\Program Files (x86)\Intel\OpenCL SDK\6.3\include"
set Intel_lib="C:\Program Files (x86)\Intel\OpenCL SDK\6.3\lib\x64"
cl.exe %compilerflags% source/gpu.cpp /I%Intel_include% /DYNAMICBASE OpenCL.lib /link /LIBPATH:%Intel_lib% %linkerflags%
