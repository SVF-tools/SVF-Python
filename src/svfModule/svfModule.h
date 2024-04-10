// #include "SVF-LLVM/LLVMUtil.h" // SVF/svf-llvm/include/SVF-LLVM/LLVMUtil.h
// #include "Graphs/SVFG.h" // SVF/svf/include/Graphs/SVFG.h
// #include "WPA/Andersen.h" // SVF/svf/include/WPA/Andersen.h
// #include "SVF-LLVM/SVFIRBuilder.h" // SVF/svf-llvm/include/SVF-LLVM/SVFIRBuilder.h
#include "Util/Options.h" // SVF/svf/include/Util/Options.h

#include "WPA/Andersen.h"


#include "MemoryModel/PointerAnalysisImpl.h"
#include "WPA/WPAStat.h"
#include "WPA/WPASolver.h"
#include "SVFIR/SVFIR.h"
#include "Graphs/ConsG.h"
#include "Util/Options.h"


#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace std;
using namespace SVF;


// namespace SVF_Header {

// static llvm::cl::opt<std::string> InputFilename(cl::Positional,
//     llvm::cl::desc("<input bitcode>"), llvm::cl::init("-"));

// Old Work.......................
// --------------------------------------------------------------------------------------------------------------

/*
input1: argc: the number of given argument
input2: _argv: the content of given argument
output:
*/
// void processArguments(int argc, char **_argv);

void processArguments(int argc, py::list argv);


// get the length of the moduleNameVec
int getModuleNameVecLen();
// input1: the index of the modelName
// input2: the target object
const char* getModuleNameVecItem(int index, char* result);

void ParseCommandLineOptions();

void buildSVFModule();

void setModuleNameVec(char* name);

//void buildSymbolTableInfo(){
//    svfModule->buildSymbolTableInfo();
//}
// build the
void pagBuild();
void createAndersenWaveDiff();

void getPTACallGraph();

// void getICFG();

// void newInstances();

// void buildFullSVFG();

void deleteVfg();
void deleteSvfg();

void releaseAndersenWaveDiff();

void releaseSVFIR();
void dumpModulesToFile(const char *s);
void releaseLLVMModuleSet();

void llvm_shutdown();

// }
// --------------------------------------------------------------------------------------------------------------


// New work.......................
// --------------------------------------------------------------------------------------------------------------
// ae.cpp.....
void updateCallGraph();

void getICFGUpdateCallGraph();


bool boolICFGMergeAdjacentNodes();
void mergeAdjacentNodes();

bool boolBufferOverflowCheck();

void bufOverflowCheckerRunOnModule();

void abstractExecutionRunOnModule();

// --------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------
// wpa.cpp........
bool boolReadJson();

void SVFIRReaderRead();

std::string optionsWriteAnder();

void preProcessBCs();

void WPAPassRunOnModule();


// --------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------
// saber.cpp
// Option<bool> LEAKCHECKER();

// Option<bool> FILECHECKER();

// Option<bool> DFREECHECKER();

// void saberMakeUniqueLeakChecker();

// void saberMakeUniqueFileChecker();

// void saberMakeUniqueDoubleFreeChecker();
void saberCheckerAllInOne();

void saberRunOnModule();
// --------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------
// cfl.cpp
bool boolCFLGraphEmpty();
void svfirBuild();
void cflCheckerAllInOne();
void cflAnalyze();
// --------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------
// dda.cpp
void ddaPassRunOnModule();
// --------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------
// llvm2svf.cpp........
std::string moduleNameVecReplaceExtension();

void writeJsonToPath(std::string jsonPath);

void outSVFIRJsonPath(std::string jsonPath);

// --------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------
// mta.cpp........
void MTARunOnModule();

void MTAValidator();

void lockValidator();
// --------------------------------------------------------------------------------------------------------------


// // --------------------------------------------------------------------------------------------------------------
// // Andersen.h........ Binding by CLASS.... AIN'T WORKING


// inline void bind_AndersenBase(py::module& m) {
//     // Register the PTATY enum with pybind11
//     py::enum_<PointerAnalysis::PTATY>(m, "PTATY1")
//         .value("Andersen_BASE", PointerAnalysis::PTATY::Andersen_BASE)
//         // Add other values of the enum here if needed
//         .export_values();
    
