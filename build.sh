#!/bin/bash

# Step 1: Clone and build SVF
if [ -z "$SVF_DIR" ] || [ ! -d "$SVF_DIR" ]; then
    git clone https://github.com/SVF-tools/SVF
    cd SVF
    source build.sh
    cd ..
else
    echo "SVF directory already exists. Skipping clone and build."
fi

# Step 2: Check environment variables
if [ -z "$SVF_DIR" ] || [ -z "$LLVM_DIR" ] || [ -z "$Z3_DIR" ]; then
    echo "Please define SVF_DIR, LLVM_DIR, and Z3_DIR."
    exit 1
fi

# Step 3: Ensure python3.10 is available
if ! command -v python3.10 &> /dev/null; then
    echo "Python 3.10 not found. Attempting to install..."

    # Detect OS
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        sudo apt update
        sudo apt install -y software-properties-common
        sudo add-apt-repository -y ppa:deadsnakes/ppa
        sudo apt update
        sudo apt install -y python3.10 python3.10-venv python3.10-dev
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        if ! command -v brew &> /dev/null; then
            echo "Homebrew not found. Please install Homebrew first: https://brew.sh/"
            exit 1
        fi
        brew install python@3.10
    else
        echo "Unsupported OS: $OSTYPE"
        exit 1
    fi
else
    echo "Python 3.10 found."
fi

# Step 4: Ensure pip is installed for python3.10
if ! python3.10 -m pip &> /dev/null; then
    echo "pip not found for Python 3.10. Installing pip..."
    curl -sS https://bootstrap.pypa.io/get-pip.py -o get-pip.py
    python3.10 get-pip.py
    rm get-pip.py
fi

# Step 5: Install Python build dependencies
python3.10 -m pip install -U pip pybind11 setuptools wheel build
PYBIND11_DIR=$(python3.10 -m pybind11 --cmakedir)

# Step 6: Build the wheel
SVF_DIR=${SVF_DIR} LLVM_DIR=${LLVM_DIR} Z3_DIR=${Z3_DIR} PYBIND11_DIR=${PYBIND11_DIR} python3.10 -m build --wheel

if [ $? -ne 0 ]; then
    echo "Building for Python 3.10 failed."
else
    echo "Successfully built for Python 3.10."
fi