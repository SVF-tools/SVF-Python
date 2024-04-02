import svfModule_pybind

class MTA(object):
    MTA = svfModule_pybind

    def MTARunOnModule(self):
        self.MTA.MTARunOnModule()
    
    def MTAValidator(self):
        self.MTA.MTAValidator()
    
    def lockValidator(self):
        self.MTA.lockValidator()


MTA = MTA()
