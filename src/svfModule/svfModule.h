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

/// Start building SVFIR here
virtual SVFIR* build();

/// Return SVFIR
SVFIR* getPAG() const
{
    return pag;
}

/// Initialize nodes and edges
//@{
void initialiseNodes();
void addEdge(NodeID src, NodeID dst, SVFStmt::PEDGEK kind,
                APOffset offset = 0, Instruction* cs = nullptr);
// @}

/// Sanity check for SVFIR
void sanityCheck();

/// Get different kinds of node
//@{
// GetValNode - Return the value node according to a LLVM Value.
NodeID getValueNode(const Value* V)
{
    // first handle gep edge if val if a constant expression
    processCE(V);

    // strip off the constant cast and return the value node
    SVFValue* svfVal = LLVMModuleSet::getLLVMModuleSet()->getSVFValue(V);
    return pag->getValueNode(svfVal);
}

/// GetObject - Return the object node (stack/global/heap/function) according to a LLVM Value
inline NodeID getObjectNode(const Value* V)
{
    SVFValue* svfVal = LLVMModuleSet::getLLVMModuleSet()->getSVFValue(V);
    return pag->getObjectNode(svfVal);
}

/// getReturnNode - Return the node representing the unique return value of a function.
inline NodeID getReturnNode(const SVFFunction *func)
{
    return pag->getReturnNode(func);
}

/// getVarargNode - Return the node representing the unique variadic argument of a function.
inline NodeID getVarargNode(const SVFFunction *func)
{
    return pag->getVarargNode(func);
}
//@}


/// Our visit overrides.
//@{
// Instructions that cannot be folded away.
virtual void visitAllocaInst(AllocaInst &AI);
void visitPHINode(PHINode &I);
void visitStoreInst(StoreInst &I);
void visitLoadInst(LoadInst &I);
void visitGetElementPtrInst(GetElementPtrInst &I);
void visitCallInst(CallInst &I);
void visitInvokeInst(InvokeInst &II);
void visitCallBrInst(CallBrInst &I);
void visitCallSite(CallBase* cs);
void visitReturnInst(ReturnInst &I);
void visitCastInst(CastInst &I);
void visitSelectInst(SelectInst &I);
void visitExtractValueInst(ExtractValueInst  &EVI);
void visitBranchInst(BranchInst &I);
void visitSwitchInst(SwitchInst &I);
void visitInsertValueInst(InsertValueInst &I)
{
    addBlackHoleAddrEdge(getValueNode(&I));
}
// TerminatorInst and UnwindInst have been removed since llvm-8.0.0
// void visitTerminatorInst(TerminatorInst &TI) {}
// void visitUnwindInst(UnwindInst &I) { /*returns void*/}

void visitBinaryOperator(BinaryOperator &I);
void visitUnaryOperator(UnaryOperator &I);
void visitCmpInst(CmpInst &I);

/// TODO: var arguments need to be handled.
/// https://llvm.org/docs/LangRef.html#id1911
void visitVAArgInst(VAArgInst&);
void visitVACopyInst(VACopyInst&) {}
void visitVAEndInst(VAEndInst&) {}
void visitVAStartInst(VAStartInst&) {}

/// <result> = freeze ty <val>
/// If <val> is undef or poison, ‘freeze’ returns an arbitrary, but fixed value of type `ty`
/// Otherwise, this instruction is a no-op and returns the input <val>
void visitFreezeInst(FreezeInst& I);

void visitExtractElementInst(ExtractElementInst &I);

void visitInsertElementInst(InsertElementInst &I)
{
    addBlackHoleAddrEdge(getValueNode(&I));
}
void visitShuffleVectorInst(ShuffleVectorInst &I)
{
    addBlackHoleAddrEdge(getValueNode(&I));
}
void visitLandingPadInst(LandingPadInst &I)
{
    addBlackHoleAddrEdge(getValueNode(&I));
}

/// Instruction not that often
void visitResumeInst(ResumeInst&)   /*returns void*/
{
}
void visitUnreachableInst(UnreachableInst&)   /*returns void*/
{
}
void visitFenceInst(FenceInst &I)   /*returns void*/
{
    addBlackHoleAddrEdge(getValueNode(&I));
}
void visitAtomicCmpXchgInst(AtomicCmpXchgInst &I)
{
    addBlackHoleAddrEdge(getValueNode(&I));
}
void visitAtomicRMWInst(AtomicRMWInst &I)
{
    addBlackHoleAddrEdge(getValueNode(&I));
}

/// Provide base case for our instruction visit.
inline void visitInstruction(Instruction&)
{
    // If a new instruction is added to LLVM that we don't handle.
    // TODO: ignore here:
}
//}@

/// connect PAG edges based on callgraph
void updateCallGraph(PTACallGraph* callgraph);


// --------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------
// SVF-LLVM/SVFIRBuilder.h"
SVFIR* SVFIRPAGBuild();

SVFIR* SVFIRGetPAG();

void SVFIRInitialiseNodes();

void SVFIRAddEdge(NodeID src, NodeID dst, SVFStmt::PEDGEK kind,
                 APOffset offset = 0, Instruction* cs = nullptr);

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