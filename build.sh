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

# Step 3: Use whatever python3 is on PATH (Ubuntu 24.04 ships 3.12; macOS via Homebrew).
# We previously enforced python3.10 via ppa:deadsnakes/ppa, but Launchpad PPA
# infrastructure has been intermittently unreachable from CI runners (HTTP 504
# from add-apt-repository), so we no longer pin a specific Python version.
PYTHON_EXEC=${PYTHON_EXEC:-python3}
if ! command -v "$PYTHON_EXEC" &> /dev/null; then
    echo "$PYTHON_EXEC not found. Please install python3 (>= 3.8)."
    exit 1
fi

# Step 4: Ensure pip is available
if ! "$PYTHON_EXEC" -m pip &> /dev/null; then
    echo "pip not found for $PYTHON_EXEC. Installing pip..."
    curl -sS https://bootstrap.pypa.io/get-pip.py -o get-pip.py
    "$PYTHON_EXEC" get-pip.py --break-system-packages
    rm get-pip.py
fi

# Step 5: Install Python build dependencies.
# --break-system-packages is required on PEP 668 environments (macOS Homebrew
# python, recent Debian/Ubuntu).  It's a no-op for older pip / non-managed
# interpreters.
PIP_FLAGS="--break-system-packages"
"$PYTHON_EXEC" -m pip install $PIP_FLAGS -U pip pybind11 setuptools wheel build
PYBIND11_DIR=$("$PYTHON_EXEC" -m pybind11 --cmakedir)

# Step 6: Build the wheel
SVF_DIR=${SVF_DIR} LLVM_DIR=${LLVM_DIR} Z3_DIR=${Z3_DIR} PYBIND11_DIR=${PYBIND11_DIR} "$PYTHON_EXEC" -m build --wheel

if [ $? -ne 0 ]; then
    echo "Wheel build failed."
    exit 1
else
    echo "Successfully built wheel for $($PYTHON_EXEC --version)."
fi
