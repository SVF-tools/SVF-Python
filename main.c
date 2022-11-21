#include <iostream>
#include "SVF-FE/LLVMUtil.h"
#include "SVF-FE/PAGBuilder.h"
#include "AnderSen.h"

using namespace llvm;
using namespace std;
using namespace SVF;

int main(int argc, char **argv) {
    int arg_num = 0;
    char **arg_value = new char*[argc];
    std::vector<std::string> moduleNameVec;
    SVFUtil::processArguments(argc, argv, arg_num, arg_value, moduleNameVec);
    cl::ParseCommandLineOptions(arg_num, arg_value, "Whole Program Points-to Analysis\n");

    SVFModule* svfModule = LLVMModuleSet::getLLVMModuleSet()->buildSVFModule(moduleNameVec);
    /// Build Program Assignment Graph (PAG)
    SVF::PAGBuilder builder;
    SVF::PAG *pag = builder.build(svfModule);
    pag->dump ("pag");

    AndersenPTA *andersenPTA = new AndersenPTA(pag);
    andersenPTA->analyze();
    andersenPTA->dump_consCG("consG");

    SVF::LLVMModuleSet::releaseLLVMModuleSet();
    SVF::PAG::releasePAG();
    delete andersenPTA;
    return 0;
}

