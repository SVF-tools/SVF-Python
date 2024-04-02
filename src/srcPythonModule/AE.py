import svfModule_pybind

class AE(object):
    AE = svfModule_pybind

    def mergeAdjacentNodes(self):
        self.AE.mergeAdjacentNodes()

    def bufOverflowCheckerRunOnModule(self):
        self.AE.bufOverflowCheckerRunOnModule()

    def abstractExecutionRunOnModule(self):
        self.AE.abstractExecutionRunOnModule()

AE = AE()