#! /bin/bash

CXX=g++

CXX_DEFS=""

CXX_LIBS="-lcurl"
CXX_FLAGS="-Wall -Wextra -Wnoexcept -fno-rtti -O3 $CXX_DEFS"

shopt -s nullglob
mkdir -p build/

pushd build/ > /dev/null

g++ $CXX_FLAGS $CXX_LIBS ../src/main.cpp -o QuoteFetch

popd > /dev/null
echo "Done..."
