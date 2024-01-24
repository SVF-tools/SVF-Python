from .package import libSvfModule
from .SVFIR import SVFIR


class SVFIRBuilder(object):
    svfIRBuilder = libSvfModule

    def build(self):
        self.svfIRBuilder.build()
        return SVFIR

SVFIRBuilder = SVFIRBuilder()