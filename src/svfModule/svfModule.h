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

#include "SVFIR/SVFIR.h"
#include "SVF-LLVM/BasicTypes.h"
#include "SVF-LLVM/ICFGBuilder.h"
#include "SVF-LLVM/LLVMModule.h"
#include "SVF-LLVM/LLVMUtil.h"

#include "Util/SVFUtil.h"
#include "SVF-LLVM/BasicTypes.h"
#include "SVFIR/SVFValue.h"
#include "Util/ThreadAPI.h"


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
// WPA/Andersen.h........ Binding by FUNCTION.... TRYING - SEEMS TO BE WORKING, FURTHER TESTING NEEDED

// AndersenBase

void AndersenBaseAnalyze();

void AndersenBaseSolveAndwritePtsToFile(const std::string& filename);

void AndersenBaseReadPtsFromFile(const std::string& filename);

void AndersenBaseSolveConstraints();
void AndersenBaseInitialize();

void AndersenBaseFinalize();

void AndersenBaseNormalizePointsTo();

void AndersenBaseCleanConsCG(NodeID id);
// --------------------------------------------------------------------------------------------------------------

// Andersen

void AndersenInitialize();

void AndersenFinalize();

void AndersenResetData();

NodeID AndersenSccRepNode(NodeID id);
NodeBS& AndersenSccSubNodes(NodeID repId);
const PointsTo& AndersenGetPts(NodeID id);

bool AndersenUnionPts(NodeID id, const PointsTo& target);
bool AndersenUnionPts(NodeID id, NodeID ptd);

void AndersenDumpTopLevelPtsTo();

void AndersenSetDetectPWC(bool flag);


// AndersenWaveDiff
void AndersenWaveDiffInitialize();
void AndersenWaveDiffSolveWorklist();
void AndersenWaveDiffProcessNode(NodeID nodeId);
void AndersenWaveDiffPostProcessNode(NodeID nodeId);
bool AndersenWaveDiffHandleLoad(NodeID id, const ConstraintEdge* load);
bool AndersenWaveDiffHandleStore(NodeID id, const ConstraintEdge* store);

// --------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------
// SVF-LLVM/SVFIRBuilder.h"
SVFIR* SVFIRPAGBuild();

SVFIR* SVFIRGetPAG();

void SVFIRInitialiseNodes();

void SVFIRAddEdge(NodeID src, NodeID dst, SVFStmt::PEDGEK kind,
                 APOffset offset, Instruction* cs);

void SVFIRSanityCheck();

NodeID SVFIRGetValueNode(const Value* V);


NodeID SVFIRGetObjectNode(const Value* V);

NodeID SVFIRGetReturnNode(const SVFFunction *func);

NodeID SVFIRGetVarargNode(const SVFFunction *func);

void SVFIRVisitAllocaInst(AllocaInst &AI);

void SVFIRVisitPHINode(PHINode &I);

void SVFIRVisitStoreInst(StoreInst &I);

void SVFIRVisitLoadInst(LoadInst &I);

void SVFIRVisitGetElementPtrInst(GetElementPtrInst &I);


void SVFIRVisitCallInst(CallInst &I);
void SVFIRVisitInvokeInst(InvokeInst &II);

void SVFIRVisitCallBrInst(CallBrInst &I);

void SVFIRVisitCallSite(CallBase* cs);

void SVFIRVisitReturnInst(ReturnInst &I);
void SVFIRVisitCastInst(CastInst &I);

void SVFIRVisitSelectInst(SelectInst &I);

void SVFIRVisitExtractValueInst(ExtractValueInst &EVI);

void SVFIRVisitBranchInst(BranchInst &I);
void SVFIRVisitSwitchInst(SwitchInst &I);

void SVFIRVisitInsertValueInst(InsertValueInst &I);

void SVFIRVisitBinaryOperator(BinaryOperator &I);

void SVFIRVisitUnaryOperator(UnaryOperator &I);
void SVFIRVisitCmpInst(CmpInst &I);

void SVFIRVisitVAArgInst(VAArgInst&);
void SVFIRVisitVACopyInst(VACopyInst&);
void SVFIRVisitVAEndInst(VAEndInst&);
void SVFIRVisitVAStartInst(VAStartInst&);

