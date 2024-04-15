import svfModule_pybind


class PTACallGraph(object):
    PTACallGraph = svfModule_pybind

    def getPTACallGraph(self):
        self.PTACallGraph.PTACallGraphBuild()

    def updateCallGraph(self):
        self.PTACallGraph.updateCallGraph()


PTACallGraph = PTACallGraph()
