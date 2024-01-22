from .package import libSvfModule
from .SVFIR import SVFIR
libSvfModule = libSvfModule


class SVFIRBuilder(object):
    global libSvfModule
    svfIRBuilder = libSvfModule

    def build(self):
        self.svfIRBuilder.build()
        return SVFIR

SVFIRBuilder = SVFIRBuilder()