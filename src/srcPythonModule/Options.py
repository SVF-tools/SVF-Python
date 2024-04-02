import svfModule_pybind

class Options(object):
    Options = svfModule_pybind

    def boolICFGMergeAdjacentNodes(self):
        return self.Options.boolICFGMergeAdjacentNodes()

    def boolBufferOverflowCheck(self):
        return self.Options.boolBufferOverflowCheck()

    def boolReadJson(self):
        return self.Options.boolReadJson()

    def optionsWriteAnder(self):
        return self.Options.optionsWriteAnder()

    def boolCFLGraphEmpty(self):
        return self.Options.boolCFLGraphEmpty()



Options = Options()