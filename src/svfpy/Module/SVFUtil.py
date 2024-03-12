import svfModule_pybind


class SVFUtil(object):
    svfUtil = svfModule_pybind

    def processArguments(self, argv):
        select = [0] * len(argv)
        for key, item in enumerate(argv):
            select[key] = item.encode('utf-8')
        self.svfUtil.processArguments(len(argv), select)


SVFUtil = SVFUtil()
