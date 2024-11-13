#!/bin/bash -e
export ROOT_PATH="/home/yantingchi/Desktop/static_analysis/tools/CFI-Enforcement"


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
export TOOL="./src/cfi-analyzer"
cd ../


