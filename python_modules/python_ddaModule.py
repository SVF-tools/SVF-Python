#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import sys
import system
from src.srcPythonModule.svfLLVMUtil import svfLLVMUtil
from src.srcPythonModule.LLVMModuleSet import LLVMModuleSet
from src.srcPythonModule.CL import CL
from src.srcPythonModule.DDA import DDA
from src.srcPythonModule.SVFIR import SVFIR
from src.srcPythonModule.Options import Options

import svfModule_pybind



def main(arg_value):
    moduleNameVec = []
    print(arg_value)
    svfLLVMUtil.processArguments(arg_value, moduleNameVec)
    print(moduleNameVec)
    CL.ParseCommandLineOptions()


    if (Options.optionsWriteAnder == "ir_annotator"):
        LLVMModuleSet.preProcessBCs()

    LLVMModuleSet.buildSVFModule()

    SVFIR.pagBuild()

    DDA.ddaPassRunOnModule()

    LLVMModuleSet.releaseLLVMModuleSet()

    


if __name__ == "__main__":
    # execute only if run as a script
    argv = sys.argv
    main(argv)
