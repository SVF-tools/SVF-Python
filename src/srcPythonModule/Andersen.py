import svfModule_pybind

class AndersenBase(object):
    AndersenBase = svfModule_pybind

    def AndersenBaseAnalyze(self):
        self.AndersenBase.AndersenBaseAnalyze()

    def AndersenBaseSolveAndwritePtsToFile(self, filename):
        self.AndersenBase.AndersenBaseSolveAndwritePtsToFile(filename)


    def AndersenBaseReadPtsFromFile(self, filename):
        self.AndersenBase.AndersenBaseReadPtsFromFile(filename)

    def AndersenBaseSolveAndwritePtsToFile(self, filename):
        self.AndersenBase.AndersenBaseSolveAndwritePtsToFile(filename)

    def AndersenBaseSolveConstraints(self):
        self.AndersenBase.AndersenBaseSolveConstraints()

    def AndersenBaseInitialize(self):
        self.AndersenBase.AndersenBaseInitialize()

    
    def AndersenBaseFinalize(self):
        self.AndersenBase.AndersenBaseFinalize()

    def AndersenBaseNormalizePointsTo(self):
        self.AndersenBase.AndersenBaseNormalizePointsTo()

    def AndersenBaseCleanConsCG(self, id):
        self.AndersenBase.AndersenBaseInitialize(id)


AndersenBase = AndersenBase()


class Andersen(object):
