import svfModule_pybind

class ICFG(object):
    ICFG = svfModule_pybind
    def ICFGBuild(self):
        self.ICFG.ICFGBuild()

    def ICFGGetICFGNodeID(self, id):
        return self.ICFG.ICFGGetICFGNodeID(id)
    
    def ICFGHasICFGNodeID(self, id):
        return self.ICFG.ICFGHasICFGNodeID(id)
    
    def ICFGHasInterICFGEdge(self, src, dst, kind):
        return self.ICFG.ICFGHasInterICFGEdge(src, dst, kind)
    
    def ICFGHasThreadICFGEdge(self, src, dst, kind):
        return self.ICFG.ICFGHasThreadICFGEdge(src, dst, kind)
    

    def ICFGGetICFGEdge(self, src, dst, kind):
        return self.ICFG.ICFGGetICFGEdge(src, dst, kind)

    
    def ICFGDump(self, file, simple):
        self.ICFG.ICFGDump(file, simple)
    
    
    def ICFGView(self):
        self.ICFG.ICFGView()
    

    def ICFGUpdateCallGraph(self, callgraph):
        self.ICFG.ICFGUpdateCallGraph(callgraph)
    


    def ICFGIsInLoopNode(self, node):
        return self.ICFG.ICFGIsInLoopNode(node)
    

    def ICFGIsInLoopInst(self, inst):
        return self.ICFG.ICFGIsInLoopInst(inst)
    


    def ICFGAddNodeToSVFLoop(self, node, loop):
        self.ICFG.ICFGAddNodeToSVFLoop(node, loop)
    


    def ICFGGetSVFLoops(self, node):
        return self.ICFG.ICFGGetSVFLoops(node)
    



    def ICFGGetIcfgNodeToSVFLoopVec(self):
        return self.ICFG.ICFGGetIcfgNodeToSVFLoopVec()
    

    def ICFGGetICFGNode(self, inst):
        return self.ICFG.ICFGGetICFGNode(inst)
    


    def ICFGGetCallICFGNode(self, inst):
        return self.ICFG.ICFGGetCallICFGNode(inst)
    


    

    def ICFGGetRetICFGNode(self, inst):
        return self.ICFG.ICFGGetRetICFGNode(inst)
    



    def ICFGGetIntraICFGNode(self, inst):
        return self.ICFG.ICFGGetIntraICFGNode(inst)
    


    def ICFGGetFunEntryICFGNode(self, fun):
        return self.ICFG.ICFGGetFunEntryICFGNode(fun)
    




    def ICFGGetFunExitICFGNode(self, fun):
        return self.ICFG.ICFGGetFunExitICFGNode(fun)
    

    def ICFGGetGlobalICFGNode(self):
        return self.ICFG.ICFGGetGlobalICFGNode()
    


    def ICFGAddGlobalICFGNode(self):
        self.ICFG.ICFGAddGlobalICFGNode()
    


    def ICFGGetSubNodes(self, node):
        return self.ICFG.ICFGGetSubNodes(node)
    

    def ICFGGetRepNode(self, node):
        return self.ICFG.ICFGGetRepNode(node)
    


    def ICFGUpdateSubAndRep(self, rep, sub):
        self.ICFG.ICFGUpdateSubAndRep(rep, sub)
    

    

ICFG = ICFG()