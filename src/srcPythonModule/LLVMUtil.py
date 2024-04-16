#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import svfModule_pybind


class LLVMUtil(object):
    LLVMUtil = svfModule_pybind

    def processArguments(self, argv, moduleNameVec):

        select = [0] * len(argv)
        for key, item in enumerate(argv):
            select[key] = item
        self.LLVMUtil.processArguments(len(argv), select)
        num = self.LLVMUtil.getModuleNameVecLen()
        i = 0
        while i < num:
            result = str()
            result = self.LLVMUtil.getModuleNameVecItem(i, result)
            moduleNameVec.append(result)
            i = i + 1


    def LLVMUtilIsCallSite(self, inst):
        return self.LLVMUtil.LLVMUtilIsCallSite(inst)
    

    def LLVMUtilIsCallSite(self, val):
        return self.LLVMUtil.LLVMUtilIsCallSite(val)
    

    def LLVMUtilGetDefFunForMultipleModule(self, fun):
        return self.LLVMUtil.LLVMUtilGetDefFunForMultipleModule(fun)
    

    def LLVMUtilGetLLVMCallSite(self, value):
        return self.LLVMUtil.LLVMUtilGetLLVMCallSite(value)
    

    def LLVMUtilGetCallee(self, cs):
        return self.LLVMUtil.LLVMUtilGetCallee(cs)
    

    def LLVMUtilGetLLVMFunction(self, val):
        return self.LLVMUtil.LLVMUtilGetLLVMFunction(val)
    


    def LLVMUtilGetProgFunction(self, funName):
        return self.LLVMUtil.LLVMUtilGetProgFunction(funName)
    

    def LLVMUtilIsProgEntryFunction(self, fun):
        return self.LLVMUtil.LLVMUtilIsProgEntryFunction(fun)
    

    def LLVMUtilIsBlackholeSym(self, val):
        return self.LLVMUtil.LLVMUtilIsBlackholeSym(val)


    def LLVMUtilIsNullPtrSym(self, val):
        return self.LLVMUtil.LLVMUtilIsNullPtrSym(val)
    


    def LLVMUtilGetPtrElementType(self, pty):
        return self.LLVMUtil.LLVMUtilGetPtrElementType(pty)
    

    def LLVMUtilGetNumOfElements(self, ety):
        return self.LLVMUtil.LLVMUtilGetNumOfElements(ety)
    

    def LLVMUtilIsObject(self, ref):
        return self.LLVMUtil.LLVMUtilIsObject(ref)
    

    def LLVMUtilIsUncalledFunction(self, fun):
        return self.LLVMUtil.LLVMUtilIsUncalledFunction(fun)
    

    def LLVMUtilArgInDeadFunction(self, val):
        return self.LLVMUtil.LLVMUtilArgInDeadFunction(val)
    

    def LLVMUtilArgInProgEntryFunction(self, val):
        return self.LLVMUtil.LLVMUtilArgInProgEntryFunction(val)
    

    def LLVMUtilIsPtrInUncalledFunction(self, value):
        return self.LLVMUtil.LLVMUtilIsPtrInUncalledFunction(value)
    


    def LLVMUtilIsNoCallerFunction(self, fun):
        return self.LLVMUtil.LLVMUtilIsNoCallerFunction(fun)
    

    def LLVMUtilIsArgOfUncalledFunction(self, val):
        return self.LLVMUtil.LLVMUtilIsArgOfUncalledFunction(val)
    

    def LLVMUtilBasicBlockHasRetInst(self, bb):
        return self.LLVMUtil.LLVMUtilBasicBlockHasRetInst(bb)
    



    def LLVMUtilFunctionDoesNotRet(self, fun):
        return self.LLVMUtil.LLVMUtilFunctionDoesNotRet(fun)
    


    def LLVMUtilGetFunReachableBBs(self, svfFun, bbs):
        return self.LLVMUtil.LLVMUtilGetFunReachableBBs(svfFun, bbs)
    




    def LLVMUtilStripConstantCasts(self, val):
        return self.LLVMUtil.LLVMUtilStripConstantCasts(val)
    

    def LLVMUtilStripAllCasts(self, val):
        return self.LLVMUtil.LLVMUtilStripAllCasts(val)

    def LLVMUtilGetFirstUseViaCastInst(self, val):
        return self.LLVMUtil.LLVMUtilGetFirstUseViaCastInst(val)
    

    def LLVMUtilIsGepConstantExpr(self, val):
        return self.LLVMUtil.LLVMUtilIsGepConstantExpr(val)
    

    def LLVMUtilIsInt2PtrConstantExpr(self, val):
        return self.LLVMUtil.LLVMUtilIsInt2PtrConstantExpr(val)
        

    

    def LLVMUtilIsPtr2IntConstantExpr(self, val):
        return self.LLVMUtil.LLVMUtilIsPtr2IntConstantExpr(val)
    

    def LLVMUtilIsCastConstantExpr(self, val):
        return self.LLVMUtil.LLVMUtilIsCastConstantExpr(val)
    

    def LLVMUtilIsSelectConstantExpr(self, val):
        return self.LLVMUtil.LLVMUtilIsSelectConstantExpr(val)
    


    def LLVMUtilIsTruncConstantExpr(self, val):
        return self.LLVMUtil.LLVMUtilIsTruncConstantExpr(val)


    def LLVMUtilIsCmpConstantExpr(self, val):
        return self.LLVMUtil.LLVMUtilIsCmpConstantExpr(val)


    def LLVMUtilIsBinaryConstantExpr(self, val):
        return self.LLVMUtil.LLVMUtilIsBinaryConstantExpr(val)
    

    def LLVMUtilIsUnaryConstantExpr(self, val):
        return self.LLVMUtil.LLVMUtilIsUnaryConstantExpr(val)

    def LLVMUtilGetDataLayout(self, mod):
        return self.LLVMUtil.LLVMUtilGetDataLayout(mod)

    def LLVMUtilGetNextInstsSVF(self, curInst, instList):
        return self.LLVMUtil.LLVMUtilGetNextInstsSVF(curInst, instList)
    

    def LLVMUtilGetPrevInstsSVF(self, curInst, instList):
        return self.LLVMUtil.LLVMUtilGetPrevInstsSVF(curInst, instList)
    


    def LLVMUtilGetNextInsts(self, curInst, instList):
        return self.LLVMUtil.LLVMUtilGetNextInsts(curInst, instList)


    def LLVMUtilGetPrevInsts(self, curInst, instList):
        return self.LLVMUtil.LLVMUtilGetPrevInsts(curInst, instList)
    

    def LLVMUtilGetBBPredecessorNum(self, BB):
        return self.LLVMUtil.LLVMUtilGetBBPredecessorNum(BB)

    def LLVMUtilIsIRFile(self, filename):
        return self.LLVMUtil.LLVMUtilIsIRFile(filename)



    # def LLVMUtilProcessArguments(self, ):
    

    def LLVMUtilGetTypeSizeInBytes(self, type):
        return self.LLVMUtil.LLVMUtilGetTypeSizeInBytes(type)

    def LLVMUtilGetTypeSizeInBytesStructType(self, sty, field_index):
        return self.LLVMUtil.LLVMUtilGetTypeSizeInBytesStructType(sty, field_index)
    




    def LLVMUtilGetSourceLoc(self, val):
        return self.LLVMUtil.LLVMUtilGetSourceLoc(val)

    def LLVMUtilGetSourceLocOfFunction(self, F):
        return self.LLVMUtil.LLVMUtilGetSourceLocOfFunction(F)
    

    def LLVMUtilIsIntrinsicInst(self, inst):
        return self.LLVMUtil.LLVMUtilIsIntrinsicInst(inst)

    def LLVMUtilIsIntrinsicFun(self, func):
        return self.LLVMUtil.LLVMUtilIsIntrinsicFun(func)
    
    def LLVMUtilGetCalledFunctions(self, F):
        return self.LLVMUtil.LLVMUtilGetCalledFunctions(F)

    def LLVMUtilRemoveFunAnnotations(self, removedFuncList):
        self.LLVMUtil.LLVMUtilRemoveFunAnnotations(removedFuncList)
    

    def LLVMUtilIsUnusedGlobalVariable(self, gbal):
        return self.LLVMUtil.LLVMUtilIsUnusedGlobalVariable(gbal)

    def LLVMUtilRemoveUnusedGlobalVariables(self, module):
        self.LLVMUtil.LLVMUtilRemoveUnusedGlobalVariables(module)





    def LLVMUtilRemoveUnusedFuncsAndAnnotationsAndGlobalVariables(self, removedFuncList):
        return self.LLVMUtil.LLVMUtilRemoveUnusedFuncsAndAnnotationsAndGlobalVariables(removedFuncList)

    def LLVMUtilGetFunction(self, name):
        return self.LLVMUtil.LLVMUtilGetFunction(name)

    def LLVMUtilIsConstDataOrAggData(self, val):
        return self.LLVMUtil.LLVMUtilIsConstDataOrAggData(val)
    

    def LLVMUtilGetGlobalRep(self, val):
        return self.LLVMUtil.LLVMUtilGetGlobalRep(val)

    def LLVMUtilIsConstantObjSymSVF(self, val):
        return self.LLVMUtil.LLVMUtilIsConstantObjSymSVF(val)
    


    def LLVMUtilIsConstantObjSymVal(self, val):
        return self.LLVMUtil.LLVMUtilIsConstantObjSymVal(val)
    



    def LLVMUtilViewCFG(self, fun):
        return self.LLVMUtil.LLVMUtilViewCFG(fun)

    def LLVMUtilViewCFGOnly(self, fun):
        return self.LLVMUtil.LLVMUtilViewCFGOnly(fun)
    

    def LLVMUtilDumpValue(self, val):
        return self.LLVMUtil.LLVMUtilDumpValue(val)

    def LLVMUtilDumpType(self, type):
        return self.LLVMUtil.LLVMUtilDumpType(type)
    


    def LLVMUtilDumpValueAndDbgInfo(self, val):
        return self.LLVMUtil.LLVMUtilDumpValueAndDbgInfo(val)

    def LLVMUtilGetSuccBBandCondValPairVec(self, switchInst, vec):
        return self.LLVMUtil.LLVMUtilGetSuccBBandCondValPairVec(switchInst, vec)
    

    def LLVMUtilGetCaseValue(self, switchInst, succBB2CondVal):
        return self.LLVMUtil.LLVMUtilGetCaseValue(switchInst, succBB2CondVal)



LLVMUtil = LLVMUtil()
