from .package import libSvfModule

libSvfModule = libSvfModule


class LLVM(object):
    global libSvfModule
    llvm = libSvfModule

    def llvm_shutdown(self):
        self.llvm.llvm_shutdown()


LLVM = LLVM()
