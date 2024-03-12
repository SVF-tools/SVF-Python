import svfModule_pybind


class SVFGBuilder(object):
    svfgBuilder = svfModule_pybind

    def buildFullSVFG(self, ander):
        self.svfgBuilder.buildFullSVFG()


SVFGBuilder = SVFGBuilder()
