from .package import libSvfModule

libSvfModule = libSvfModule


class VFG(object):
    global libSvfModule
    vfg = libSvfModule

    def newInstances(self, callgraph):
        self.vfg.newInstances()

    def delete(self):
        self.vfg.deleteSvfg()


VFG = VFG()

