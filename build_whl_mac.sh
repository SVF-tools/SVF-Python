#!/bin/bash

PYENV_ROOT="/Users/z5489735/.pyenv/versions/"
PROJECT_DIR=$(pwd)


mkdir -p dist

for PYTHON_DIR in ${PYENV_ROOT}*/bin/python*; do
    PYTHON_BIN="${PYTHON_DIR}"
    if [[ -x "$PYTHON_BIN" ]]; then
        rm -rf build pysvf/libs
        $PYTHON_BIN -m pip install -U pip setuptools wheel pybind11

        $PYTHON_BIN setup_mac.py bdist_wheel --plat-name macosx_11_0_arm64

    fi
done