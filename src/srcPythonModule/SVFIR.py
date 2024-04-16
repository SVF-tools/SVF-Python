import svfModule_pybind

class ICFG(object):
    pass


class SVFIR(object):
    svfIR = svfModule_pybind
    def pagBuild(self):
        self.svfIR.pagBuild()

    def getICFG(self):
        self.svfIR.ICFGBuild()

    def releaseSVFIR(self):
        self.svfIR.releaseSVFIR()

    def getICFGUpdateCallGraph(self):
        self.svfIR.getICFGUpdateCallGraph()

    def SVFIRReaderRead(self):
        self.svfIR.SVFIRReaderRead()

    def SVFIRBuild(self):
        self.svfIR.svfirBuild()
    

    def SVFIRPAGBuild(self):
        return self.svfIR.SVFIRPAGBuild()

    def SVFIRGetPAG(self):
        return self.svfIR.SVFIRGetPAG()

    def SVFIRInitialiseNodes(self):
        return self.svfIR.SVFIRInitialiseNodes()

    def SVFIRSanityCheck(self):
        return self.svfIR.SVFIRSanityCheck()

    def SVFIRGetValueNode(self, V):
        return self.svfIR.SVFIRGetValueNode(V)

    def SVFIRGetObjectNode(self, V):
        return self.svfIR.SVFIRGetObjectNode(V)
    



    def SVFIRGetReturnNode(self, func):
        return self.svfIR.SVFIRGetReturnNode(func)

    def SVFIRGetVarargNode(self, func):
        return self.svfIR.SVFIRGetVarargNode(func)

    def SVFIRVisitAllocaInst(self, AI):
        self.svfIR.SVFIRVisitAllocaInst(AI)

    def SVFIRVisitPHINode(self, I):
        self.svfIR.SVFIRVisitPHINode(I)

    def SVFIRVisitStoreInst(self, I):
        self.svfIR.SVFIRVisitStoreInst(I)

    def SVFIRVisitLoadInst(self, I):
        self.svfIR.SVFIRVisitLoadInst(I)





    def SVFIRVisitGetElementPtrInst(self, I):
        self.svfIR.SVFIRVisitGetElementPtrInst(I)


    def SVFIRVisitCallInst(self, I):
        self.svfIR.SVFIRVisitCallInst(I)

    def SVFIRVisitInvokeInst(self, II):
        self.svfIR.SVFIRVisitInvokeInst(II)

    def SVFIRVisitCallBrInst(self, I):
        self.svfIR.SVFIRVisitCallBrInst(I)


    def SVFIRVisitCallSite(self, cs):
        self.svfIR.SVFIRVisitCallSite(cs)

    def SVFIRVisitReturnInst(self, I):
        self.svfIR.SVFIRVisitReturnInst(I)



    def SVFIRVisitCastInst(self, I):
        self.svfIR.SVFIRVisitCastInst(I)

    def SVFIRVisitSelectInst(self, I):
        self.svfIR.SVFIRVisitSelectInst(I)

    def SVFIRVisitExtractValueInst(self, EVI):
        self.svfIR.SVFIRVisitExtractValueInst(EVI)

    def SVFIRVisitBranchInst(self, I):
        self.svfIR.SVFIRVisitBranchInst(I)

    def SVFIRVisitSwitchInst(self, I):
        self.svfIR.SVFIRVisitSwitchInst(I)


    def SVFIRVisitInsertValueInst(self, I):
        self.svfIR.SVFIRVisitInsertValueInst(I)




    def SVFIRVisitBinaryOperator(self, I):
        self.svfIR.SVFIRVisitBinaryOperator(I)

    def SVFIRVisitUnaryOperator(self, I):
        self.svfIR.SVFIRVisitUnaryOperator(I)

    def SVFIRVisitCmpInst(self, I):
        self.svfIR.SVFIRVisitCmpInst(I)

    def SVFIRVisitVAArgInst(self, inst):
        self.svfIR.SVFIRVisitVAArgInst(inst)

    def SVFIRVisitVACopyInst(self, inst):
        self.svfIR.SVFIRVisitVACopyInst(inst)

    def SVFIRVisitVAEndInst(self, inst):
        self.svfIR.SVFIRVisitVAEndInst(inst)



    def SVFIRVisitVAStartInst(self, inst):
        self.svfIR.SVFIRVisitVAStartInst(inst)

    def SVFIRVisitFreezeInst(self, I):
        self.svfIR.SVFIRVisitFreezeInst(I)

    def SVFIRVisitExtractElementInst(self, I):
        self.svfIR.SVFIRVisitExtractElementInst(I)

    def SVFIRVisitInsertElementInst(self, I):
        self.svfIR.SVFIRVisitInsertElementInst(I)

    def SVFIRVisitShuffleVectorInst(self, I):
        self.svfIR.SVFIRVisitShuffleVectorInst(I)

    def SVFIRVisitLandingPadInst(self, I):
        self.svfIR.SVFIRVisitLandingPadInst(I)



    def SVFIRVisitResumeInst(self, inst):
        self.svfIR.SVFIRVisitResumeInst(inst)

    def SVFIRVisitUnreachableInst(self, inst):
        self.svfIR.SVFIRVisitUnreachableInst(inst)

    def SVFIRVisitFenceInst(self, I):
        self.svfIR.SVFIRVisitFenceInst(I)

    def SVFIRVisitAtomicCmpXchgInst(self, I):
        self.svfIR.SVFIRVisitAtomicCmpXchgInst(I)

    def SVFIRVisitAtomicRMWInst(self, I):
        self.svfIR.SVFIRVisitAtomicRMWInst(I)

    def SVFIRVisitInstruction(self, inst):
        self.svfIR.SVFIRVisitInstruction(inst)

    def SVFIRUpdateCallGraph(self, callgraph):
        self.svfIR.updateCallGraph(callgraph)


SVFIR = SVFIR()
