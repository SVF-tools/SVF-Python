#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import sys
from src.svfpy.Module.SVFG import SVFG
from src.svfpy.Module.CL import CL
from src.svfpy.Module.svfLLVMUtil import LLVMUtil
from src.svfpy.Module.LLVMModuleSet import LLVMModuleSet
from src.svfpy.Module.SVFIRBuilder import SVFIRBuilder
from src.svfpy.Module.AndersenWaveDiff import AndersenWaveDiff
from src.svfpy.Module.SVFIR import SVFIR
from src.svfpy.Module.VFG import VFG
from src.svfpy.Module.SVFGBuilder import SVFGBuilder
from src.svfpy.Module.SVFUtil import SVFUtil


def main(arg_value):
    moduleNameVec = []
    print(arg_value)
    LLVMUtil.processArguments(arg_value, moduleNameVec)
    print(moduleNameVec)
    CL.ParseCommandLineOptions()

    LLVMModuleSet.buildSVFModule(moduleNameVec)

    # # svfModule.buildSymbolTableInfo()

    pag = SVFIRBuilder.build()

    ander = AndersenWaveDiff.createAndersenWaveDiff(pag)
    callgraph = ander.getPTACallGraph()
    pag.getICFG()

    VFG.newInstances(callgraph)

    SVFGBuilder.buildFullSVFG(ander)

    VFG.delete()
    SVFG.delete()
    AndersenWaveDiff.releaseAndersenWaveDiff()
    SVFIR.releaseSVFIR()
    LLVMModuleSet.dumpModulesToFile('.svf.bc')
    LLVMModuleSet.releaseLLVMModuleSet()


if __name__ == "__main__":
    # execute only if run as a script
    argv = sys.argv
    main(argv)
