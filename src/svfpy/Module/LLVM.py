from .package import libSvfModule


class LLVM(object):
    llvm = libSvfModule

    def llvm_shutdown(self):
        self.llvm.llvm_shutdown()


LLVM = LLVM()
