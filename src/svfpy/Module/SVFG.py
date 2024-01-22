from .package import libSvfModule
libSvfModule = libSvfModule


class SVFG(object):
    global libSvfModule
    svfg = libSvfModule

    def delete(self):
        self.svfg.deleteSvfg()


SVFG = SVFG()