void SVFIRVisitFreezeInst(FreezeInst& I);

void SVFIRVisitExtractElementInst(ExtractElementInst &I);

void SVFIRVisitInsertElementInst(InsertElementInst &I);
void SVFIRVisitShuffleVectorInst(ShuffleVectorInst &I);
void SVFIRVisitLandingPadInst(LandingPadInst &I);
void SVFIRVisitResumeInst(ResumeInst&);
void SVFIRVisitUnreachableInst(UnreachableInst&);
void SVFIRVisitFenceInst(FenceInst &I);
void SVFIRVisitAtomicCmpXchgInst(AtomicCmpXchgInst &I);
void SVFIRVisitAtomicRMWInst(AtomicRMWInst &I);
void SVFIRVisitInstruction(Instruction&);
void SVFIRUpdateCallGraph(PTACallGraph* callgraph);
// --------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------
// SVF-LLVM/LLVMUtil.h"

/// Whether an instruction is a call or invoke instruction
bool LLVMUtilIsCallSite(const Instruction* inst);
/// Whether an instruction is a call or invoke instruction
bool LLVMUtilIsCallSite(const Value* val);

/// Get the definition of a function across multiple modules
const Function* LLVMUtilGetDefFunForMultipleModule(const Function* fun);

/// Return LLVM callsite given a value
const CallBase* LLVMUtilGetLLVMCallSite(const Value* value);

const Function* LLVMUtilGetCallee(const CallBase* cs);

/// Return LLVM function if this value is
const Function* LLVMUtilGetLLVMFunction(const Value* val);

/// Get program entry function from module.
const Function* LLVMUtilGetProgFunction(const std::string& funName);

/// Check whether a function is an entry function (i.e., main)
bool LLVMUtilIsProgEntryFunction(const Function* fun);
/// Check whether this value is a black hole
bool LLVMUtilIsBlackholeSym(const Value* val);

/// Check whether this value is a black hole
bool LLVMUtilIsNullPtrSym(const Value* val);
Type* LLVMUtilGetPtrElementType(const PointerType* pty);

/// Return size of this object based on LLVM value
u32_t LLVMUtilGetNumOfElements(const Type* ety);


/// Return true if this value refers to a object
bool LLVMUtilIsObject(const Value* ref);

/// Method for dead function, which does not have any possible caller
/// function address is not taken and never be used in call or invoke instruction
//@{
/// whether this is a function without any possible caller?
bool LLVMUtilIsUncalledFunction(const Function* fun);

/// whether this is an argument in dead function
bool LLVMUtilArgInDeadFunction(const Value* val);
//@}

/// Return true if this is an argument of a program entry function (e.g. main)
bool LLVMUtilArgInProgEntryFunction(const Value* val);
/// Return true if this is value in a dead function (function without any caller)
bool LLVMUtilIsPtrInUncalledFunction(const Value* value);
//@}

//@}

/// Function does not have any possible caller in the call graph
//@{
/// Return true if the function does not have a caller (either it is a main function or a dead function)
bool LLVMUtilIsNoCallerFunction(const Function* fun);

/// Return true if the argument in a function does not have a caller
bool LLVMUtilIsArgOfUncalledFunction (const Value*  val);

/// Return true if the function has a return instruction
bool LLVMUtilBasicBlockHasRetInst(const BasicBlock* bb);

/// Return true if the function has a return instruction reachable from function
/// entry
bool LLVMUtilFunctionDoesNotRet(const Function* fun);

/// Get reachable basic block from function entry
void LLVMUtilGetFunReachableBBs(const Function* svfFun,
                        std::vector<const SVFBasicBlock*>& bbs);

/// Strip off the constant casts
const Value* LLVMUtilStripConstantCasts(const Value* val);
/// Strip off the all casts
const Value* LLVMUtilStripAllCasts(const Value* val);

/// Return the bitcast instruction right next to val, otherwise
/// return nullptr
const Value* LLVMUtilGetFirstUseViaCastInst(const Value* val);
/// Return corresponding constant expression, otherwise return nullptr
//@{
const ConstantExpr* LLVMUtilIsGepConstantExpr(const Value* val);

const ConstantExpr* LLVMUtilIsInt2PtrConstantExpr(const Value* val);

