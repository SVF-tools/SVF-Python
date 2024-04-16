#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import sys
import system
from src.srcPythonModule.LLVMUtil import LLVMUtil
from src.srcPythonModule.LLVMModuleSet import LLVMModuleSet
from src.srcPythonModule.CL import CL
from src.srcPythonModule.AE import AE
from src.srcPythonModule.AndersenWaveDiff import AndersenWaveDiff
from src.srcPythonModule.SVFIR import SVFIR
from src.srcPythonModule.Options import Options
from src.srcPythonModule.PTACallGraph import PTACallGraph


def main(arg_value):
    moduleNameVec = []
    print(arg_value)
    LLVMUtil.processArguments(arg_value, moduleNameVec)
    print(moduleNameVec)
    CL.ParseCommandLineOptions()

    LLVMModuleSet.buildSVFModule()

    SVFIR.pagBuild()

    # General Stats; CallGraph Stats (Andersen analysis); 
    # Andersen Pointer Analysis Stats; Persistent Points-To Cache Statistics: Andersen's analysis bitvector
    AndersenWaveDiff.createAndersenWaveDiff() 

    PTACallGraph.getPTACallGraph()

    PTACallGraph.updateCallGraph()

    SVFIR.getICFGUpdateCallGraph()
    if (Options.boolICFGMergeAdjacentNodes()):
        AE.mergeAdjacentNodes()
    

    if (Options.boolBufferOverflowCheck()):
        AE.bufOverflowCheckerRunOnModule()
    else:
        AE.abstractExecutionRunOnModule()


    LLVMModuleSet.releaseLLVMModuleSet()


if __name__ == "__main__":
    # execute only if run as a script
    argv = sys.argv
    main(argv)
