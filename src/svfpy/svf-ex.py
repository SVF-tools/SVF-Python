#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import sys
from Module.SVFG import SVFG
from Module.CL import CL
from Module.svfLLVMUtil import LLVMUtil
from Module.LLVMModuleSet import LLVMModuleSet
from Module.SVFIRBuilder import SVFIRBuilder
from Module.AndersenWaveDiff import AndersenWaveDiff
from Module.SVFIR import SVFIR
from Module.VFG import VFG
from Module.SVFGBuilder import SVFGBuilder
from Module.SVFUtil import SVFUtil


def main(arg_value):
    moduleNameVec = []

    LLVMUtil.processArguments(arg_value, moduleNameVec)
    CL.ParseCommandLineOptions()

    LLVMModuleSet.buildSVFModule(moduleNameVec)

    # svfModule.buildSymbolTableInfo()

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
