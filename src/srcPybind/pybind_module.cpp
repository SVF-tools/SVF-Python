#include <pybind11/pybind11.h>
#include "svfModule.h"

namespace py = pybind11;

PYBIND11_MODULE(svfModule_pybind, m) {
    // Old work.......................
    // --------------------------------------------------------------------------------

    m.def("processArguments", &processArguments, "Func 1");

    // --------------------------------------------------------------------------------
    // PASSED

    m.def("getModuleNameVecLen", &getModuleNameVecLen, "Func 2");
    m.def("getModuleNameVecItem", &getModuleNameVecItem, "Func 3");
    m.def("ParseCommandLineOptions", &ParseCommandLineOptions, "Func 4");

    m.def("buildSVFModule", &buildSVFModule, "Func 5");
    m.def("setModuleNameVec", &setModuleNameVec, "Func 6");
    
    // // m.def("buildSymbolTableInfo", &buildSymbolTableInfo, "Func 7"); // Not Defined in cpp
    
    m.def("pagBuild", &pagBuild, "Func 8");
    m.def("createAndersenWaveDiff", &createAndersenWaveDiff, "Func 9");
    m.def("getPTACallGraph", &getPTACallGraph, "Func 10");
    // m.def("getICFG", &getICFG, "Func 11");

    // m.def("newInstances", &newInstances, "Func 12");

    // // functioning part?
    // m.def("buildFullSVFG", &buildFullSVFG, "Func 13");
    m.def("deleteSvfg", &deleteSvfg, "Func 14");
    m.def("deleteVfg", &deleteVfg, "Func 15");
    // 

    m.def("releaseAndersenWaveDiff", &releaseAndersenWaveDiff, "Func 16");
    m.def("releaseSVFIR", &releaseSVFIR, "Func 17");
    m.def("dumpModulesToFile", &dumpModulesToFile, "Func 18");
    m.def("releaseLLVMModuleSet", &releaseLLVMModuleSet, "Func 19");
    m.def("llvm_shutdown", &llvm_shutdown, "Func 20");
    // // --------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------

    // New work.......................
    // --------------------------------------------------------------------------------
    // ae.cpp

    m.def("updateCallGraph", &updateCallGraph, "Func 21");
    m.def("getICFGUpdateCallGraph", &getICFGUpdateCallGraph, "Func 22");
    m.def("boolICFGMergeAdjacentNodes", &boolICFGMergeAdjacentNodes, "Func 23");
    m.def("mergeAdjacentNodes", &mergeAdjacentNodes, "Func 24");
    m.def("boolBufferOverflowCheck", &boolBufferOverflowCheck, "Func 25");
    m.def("bufOverflowCheckerRunOnModule", &bufOverflowCheckerRunOnModule, "Func 26");
    m.def("abstractExecutionRunOnModule", &abstractExecutionRunOnModule, "Func 27");


    // --------------------------------------------------------------------------------
    // wpa.cpp
    m.def("boolReadJson", &boolReadJson, "Func 28");
    m.def("SVFIRReaderRead", &SVFIRReaderRead, "Func 29");
    m.def("optionsWriteAnder", &optionsWriteAnder, "Func 30");
    m.def("preProcessBCs", &preProcessBCs, "Func 31");
    m.def("WPAPassRunOnModule", &WPAPassRunOnModule, "Func 32");

    // --------------------------------------------------------------------------------


    // --------------------------------------------------------------------------------
    // sapper.cpp
    // m.def("LEAKCHECKER", &LEAKCHECKER, "Func 33");
    // m.def("FILECHECKER", &FILECHECKER, "Func 34");
    // m.def("DFREECHECKER", &DFREECHECKER, "Func 35");
    // m.def("saberMakeUniqueLeakChecker", &saberMakeUniqueLeakChecker, "Func 36");
    // m.def("saberMakeUniqueFileChecker", &saberMakeUniqueFileChecker, "Func 37");
    // m.def("saberMakeUniqueDoubleFreeChecker", &saberMakeUniqueDoubleFreeChecker, "Func 38");
    m.def("saberCheckerAllInOne", &saberCheckerAllInOne, "Func 33");
    m.def("saberRunOnModule", &saberRunOnModule, "Func 34");
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    // cfl.cpp
    m.def("boolCFLGraphEmpty", &boolCFLGraphEmpty, "Func 34");
    m.def("svfirBuild", &svfirBuild, "Func 35");
    m.def("cflCheckerAllInOne", &cflCheckerAllInOne, "Func 36");
    m.def("cflAnalyze", &cflAnalyze, "Func 37");
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    // dda.cpp
    m.def("ddaPassRunOnModule", &ddaPassRunOnModule, "Func 38");
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------------------------------------
    // llvm2svf.cpp........
    m.def("moduleNameVecReplaceExtension", &moduleNameVecReplaceExtension, "Func 39");
    m.def("writeJsonToPath", &writeJsonToPath, "Func 40");
    m.def("outSVFIRJsonPath", &outSVFIRJsonPath, "Func 41");
    // --------------------------------------------------------------------------------------------------------------


}