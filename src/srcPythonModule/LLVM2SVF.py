import svfModule_pybind

class LLVM2SVF(object):
    LLVM2SVF = svfModule_pybind

    def moduleNameVecReplaceExtension(self):
        return self.LLVM2SVF.moduleNameVecReplaceExtension()

    def writeJsonToPath(self, jsonPath):
        self.LLVM2SVF.writeJsonToPath(jsonPath)

    def outSVFIRJsonPath(self, jsonPath):
        self.LLVM2SVF.outSVFIRJsonPath(jsonPath)

LLVM2SVF = LLVM2SVF()