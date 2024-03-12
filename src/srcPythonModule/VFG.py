import svfModule_pybind


class VFG(object):
    vfg = svfModule_pybind

    def newInstances(self, callgraph):
        self.vfg.newInstances()

    def delete(self):
        self.vfg.deleteSvfg()


VFG = VFG()
