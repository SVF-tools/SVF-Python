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

# Step 4: Bootstrap a fresh pip / setuptools / wheel that have a RECORD file.
#
# OS package managers ship pip / setuptools / wheel WITHOUT RECORD files, so
# any later `pip install -U pip` (or wheel, or setuptools) fails with
# `uninstall-no-record-file` — and `--break-system-packages` does NOT solve
# that, since it's a separate uninstall-time check from PEP 668.
#
# Some downstream build deps require a recent pip, so we DO want to upgrade.
# On Linux: remove the apt-managed copies first so they don't shadow the
# pip-installed ones, then bootstrap via get-pip.py.  On macOS: skip the apt
# remove step (brew bundles them with the python formula and they can't be
# removed individually) — `--ignore-installed` on the bootstrap line keeps
# pip from trying to uninstall brew's copy.
if [[ "$OSTYPE" == "linux-gnu"* ]] && command -v apt-get >/dev/null 2>&1; then
    sudo apt-get remove -y --purge python3-pip python3-setuptools python3-wheel 2>/dev/null || true
fi
curl -sS https://bootstrap.pypa.io/get-pip.py -o get-pip.py
"$PYTHON_EXEC" get-pip.py --break-system-packages --ignore-installed
rm get-pip.py

# Step 5: Install Python build dependencies.
# --break-system-packages still required on PEP 668 envs.  pip is now
# upgradeable because get-pip.py installed a copy with a RECORD file.
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
