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

#include "SVFIR/SVFIR.h"
#include "SVF-LLVM/BasicTypes.h"
#include "SVF-LLVM/ICFGBuilder.h"
#include "SVF-LLVM/LLVMModule.h"
#include "SVF-LLVM/LLVMUtil.h"


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


// --------------------------------------------------------------------------------------------------------------
// SVF-LLVM/SVFIRBuilder.h"
SVFIR* SVFIRPAGBuild() {
    SVFIRBuilder _builder(svfModule);
    builder = &_builder;
    pag = _builder.build();
    return pag;
}   

SVFIR* SVFIRGetPAG() {
    return builder->getPAG();
}

void SVFIRInitialiseNodes() {
    builder->initialiseNodes();
}

void SVFIRAddEdge(NodeID src, NodeID dst, SVFStmt::PEDGEK kind,
                 APOffset offset, Instruction* cs) {
    builder->addEdge(src, dst, kind, offset, cs);
}

void SVFIRSanityCheck() {
    builder->sanityCheck();
}

NodeID SVFIRGetValueNode(const Value* V)
{
    return builder->getValueNode(V);
}


NodeID SVFIRGetObjectNode(const Value* V)
{
    return builder->getObjectNode(V);
}

NodeID SVFIRGetReturnNode(const SVFFunction *func)
{
    return builder->getReturnNode(func);
}

NodeID SVFIRGetVarargNode(const SVFFunction *func)
{
    return builder->getVarargNode(func);
}

void SVFIRVisitAllocaInst(AllocaInst &AI) {
    builder->visitAllocaInst(AI);
}

void SVFIRVisitPHINode(PHINode &I) {
    builder->visitPHINode(I);
}

void SVFIRVisitStoreInst(StoreInst &I) {
    builder->visitStoreInst(I);
}

void SVFIRVisitLoadInst(LoadInst &I) {
    builder->visitLoadInst(I);
}

void SVFIRVisitGetElementPtrInst(GetElementPtrInst &I) {
    builder->visitGetElementPtrInst(I);
}


void SVFIRVisitCallInst(CallInst &I) {
    builder->visitCallInst(I);
}

void SVFIRVisitInvokeInst(InvokeInst &II) {
    builder->visitInvokeInst(II);
}


void SVFIRVisitCallBrInst(CallBrInst &I) {
    builder->visitCallBrInst(I);

}

void SVFIRVisitCallSite(CallBase* cs) {
    builder->visitCallSite(cs);

}

void SVFIRVisitReturnInst(ReturnInst &I) {
    builder->visitReturnInst(I);

}
void SVFIRVisitCastInst(CastInst &I) {
    builder->visitCastInst(I);
}

void SVFIRVisitSelectInst(SelectInst &I) {
    builder->visitSelectInst(I);
}

void SVFIRVisitExtractValueInst(ExtractValueInst &EVI) {
    builder->visitExtractValueInst(EVI);
}

void SVFIRVisitBranchInst(BranchInst &I) {
    builder->visitBranchInst(I);
}
void SVFIRVisitSwitchInst(SwitchInst &I) {
    builder->visitSwitchInst(I);
}

void SVFIRVisitInsertValueInst(InsertValueInst &I) {
    builder->visitInsertValueInst(I);
}

void SVFIRVisitBinaryOperator(BinaryOperator &I) {
    builder->visitBinaryOperator(I);
}

void SVFIRVisitUnaryOperator(UnaryOperator &I) {
    builder->visitUnaryOperator(I);
}
void SVFIRVisitCmpInst(CmpInst &I) {
    builder->visitCmpInst(I);
}

void SVFIRVisitVAArgInst(VAArgInst& inst) {
    builder->visitVAArgInst(inst);
}
void SVFIRVisitVACopyInst(VACopyInst& inst) {
    builder->visitVACopyInst(inst);
}
void SVFIRVisitVAEndInst(VAEndInst& inst) {
    builder->visitVAEndInst(inst);
}
void SVFIRVisitVAStartInst(VAStartInst& inst) {
    builder->visitVAStartInst(inst);
}

void SVFIRVisitFreezeInst(FreezeInst& I) {
    builder->visitFreezeInst(I);
}

void SVFIRVisitExtractElementInst(ExtractElementInst &I) {
    builder->visitExtractElementInst(I);
}

