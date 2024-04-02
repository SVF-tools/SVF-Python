import svfModule_pybind

class ICFG(object):
    pass


class SVFIR(object):
    svfIR = svfModule_pybind
    def pagBuild(self):
        self.svfIR.pagBuild()

    def getICFG(self):
        self.svfIR.getICFG()

    def releaseSVFIR(self):
        self.svfIR.getICFG()

    def getICFGUpdateCallGraph(self):
        self.svfIR.getICFGUpdateCallGraph()


SVFIR = SVFIR()
