import svfModule_pybind


class SVFG(object):
    svfg = svfModule_pybind

    def delete(self):
        self.svfg.deleteSvfg()


SVFG = SVFG()
