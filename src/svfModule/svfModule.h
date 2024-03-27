// #include "SVF-LLVM/LLVMUtil.h" // SVF/svf-llvm/include/SVF-LLVM/LLVMUtil.h
// #include "Graphs/SVFG.h" // SVF/svf/include/Graphs/SVFG.h
// #include "WPA/Andersen.h" // SVF/svf/include/WPA/Andersen.h
// #include "SVF-LLVM/SVFIRBuilder.h" // SVF/svf-llvm/include/SVF-LLVM/SVFIRBuilder.h
#include "Util/Options.h" // SVF/svf/include/Util/Options.h

#include <pybind11/pybind11.h>

namespace py = pybind11;


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

