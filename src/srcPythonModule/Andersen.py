import svfModule_pybind

class AndersenBase(object):
    AndersenBase = svfModule_pybind

    # AndersenBase
    # ---------------------------------------------------------------------------
    def AndersenBaseBuild(self):
        self.AndersenBase.AndersenBaseBuild()

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
    # ---------------------------------------------------------------------------



AndersenBase = AndersenBase()


class Andersen(object):
    Andersen = svfModule_pybind
    # Andersen
    # ---------------------------------------------------------------------------
    def AndersenBuild(self):
        self.Andersen.AndersenBuild()

    def AndersenInitialize(self):
        self.Andersen.AndersenInitialize()

    def AndersenFinalize(self):
        self.Andersen.AndersenFinalize()

    def AndersenResetData(self):
        self.Andersen.AndersenResetData()


    def AndersenSccRepNode(self, id):
        return self.Andersen.AndersenSccRepNode(id)

    def AndersenSccSubNodes(self, repId):
        return self.Andersen.AndersenSccSubNodes(repId)
    
    def AndersenGetPts(self, id):
        return self.Andersen.AndersenGetPts(id)


    def AndersenUnionPts(self, id, target):
        return self.Andersen.AndersenUnionPtsPointsTo(id, target)
    
    def AndersenUnionPts(self, id, ptd):
        return self.Andersen.AndersenUnionPtsNodeID(id, ptd)


    def AndersenDumpTopLevelPtsTo(self):
        self.Andersen.AndersenDumpTopLevelPtsTo()

    def AndersenSetDetectPWC(self, flag):
        self.Andersen.AndersenSetDetectPWC(flag)
    # ---------------------------------------------------------------------------

Andersen = Andersen()