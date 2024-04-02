import svfModule_pybind

class SABER(object):
    SABER = svfModule_pybind

    def saberCheckerAllInOne(self):
        self.SABER.saberCheckerAllInOne()

    def saberRunOnModule(self):
        self.SABER.saberRunOnModule()

SABER = SABER()