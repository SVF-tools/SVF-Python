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
#include "SVFIR/SVFFileSystem.h"
#include "SABER/LeakChecker.h"
#include "SABER/FileChecker.h"
#include "SABER/DoubleFreeChecker.h"
#include "Util/Z3Expr.h"

#include "CFL/CFLAlias.h"
#include "CFL/CFLVF.h"

#include "DDA/DDAPass.h"

#include "SVFIR/SVFFileSystem.h"

#include "MTA/MTA.h"
#include "MTA/MTAResultValidator.h"
#include "MTA/LockResultValidator.h"


#include "svfModule.h"

using namespace llvm;
using namespace std;
using namespace SVF;


// Old work.......................
// --------------------------------------------------------------------------------------------------------------
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
// --------------------------------------------------------------------------------------------------------------

// New work.......................
// --------------------------------------------------------------------------------------------------------------
// cfl.cpp.....
SVFIR* svfir;
std::unique_ptr<CFLBase> cfl;

// mta.cpp.....
MTA* mta;



// --------------------------------------------------------------------------------------------------------------


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
void pagBuild(){
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


void saberRunOnModule(){
    saber->runOnModule(pag);
}

// --------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------
// cfl.cpp........
bool boolCFLGraphEmpty(){
    return Options::CFLGraph().empty();
}

void svfirBuild(){
    SVFIRBuilder _builder(svfModule);
    builder = &_builder;
    svfir = _builder.build();
}

void cflCheckerAllInOne(){
    if (Options::CFLSVFG())
        cfl = std::make_unique<CFLVF>(svfir);
    else if (Options::POCRHybrid())
        cfl = std::make_unique<POCRHybrid>(svfir);
    else if (Options::POCRAlias())
        cfl = std::make_unique<POCRAlias>(svfir);
    else
        cfl = std::make_unique<CFLAlias>(svfir);
}

void cflAnalyze(){
    cfl->analyze();
}

// --------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------
// dda.cpp........
static Option<bool> DAA(
    "daa",
    "Demand-Driven Alias Analysis Pass",
    false
);

void ddaPassRunOnModule(){
    DDAPass dda;
    dda.runOnModule(pag);
}
// --------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------
// llvm2svf.cpp........
static std::string replaceExtension(const std::string& path)
{
    size_t pos = path.rfind('.');
    if (pos == std::string::npos ||
            (path.substr(pos) != ".bc" && path.substr(pos) != ".ll"))
    {
        SVFUtil::errs() << "Error: expect file with extension .bc or .ll\n";
        exit(EXIT_FAILURE);
    }
    return path.substr(0, pos) + ".svf.json";
}

std::string moduleNameVecReplaceExtension(){
    return replaceExtension(moduleNameVec.front());
}

void writeJsonToPath(std::string jsonPath){
    SVFIRWriter::writeJsonToPath(pag, jsonPath);
}

void outSVFIRJsonPath(std::string jsonPath){
    SVFUtil::outs() << "SVF IR is written to '" << jsonPath << "'\n";
}

// --------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------
// mta.cpp........
void MTARunOnModule(){
    MTA _mta;
    _mta.runOnModule(pag);
    mta = &_mta;
}

void MTAValidator(){
    MTAResultValidator MTAValidator(mta->getMHP());
    MTAValidator.analyze();
}

void lockValidator(){
    LockResultValidator lockvalidator(mta->getLockAnalysis());
    lockvalidator.analyze();
}
// --------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------
// Andersen.h........ Binding by FUNCTION.... TRYING

// AndersenBase
void AndersenBaseAnalyze() {
    AndersenBase* ab = new AndersenBase(pag);
    ab->analyze();
}

void AndersenBaseSolveAndwritePtsToFile(const std::string& filename) {
    AndersenBase* ab = new AndersenBase(pag);
    ab->solveAndwritePtsToFile(filename);
}

void AndersenBaseReadPtsFromFile(const std::string& filename) {
    AndersenBase* ab = new AndersenBase(pag);
    ab->readPtsFromFile(filename);
}

void AndersenBaseSolveConstraints() {
    AndersenBase* ab = new AndersenBase(pag);
    ab->solveConstraints();
}

void AndersenBaseInitialize() {
    AndersenBase* ab = new AndersenBase(pag);
    ab->initialize();
}

void AndersenBaseFinalize() {
    AndersenBase* ab = new AndersenBase(pag);
    ab->finalize();
}

void AndersenBaseNormalizePointsTo() {
    AndersenBase* ab = new AndersenBase(pag);
    ab->normalizePointsTo();
}

void AndersenBaseCleanConsCG(NodeID id) {
    AndersenBase* ab = new AndersenBase(pag);
    ab->cleanConsCG(id);
}

// Andersen

/// Initialize analysis
void AndersenInitialize() {
    Andersen* ander = new Andersen(pag);
    ander->initialize();
}

/// Finalize analysis
void AndersenFinalize() {
    Andersen* ander = new Andersen(pag);
    ander->finalize();
}

/// Reset data
void AndersenResetData() {
    Andersen* ander = new Andersen(pag);
    ander->resetData();
}


NodeID AndersenSccRepNode(NodeID id) {
    Andersen* ander = new Andersen(pag);
    return ander->sccRepNode(id);
}
NodeBS& AndersenSccSubNodes(NodeID repId) {
    Andersen* ander = new Andersen(pag);
    return ander->sccSubNodes(repId);
}
//@}

/// Operation of points-to set
const PointsTo& AndersenGetPts(NodeID id) {
    Andersen* ander = new Andersen(pag);
    return ander->getPts(id);
}

bool AndersenUnionPts(NodeID id, const PointsTo& target)
{
    Andersen* ander = new Andersen(pag);
    return ander->unionPts(id, target);
}
bool AndersenUnionPts(NodeID id, NodeID ptd)
{
    Andersen* ander = new Andersen(pag);
    return ander->unionPts(id, ptd);
}


void AndersenDumpTopLevelPtsTo() {
    Andersen* ander = new Andersen(pag);
    ander->dumpTopLevelPtsTo();
}

void AndersenSetDetectPWC(bool flag)
{
    Andersen* ander = new Andersen(pag);
    ander->setDetectPWC(flag);
}


// AndersenWaveDiff

/// Create an singleton instance directly instead of invoking llvm pass manager

// Created already.....
// static AndersenWaveDiff* createAndersenWaveDiff(SVFIR* _pag)
// {
//     if(diffWave==nullptr)
//     {
//         diffWave = new AndersenWaveDiff(_pag, AndersenWaveDiff_WPA, false);
//         diffWave->analyze();
//         return diffWave;
//     }
//     return diffWave;
// }

// static void releaseAndersenWaveDiff()
// {
//     if (diffWave)
//         delete diffWave;
//     diffWave = nullptr;
// }
// Created already.....

void AndersenWaveDiffInitialize() {
    AndersenWaveDiff* diffWave = new AndersenWaveDiff(pag);
    diffWave->initialize();
}
void AndersenWaveDiffSolveWorklist() {
    AndersenWaveDiff* diffWave = new AndersenWaveDiff(pag);
    diffWave->solveWorklist();
}
void AndersenWaveDiffProcessNode(NodeID nodeId) {
    AndersenWaveDiff* diffWave = new AndersenWaveDiff(pag);
    diffWave->processNode(nodeId);
}
void AndersenWaveDiffPostProcessNode(NodeID nodeId){
    AndersenWaveDiff* diffWave = new AndersenWaveDiff(pag);
    diffWave->postProcessNode(nodeId);
}
bool AndersenWaveDiffHandleLoad(NodeID id, const ConstraintEdge* load) {
    AndersenWaveDiff* diffWave = new AndersenWaveDiff(pag);
    return diffWave->handleLoad(id, load);
}
bool AndersenWaveDiffHandleStore(NodeID id, const ConstraintEdge* store) {
    AndersenWaveDiff* diffWave = new AndersenWaveDiff(pag);
    return diffWave->handleStore(id, store);
}

// --------------------------------------------------------------------------------------------------------------

