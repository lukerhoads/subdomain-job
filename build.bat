@ECHO ON

RMDIR /Q /S build
MKDIR build
PUSHD build

conan install ..
cmake ..
cmake --build . --config Release

bin\main.exe
