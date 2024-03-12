import svfModule_pybind


class CL(object):
    _cl = svfModule_pybind

    def ParseCommandLineOptions(self):
        self._cl.ParseCommandLineOptions()


CL = CL()
