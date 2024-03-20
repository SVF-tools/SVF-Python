#include "SVF-LLVM/LLVMUtil.h" // SVF/svf-llvm/include/SVF-LLVM/LLVMUtil.h          -> SVF/svf-llvm/lib/LLVMUtil.cpp
#include "Graphs/SVFG.h" // SVF/svf/include/Graphs/SVFG.h                           -> SVF/svf/lib/Graphs/SVFG.cpp
#include "WPA/Andersen.h" // SVF/svf/include/WPA/Andersen.h                         -> SVF/svf/lib/WPA/Andersen.cpp
#include "SVF-LLVM/SVFIRBuilder.h" // SVF/svf-llvm/include/SVF-LLVM/SVFIRBuilder.h  -> SVF/svf-llvm/lib/SVFIRBuilder.cpp
#include "Util/Options.h" // SVF/svf/include/Util/Options.h                         -> SVF/svf/lib/Util/Options.cpp


#include "WPA/WPAPass.h"
#include "Util/CommandLine.h"
#include "AE/Svfexe/ICFGSimplification.h"

#include "AE/Svfexe/BufOverflowChecker.h"
#include "AE/Core/RelExeState.h"
#include "AE/Core/RelationSolver.h"


#include "WPA/WPAPass.h"
#include "Util/Options.h"
#include "SVFIR/SVFFileSystem.h"



#include "SABER/LeakChecker.h"
#include "SABER/FileChecker.h"
#include "SABER/DoubleFreeChecker.h"
#include "Util/Z3Expr.h"



#include "svfModule.h"

using namespace llvm;
using namespace std;
using namespace SVF;


static llvm::cl::opt<std::string> InputFilename(cl::Positional,
    llvm::cl::desc("<input bitcode>"), llvm::cl::init("-"));
//
char **arg_value = NULL;
int arg_num = 0;
int moduleNameVecLen = 0;

std::vector<std::string> moduleNameVec;
SVFModule* svfModule;
SVFIRBuilder* builder;
SVFIR* pag;
AndersenWaveDiff* ander;
PTACallGraph* callgraph;
ICFG* icfg;
VFG* vfg;
SVFGBuilder* svfBuilder;
SVFG* svfg;

std::unique_ptr<LeakChecker> saber;


// Old work.......................
// --------------------------------------------------------------------------------------------------------------
// /*
// input1: argc: the number of given argument
// input2: _argv: the content of given argument
// output:
// */

void processArguments(int argc, py::list argv) {
    // Convert Python list to C++ vector of strings
    std::vector<std::string> argv_vector;
    for (const auto& item : argv) {
        argv_vector.push_back(py::str(item));
    }

    arg_value = new char*[argc];

    // Convert the vector to char**
    char** _argv = new char*[argc];
    for (int i = 0; i < argc; ++i) {
        _argv[i] = new char[argv_vector[i].size() + 1];
        std::strcpy(_argv[i], argv_vector[i].c_str());
    }

    // Assuming arg_num and moduleNameVec are already declared
    // init the arg_num and moduleNameVec
    LLVMUtil::processArguments(argc, _argv, arg_num, arg_value, moduleNameVec);

}



// get the length of the moduleNameVec
int getModuleNameVecLen(){
    return moduleNameVec.size();
}
// input1: the index of the modelName
// input2: the target object
const char* getModuleNameVecItem(int index, char* result){
    printf("moduleNameVec.size():%ld, index: %d\n", moduleNameVec.size(), index);
    if(index < moduleNameVec.size()){
        // printf("moduleNameVec[index]:%s\n", moduleNameVec[index].c_str());
        // which is equal to result = moduleNameVec[index]
        snprintf(result, strlen(moduleNameVec[index].c_str()) + 1, "%s", moduleNameVec[index].c_str());
    }
    return result;
}

void ParseCommandLineOptions(){
    printf("arg_num: %d\n", arg_num);
    for(int i = 0; i < arg_num; i++){
        printf("arg_value[%d]:%s\n", i, arg_value[i]);
    }
    // fill in the values of all of the command line option variables once argc and argv are available.
    // cl::ParseCommandLineOptions(arg_num, arg_value, "Whole Program Points-to Analysis\n");

    moduleNameVec = OptionBase::parseOptions(arg_num, arg_value, "Whole Program Points-to Analysis",
                                 "[options] <input-bitcode...>");

    printf("---------- Parse Successful ----------\n");
}

