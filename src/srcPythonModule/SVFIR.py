import svfModule_pybind


class ICFG(object):
    pass


class SVFIR(object):
    svfIR = svfModule_pybind

    def getICFG(self):
        self.svfIR.getICFG()

    def releaseSVFIR(self):
        self.svfIR.getICFG()


SVFIR = SVFIR()
