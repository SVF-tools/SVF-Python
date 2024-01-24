from .package import libSvfModule
from .Andersen import Andersen


class AndersenWaveDiff(object):
    andersenWaveDiff = libSvfModule

    def createAndersenWaveDiff(self, pag):
        self.andersenWaveDiff.createAndersenWaveDiff()
        return Andersen

    def releaseAndersenWaveDiff(self):
        self.andersenWaveDiff.releaseAndersenWaveDiff()


AndersenWaveDiff = AndersenWaveDiff()
