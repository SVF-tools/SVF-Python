from .package import libSvfModule


class CL(object):
    _cl = libSvfModule

    def ParseCommandLineOptions(self):
        self._cl.ParseCommandLineOptions()


CL = CL()
