#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import svfModule_pybind


class LLVMUtil(object):
    LLVMUtil = svfModule_pybind

    def processArguments(self, argv, moduleNameVec):

        select = [0] * len(argv)
        for key, item in enumerate(argv):
            select[key] = item
        svfModule_pybind.processArguments(len(argv), select)
        num = svfModule_pybind.getModuleNameVecLen()
        i = 0
        while i < num:
            result = str()
            result = svfModule_pybind.getModuleNameVecItem(i, result)
            moduleNameVec.append(result)
            i = i + 1



LLVMUtil = LLVMUtil()
