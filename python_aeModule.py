#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import sys
from src.srcPythonModule.SVFG import SVFG
from src.srcPythonModule.CL import CL
from src.srcPythonModule.svfLLVMUtil import LLVMUtil
from src.srcPythonModule.LLVMModuleSet import LLVMModuleSet
from src.srcPythonModule.SVFIRBuilder import SVFIRBuilder
from src.srcPythonModule.AndersenWaveDiff import AndersenWaveDiff
from src.srcPythonModule.SVFIR import SVFIR
from src.srcPythonModule.VFG import VFG
from src.srcPythonModule.SVFGBuilder import SVFGBuilder
from src.srcPythonModule.SVFUtil import SVFUtil
import svfModule_pybind


def main(arg_value):
    moduleNameVec = []
    print(arg_value)
    LLVMUtil.processArguments(arg_value, moduleNameVec)
    print(moduleNameVec)
    CL.ParseCommandLineOptions()

    LLVMModuleSet.buildSVFModule(moduleNameVec)

    # # svfModule.buildSymbolTableInfo()

    pag = SVFIRBuilder.build()

    # General Stats; CallGraph Stats (Andersen analysis); 
    # Andersen Pointer Analysis Stats; Persistent Points-To Cache Statistics: Andersen's analysis bitvector
    ander = AndersenWaveDiff.createAndersenWaveDiff(pag) 

    callgraph = ander.getPTACallGraph()
    pag.getICFG()

    # svfModule_pybind.updateCallGraph()
    # svfModule_pybind.getICFGUpdateCallGraph()
    # if (svfModule_pybind.boolICFGMergeAdjacentNodes()):
    #     svfModule_pybind.mergeAdjacentNodes()
    

    # if (svfModule_pybind.boolBufferOverflowCheck()):
    #     svfModule_pybind.bufOverflowCheckerRunOnModule()
    # else:
    #     svfModule_pybind.abstractExecutionRunOnModule()


    # AndersenWaveDiff.releaseAndersenWaveDiff()
    # SVFIR.releaseSVFIR()
    # LLVMModuleSet.dumpModulesToFile('.svf.bc')
    # LLVMModuleSet.releaseLLVMModuleSet()


if __name__ == "__main__":
    # execute only if run as a script
    argv = sys.argv
    main(argv)
