from .package import libSvfModule


class PTACallGraph(object):
    pass


class Andersen(object):
    andersen = libSvfModule

    def getPTACallGraph(self):
        self.andersen.getPTACallGraph()
        return PTACallGraph


Andersen = Andersen()
