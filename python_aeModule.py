#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import sys
from src.srcPythonModule.CL import CL
from src.srcPythonModule.svfLLVMUtil import LLVMUtil

import svfModule_pybind


def main(arg_value):
    moduleNameVec = []
    print(arg_value)
    LLVMUtil.processArguments(arg_value, moduleNameVec)
    print(moduleNameVec)
    svfModule_pybind.ParseCommandLineOptions()

    svfModule_pybind.buildSVFModule()

    # # # svfModule.buildSymbolTableInfo()

    svfModule_pybind.pagBuild()

    # General Stats; CallGraph Stats (Andersen analysis); 
    # Andersen Pointer Analysis Stats; Persistent Points-To Cache Statistics: Andersen's analysis bitvector
    svfModule_pybind.createAndersenWaveDiff() 

    svfModule_pybind.getPTACallGraph()
    # svfModule_pybind.getICFG()

    svfModule_pybind.updateCallGraph()
    svfModule_pybind.getICFGUpdateCallGraph()
    if (svfModule_pybind.boolICFGMergeAdjacentNodes()):
        svfModule_pybind.mergeAdjacentNodes()
    

    if (svfModule_pybind.boolBufferOverflowCheck()):
        svfModule_pybind.bufOverflowCheckerRunOnModule()
    else:
        svfModule_pybind.abstractExecutionRunOnModule()


    svfModule_pybind.releaseLLVMModuleSet()


if __name__ == "__main__":
    # execute only if run as a script
    argv = sys.argv
    main(argv)
