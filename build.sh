#! /bin/bash

CXX=clang++

CXX_DEFS="-DDEBUG"

CXX_LIBS="-lcurl"
CXX_FLAGS="-Wall -Wextra -Wnoexcept -fno-rtti -O0 -g $CXX_DEFS"

shopt -s nullglob
mkdir -p build/

pushd build/ > /dev/null

g++ $CXX_FLAGS $CXX_LIBS ../src/main.cpp -o QuoteFetch

popd > /dev/null
echo "Done..."
