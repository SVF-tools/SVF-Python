import svfModule_pybind

class WPA(object):
    WPA = svfModule_pybind

    def WPAPassRunOnModule(self):
        self.WPA.WPAPassRunOnModule()

WPA = WPA()