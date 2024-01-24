from .package import libSvfModule


class VFG(object):
    vfg = libSvfModule

    def newInstances(self, callgraph):
        self.vfg.newInstances()

    def delete(self):
        self.vfg.deleteSvfg()


VFG = VFG()