void SVFIRVisitInsertElementInst(InsertElementInst &I)
{
    builder->visitInsertElementInst(I);
}
void SVFIRVisitShuffleVectorInst(ShuffleVectorInst &I)
{
    builder->visitShuffleVectorInst(I);
}
void SVFIRVisitLandingPadInst(LandingPadInst &I)
{
    builder->visitLandingPadInst(I);
}

void SVFIRVisitResumeInst(ResumeInst& inst) {
    builder->visitResumeInst(inst);
}
void SVFIRVisitUnreachableInst(UnreachableInst& inst) {
    builder->visitUnreachableInst(inst);
}

void SVFIRVisitFenceInst(FenceInst &I){
    builder->visitFenceInst(I);
}
void SVFIRVisitAtomicCmpXchgInst(AtomicCmpXchgInst &I) {
    builder->visitAtomicCmpXchgInst(I);
}
void SVFIRVisitAtomicRMWInst(AtomicRMWInst &I) {
    builder->visitAtomicRMWInst(I);
}

/// Provide base case for our instruction visit.
void SVFIRVisitInstruction(Instruction& inst) {
    builder->visitInstruction(inst);
}
void SVFIRUpdateCallGraph(PTACallGraph* callgraph) {
    builder->updateCallGraph(callgraph);
}
// --------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------
// SVF-LLVM/LLVMUtil.h"

/// Whether an instruction is a call or invoke instruction
bool LLVMUtilIsCallSite(const Instruction* inst)
{
    return LLVMUtil::isCallSite(inst);
}
/// Whether an instruction is a call or invoke instruction
bool LLVMUtilIsCallSite(const Value* val)
{
    return LLVMUtil::isCallSite(val);
}

/// Get the definition of a function across multiple modules
const Function* LLVMUtilGetDefFunForMultipleModule(const Function* fun) {
    return LLVMUtil::getDefFunForMultipleModule(fun);
};

/// Return LLVM callsite given a value
const CallBase* LLVMUtilGetLLVMCallSite(const Value* value)
{
    return LLVMUtil::getLLVMCallSite(value);
}

const Function* LLVMUtilGetCallee(const CallBase* cs)
{
    return LLVMUtil::getCallee(cs);
}

/// Return LLVM function if this value is
const Function* LLVMUtilGetLLVMFunction(const Value* val)
{

    return LLVMUtil::getLLVMFunction(val);
}

/// Get program entry function from module.
const Function* LLVMUtilGetProgFunction(const std::string& funName) {
    return LLVMUtil::getProgFunction(funName);
}

/// Check whether a function is an entry function (i.e., main)
bool LLVMUtilIsProgEntryFunction(const Function* fun)
{
    return LLVMUtil::isProgEntryFunction(fun);
}

/// Check whether this value is a black hole
bool LLVMUtilIsBlackholeSym(const Value* val)
{
    return LLVMUtil::isBlackholeSym(val);
}

/// Check whether this value is a black hole
bool LLVMUtilIsNullPtrSym(const Value* val)
{
    return LLVMUtil::isNullPtrSym(val);
}

Type* LLVMUtilGetPtrElementType(const PointerType* pty)
{
    return LLVMUtil::getPtrElementType(pty);
}

/// Return size of this object based on LLVM value
u32_t LLVMUtilGetNumOfElements(const Type* ety) {
    return LLVMUtil::getNumOfElements(ety);

}


/// Return true if this value refers to a object
bool LLVMUtilIsObject(const Value* ref) {
    return LLVMUtil::isObject(ref);
}

/// Method for dead function, which does not have any possible caller
/// function address is not taken and never be used in call or invoke instruction
//@{
/// whether this is a function without any possible caller?
bool LLVMUtilIsUncalledFunction(const Function* fun) {
    return LLVMUtil::isUncalledFunction(fun);

}

/// whether this is an argument in dead function
bool LLVMUtilArgInDeadFunction(const Value* val)
{
    return LLVMUtil::ArgInDeadFunction(val);
}
//@}

/// Return true if this is an argument of a program entry function (e.g. main)
bool LLVMUtilArgInProgEntryFunction(const Value* val)
{
    return LLVMUtil::ArgInProgEntryFunction(val);
}
/// Return true if this is value in a dead function (function without any caller)
bool LLVMUtilIsPtrInUncalledFunction(const Value* value) {
    return LLVMUtil::isPtrInUncalledFunction(value);
}
//@}

//@}

/// Function does not have any possible caller in the call graph
//@{
/// Return true if the function does not have a caller (either it is a main function or a dead function)
bool LLVMUtilIsNoCallerFunction(const Function* fun)
{
    return LLVMUtil::isNoCallerFunction(fun);
}

