import svfModule_pybind


class SVFModule(object):
    svfModule = svfModule_pybind

    def buildSymbolTableInfo(self):
        self.svfModule.buildSymbolTableInfo()


SVFModule = SVFModule()