void buildSVFModule(){
    svfModule = LLVMModuleSet::getLLVMModuleSet()->buildSVFModule(moduleNameVec);
}

void setModuleNameVec(char* name){
    if(name != nullptr){
        std::string s(name);
        // printf("name:%s\n", name);
        moduleNameVec.push_back(s);
    }
}

//void buildSymbolTableInfo(){
//    svfModule->buildSymbolTableInfo();
//}
// build the
void build(){
    SVFIRBuilder _builder(svfModule);
    builder = &_builder;
    pag = _builder.build();
}
void createAndersenWaveDiff(){
    ander = AndersenWaveDiff::createAndersenWaveDiff(pag);
}

void getPTACallGraph(){
    callgraph = ander->getPTACallGraph();
}

void getICFG(){
    icfg = pag->getICFG();
}

void newInstances(){
    vfg = new VFG(callgraph);
}

void buildFullSVFG(){
    SVFGBuilder _svfBuilder;
    svfBuilder = &_svfBuilder;
    _svfBuilder.buildFullSVFG(ander);
}

void deleteVfg(){
    delete vfg;
}
void deleteSvfg(){
    delete svfg;
}

void releaseAndersenWaveDiff(){
    AndersenWaveDiff::releaseAndersenWaveDiff();
}

void releaseSVFIR(){
    SVFIR::releaseSVFIR();
}

void dumpModulesToFile(const char *s){
    LLVMModuleSet::getLLVMModuleSet()->dumpModulesToFile(s);
}
void releaseLLVMModuleSet(){
    SVF::LLVMModuleSet::releaseLLVMModuleSet();
}

void llvm_shutdown(){
    llvm::llvm_shutdown();
}


// --------------------------------------------------------------------------------------------------------------



// New work.......................
// --------------------------------------------------------------------------------------------------------------
// ae.cpp.....
void updateCallGraph(){
    builder->updateCallGraph(callgraph);
}

void getICFGUpdateCallGraph(){
    pag->getICFG()->updateCallGraph(callgraph);
}


bool boolICFGMergeAdjacentNodes(){
    return Options::ICFGMergeAdjacentNodes();
}

void mergeAdjacentNodes(){
    ICFGSimplification::mergeAdjacentNodes(pag->getICFG());
}

bool boolBufferOverflowCheck(){
    return Options::BufferOverflowCheck();
}

void bufOverflowCheckerRunOnModule(){
    BufOverflowChecker ae;
    ae.runOnModule(pag->getICFG());
}

void abstractExecutionRunOnModule(){
    AbstractExecution ae;
    ae.runOnModule(pag->getICFG());
}


// --------------------------------------------------------------------------------------------------------------
// wpa.cpp........
bool boolReadJson(){
    return Options::ReadJson();
}

void SVFIRReaderRead(){
    pag = SVFIRReader::read(moduleNameVec.front());
}

std::string optionsWriteAnder(){
    return Options::WriteAnder();
}

void preProcessBCs(){
    LLVMModuleSet::preProcessBCs(moduleNameVec);
}

void WPAPassRunOnModule(){
    WPAPass wpa;
    wpa.runOnModule(pag);
}

// --------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------
// saber.cpp........
static Option<bool> LEAKCHECKER(
    "leak",
    "Memory Leak Detection",
    false
);

static Option<bool> FILECHECKER(
    "fileck",
    "File Open/Close Detection",
    false
);

static Option<bool> DFREECHECKER(
    "dfree",
    "Double Free Detection",
    false
);

void saberCheckerAllInOne(){
    if(LEAKCHECKER())
        saber = std::make_unique<LeakChecker>();
    else if(FILECHECKER())
        saber = std::make_unique<FileChecker>();
    else if(DFREECHECKER())
        saber = std::make_unique<DoubleFreeChecker>();
    else
        saber = std::make_unique<LeakChecker>();
}

// void saberMakeUniqueLeakChecker(){
//     saber = std::make_unique<LeakChecker>();
// }

// void saberMakeUniqueFileChecker(){
//     saber = std::make_unique<FileChecker>();
// }

// void saberMakeUniqueDoubleFreeChecker(){
//     saber = std::make_unique<DoubleFreeChecker>();
// }

void saberRunOnModule(){
    saber->runOnModule(pag);
}

// --------------------------------------------------------------------------------------------------------------