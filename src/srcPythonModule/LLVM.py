import svfModule_pybind


class LLVM(object):
    llvm = svfModule_pybind

    def llvm_shutdown(self):
        self.llvm.llvm_shutdown()


LLVM = LLVM()
