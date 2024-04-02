#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import sys
import system
from src.srcPythonModule.svfLLVMUtil import svfLLVMUtil
from src.srcPythonModule.LLVMModuleSet import LLVMModuleSet
from src.srcPythonModule.CL import CL
from src.srcPythonModule.WPA import WPA
from src.srcPythonModule.SVFIR import SVFIR
from src.srcPythonModule.Options import Options


# from svfModule_pybind import *


def main(arg_value):
    moduleNameVec = []
    print(arg_value)
    svfLLVMUtil.processArguments(arg_value, moduleNameVec)
    print(moduleNameVec)
    CL.ParseCommandLineOptions()

    if (Options.boolReadJson()):
        SVFIR.SVFIRReaderRead()

    else:
        if (Options.optionsWriteAnder == "ir_annotator"):
            LLVMModuleSet.preProcessBCs()

        LLVMModuleSet.buildSVFModule()

        SVFIR.pagBuild()

    WPA.WPAPassRunOnModule()
    LLVMModuleSet.releaseLLVMModuleSet()

    


if __name__ == "__main__":
    # execute only if run as a script
    argv = sys.argv
    main(argv)
