import ctypes
from .package import libSvfModule
from .SVFModule import SVFModule


class LLVMModuleSet(object):
    llv = libSvfModule

    def buildSVFModule(self, moduleNameVec):
        for i in moduleNameVec:
            self.llv.setModuleNameVec(ctypes.c_char_p(i.encode('utf-8')))
        self.llv.buildSVFModule()
        return SVFModule

    def dumpModulesToFile(self, s):
        self.llv.dumpModulesToFile(ctypes.c_char_p(s.encode('utf-8')))

    def releaseLLVMModuleSet(self):
        self.llv.releaseLLVMModuleSet()


LLVMModuleSet = LLVMModuleSet()
