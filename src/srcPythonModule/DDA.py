import svfModule_pybind

class DDA(object):
    DDA = svfModule_pybind

    def ddaPassRunOnModule(self):
        self.DDA.ddaPassRunOnModule()

DDA = DDA()