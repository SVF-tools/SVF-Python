#!/bin/bash

PROJECTHOME=$(pwd)
sysOS=`uname -s`
LLVMHome="llvm-16.0.0.obj"
Z3Home="z3.obj"
install_path=`npm root`
export LLVM_DIR=$install_path/$LLVMHome
export Z3_DIR=$install_path/$Z3Home
export svfModule_DIR="/Users/jiaweideng/Documents/SVF/SVF-pybind11-master/SVF-Python/src/svfModule"

export PATH=$LLVM_DIR/bin:$PATH
export PATH=$PROJECTHOME/bin:$PATH
echo "export LLVM_DIR=$install_path/$LLVMHome" >> ~/.bashrc
echo "export Z3_DIR=$install_path/$Z3Home" >> ~/.bashrc
echo "export PATH=$LLVM_DIR/bin:$PROJECTHOME/bin:$PATH" >> ~/.bashrc
echo "export svfModule_DIR=$svfModule_DIR" >> ~/.bashrc

if [[ $sysOS == "Darwin" ]]
then 
    # export SVF_DIR=$install_path/SVF
    export SVF_DIR=/Users/jiaweideng/Documents/SVF/SVF
elif [[ $sysOS == "Linux" ]]
then 
    # export SVF_DIR=$install_path/SVF
    export SVF_DIR=/Users/jiaweideng/Documents/SVF/SVF
fi 

echo "LLVM_DIR="$LLVM_DIR
echo "SVF_DIR="$SVF_DIR
echo "Z3_DIR="$Z3_DIR
echo "svfModule_DIR="$svfModule_DIR