#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import sys
from src.srcPythonModule.CL import CL
from src.srcPythonModule.svfLLVMUtil import LLVMUtil

import svfModule_pybind

# from svfModule_pybind import *


def main(arg_value):
    moduleNameVec = []
    print(arg_value)
    LLVMUtil.processArguments(arg_value, moduleNameVec)
    print(moduleNameVec)
    svfModule_pybind.ParseCommandLineOptions()


    if (svfModule_pybind.optionsWriteAnder == "ir_annotator"):
        svfModule_pybind.preProcessBCs()

    svfModule_pybind.buildSVFModule()
    svfModule_pybind.pagBuild()



    svfModule_pybind.MTARunOnModule()
    svfModule_pybind.MTAValidator()
    svfModule_pybind.lockValidator()
    svfModule_pybind.releaseLLVMModuleSet()

    


if __name__ == "__main__":
    # execute only if run as a script
    argv = sys.argv
    main(argv)
