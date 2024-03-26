#!/bin/bash

PROJECTHOME=$(pwd)
sysOS=`uname -s`
LLVMHome="llvm-16.0.0.obj"
Z3Home="z3.obj"
install_path=`npm root`
export LLVM_DIR=$install_path/SVF/$LLVMHome
export Z3_DIR=$install_path/SVF/$Z3Home
export svfModule_DIR="./src/svfModule"
export SVF_DIR=$install_path/SVF

echo "LLVM_DIR="$LLVM_DIR
echo "SVF_DIR="$SVF_DIR
echo "Z3_DIR="$Z3_DIR
echo "svfModule_DIR="$svfModule_DIR