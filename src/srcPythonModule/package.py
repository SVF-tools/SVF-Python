import ctypes

import os
import sys

if sys.platform == 'linux':
    libSvfModule = ctypes.cdll.LoadLibrary(os.getcwd() + '/src/svfModule/libsvfModule.so')
elif sys.platform == 'darwin':
    libSvfModule = ctypes.cdll.LoadLibrary(os.getcwd() + '/src/svfModule/libsvfModule.dylib')
