import svfModule_pybind

class Options(object):
    Options = svfModule_pybind

    def boolICFGMergeAdjacentNodes(self):
        self.Options.boolICFGMergeAdjacentNodes()

    def boolBufferOverflowCheck(self):
        self.Options.boolBufferOverflowCheck()


Options = Options()