const ConstantExpr* LLVMUtilIsPtr2IntConstantExpr(const Value* val);

const ConstantExpr* LLVMUtilIsCastConstantExpr(const Value* val);

const ConstantExpr* LLVMUtilIsSelectConstantExpr(const Value* val);

const ConstantExpr* LLVMUtilIsTruncConstantExpr(const Value* val);
const ConstantExpr* LLVMUtilIsCmpConstantExpr(const Value* val);
const ConstantExpr* LLVMUtilIsBinaryConstantExpr(const Value* val);
const ConstantExpr* LLVMUtilIsUnaryConstantExpr(const Value* val);
DataLayout* LLVMUtilGetDataLayout(Module* mod);

/// Get the next instructions following control flow
void LLVMUtilGetNextInsts(const Instruction* curInst,
                  std::vector<const SVFInstruction*>& instList);

/// Get the previous instructions following control flow
void LLVMUtilGetPrevInsts(const Instruction* curInst,
                  std::vector<const SVFInstruction*>& instList);

/// Get the next instructions following control flow
void LLVMUtilGetNextInsts(const Instruction* curInst,
                  std::vector<const Instruction*>& instList);

/// Get the previous instructions following control flow
void LLVMUtilGetPrevInsts(const Instruction* curInst,
                  std::vector<const Instruction*>& instList);

/// Get num of BB's predecessors
u32_t LLVMUtilGetBBPredecessorNum(const BasicBlock* BB);

/// Check whether a file is an LLVM IR file
bool LLVMUtilIsIRFile(const std::string& filename);

/// Parse argument for multi-module analysis
void LLVMUtilProcessArguments(int argc, char** argv, int& arg_num, char** arg_value,
                      std::vector<std::string>& moduleNameVec);

/// Helper method to get the size of the type from target data layout
//@{
u32_t LLVMUtilGetTypeSizeInBytes(const Type* type);
u32_t LLVMUtilGetTypeSizeInBytes(const StructType* sty, u32_t field_index);

const std::string LLVMUtilGetSourceLoc(const Value* val);
const std::string LLVMUtilGetSourceLocOfFunction(const Function* F);
bool LLVMUtilIsIntrinsicInst(const Instruction* inst);
bool LLVMUtilIsIntrinsicFun(const Function* func);

/// Get all called funcions in a parent function
std::vector<const Function *> LLVMUtilGetCalledFunctions(const Function *F);
void LLVMUtilRemoveFunAnnotations(Set<Function*>& removedFuncList);
bool LLVMUtilIsUnusedGlobalVariable(const GlobalVariable& global);
void LLVMUtilRemoveUnusedGlobalVariables(Module* module);
/// Delete unused functions, annotations and global variables in extapi.bc
void LLVMUtilRemoveUnusedFuncsAndAnnotationsAndGlobalVariables(Set<Function*> removedFuncList);

/// Get the corresponding Function based on its name
const SVFFunction* LLVMUtilGetFunction(const std::string& name);

/// Return true if the value refers to constant data, e.g., i32 0
bool LLVMUtilIsConstDataOrAggData(const Value* val);

/// find the unique defined global across multiple modules
const Value* LLVMUtilGetGlobalRep(const Value* val);

/// Check whether this value points-to a constant object
bool LLVMUtilIsConstantObjSym(const SVFValue* val);

/// Check whether this value points-to a constant object
bool LLVMUtilIsConstantObjSym(const Value* val);
// Dump Control Flow Graph of llvm function, with instructions
void LLVMUtilViewCFG(const Function* fun);

// Dump Control Flow Graph of llvm function, without instructions
void LLVMUtilViewCFGOnly(const Function* fun);

std::string LLVMUtilDumpValue(const Value* val);
std::string LLVMUtilDumpType(const Type* type);

std::string LLVMUtilDumpValueAndDbgInfo(const Value* val);
void LLVMUtilGetSuccBBandCondValPairVec(const SwitchInst &switchInst, SuccBBAndCondValPairVec &vec);
s64_t LLVMUtilGetCaseValue(const SwitchInst &switchInst, SuccBBAndCondValPair &succBB2CondVal);
// --------------------------------------------------------------------------------------------------------------