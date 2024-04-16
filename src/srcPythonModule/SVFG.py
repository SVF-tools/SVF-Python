import svfModule_pybind


class SVFG(object):
    SVFG = svfModule_pybind

    def SVFGBuild(self):
        self.SVFG.SVFGBuild()

    def SVFGClearMSSA(self):
        self.SVFG.SVFGClearMSSA()

    def SVFGGetPTA(self):
        return self.SVFG.SVFGGetPTA()
    

    def SVFGGetSVFGNode(self, id):
        return self.SVFG.SVFGGetSVFGNode(id)

    def SVFGHasSVFGNode(self, id):
        return self.SVFG.SVFGHasSVFGNode(id)

    def SVFGGetInterVFEdgesForIndirectCallSite(self, cs, callee, edges):
        self.SVFG.SVFGGetInterVFEdgesForIndirectCallSite(cs, callee, edges)



    
    def SVFGDump(self, file, simple):
        self.SVFG.SVFGDump(file, simple)

    def SVFGConnectCallerAndCallee(self, cs, callee, edges):
        self.SVFG.SVFGConnectCallerAndCallee(cs, callee, edges)
    
    def SVFGGetDefSVFGNode(self, pagNode):
        return self.SVFG.SVFGGetDefSVFGNode(pagNode)
    

    def SVFGHasSVFGNode(self, pagNode):
        return self.SVFG.SVFGHasSVFGNode(pagNode)
    

    def SVFGPerformStat(self):
        self.SVFG.SVFGPerformStat()




    
    def SVFGHasActualINSVFGNodes(self, cs):
        return self.SVFG.SVFGHasActualINSVFGNodes(cs)

    def SVFGHasActualOUTSVFGNodes(self, cs):
        return self.SVFG.SVFGHasActualOUTSVFGNodes(cs)



    
    def SVFGHasFormalINSVFGNodes(self, fun):
        return self.SVFG.SVFGHasFormalINSVFGNodes(fun)

    def SVFGHasFormalOUTSVFGNodes(self, fun):
        return self.SVFG.SVFGHasFormalOUTSVFGNodes(fun)
    
    def SVFGGetActualINSVFGNodes(self, cs):
        return self.SVFG.SVFGGetActualINSVFGNodes(cs)
    

    def SVFGGetActualOUTSVFGNodes(self, cs):
        return self.SVFG.SVFGGetActualOUTSVFGNodes(cs)
    

    def SVFGGetFormalINSVFGNodes(self, fun):
        return self.SVFG.SVFGGetFormalINSVFGNodes(fun)
    

    def SVFGGetFormalOUTSVFGNodes(self, fun):
        return self.SVFG.SVFGGetFormalOUTSVFGNodes(fun)
    


    def SVFGIsFunEntrySVFGNode(self, node):
        return self.SVFG.SVFGIsFunEntrySVFGNode(node)
    

    def SVFGIsCallSiteRetSVFGNode(self, node):
        return self.SVFG.SVFGIsCallSiteRetSVFGNode(node)
    

    def SVFGRemoveSVFGEdge(self, edge):
        self.SVFG.SVFGRemoveSVFGEdge(edge)
    

    def SVFGRemoveSVFGNode(self, node):
        self.SVFG.SVFGRemoveSVFGNode(node)





    def SVFGAddSVFGEdge(self, edge):
        return self.SVFG.SVFGAddSVFGEdge(edge)
    

    def SVFGGetSVFGNodeNum(self):
        return self.SVFG.SVFGGetSVFGNodeNum()
    

    def SVFGAddDummyVersionPropSVFGNode(self, object, version):
        return self.SVFG.SVFGAddDummyVersionPropSVFGNode(object, version)
    

    def SVFGWriteToFile(self, filename):
        self.SVFG.SVFGWriteToFile(filename)


    def SVFGReadFile(self, filename):
        self.SVFG.SVFGReadFile(filename)

    def SVFGGetMRVERFromString(self, input):
        return self.SVFG.SVFGGetMRVERFromString(input)


SVFG = SVFG()
