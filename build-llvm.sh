#!/bin/bash -e


# LLVM version: 14.0.6 

ROOT=$(pwd)

# Download the release version
echo "Downloading from the llvm release..."
wget https://github.com/llvm/llvm-project/releases/download/llvmorg-14.0.6/clang+llvm-14.0.6-x86_64-linux-gnu-rhel-8.4.tar.xz

echo "Extract the file..."
tar -xf clang+llvm-14.0.6-x86_64-linux-gnu-rhel-8.4.tar.xz
rm -f clang+llvm-14.0.6-x86_64-linux-gnu-rhel-8.4.tar.xz

echo "Setting LLVM HOME..."
mv clang+llvm-14.0.6-x86_64-linux-gnu-rhel-8.4 llvm-project-14
source enable
