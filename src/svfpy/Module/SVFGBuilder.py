from .package import libSvfModule
libSvfModule = libSvfModule


class SVFGBuilder(object):
    global libSvfModule
    svfgBuilder = libSvfModule

    def buildFullSVFG(self, ander):
        self.svfgBuilder.buildFullSVFG()


SVFGBuilder = SVFGBuilder()
