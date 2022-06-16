#include "SVF-FE/LLVMUtil.h"
#include "Graphs/SVFG.h"
#include "WPA/Andersen.h"
#include "SVF-FE/SVFIRBuilder.h"
#include "Util/Options.h"

using namespace llvm;
using namespace std;
using namespace SVF;

extern "C" {
    char **arg_value = NULL;
    int arg_num = 0;
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
    void processArguments(int argc, char **argv);
    void ParseCommandLineOptions();
    void buildSVFModule();
    void buildSymbolTableInfo();
    void build();
    void createAndersenWaveDiff();
    void getPTACallGraph();
    void getICFG();
    void newInstances();
    void buildFullSVFG();
    void deleteSvfg();
    void deleteVfg();
    void releaseAndersenWaveDiff();
    void releaseSVFIR();
    void dumpModulesToFile(const char *s);
    void releaseLLVMModuleSet();
    void llvm_shutdown();
}


void processArguments(int argc, char **argv){
    // printf("len: %d\n", argc);
    // for(int i = 0; i < argc; i++){
    //     printf("argv[%d]:%s\n", i, argv[i]);
    // }
    arg_value = new char*[argc];
    SVFUtil::processArguments(argc, argv, arg_num, arg_value, moduleNameVec);
}

void ParseCommandLineOptions(){
     cl::ParseCommandLineOptions(arg_num, arg_value, "Whole Program Points-to Analysis\n");
}

void buildSVFModule(){
    svfModule = LLVMModuleSet::getLLVMModuleSet()->buildSVFModule(moduleNameVec);
}

void buildSymbolTableInfo(){
    svfModule->buildSymbolTableInfo();
}

void build(){
    SVFIRBuilder _builder;
    pag = _builder.build(svfModule);
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
