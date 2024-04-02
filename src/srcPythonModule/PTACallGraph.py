import svfModule_pybind


class PTACallGraph(object):
    PTACallGraph = svfModule_pybind

    def getPTACallGraph(self):
        self.PTACallGraph.getPTACallGraph()

    def updateCallGraph(self):
        self.PTACallGraph.updateCallGraph()


PTACallGraph = PTACallGraph()
