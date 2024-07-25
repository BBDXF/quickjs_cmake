# quickjs_cmake
The cmake build system for quickjs, and a demo to explain how to use it. include js call C interface and class.

Especially, tt support build quickjs on windows platform.

It support these features:
- [X] support quickjs shared library
- [X] support qjsc application
- [X] support qjs with repl.js and qjscalc.js
- [X] output quickjs headers
- [X] output source: repl.js.c, qjscalc.js.c
- [X] output libquickjs.dll/so
- [X] cmake auto setting quickjs VERSION to code

# demo features
- [ ] Run js code in C
- [ ] Run js code to use std, os module
- [ ] Call js function/code from C
- [ ] Extend js function/class using C
- [ ] js http fetch impletation
- [ ] js UI impletation

# Platform Support
- [X] Windows + Mingw-w64, test passed
- [ ] Linux + gcc, not tested, should pass
- [ ] Android + gcc, not tested, should pass

** Why not support MSVC/Clang? **
> Because quickjs use a lot of gcc features, headers and pthread library.  
> If we chang it, it's a big change to source code, and can't trace the latest version.
> The original code is ok for mingw in Win32. 
> So build on windows platform with Mingw-W64-UCRT64 is the best way on it! 

# How to use with latest quickjs?

```bash
cd <dir>
git submodule update

# linux
# custom build dir and build with release version
mkdir build
cmake .. -D CMAKE_BUILD_TYPE=Release

# windows
# add C:\msys64\ucrt64\bin to PATH, then
set CMAKE_C_COMPILER=gcc.exe
set CMAKE_CXX_COMPILER=g++.exe
cmake --build . --config Release -v
```

# Author
BBDXF @ 2024-07


