#include <pybind11/pybind11.h>
#include "svfModule.h"

namespace py = pybind11;

PYBIND11_MODULE(svfModule_pybind, m) {
    m.def("processArguments", &processArguments, "Func 1");

    // --------------------------------------------------------------------------------
    // PASSED

    m.def("getModuleNameVecLen", &getModuleNameVecLen, "Func 2");
    m.def("getModuleNameVecItem", &getModuleNameVecItem, "Func 3");
    m.def("ParseCommandLineOptions", &ParseCommandLineOptions, "Func 4");

    m.def("buildSVFModule", &buildSVFModule, "Func 5");
    m.def("setModuleNameVec", &setModuleNameVec, "Func 6");
    
    // // m.def("buildSymbolTableInfo", &buildSymbolTableInfo, "Func 7"); // Not Defined in cpp
    
    m.def("build", &build, "Func 8");
    m.def("createAndersenWaveDiff", &createAndersenWaveDiff, "Func 9");
    m.def("getPTACallGraph", &getPTACallGraph, "Func 10");
    m.def("getICFG", &getICFG, "Func 11");

    m.def("newInstances", &newInstances, "Func 12");

    // functioning part?
    m.def("buildFullSVFG", &buildFullSVFG, "Func 13");
    m.def("deleteSvfg", &deleteSvfg, "Func 14");
    m.def("deleteVfg", &deleteVfg, "Func 15");
    // 

    m.def("releaseAndersenWaveDiff", &releaseAndersenWaveDiff, "Func 16");
    m.def("releaseSVFIR", &releaseSVFIR, "Func 17");
    m.def("dumpModulesToFile", &dumpModulesToFile, "Func 18");
    m.def("releaseLLVMModuleSet", &releaseLLVMModuleSet, "Func 19");
    m.def("llvm_shutdown", &llvm_shutdown, "Func 20");
    // // --------------------------------------------------------------------------------
}