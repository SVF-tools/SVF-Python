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
from src.srcPythonModule.ICFG import ICFG
from src.srcPythonModule.VFG import VFG
from src.srcPythonModule.SVFG import SVFG


def main(arg_value):
    moduleNameVec = []
    print(arg_value)
    LLVMUtil.processArguments(arg_value, moduleNameVec)
    print(moduleNameVec)
    CL.ParseCommandLineOptions()

    LLVMModuleSet.buildSVFModule()

    # Build Program Assignment Graph (SVFIR)
    SVFIR.pagBuild()

    # Create Andersen's pointer analysis
    AndersenWaveDiff.createAndersenWaveDiff()


    # Call Graph
    PTACallGraph.getPTACallGraph

    # ICFG
    ICFG.ICFGBuild()

    # Value-Flow Graph (VFG)
    VFG.VFGBuild()

    # Sparse value-flow graph (SVFG)
    SVFG.SVFGBuild()


    # clean up memory
    # VFG.VFGDelete()
    # SVFG.SVFGDelete()

    # AndersenWaveDiff.releaseAndersenWaveDiff()
    # SVFIR.releaseSVFIR()

    # LLVMModuleSet.dumpModulesToFile(".svf.bc")
    # LLVMModuleSet.releaseLLVMModuleSet()

    # LLVMUtil.LLVMShutdown()


if __name__ == "__main__":
    # execute only if run as a script
    argv = sys.argv
    main(argv)
