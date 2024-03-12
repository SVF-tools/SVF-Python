import svfModule_pybind
from .SVFModule import SVFModule


class LLVMModuleSet(object):
    llv = svfModule_pybind

    def buildSVFModule(self, moduleNameVec):
        for i in moduleNameVec:
            svfModule_pybind.setModuleNameVec(i)
        self.llv.buildSVFModule()
        return SVFModule

    def dumpModulesToFile(self, s):
        svfModule_pybind.dumpModulesToFile(s)

    def releaseLLVMModuleSet(self):
        svfModule_pybind.releaseLLVMModuleSet()


LLVMModuleSet = LLVMModuleSet()
