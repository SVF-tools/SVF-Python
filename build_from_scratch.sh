#!/usr/bin/env bash
#
# This script is a simplified example for building and packaging an SVF-based project
# with a configurable LLVM version on macOS. It will exit immediately on any error.

# ================== USER-CONFIGURABLE VARIABLES ==================

# The default LLVM version if none is provided externally:
LLVM_VERSION_DEFAULT="16"

# If $LLVM_VERSION is set in the environment, use it; otherwise use the default above.
LLVM_VERSION="${LLVM_VERSION:-$LLVM_VERSION_DEFAULT}"

# The path to the LLVM CMake config directory (adjust if needed).
# Typically on macOS with Homebrew, it might look like /usr/local/opt/llvm@16/lib/cmake/llvm
LLVM_DIR="${LLVM_DIR:-/usr/local/opt/llvm@${LLVM_VERSION}/lib/cmake/llvm}"

# The path to Z3 (adjust if needed).
Z3_DIR="${Z3_DIR:-/usr/local/opt/z3}"

# Python interpreter to use for building/packaging
PYTHON_BIN="${PYTHON_BIN:-python3}"

# Version string for packaging
VERSION="${VERSION:-0.1.3.dev7}"

# =================================================================

# Exit immediately on error (-e), treat unset variables as errors (-u),
# and fail if any command in a pipeline fails (-o pipefail).
set -e

echo "--------------------------------------"
echo "Building with the following settings:"
echo "LLVM_VERSION:    $LLVM_VERSION"
echo "LLVM_DIR:        $LLVM_DIR"
echo "Z3_DIR:          $Z3_DIR"
echo "PYTHON_BIN:      $PYTHON_BIN"
echo "PACKAGE VERSION: $VERSION"
echo "--------------------------------------"

# Source directory is the current working directory
SOURCE_DIR="$PWD"

# 1) Clone SVF with depth = 1
#    (If it already exists, you can remove or skip this step.)
if [ ! -d "$SOURCE_DIR/SVF" ]; then
  echo "Cloning SVF repo..."
  git clone --depth 1 https://github.com/SVF-tools/SVF SVF
fi

# 2) Get pybind11 cmake directory
PYBIND_DIR=$($PYTHON_BIN -m pybind11 --cmakedir)

# 3) Check OS (currently only for macOS)
sysOS=$(uname)
if [[ "$sysOS" != "Darwin" ]]; then
  echo "Error: This script only supports macOS (Darwin). Detected OS: $sysOS"
  exit 1
fi

# 4) On macOS, build SVF
echo "Entering SVF directory to run build.sh..."
cd "$SOURCE_DIR/SVF"
source build.sh    # <--- Adjust if your build script name/path is different
cd "$SOURCE_DIR"

# 5) Create a Release-build directory (recreating if it exists)
if [ -d "Release-build" ]; then
  echo "Removing old Release-build..."
  rm -rf Release-build
fi

echo "Creating new Release-build directory..."
mkdir Release-build
cd Release-build

# 6) Run CMake
cmake \
  -DSVF_DIR="$SOURCE_DIR/SVF/" \
  -DLLVM_DIR="$LLVM_DIR" \
  -DZ3_DIR="$Z3_DIR" \
  -DCMAKE_PREFIX_PATH="$PYBIND_DIR" \
  ..

# 7) Build with make
echo "Compiling with make..."
make -j4

# 8) Go back to source directory
cd "$SOURCE_DIR"

# 9) Print some environment info
echo "--------------------------------------"
echo "SVF_DIR=$SOURCE_DIR/SVF"
echo "LLVM_DIR=$LLVM_DIR"
echo "Z3_DIR=$Z3_DIR"
echo "--------------------------------------"

# 10) Cleanup old build artifacts (adjust as needed)
rm -rf build pysvf/SVF

# 11) Upgrade pip, setuptools, wheel, pybind11
"$PYTHON_BIN" -m pip install -U pip setuptools wheel pybind11

# 12) Finally, build the wheel package
echo "Building Python wheel for macOS arm64 (example: macosx_11_0_arm64) ..."
SVF_DIR="$SOURCE_DIR/SVF" \
LLVM_DIR="$LLVM_DIR" \
Z3_DIR="$Z3_DIR" \
VERSION="$VERSION" \
BUILD_TYPE="Debug" \
"$PYTHON_BIN" setup_mac.py bdist_wheel --plat-name macosx_11_0_arm64

echo "--------------------------------------"
echo "All done! The wheel file should be in dist/."
echo "--------------------------------------"