//     py::class_<AndersenBase>(m, "AndersenBase")
//         .def(py::init<SVFIR*, PointerAnalysis::PTATY, bool>(), py::arg("_pag"), py::arg("type") = PointerAnalysis::PTATY::Andersen_BASE, py::arg("alias_check") = true)
//         .def("analyze", &AndersenBase::analyze)
//         .def("solveAndwritePtsToFile", &AndersenBase::solveAndwritePtsToFile)
//         .def("readPtsFromFile", &AndersenBase::readPtsFromFile)

//         .def("solveConstraints", &AndersenBase::solveConstraints)
//         .def("initialize", &AndersenBase::initialize)
//         .def("finalize", &AndersenBase::finalize)

//         .def("updateCallGraph", &AndersenBase::updateCallGraph)

//         .def_static("classof", [](const AndersenBase*) { return Andersen::classof((const AndersenBase*) nullptr); })
//         .def_static("classof", [](const PointerAnalysis *pta) { return Andersen::classof(pta); })


//         .def("getConstraintGraph", &AndersenBase::getConstraintGraph)


//         .def("printStat", &AndersenBase::printStat)
//         .def("normalizePointsTo", &AndersenBase::normalizePointsTo)

//         .def("cleanConsCG", &AndersenBase::cleanConsCG);
// }



// inline void bind_Anderson(py::module& m) {
//     // Register the PTATY enum with pybind11
//     py::enum_<PointerAnalysis::PTATY>(m, "PTATY2")
//         .value("Andersen_WPA", PointerAnalysis::PTATY::Andersen_WPA)
//         // Add other values of the enum here if needed
//         .export_values();

//     py::class_<Andersen>(m, "Andersen")
//         .def(py::init<SVFIR*, PointerAnalysis::PTATY, bool>(), py::arg("_pag"), py::arg("type") = PointerAnalysis::PTATY::Andersen_WPA, py::arg("alias_check") = true)
//         .def("initialize", &Andersen::initialize)
//         .def("finalize", &Andersen::finalize)
//         .def("resetData", &Andersen::resetData)


//         .def_static("classof", [](const Andersen*) { return Andersen::classof((const Andersen*) nullptr); })
//         .def_static("classof", [](const PointerAnalysis *pta) { return Andersen::classof(pta); })

//         .def("sccRepNode", &Andersen::sccRepNode)
//         .def("sccSubNodes", &Andersen::sccSubNodes)

//         .def("getPts", &Andersen::getPts)
//         .def("unionPts", (bool (Andersen::*)(NodeID, const PointsTo&)) &Andersen::unionPts)
//         .def("unionPts", (bool (Andersen::*)(NodeID, NodeID)) &Andersen::unionPts)


//         .def("dumpTopLevelPtsTo", &Andersen::dumpTopLevelPtsTo)
//         .def("setDetectPWC", &Andersen::setDetectPWC);
// }

// inline void bind_AndersenWaveDiff(py::module& m) {
//     // Register the PTATY enum with pybind11
//     py::enum_<PointerAnalysis::PTATY>(m, "PTATY3")
//         .value("AndersenWaveDiff_WPA", PointerAnalysis::PTATY::AndersenWaveDiff_WPA)
//         // Add other values of the enum here if needed
//         .export_values();
//     py::class_<AndersenWaveDiff, Andersen>(m, "AndersenWaveDiff")
//         .def(py::init<SVFIR*, PointerAnalysis::PTATY, bool>(), py::arg("_pag"), py::arg("type") = PointerAnalysis::PTATY::AndersenWaveDiff_WPA, py::arg("alias_check") = true)
//         .def_static("createAndersenWaveDiff", &AndersenWaveDiff::createAndersenWaveDiff)
//         .def_static("releaseAndersenWaveDiff", &AndersenWaveDiff::releaseAndersenWaveDiff)
//         .def("initialize", &AndersenWaveDiff::initialize)
//         .def("solveWorklist", &AndersenWaveDiff::solveWorklist)
//         .def("processNode", &AndersenWaveDiff::processNode)
//         .def("postProcessNode", &AndersenWaveDiff::postProcessNode)
//         .def("handleLoad", &AndersenWaveDiff::handleLoad)
//         .def("handleStore", &AndersenWaveDiff::handleStore);
// }
// // --------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------
// Andersen.h........ Binding by FUNCTION.... TRYING

// AndersenBase

void AndersenBaseAnalyze();
// --------------------------------------------------------------------------------------------------------------

