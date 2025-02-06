import os
import sys

CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))

EXTAPI_BC_PATH = os.path.join(CURRENT_DIR, "SVF/Release-build/lib/", "extapi.bc")

SVF_DIR = os.path.join(CURRENT_DIR, "SVF")
Z3_DIR = os.path.join(CURRENT_DIR, "z3.obj")

# echo SVF_DIR and Z3_DIR
print(f"SVF_DIR: {SVF_DIR}")
print(f"Z3_DIR: {Z3_DIR}")

os.environ["PYBIND_EXTAPI_BC_PATH"] = EXTAPI_BC_PATH

from .pysvf import *