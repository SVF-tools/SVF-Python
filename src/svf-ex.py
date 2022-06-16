#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
# import os
# sys.path.append(os.getcwd() + '/py')

from svfModule import SVFG, SVFUtil, cl
from svfModule import LLVMModuleSet
from svfModule import SVFIRBuilder
from svfModule import AndersenWaveDiff
from svfModule import SVFIR, VFG, SVFGBuilder



def main(argv):

    SVFUtil.processArguments(argv)
    cl.ParseCommandLineOptions()

    svfModule = LLVMModuleSet.buildSVFModule()

    svfModule.buildSymbolTableInfo()
    
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
    main(sys.argv)