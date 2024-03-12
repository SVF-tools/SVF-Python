#include "SVF-LLVM/LLVMUtil.h" // SVF/svf-llvm/include/SVF-LLVM/LLVMUtil.h          -> SVF/svf-llvm/lib/LLVMUtil.cpp
#include "Graphs/SVFG.h" // SVF/svf/include/Graphs/SVFG.h                           -> SVF/svf/lib/Graphs/SVFG.cpp
#include "WPA/Andersen.h" // SVF/svf/include/WPA/Andersen.h                         -> SVF/svf/lib/WPA/Andersen.cpp
#include "SVF-LLVM/SVFIRBuilder.h" // SVF/svf-llvm/include/SVF-LLVM/SVFIRBuilder.h  -> SVF/svf-llvm/lib/SVFIRBuilder.cpp
#include "Util/Options.h" // SVF/svf/include/Util/Options.h                         -> SVF/svf/lib/Util/Options.cpp
#include "svfModule.h"

using namespace llvm;
using namespace std;
using namespace SVF;

// extern "C" {
//     static llvm::cl::opt<std::string> InputFilename(cl::Positional,
//         llvm::cl::desc("<input bitcode>"), llvm::cl::init("-"));
// //
//     char **arg_value = NULL;
//     int arg_num = 0;
//     int moduleNameVecLen = 0;
// //
//     std::vector<std::string> moduleNameVec;
//     SVFModule* svfModule;
//     SVFIRBuilder *builder;
//     SVFIR* pag;
//     Andersen* ander;
//     PTACallGraph* callgraph;
//     ICFG* icfg;
//     VFG* vfg;
//     SVFGBuilder* svfBuilder;
//     SVFG* svfg;
//     void processArguments(int argc, char **argv);
//     int getModuleNameVecLen();
//     const char* getModuleNameVecItem(int index, char* result);
//     void ParseCommandLineOptions();
//     void buildSVFModule();
//     void setModuleNameVec(char* name);
//     void buildSymbolTableInfo();
//     void build();
//     void createAndersenWaveDiff();
//     void getPTACallGraph();
//     void getICFG();
//     void newInstances();
//     void buildFullSVFG();
//     void deleteSvfg();
//     void deleteVfg();
//     void releaseAndersenWaveDiff();
//     void releaseSVFIR();
//     void dumpModulesToFile(const char *s);
//     void releaseLLVMModuleSet();
//     void llvm_shutdown();
// }

static llvm::cl::opt<std::string> InputFilename(cl::Positional,
    llvm::cl::desc("<input bitcode>"), llvm::cl::init("-"));
//
char **arg_value = NULL;
int arg_num = 0;
int moduleNameVecLen = 0;
//
std::vector<std::string> moduleNameVec;
SVFModule* svfModule;
SVFIRBuilder *builder;
SVFIR* pag;
Andersen* ander;
PTACallGraph* callgraph;
ICFG* icfg;
VFG* vfg;
SVFGBuilder* svfBuilder;
SVFG* svfg;

// /*
// input1: argc: the number of given argument
// input2: _argv: the content of given argument
// output:
// */
// void processArguments(int argc, char **_argv){
//     // TODD: delete free
//     arg_value = new char*[argc];
//     // init the arg_num and arg_value and moduleNameVec
//     LLVMUtil::processArguments(argc, _argv, arg_num, arg_value, moduleNameVec);
//     Convert Python list to C++ vector of strings
// }

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
    // return "return";
}

void ParseCommandLineOptions(){
    printf("arg_num: %d\n", arg_num);
    for(int i = 0; i < arg_num; i++){
        printf("arg_value[%d]:%s\n", i, arg_value[i]);
    }
    // fill in the values of all of the command line option variables once argc and argv are available.
    cl::ParseCommandLineOptions(arg_num, arg_value, "Whole Program Points-to Analysis\n");
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
