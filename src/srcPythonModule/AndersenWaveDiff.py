import svfModule_pybind
from .Andersen import Andersen


class AndersenWaveDiff(object):
    andersenWaveDiff = svfModule_pybind

    def createAndersenWaveDiff(self, pag):
        self.andersenWaveDiff.createAndersenWaveDiff()
        return Andersen

    def releaseAndersenWaveDiff(self):
        self.andersenWaveDiff.releaseAndersenWaveDiff()


AndersenWaveDiff = AndersenWaveDiff()
