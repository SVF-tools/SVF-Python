import svfModule_pybind


class VFG(object):
    VFG = svfModule_pybind

    def VFGBuild(self):
        self.VFG.VFGBuild()

    def VFGDelete(self):
        self.VFG.deleteVfg()

    def VFGGetKind(self):
        return self.VFG.VFGGetKind()
    
    def VFGIsPtrOnlySVFG(self):
        return self.VFG.VFGIsPtrOnlySVFG()

    def VFGGetPAG(self):
        return self.VFG.VFGGetPAG()
    
    def VFGGetCallGraph(self):
        return self.VFG.VFGGetCallGraph()
    



    def VFGGetVFGNode(self, id):
        return self.VFG.VFGGetVFGNode(id)
    
    def VFGHasVFGNode(self, id):
        return self.VFG.VFGHasVFGNode(id)
    

    def VFGGetGlobalVFGNodes(self):
        return self.VFG.VFGGetGlobalVFGNodes()
    
    def VFGGetIntraVFGEdge(self, src, dst, kind):
        return self.VFG.VFGGetIntraVFGEdge(src, dst, kind)

    def VFGDump(self, file, simple):
        self.VFG.VFGDump(file, simple)



    def VFGView(self):
        self.VFG.VFGView()

    def VFGUpdateCallGraph(self, pta):
        self.VFG.VFGUpdateCallGraph(pta)
    
    def VFGConnectCallerAndCallee(self, cs, callee, edges):
        self.VFG.VFGConnectCallerAndCallee(cs, callee, edges)
    

    def VFGGetCallSiteID(self, cs, func):
        return self.VFG.VFGGetCallSiteID(cs, func)
    
    
    def VFGGetCallSite(self, id):
        return self.VFG.VFGGetCallSite(id)
    

    def VFGGetDefVFGNode(self, pagNode):
        return self.VFG.VFGGetDefVFGNode(pagNode)
    
    
    def VFGGetLHSTopLevPtr(self, node):
        return self.VFG.VFGGetLHSTopLevPtr(node)
    

    def VFGGetStmtVFGNode(self, pagEdge):
        return self.VFG.VFGGetStmtVFGNode(pagEdge)


    def VFGGetIntraPHIVFGNode(self, pagNode):
        return self.VFG.VFGGetIntraPHIVFGNode(pagNode)


    def VFGGetBinaryOPVFGNode(self, pagNode):
        return self.VFG.VFGGetBinaryOPVFGNode(pagNode)


    def VFGGetUnaryOPVFGNode(self, pagNode):
        return self.VFG.VFGGetUnaryOPVFGNode(pagNode)


    def VFGGetBranchVFGNode(self, pagNode):
        return self.VFG.VFGGetBranchVFGNode(pagNode)


    def VFGGetCmpVFGNode(self, pagNode):
        return self.VFG.VFGGetCmpVFGNode(pagNode)



    def VFGGetActualParmVFGNode(self, aparm, cs):
        return self.VFG.VFGGetActualParmVFGNode(aparm, cs)


    def VFGGetActualRetVFGNode(self, aret):
        return self.VFG.VFGGetActualRetVFGNode(aret)


    def VFGGetFormalParmVFGNode(self, fparm):
        return self.VFG.VFGGetFormalParmVFGNode(fparm)
    

    def VFGGetFormalRetVFGNode(self, fret):
        return self.VFG.VFGGetFormalRetVFGNode(fret)
    


    def VFGIsFunEntryVFGNode(self, node):
        return self.VFG.VFGIsFunEntryVFGNode(node)
    
    
    def VFGHasBlackHoleConstObjAddrAsDef(self, pagNode):
        return self.VFG.VFGGetLHSTopLevPtr(pagNode)
    

    def VFGGetVFGNodes(self, fun):
        return self.VFG.VFGGetVFGNodes(fun)


    def VFGHasVFGNodes(self, fun):
        return self.VFG.VFGHasVFGNodes(fun)


    def VFGNodes(self, fun):
        return self.VFG.VFGNodes(fun)


    def VFGGetVFGNodeBegin(self, fun):
        return self.VFG.VFGGetVFGNodeBegin(fun)


    def VFGGetVFGNodeEnd(self, fun):
        return self.VFG.VFGGetVFGNodeEnd(fun)


    def VFGAddIntraDirectVFEdge(self, srcId, dstId):
        return self.VFG.VFGAddIntraDirectVFEdge(srcId, dstId)


    def VFGAddCallEdge(self, srcId, dstId, csId):
        return self.VFG.VFGAddCallEdge(srcId, dstId, csId)

    def VFGAddRetEdge(self, srcId, dstId, csId):
        return self.VFG.VFGAddRetEdge(srcId, dstId, csId)



    def VFGRemoveVFGEdge(self, edge):
        self.VFG.VFGRemoveVFGEdge(edge)


    def VFGRemoveVFGNode(self, node):
        self.VFG.VFGRemoveVFGNode(node)


    def VFGHasIntraVFGEdge(self, src, dst, kind):
        return self.VFG.VFGHasIntraVFGEdge(src, dst, kind)


    def VFGHasInterVFGEdge(self, src, dst, kind, csId):
        return self.VFG.VFGHasInterVFGEdge(src, dst, kind, csId)

    def VFGHasThreadVFGEdge(self, src, dst, kind):
        return self.VFG.VFGHasThreadVFGEdge(src, dst, kind)
    

    def VFGAddVFGEdge(self, edge):
        self.VFG.VFGAddVFGEdge(edge)
    

VFG = VFG()
