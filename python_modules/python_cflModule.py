#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import sys
import system
from src.srcPythonModule.LLVMUtil import LLVMUtil
from src.srcPythonModule.LLVMModuleSet import LLVMModuleSet
from src.srcPythonModule.CL import CL
from src.srcPythonModule.CFL import CFL
from src.srcPythonModule.SVFIR import SVFIR
from src.srcPythonModule.Options import Options

# from svfModule_pybind import *


def main(arg_value):
    moduleNameVec = []
    print(arg_value)
    LLVMUtil.processArguments(arg_value, moduleNameVec)
    print(moduleNameVec)
    CL.ParseCommandLineOptions()


    if (Options.optionsWriteAnder == "ir_annotator"):
        LLVMModuleSet.preProcessBCs()


    if (Options.boolCFLGraphEmpty):
        LLVMModuleSet.buildSVFModule()
        SVFIR.svfirBuild()

    CFL.cflCheckerAllInOne()

    CFL.cflAnalyze()

    SVFIR.releaseSVFIR()
    LLVMModuleSet.releaseLLVMModuleSet()

    


if __name__ == "__main__":
    # execute only if run as a script
    argv = sys.argv
    main(argv)
