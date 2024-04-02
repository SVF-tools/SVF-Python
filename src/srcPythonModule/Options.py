import svfModule_pybind

class Options(object):
    Options = svfModule_pybind

    def boolICFGMergeAdjacentNodes(self):
        self.Options.boolICFGMergeAdjacentNodes()

    def boolBufferOverflowCheck(self):
        self.Options.boolBufferOverflowCheck()

    def boolReadJson(self):
        self.Options.boolReadJson()

    def optionsWriteAnder(self):
        self.Options.optionsWriteAnder()

    def boolCFLGraphEmpty(self):
        self.Options.boolCFLGraphEmpty()



Options = Options()