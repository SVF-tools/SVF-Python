import svfModule_pybind

class AndersenWaveDiff(object):
    AndersenWaveDiff = svfModule_pybind

    def createAndersenWaveDiff(self):
        self.AndersenWaveDiff.createAndersenWaveDiff()

    def releaseAndersenWaveDiff(self):
        self.AndersenWaveDiff.releaseAndersenWaveDiff()

    def AndersenWaveDiffInitialize(self):
        self.AndersenWaveDiff.AndersenWaveDiffInitialize()


    def AndersenWaveDiffSolveWorklist(self):
        self.AndersenWaveDiff.AndersenWaveDiffSolveWorklist()

    def AndersenWaveDiffProcessNode(self, nodeId):
        self.AndersenWaveDiff.AndersenWaveDiffProcessNode(nodeId)

    def AndersenWaveDiffPostProcessNode(self, nodeId):
        self.AndersenWaveDiff.AndersenWaveDiffPostProcessNode(nodeId)

    def AndersenWaveDiffHandleLoad(self, id, load):
        return self.AndersenWaveDiff.AndersenWaveDiffHandleLoad(id, load)
    
    def AndersenWaveDiffHandleStore(self, id, store):
        return self.AndersenWaveDiff.AndersenWaveDiffHandleStore(id, store)

AndersenWaveDiff = AndersenWaveDiff()
