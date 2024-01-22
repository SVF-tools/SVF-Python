from .package import libSvfModule

libSvfModule = libSvfModule


class ICFG(object):
    pass


class SVFIR(object):
    global libSvfModule
    svfIR = libSvfModule

    def getICFG(self):
        self.svfIR.getICFG()

    def releaseSVFIR(self):
        self.svfIR.getICFG()


SVFIR = SVFIR()
