#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import sys
import system
from src.srcPythonModule.LLVMUtil import LLVMUtil
from src.srcPythonModule.LLVMModuleSet import LLVMModuleSet
from src.srcPythonModule.CL import CL
from src.srcPythonModule.SABER import SABER
from src.srcPythonModule.SVFIR import SVFIR
from src.srcPythonModule.Options import Options




def main(arg_value):
    moduleNameVec = []
    print(arg_value)
    LLVMUtil.processArguments(arg_value, moduleNameVec)
    print(moduleNameVec)
    CL.ParseCommandLineOptions()


    if (Options.optionsWriteAnder == "ir_annotator"):
        LLVMModuleSet.preProcessBCs()

    LLVMModuleSet.buildSVFModule()

    SVFIR.pagBuild()

    SABER.saberCheckerAllInOne()
    
    SABER.saberRunOnModule()

    LLVMModuleSet.releaseLLVMModuleSet()

    


if __name__ == "__main__":
    # execute only if run as a script
    argv = sys.argv
    main(argv)
