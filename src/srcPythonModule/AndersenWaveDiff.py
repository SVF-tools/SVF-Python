import svfModule_pybind

class AndersenWaveDiff(object):
    AndersenWaveDiff = svfModule_pybind

    def createAndersenWaveDiff(self):
        self.AndersenWaveDiff.createAndersenWaveDiff()

    def releaseAndersenWaveDiff(self):
        self.AndersenWaveDiff.releaseAndersenWaveDiff()


AndersenWaveDiff = AndersenWaveDiff()
