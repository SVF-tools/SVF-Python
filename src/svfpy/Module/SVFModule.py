from .package import libSvfModule


class SVFModule(object):
    svfModule = libSvfModule

    def buildSymbolTableInfo(self):
        self.svfModule.buildSymbolTableInfo()


SVFModule = SVFModule()