/// Return true if the argument in a function does not have a caller
bool LLVMUtilIsArgOfUncalledFunction (const Value*  val)
{
    return LLVMUtil::isArgOfUncalledFunction(val);
}
//@}

/// Return true if the function has a return instruction
bool LLVMUtilBasicBlockHasRetInst(const BasicBlock* bb) {
    return LLVMUtil::basicBlockHasRetInst(bb);
}

/// Return true if the function has a return instruction reachable from function
/// entry
bool LLVMUtilFunctionDoesNotRet(const Function* fun) {
    return LLVMUtil::functionDoesNotRet(fun);

}

/// Get reachable basic block from function entry
void LLVMUtilGetFunReachableBBs(const Function* svfFun,
                        std::vector<const SVFBasicBlock*>& bbs) {
    return LLVMUtil::getFunReachableBBs(svfFun, bbs);
}

/// Strip off the constant casts
const Value* LLVMUtilStripConstantCasts(const Value* val) {
    return LLVMUtil::stripConstantCasts(val);
}

/// Strip off the all casts
const Value* LLVMUtilStripAllCasts(const Value* val) {
    return LLVMUtil::stripAllCasts(val);
}

/// Return the bitcast instruction right next to val, otherwise
/// return nullptr
const Value* LLVMUtilGetFirstUseViaCastInst(const Value* val) {
    return LLVMUtil::getFirstUseViaCastInst(val);
}

/// Return corresponding constant expression, otherwise return nullptr
//@{
const ConstantExpr* LLVMUtilIsGepConstantExpr(const Value* val)
{
    return LLVMUtil::isGepConstantExpr(val);
}

const ConstantExpr* LLVMUtilIsInt2PtrConstantExpr(const Value* val)
{
    return LLVMUtil::isInt2PtrConstantExpr(val);
}

const ConstantExpr* LLVMUtilIsPtr2IntConstantExpr(const Value* val)
{
    return LLVMUtil::isPtr2IntConstantExpr(val);
}

const ConstantExpr* LLVMUtilIsCastConstantExpr(const Value* val)
{
    return LLVMUtil::isCastConstantExpr(val);
}

const ConstantExpr* LLVMUtilIsSelectConstantExpr(const Value* val)
{
    return LLVMUtil::isSelectConstantExpr(val);
}

const ConstantExpr* LLVMUtilIsTruncConstantExpr(const Value* val)
{
    return LLVMUtil::isTruncConstantExpr(val);
}

const ConstantExpr* LLVMUtilIsCmpConstantExpr(const Value* val)
{
    return LLVMUtil::isCmpConstantExpr(val);
}

const ConstantExpr* LLVMUtilIsBinaryConstantExpr(const Value* val)
{
    return LLVMUtil::isBinaryConstantExpr(val);
}

const ConstantExpr* LLVMUtilIsUnaryConstantExpr(const Value* val)
{
    return LLVMUtil::isUnaryConstantExpr(val);
}
//@}

DataLayout* LLVMUtilGetDataLayout(Module* mod)
{
    return LLVMUtil::getDataLayout(mod);
}

/// Get the next instructions following control flow
void LLVMUtilGetNextInsts(const Instruction* curInst,
                  std::vector<const SVFInstruction*>& instList) {
    return LLVMUtil::getNextInsts(curInst, instList);
}

/// Get the previous instructions following control flow
void LLVMUtilGetPrevInsts(const Instruction* curInst,
                  std::vector<const SVFInstruction*>& instList) {
    return LLVMUtil::getPrevInsts(curInst, instList);
}

/// Get the next instructions following control flow
void LLVMUtilGetNextInsts(const Instruction* curInst,
                  std::vector<const Instruction*>& instList) {
    return LLVMUtil::getNextInsts(curInst, instList);
}

/// Get the previous instructions following control flow
void LLVMUtilGetPrevInsts(const Instruction* curInst,
                  std::vector<const Instruction*>& instList) {
    return LLVMUtil::getPrevInsts(curInst, instList);
}

/// Get num of BB's predecessors
u32_t LLVMUtilGetBBPredecessorNum(const BasicBlock* BB) {
    return LLVMUtil::getBBPredecessorNum(BB);
}

/// Check whether a file is an LLVM IR file
bool LLVMUtilIsIRFile(const std::string& filename) {
    return LLVMUtil::isIRFile(filename);

}

