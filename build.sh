#! /bin/bash

CXX=clang++

CXX_DEFS=""

CXX_LIBS="-lcurl"
CXX_FLAGS="-Wall -Wextra -fno-exceptions -fno-rtti -O3 -flto $CXX_DEFS"

shopt -s nullglob
mkdir -p build/

pushd build/ > /dev/null

g++ $CXX_FLAGS $CXX_LIBS ../src/main.cpp -o QuoteFetch

popd > /dev/null
echo "Done..."
