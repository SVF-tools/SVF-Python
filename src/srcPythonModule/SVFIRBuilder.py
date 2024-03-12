import svfModule_pybind
from .SVFIR import SVFIR


class SVFIRBuilder(object):
    svfIRBuilder = svfModule_pybind

    def build(self):
        self.svfIRBuilder.build()
        return SVFIR

SVFIRBuilder = SVFIRBuilder()