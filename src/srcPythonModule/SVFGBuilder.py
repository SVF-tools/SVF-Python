from .package import libSvfModule


class SVFGBuilder(object):
    svfgBuilder = libSvfModule

    def buildFullSVFG(self, ander):
        self.svfgBuilder.buildFullSVFG()


SVFGBuilder = SVFGBuilder()
