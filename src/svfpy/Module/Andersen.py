from .package import libSvfModule

libSvfModule = libSvfModule


class PTACallGraph(object):
    pass


class Andersen(object):
    global libSvfModule
    andersen = libSvfModule

    def getPTACallGraph(self):
        self.andersen.getPTACallGraph()
        return PTACallGraph


Andersen = Andersen()
