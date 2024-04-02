#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import svfModule_pybind


class svfLLVMUtil(object):
    svfLLVMUtil = svfModule_pybind

    def processArguments(self, argv, moduleNameVec):

        select = [0] * len(argv)
        for key, item in enumerate(argv):
            select[key] = item
        self.svfLLVMUtil.processArguments(len(argv), select)
        num = self.svfLLVMUtil.getModuleNameVecLen()
        i = 0
        while i < num:
            result = str()
            result = self.svfLLVMUtil.getModuleNameVecItem(i, result)
            moduleNameVec.append(result)
            i = i + 1
            



svfLLVMUtil = svfLLVMUtil()
