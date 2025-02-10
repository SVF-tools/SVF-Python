#!/usr/bin/env bash
#
# This script is a simplified example for building and packaging an SVF-based project
# with a configurable LLVM version on Linux. It will exit immediately on any error.

# ================== USER-CONFIGURABLE VARIABLES ==================

# The default LLVM version if none is provided externally:
LLVM_VERSION_DEFAULT="16"

# If $LLVM_VERSION is set in the environment, use it; otherwise use the default above.
LLVM_VERSION="${LLVM_VERSION:-$LLVM_VERSION_DEFAULT}"

# On Linux, the LLVM CMake config path could be something like /usr/lib/llvm-16/cmake
# Adjust as necessary for your system or prebuilt LLVM.
LLVM_DIR="${LLVM_DIR:-/usr/lib/llvm-${LLVM_VERSION}/lib/cmake/llvm}"

# The path to Z3
Z3_DIR="${Z3_DIR:-/usr/local/lib/z3}"   # Adjust to your actual install path

# Python interpreter to use for building/packaging
PYTHON_BIN="${PYTHON_BIN:-python3}"

# Version string for packaging
VERSION="${VERSION:-0.1.3.dev7}"

# By default, let's produce a manylinux2014_x86_64 wheel for broad compatibility.
# (If you don't need manylinux, you can choose a simpler "linux_x86_64" or omit this argument.)
PLAT_NAME="${PLAT_NAME:-manylinux2014_x86_64}"

# =================================================================

# Exit immediately on error (-e). You can add -u, -o pipefail if desired.
set -e

echo "--------------------------------------"
echo "Building with the following settings:"
echo "LLVM_VERSION:    $LLVM_VERSION"
echo "LLVM_DIR:        $LLVM_DIR"
echo "Z3_DIR:          $Z3_DIR"
echo "PYTHON_BIN:      $PYTHON_BIN"
echo "PACKAGE VERSION: $VERSION"
echo "PLAT_NAME:       $PLAT_NAME"
echo "--------------------------------------"

# Source directory is the current working directory
SOURCE_DIR="$PWD"

# 1) Check OS (currently only for Linux)
sysOS=$(uname)
if [[ "$sysOS" != "Linux" ]]; then
  echo "Error: This script only supports Linux. Detected OS: $sysOS"
  exit 1
fi

# 2) Clone SVF with depth = 1 (If it doesn't exist yet)
if [ ! -d "$SOURCE_DIR/SVF" ]; then
  echo "Cloning SVF repo..."
  git clone --depth 1 https://github.com/SVF-tools/SVF SVF
fi

# 3) (Optional) If you have a separate build script for SVF, source it
#    or directly run the commands needed to build SVF.
#    This step depends on your SVF repo structure. For example:
echo "Entering SVF directory to run build.sh..."
cd "$SOURCE_DIR/SVF"
source build.sh  # Adjust if your build script name/path is different
cd "$SOURCE_DIR"

# 4) Create a Release-build directory (recreating if it exists)
if [ -d "Release-build" ]; then
  echo "Removing old Release-build..."
  rm -rf Release-build
fi

echo "Creating new Release-build directory..."
mkdir Release-build
cd Release-build

# 5) Run CMake for your project
cmake \
  -DSVF_DIR="$SOURCE_DIR/SVF" \
  -DLLVM_DIR="$LLVM_DIR" \
  -DZ3_DIR="$Z3_DIR" \
  ..

# 6) Build with make (or cmake --build)
echo "Compiling with make..."
make -j4

# 7) Go back to source directory
cd "$SOURCE_DIR"

# 8) Print some environment info
echo "--------------------------------------"
echo "SVF_DIR=$SOURCE_DIR/SVF"
echo "LLVM_DIR=$LLVM_DIR"
echo "Z3_DIR=$Z3_DIR"
echo "--------------------------------------"

# 9) Cleanup old build artifacts in your Python package (if needed)
rm -rf build pysvf/SVF

# 10) Upgrade pip, setuptools, wheel, pybind11
"$PYTHON_BIN" -m pip install -U pip setuptools wheel pybind11

# 11) Finally, build the wheel package
#     We'll specify a Linux platform name. For manylinux you can do:
#       bdist_wheel --plat-name manylinux2014_x86_64
#     or if you want it local: linux_x86_64
echo "Building Python wheel for $PLAT_NAME ..."
SVF_DIR="$SOURCE_DIR/SVF" \
LLVM_DIR="$LLVM_DIR" \
Z3_DIR="$Z3_DIR" \
VERSION="$VERSION" \
BUILD_TYPE="Debug" \
"$PYTHON_BIN" setup_linux.py bdist_wheel --plat-name "$PLAT_NAME"

echo "--------------------------------------"
echo "All done! The wheel file should be in dist/."
echo "--------------------------------------"
