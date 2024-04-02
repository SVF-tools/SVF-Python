import svfModule_pybind


class LLVMModuleSet(object):
    LLVMModuleSet = svfModule_pybind

    def buildSVFModule(self):
        self.LLVMModuleSet.buildSVFModule()

    def dumpModulesToFile(self, s):
        self.LLVMModuleSet.dumpModulesToFile(s)

    def releaseLLVMModuleSet(self):
        self.LLVMModuleSet.releaseLLVMModuleSet()

    def preProcessBCs(self):
        self.LLVMModuleSet.preProcessBCs()


LLVMModuleSet = LLVMModuleSet()