/// Parse argument for multi-module analysis
void LLVMUtilProcessArguments(int argc, char** argv, int& arg_num, char** arg_value,
                      std::vector<std::string>& moduleNameVec) {
    return LLVMUtil::processArguments(argc, argv, arg_num, arg_value, moduleNameVec);
}

/// Helper method to get the size of the type from target data layout
//@{
u32_t LLVMUtilGetTypeSizeInBytes(const Type* type) {
    return LLVMUtil::getTypeSizeInBytes(type);
}
u32_t LLVMUtilGetTypeSizeInBytes(const StructType* sty, u32_t field_index) {
    return LLVMUtil::getTypeSizeInBytes(sty, field_index);
}
//@}

const std::string LLVMUtilGetSourceLoc(const Value* val) {
    return LLVMUtil::getSourceLoc(val);
}
const std::string LLVMUtilGetSourceLocOfFunction(const Function* F) {
    return LLVMUtil::getSourceLocOfFunction(F);
}

bool LLVMUtilIsIntrinsicInst(const Instruction* inst) {
    return LLVMUtil::isIntrinsicInst(inst);

}
bool LLVMUtilIsIntrinsicFun(const Function* func) {
    return LLVMUtil::isIntrinsicFun(func);
}

/// Get all called funcions in a parent function
std::vector<const Function *> LLVMUtilGetCalledFunctions(const Function *F) {
    return LLVMUtil::getCalledFunctions(F);
}
void LLVMUtilRemoveFunAnnotations(Set<Function*>& removedFuncList) {
    return LLVMUtil::removeFunAnnotations(removedFuncList);
}
bool LLVMUtilIsUnusedGlobalVariable(const GlobalVariable& global) {
    return LLVMUtil::isUnusedGlobalVariable(global);
}
void LLVMUtilRemoveUnusedGlobalVariables(Module* module) {
    return LLVMUtil::removeUnusedGlobalVariables(module);
}
/// Delete unused functions, annotations and global variables in extapi.bc
void LLVMUtilRemoveUnusedFuncsAndAnnotationsAndGlobalVariables(Set<Function*> removedFuncList) {
    return LLVMUtil::removeUnusedFuncsAndAnnotationsAndGlobalVariables(removedFuncList);
}

/// Get the corresponding Function based on its name
const SVFFunction* LLVMUtilGetFunction(const std::string& name) {
    return LLVMUtil::getFunction(name);
}

/// Return true if the value refers to constant data, e.g., i32 0
bool LLVMUtilIsConstDataOrAggData(const Value* val)
{
    return LLVMUtil::isConstDataOrAggData(val);
}

/// find the unique defined global across multiple modules
const Value* LLVMUtilGetGlobalRep(const Value* val) {
    return LLVMUtil::getGlobalRep(val);
}

/// Check whether this value points-to a constant object
bool LLVMUtilIsConstantObjSym(const SVFValue* val) {
    return LLVMUtil::isConstantObjSym(val);
}

/// Check whether this value points-to a constant object
bool LLVMUtilIsConstantObjSym(const Value* val) {
    return LLVMUtil::isConstantObjSym(val);
}

// Dump Control Flow Graph of llvm function, with instructions
void LLVMUtilViewCFG(const Function* fun) {
    return LLVMUtil::viewCFG(fun);
}

// Dump Control Flow Graph of llvm function, without instructions
void LLVMUtilViewCFGOnly(const Function* fun) {
    return LLVMUtil::viewCFGOnly(fun);

}

std::string LLVMUtilDumpValue(const Value* val) {
    return LLVMUtil::dumpValue(val);
}

std::string LLVMUtilDumpType(const Type* type) {
    return LLVMUtil::dumpType(type);
}

std::string LLVMUtilDumpValueAndDbgInfo(const Value* val) {
    return LLVMUtil::dumpValueAndDbgInfo(val);
}

void LLVMUtilGetSuccBBandCondValPairVec(const SwitchInst &switchInst, SuccBBAndCondValPairVec &vec) {
    return LLVMUtil::getSuccBBandCondValPairVec(switchInst, vec);
}

/**
 * Note: default case value is nullptr
 */
s64_t LLVMUtilGetCaseValue(const SwitchInst &switchInst, SuccBBAndCondValPair &succBB2CondVal) {
    return LLVMUtil::getCaseValue(switchInst, succBB2CondVal);
}


// --------------------------------------------------------------------------------------------------------------