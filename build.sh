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

# Step 4: Set up an isolated Python venv for the build.
#
# OS package managers (apt on Linux, brew on macOS) ship pip / setuptools /
# wheel WITHOUT RECORD files, and the python interpreter itself is marked
# EXTERNALLY-MANAGED (PEP 668).  Both `pip install -U pip` and
# `--break-system-packages` workarounds collide with the no-RECORD problem
# the moment any package needs to be uninstalled.
#
# A venv side-steps both: it gives us a fully-managed pip with proper
# RECORD files inside an isolated prefix, no system-site interference.
# This is exactly what PEP 668 itself recommends.
VENV_DIR="${VENV_DIR:-$(pwd)/.svfpy-build-venv}"
"$PYTHON_EXEC" -m venv --clear "$VENV_DIR"
# shellcheck disable=SC1091
source "$VENV_DIR/bin/activate"

# Step 5: Install Python build dependencies into the venv.
python -m pip install -U pip pybind11 setuptools wheel build
PYBIND11_DIR=$(python -m pybind11 --cmakedir)

# Step 6: Build the wheel using the venv's python.
SVF_DIR=${SVF_DIR} LLVM_DIR=${LLVM_DIR} Z3_DIR=${Z3_DIR} PYBIND11_DIR=${PYBIND11_DIR} python -m build --wheel

if [ $? -ne 0 ]; then
    echo "Wheel build failed."
    exit 1
else
    echo "Successfully built wheel for $($PYTHON_EXEC --version)."
fi
