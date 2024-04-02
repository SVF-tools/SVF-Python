from .package import libSvfModule


class SVFG(object):
    svfg = libSvfModule

    def delete(self):
        self.svfg.deleteSvfg()


SVFG = SVFG()
