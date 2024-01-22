import ctypes
from .package import libSvfModule

libSvfModule = libSvfModule


class SVFUtil(object):
    global libSvfModule
    svfUtil = libSvfModule

    def processArguments(self, argv):
        select = (ctypes.c_char_p * len(argv))()
        for key, item in enumerate(argv):
            select[key] = item.encode('utf-8')
        self.svfUtil.processArguments(len(argv), select)


SVFUtil = SVFUtil()
