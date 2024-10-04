#!/bin/bash -e

export CC=clang
export CXX=clang++

# Check the cmake binary
CMAKE="cmake" ;

# Clean
rm -rf build/ ; 

# Build
mkdir build ; 
cd build ; 
${CMAKE} -DCMAKE_BUILD_TYPE=Debug ../ ; 
make 
cd ../


