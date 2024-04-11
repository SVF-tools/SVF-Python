import svfModule_pybind

class Andersen(object):
    Andersen = svfModule_pybind

    def AndersenBaseAnalyze(self):
        self.Andersen.AndersenBaseAnalyze()

    def AndersenBaseSolveAndwritePtsToFile(self, filename):
        self.Andersen.AndersenBaseSolveAndwritePtsToFile(filename)


    def AndersenBaseReadPtsFromFile(self, filename):
        self.Andersen.AndersenBaseReadPtsFromFile(filename)

    def AndersenBaseSolveAndwritePtsToFile(self, filename):
        self.Andersen.AndersenBaseSolveAndwritePtsToFile(filename)

    def AndersenBaseSolveConstraints(self):
        self.Andersen.AndersenBaseSolveConstraints()

    def AndersenBaseInitialize(self):
        self.Andersen.AndersenBaseInitialize()

    
    def AndersenBaseFinalize(self):
        self.Andersen.AndersenBaseFinalize()

    def AndersenBaseNormalizePointsTo(self):
        self.Andersen.AndersenBaseNormalizePointsTo()

    def AndersenBaseCleanConsCG(self, id):
        self.Andersen.AndersenBaseInitialize(id)


Andersen = Andersen()
