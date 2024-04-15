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

    def svfirBuild(self):
        self.svfIR.svfirBuild()


SVFIR = SVFIR()
