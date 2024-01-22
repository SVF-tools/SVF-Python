from .package import libSvfModule

libSvfModule = libSvfModule


class CL(object):
    global libSvfModule
    _cl = libSvfModule

    def ParseCommandLineOptions(self):
        self._cl.ParseCommandLineOptions()


CL = CL()
