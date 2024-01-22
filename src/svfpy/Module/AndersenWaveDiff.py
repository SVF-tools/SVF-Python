from .package import libSvfModule
from .Andersen import Andersen

libSvfModule = libSvfModule


class AndersenWaveDiff(object):
    global libSvfModule
    andersenWaveDiff = libSvfModule

    def createAndersenWaveDiff(self, pag):
        self.andersenWaveDiff.createAndersenWaveDiff()
        return Andersen

    def releaseAndersenWaveDiff(self):
        self.andersenWaveDiff.releaseAndersenWaveDiff()


AndersenWaveDiff = AndersenWaveDiff()
