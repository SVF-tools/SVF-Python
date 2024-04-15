#include <pybind11/pybind11.h>
#include "svfModule.h"

namespace py = pybind11;

PYBIND11_MODULE(svfModule_pybind, m) {
    // Old work.......................
    // --------------------------------------------------------------------------------

    m.def("processArguments", &processArguments, "Func 1");

    // --------------------------------------------------------------------------------
    // PASSED

    m.def("getModuleNameVecLen", &getModuleNameVecLen, "Func 2");
    m.def("getModuleNameVecItem", &getModuleNameVecItem, "Func 3");
    m.def("ParseCommandLineOptions", &ParseCommandLineOptions, "Func 4");

    m.def("buildSVFModule", &buildSVFModule, "Func 5");
    m.def("setModuleNameVec", &setModuleNameVec, "Func 6");
    
    // // m.def("buildSymbolTableInfo", &buildSymbolTableInfo, "Func 7"); // Not Defined in cpp
    
    m.def("pagBuild", &pagBuild, "Func 8");
    m.def("createAndersenWaveDiff", &createAndersenWaveDiff, "Func 9");
    m.def("getPTACallGraph", &getPTACallGraph, "Func 10");
    m.def("getICFG", &getICFG, "Func 11");

    m.def("VFGNewInstances", &VFGNewInstances, "Func 12");

    // functioning part?
    m.def("buildFullSVFG", &buildFullSVFG, "Func 13");
    m.def("deleteSvfg", &deleteSvfg, "Func 14");
    m.def("deleteVfg", &deleteVfg, "Func 15");
    // 

    m.def("releaseAndersenWaveDiff", &releaseAndersenWaveDiff, "Func 16");
    m.def("releaseSVFIR", &releaseSVFIR, "Func 17");
    m.def("dumpModulesToFile", &dumpModulesToFile, "Func 18");
    m.def("releaseLLVMModuleSet", &releaseLLVMModuleSet, "Func 19");
    m.def("llvm_shutdown", &llvm_shutdown, "Func 20");
    // // --------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------

    // New work.......................
    // --------------------------------------------------------------------------------
    // ae.cpp

    m.def("updateCallGraph", &updateCallGraph, "Func 21");
    m.def("getICFGUpdateCallGraph", &getICFGUpdateCallGraph, "Func 22");
    m.def("boolICFGMergeAdjacentNodes", &boolICFGMergeAdjacentNodes, "Func 23");
    m.def("mergeAdjacentNodes", &mergeAdjacentNodes, "Func 24");
    m.def("boolBufferOverflowCheck", &boolBufferOverflowCheck, "Func 25");
    m.def("bufOverflowCheckerRunOnModule", &bufOverflowCheckerRunOnModule, "Func 26");
    m.def("abstractExecutionRunOnModule", &abstractExecutionRunOnModule, "Func 27");


    // --------------------------------------------------------------------------------
    // wpa.cpp
    m.def("boolReadJson", &boolReadJson, "Func 28");
    m.def("SVFIRReaderRead", &SVFIRReaderRead, "Func 29");
    m.def("optionsWriteAnder", &optionsWriteAnder, "Func 30");
    m.def("preProcessBCs", &preProcessBCs, "Func 31");
    m.def("WPAPassRunOnModule", &WPAPassRunOnModule, "Func 32");

    // --------------------------------------------------------------------------------


    // --------------------------------------------------------------------------------
    // saber.cpp
    // m.def("LEAKCHECKER", &LEAKCHECKER, "Func 33");
    // m.def("FILECHECKER", &FILECHECKER, "Func 34");
    // m.def("DFREECHECKER", &DFREECHECKER, "Func 35");
    // m.def("saberMakeUniqueLeakChecker", &saberMakeUniqueLeakChecker, "Func 36");
    // m.def("saberMakeUniqueFileChecker", &saberMakeUniqueFileChecker, "Func 37");
    // m.def("saberMakeUniqueDoubleFreeChecker", &saberMakeUniqueDoubleFreeChecker, "Func 38");
    m.def("saberCheckerAllInOne", &saberCheckerAllInOne, "Func 33");
    m.def("saberRunOnModule", &saberRunOnModule, "Func 34");
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    // cfl.cpp
    m.def("boolCFLGraphEmpty", &boolCFLGraphEmpty, "Func 34");
    m.def("svfirBuild", &svfirBuild, "Func 35");
    m.def("cflCheckerAllInOne", &cflCheckerAllInOne, "Func 36");
    m.def("cflAnalyze", &cflAnalyze, "Func 37");
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    // dda.cpp
    m.def("ddaPassRunOnModule", &ddaPassRunOnModule, "Func 38");
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------------------------------------
    // llvm2svf.cpp........
    m.def("moduleNameVecReplaceExtension", &moduleNameVecReplaceExtension, "Func 39");
    m.def("writeJsonToPath", &writeJsonToPath, "Func 40");
    m.def("outSVFIRJsonPath", &outSVFIRJsonPath, "Func 41");
    // --------------------------------------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------------------------------------
    // mta.cpp........
    m.def("MTARunOnModule", &MTARunOnModule, "Func 42");
    m.def("MTAValidator", &MTAValidator, "Func 43");
    m.def("lockValidator", &lockValidator, "Func 44");
    // --------------------------------------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------------------------------------
    // Andersen.h........
    // AndersenBase
    m.def("AndersenBaseAnalyze", &AndersenBaseAnalyze, "Func 45");

    m.def("AndersenBaseSolveAndwritePtsToFile", &AndersenBaseSolveAndwritePtsToFile, "Func 46");
    m.def("AndersenBaseReadPtsFromFile", &AndersenBaseReadPtsFromFile, "Func 47");
    m.def("AndersenBaseSolveConstraints", &AndersenBaseSolveConstraints, "Func 48");

    m.def("AndersenBaseInitialize", &AndersenBaseInitialize, "Func 49");
    m.def("AndersenBaseFinalize", &AndersenBaseFinalize, "Func 50");
    m.def("AndersenBaseNormalizePointsTo", &AndersenBaseNormalizePointsTo, "Func 51");

    m.def("AndersenBaseCleanConsCG", &AndersenBaseCleanConsCG, "Func 52");

    // Andersen
    m.def("AndersenInitialize", &AndersenInitialize, "Func 53");

    m.def("AndersenFinalize", &AndersenFinalize, "Func 54");
    m.def("AndersenResetData", &AndersenResetData, "Func 55");
    m.def("AndersenSccRepNode", &AndersenSccRepNode, "Func 56");

    m.def("AndersenSccSubNodes", &AndersenSccSubNodes, "Func 57");
    m.def("AndersenGetPts", &AndersenGetPts, "Func 58");


    m.def("AndersenUnionPts", (bool (*)(NodeID, const PointsTo&)) &AndersenUnionPts, "Func 59");
    m.def("AndersenUnionPts", (bool (*)(NodeID, NodeID)) &AndersenUnionPts, "Func 60");

    m.def("AndersenDumpTopLevelPtsTo", &AndersenDumpTopLevelPtsTo, "Func 61");

    m.def("AndersenSetDetectPWC", &AndersenSetDetectPWC, "Func 62");

    // AndersenWaveDiff
    m.def("AndersenWaveDiffInitialize", &AndersenWaveDiffInitialize, "Func 63");

    m.def("AndersenWaveDiffSolveWorklist", &AndersenWaveDiffSolveWorklist, "Func 64");
    m.def("AndersenWaveDiffProcessNode", &AndersenWaveDiffProcessNode, "Func 65");
    m.def("AndersenWaveDiffPostProcessNode", &AndersenWaveDiffPostProcessNode, "Func 66");

    m.def("AndersenWaveDiffHandleLoad", &AndersenWaveDiffHandleLoad, "Func 67");
    m.def("AndersenWaveDiffHandleStore", &AndersenWaveDiffHandleStore, "Func 68");
    // --------------------------------------------------------------------------------------------------------------


    // --------------------------------------------------------------------------------------------------------------
    // SVF-LLVM/SVFIRBuilder.h"
    m.def("SVFIRPAGBuild", &SVFIRPAGBuild, "Func 69");

    m.def("SVFIRGetPAG", &SVFIRGetPAG, "Func 70");
    m.def("SVFIRInitialiseNodes", &SVFIRInitialiseNodes, "Func 71");

    // m.def("SVFIRAddEdge", &SVFIRAddEdge, "Func 72");
    m.def("SVFIRSanityCheck", &SVFIRSanityCheck, "Func 73");
    m.def("SVFIRGetValueNode", &SVFIRGetValueNode, "Func 74");

    m.def("SVFIRGetObjectNode", &SVFIRGetObjectNode, "Func 75");
    m.def("SVFIRGetReturnNode", &SVFIRGetReturnNode, "Func 76");

    m.def("SVFIRGetVarargNode", &SVFIRGetVarargNode, "Func 77");
    m.def("SVFIRVisitAllocaInst", &SVFIRVisitAllocaInst, "Func 78");

    m.def("SVFIRVisitPHINode", &SVFIRVisitPHINode, "Func 79");
    m.def("SVFIRVisitStoreInst", &SVFIRVisitStoreInst, "Func 80");

    m.def("SVFIRVisitLoadInst", &SVFIRVisitLoadInst, "Func 81");
    m.def("SVFIRVisitGetElementPtrInst", &SVFIRVisitGetElementPtrInst, "Func 82");
    m.def("SVFIRVisitCallInst", &SVFIRVisitCallInst, "Func 83");
    m.def("SVFIRVisitInvokeInst", &SVFIRVisitInvokeInst, "Func 84");
    m.def("SVFIRVisitCallBrInst", &SVFIRVisitCallBrInst, "Func 85");
    m.def("SVFIRVisitCallSite", &SVFIRVisitCallSite, "Func 86");
    m.def("SVFIRVisitReturnInst", &SVFIRVisitReturnInst, "Func 87");
    m.def("SVFIRVisitCastInst", &SVFIRVisitCastInst, "Func 88");
    m.def("SVFIRVisitSelectInst", &SVFIRVisitSelectInst, "Func 89");
    m.def("SVFIRVisitExtractValueInst", &SVFIRVisitExtractValueInst, "Func 90");
    m.def("SVFIRVisitBranchInst", &SVFIRVisitBranchInst, "Func 91");

    m.def("SVFIRVisitSwitchInst", &SVFIRVisitSwitchInst, "Func 92");

    m.def("SVFIRVisitInsertValueInst", &SVFIRVisitInsertValueInst, "Func 93");
    m.def("SVFIRVisitBinaryOperator", &SVFIRVisitBinaryOperator, "Func 94");
    
    m.def("SVFIRVisitUnaryOperator", &SVFIRVisitUnaryOperator, "Func 95");
    m.def("SVFIRVisitCmpInst", &SVFIRVisitCmpInst, "Func 96");
    m.def("SVFIRVisitVAArgInst", &SVFIRVisitVAArgInst, "Func 97");

    m.def("SVFIRVisitVACopyInst", &SVFIRVisitVACopyInst, "Func 98");
    m.def("SVFIRVisitVAEndInst", &SVFIRVisitVAEndInst, "Func 99");

    m.def("SVFIRVisitVAStartInst", &SVFIRVisitVAStartInst, "Func 100");
    m.def("SVFIRVisitFreezeInst", &SVFIRVisitFreezeInst, "Func 101");

    m.def("SVFIRVisitExtractElementInst", &SVFIRVisitExtractElementInst, "Func 102");
    m.def("SVFIRVisitInsertElementInst", &SVFIRVisitInsertElementInst, "Func 103");

    m.def("SVFIRVisitInsertElementInst", &SVFIRVisitInsertElementInst, "Func 104");

    m.def("SVFIRVisitShuffleVectorInst", &SVFIRVisitShuffleVectorInst, "Func 105");
    m.def("SVFIRVisitLandingPadInst", &SVFIRVisitLandingPadInst, "Func 106");
    
    m.def("SVFIRVisitResumeInst", &SVFIRVisitResumeInst, "Func 107");
    m.def("SVFIRVisitUnreachableInst", &SVFIRVisitUnreachableInst, "Func 108");
    m.def("SVFIRVisitFenceInst", &SVFIRVisitFenceInst, "Func 109");

    m.def("SVFIRVisitAtomicCmpXchgInst", &SVFIRVisitAtomicCmpXchgInst, "Func 110");
    m.def("SVFIRVisitAtomicRMWInst", &SVFIRVisitAtomicRMWInst, "Func 111");

    m.def("SVFIRVisitInstruction", &SVFIRVisitInstruction, "Func 112");
    m.def("SVFIRUpdateCallGraph", &SVFIRUpdateCallGraph, "Func 113");

    // --------------------------------------------------------------------------------------------------------------


    // --------------------------------------------------------------------------------------------------------------
    // SVF-LLVM/LLVMUtil.h"

    m.def("LLVMUtilIsCallSiteInst", (bool (*)(const Instruction*)) &LLVMUtilIsCallSite, "Func 69");
    m.def("LLVMUtilIsCallSiteVal", (bool (*)(const Value*)) &LLVMUtilIsCallSite, "Func 70");
    m.def("LLVMUtilGetDefFunForMultipleModule", &LLVMUtilGetDefFunForMultipleModule, "Func 71");

    m.def("LLVMUtilGetLLVMCallSite", &LLVMUtilGetLLVMCallSite, "Func 72");
    m.def("LLVMUtilGetCallee", &LLVMUtilGetCallee, "Func 73");
    m.def("LLVMUtilGetLLVMFunction", &LLVMUtilGetLLVMFunction, "Func 74");

    m.def("LLVMUtilGetProgFunction", &LLVMUtilGetProgFunction, "Func 75");
    m.def("LLVMUtilIsProgEntryFunction", &LLVMUtilIsProgEntryFunction, "Func 76");

    m.def("LLVMUtilIsBlackholeSym", &LLVMUtilIsBlackholeSym, "Func 77");
    m.def("LLVMUtilIsNullPtrSym", &LLVMUtilIsNullPtrSym, "Func 78");

    m.def("LLVMUtilGetPtrElementType", &LLVMUtilGetPtrElementType, "Func 79");
    m.def("LLVMUtilGetNumOfElements", &LLVMUtilGetNumOfElements, "Func 80");




    m.def("LLVMUtilIsObject", &LLVMUtilIsObject, "Func 69");
    m.def("LLVMUtilIsUncalledFunction", &LLVMUtilIsUncalledFunction, "Func 70");

    m.def("LLVMUtilArgInDeadFunction", &LLVMUtilArgInDeadFunction, "Func 71");

    m.def("LLVMUtilArgInProgEntryFunction", &LLVMUtilArgInProgEntryFunction, "Func 72");
    m.def("LLVMUtilIsPtrInUncalledFunction", &LLVMUtilIsPtrInUncalledFunction, "Func 73");
    m.def("LLVMUtilIsNoCallerFunction", &LLVMUtilIsNoCallerFunction, "Func 74");

    m.def("LLVMUtilIsArgOfUncalledFunction", &LLVMUtilIsArgOfUncalledFunction, "Func 75");
    m.def("LLVMUtilBasicBlockHasRetInst", &LLVMUtilBasicBlockHasRetInst, "Func 78");

    m.def("LLVMUtilFunctionDoesNotRet", &LLVMUtilFunctionDoesNotRet, "Func 79");
    m.def("LLVMUtilGetFunReachableBBs", &LLVMUtilGetFunReachableBBs, "Func 80");



    m.def("LLVMUtilStripConstantCasts", &LLVMUtilStripConstantCasts, "Func 71");

    m.def("LLVMUtilStripAllCasts", &LLVMUtilStripAllCasts, "Func 72");
    m.def("LLVMUtilGetFirstUseViaCastInst", &LLVMUtilGetFirstUseViaCastInst, "Func 73");
    m.def("LLVMUtilIsGepConstantExpr", &LLVMUtilIsGepConstantExpr, "Func 74");

    m.def("LLVMUtilIsInt2PtrConstantExpr", &LLVMUtilIsInt2PtrConstantExpr, "Func 75");
    m.def("LLVMUtilIsPtr2IntConstantExpr", &LLVMUtilIsPtr2IntConstantExpr, "Func 76");

    m.def("LLVMUtilIsCastConstantExpr", &LLVMUtilIsCastConstantExpr, "Func 77");
    m.def("LLVMUtilIsSelectConstantExpr", &LLVMUtilIsSelectConstantExpr, "Func 78");

    m.def("LLVMUtilIsTruncConstantExpr", &LLVMUtilIsTruncConstantExpr, "Func 79");
    m.def("LLVMUtilIsCmpConstantExpr", &LLVMUtilIsCmpConstantExpr, "Func 80");


    m.def("LLVMUtilIsBinaryConstantExpr", &LLVMUtilIsBinaryConstantExpr, "Func 75");
    m.def("LLVMUtilIsUnaryConstantExpr", &LLVMUtilIsUnaryConstantExpr, "Func 76");

    m.def("LLVMUtilGetDataLayout", &LLVMUtilGetDataLayout, "Func 77");
    m.def("LLVMUtilGetNextInstsSVF", (void (*)(const Instruction*, std::vector<const SVFInstruction*>&)) &LLVMUtilGetNextInsts);

    m.def("LLVMUtilGetPrevInstsSVF", (void (*)(const Instruction*, std::vector<const SVFInstruction*>&)) &LLVMUtilGetPrevInsts, "Func 79");
    m.def("LLVMUtilGetNextInsts", (void (*)(const Instruction*, std::vector<const Instruction*>&)) &LLVMUtilGetNextInsts);
    m.def("LLVMUtilGetPrevInsts", (void (*)(const Instruction*, std::vector<const Instruction*>&)) &LLVMUtilGetPrevInsts, "Func 77");


    m.def("LLVMUtilGetBBPredecessorNum", &LLVMUtilGetBBPredecessorNum, "Func 78");

    m.def("LLVMUtilIsIRFile", &LLVMUtilIsIRFile, "Func 79");
    // m.def("LLVMUtilProcessArguments", &LLVMUtilProcessArguments, "Func 80");


    m.def("LLVMUtilGetTypeSizeInBytes", (u32_t (*)(const Type*)) &LLVMUtilGetTypeSizeInBytes);
    m.def("LLVMUtilGetTypeSizeInBytesStructType", (u32_t (*)(const StructType*, u32_t)) &LLVMUtilGetTypeSizeInBytes);




    m.def("LLVMUtilGetSourceLoc", &LLVMUtilGetSourceLoc, "Func 77");
    m.def("LLVMUtilGetSourceLocOfFunction", &LLVMUtilGetSourceLocOfFunction, "Func 78");

    m.def("LLVMUtilIsIntrinsicInst", &LLVMUtilIsIntrinsicInst, "Func 79");
    m.def("LLVMUtilIsIntrinsicFun", &LLVMUtilIsIntrinsicFun, "Func 80");


    m.def("LLVMUtilGetCalledFunctions", &LLVMUtilGetCalledFunctions, "Func 75");
    m.def("LLVMUtilRemoveFunAnnotations", &LLVMUtilRemoveFunAnnotations, "Func 76");



    m.def("LLVMUtilIsUnusedGlobalVariable", &LLVMUtilIsUnusedGlobalVariable, "Func 77");
    m.def("LLVMUtilRemoveUnusedGlobalVariables", &LLVMUtilRemoveUnusedGlobalVariables, "Func 78");


    m.def("LLVMUtilRemoveUnusedFuncsAndAnnotationsAndGlobalVariables", &LLVMUtilRemoveUnusedFuncsAndAnnotationsAndGlobalVariables, "Func 79");
    m.def("LLVMUtilGetFunction", &LLVMUtilGetFunction, "Func 80");


    m.def("LLVMUtilIsConstDataOrAggData", &LLVMUtilIsConstDataOrAggData, "Func 75");
    m.def("LLVMUtilGetGlobalRep", &LLVMUtilGetGlobalRep, "Func 76");




    m.def("LLVMUtilIsConstDataOrAggData", &LLVMUtilIsConstDataOrAggData, "Func 77");
    m.def("LLVMUtilGetSourceLocOfFunction", &LLVMUtilGetSourceLocOfFunction, "Func 78");

    m.def("LLVMUtilIsIntrinsicInst", &LLVMUtilIsIntrinsicInst, "Func 79");
    m.def("LLVMUtilIsIntrinsicFun", &LLVMUtilIsIntrinsicFun, "Func 80");


    m.def("LLVMUtilGetCalledFunctions", &LLVMUtilGetCalledFunctions, "Func 75");
    m.def("LLVMUtilRemoveFunAnnotations", &LLVMUtilRemoveFunAnnotations, "Func 76");



    m.def("LLVMUtilIsUnusedGlobalVariable", &LLVMUtilIsUnusedGlobalVariable, "Func 77");
    m.def("LLVMUtilRemoveUnusedGlobalVariables", &LLVMUtilRemoveUnusedGlobalVariables, "Func 78");


    m.def("LLVMUtilIsConstantObjSymSVF", (bool (*)(const SVFValue*)) &LLVMUtilIsConstantObjSym);
    m.def("LLVMUtilIsConstantObjSymVal", (bool (*)(const Value*)) &LLVMUtilIsConstantObjSym);



    m.def("LLVMUtilViewCFG", &LLVMUtilViewCFG, "Func 77");
    m.def("LLVMUtilViewCFGOnly", &LLVMUtilViewCFGOnly, "Func 78");

    m.def("LLVMUtilDumpValue", &LLVMUtilDumpValue, "Func 79");
    m.def("LLVMUtilDumpType", &LLVMUtilDumpType, "Func 80");


    m.def("LLVMUtilDumpValueAndDbgInfo", &LLVMUtilDumpValueAndDbgInfo, "Func 75");
    m.def("LLVMUtilGetSuccBBandCondValPairVec", &LLVMUtilGetSuccBBandCondValPairVec, "Func 76");

    // m.def("LLVMUtilGetCaseValue", &LLVMUtilGetCaseValue, "Func 76");

    // --------------------------------------------------------------------------------------------------------------


    // --------------------------------------------------------------------------------------------------------------
    // Graphs/ICFG.h

    m.def("ICFGGetICFGNodeID", (ICFGNode* (*)(NodeID)) &ICFGGetICFGNode);
    m.def("ICFGHasICFGNodeID", (ICFGNode* (*)(NodeID)) &ICFGHasICFGNode, "Func 71");


    m.def("ICFGHasIntraICFGEdge", &ICFGHasIntraICFGEdge, "Func 72");
    m.def("ICFGHasInterICFGEdge", &ICFGHasInterICFGEdge, "Func 73");
    m.def("ICFGHasThreadICFGEdge", &ICFGHasThreadICFGEdge, "Func 74");

    m.def("ICFGGetICFGEdge", &ICFGGetICFGEdge, "Func 75");
    m.def("ICFGDump", &ICFGDump, "Func 76");

    m.def("ICFGView", &ICFGView, "Func 77");
    m.def("ICFGUpdateCallGraph", &ICFGUpdateCallGraph, "Func 78");

    m.def("ICFGIsInLoopNode", (bool (*)(const ICFGNode*)) &ICFGIsInLoop);
    m.def("ICFGIsInLoopInst", (bool (*)(const SVFInstruction*)) &ICFGIsInLoop);


    m.def("ICFGAddNodeToSVFLoop", &ICFGAddNodeToSVFLoop, "Func 72");
    m.def("ICFGGetSVFLoops", &ICFGGetSVFLoops, "Func 73");
    m.def("ICFGGetIcfgNodeToSVFLoopVec", &ICFGGetIcfgNodeToSVFLoopVec, "Func 74");

    m.def("ICFGGetICFGNodeInst", (ICFGNode* (*)(const SVFInstruction*)) &ICFGGetICFGNode);


    m.def("ICFGGetCallICFGNode", &ICFGGetCallICFGNode, "Func 72");
    m.def("ICFGGetRetICFGNode", &ICFGGetRetICFGNode, "Func 73");
    m.def("ICFGGetIntraICFGNode", &ICFGGetIntraICFGNode, "Func 74");

    m.def("ICFGGetFunEntryICFGNode", &ICFGGetFunEntryICFGNode, "Func 75");
    m.def("ICFGGetFunExitICFGNode", &ICFGGetFunExitICFGNode, "Func 76");

    m.def("ICFGGetGlobalICFGNode", &ICFGGetGlobalICFGNode, "Func 77");
    m.def("ICFGAddGlobalICFGNode", &ICFGAddGlobalICFGNode, "Func 78");



    m.def("ICFGGetSubNodes", &ICFGGetSubNodes, "Func 72");
    m.def("ICFGGetRepNode", &ICFGGetRepNode, "Func 73");
    m.def("ICFGUpdateSubAndRep", &ICFGUpdateSubAndRep, "Func 74");
    // --------------------------------------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------------------------------------
    // Graphs/VFG.h

    // --------------------------------------------------------------------------------------------------------------

}