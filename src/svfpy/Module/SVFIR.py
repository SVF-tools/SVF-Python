from .package import libSvfModule


class ICFG(object):
    pass


class SVFIR(object):
    svfIR = libSvfModule

    def getICFG(self):
        self.svfIR.getICFG()

    def releaseSVFIR(self):
        self.svfIR.getICFG()


SVFIR = SVFIR()
