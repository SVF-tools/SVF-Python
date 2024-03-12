import svfModule_pybind


class PTACallGraph(object):
    pass


class Andersen(object):
    andersen = svfModule_pybind

    def getPTACallGraph(self):
        self.andersen.getPTACallGraph()
        return PTACallGraph


Andersen = Andersen()
