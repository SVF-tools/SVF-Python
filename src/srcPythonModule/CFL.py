import svfModule_pybind

class CFL(object):
    CFL = svfModule_pybind

    def cflCheckerAllInOne(self):
        self.CFL.cflCheckerAllInOne()

    def cflAnalyze(self):
        self.CFL.cflAnalyze()

CFL = CFL()