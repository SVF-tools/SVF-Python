from .package import libSvfModule

libSvfModule = libSvfModule


class SVFModule(object):
    global libSvfModule
    svfModule = libSvfModule

    def buildSymbolTableInfo(self):
        self.svfModule.buildSymbolTableInfo()


SVFModule = SVFModule()
