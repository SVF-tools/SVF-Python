import os
import sys
import subprocess

CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))

EXTAPI_BC_PATH = os.path.join(CURRENT_DIR, "SVF/Release-build/lib/", "extapi.bc")
SVF_DIR = os.path.join(CURRENT_DIR, "SVF")
Z3_DIR = os.path.join(CURRENT_DIR, "SVF/z3.obj")
LLVM_DIR = os.path.join(CURRENT_DIR, "SVF/llvm-16.0.0.obj")

# Set environment variables
os.environ["Z3_DIR"] = Z3_DIR
os.environ["SVF_DIR"] = SVF_DIR
os.environ["LLVM_DIR"] = LLVM_DIR
os.environ["SVF_EXTAPI_DIR"] = os.path.join(SVF_DIR, "Release-build/lib")

from .pysvf import *

# Print environment paths for verification
print(f"SVF_DIR: {SVF_DIR}")
print(f"Z3_DIR: {Z3_DIR}")
