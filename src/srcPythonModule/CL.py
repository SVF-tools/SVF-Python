import svfModule_pybind

class CL(object):
    CL = svfModule_pybind

    def ParseCommandLineOptions(self):
        self.CL.ParseCommandLineOptions()


CL = CL()
