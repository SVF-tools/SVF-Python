import os
import sys

CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))

EXTAPI_BC_PATH = os.path.join(CURRENT_DIR, "libs", "extapi.bc")

os.environ["PYBIND_EXTAPI_BC_PATH"] = EXTAPI_BC_PATH

from .pysvf import *