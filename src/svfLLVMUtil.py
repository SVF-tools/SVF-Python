#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import ctypes

import os
import sys

if(sys.platform ==  'linux'):
    libsvfLLVMUtil = ctypes.cdll.LoadLibrary(os.getcwd() + '/src/svfLLVMUtil/libsvfLLVMUtil.so')
elif(sys.platform == 'darwin'):
    libsvfLLVMUtil = ctypes.cdll.LoadLibrary(os.getcwd() + '/src/svfLLVMUtil/libsvfLLVMUtil.dylib')


class LLVMUtil(object):
    global libsvfLLVMUtil
    LLVMUtil = libsvfLLVMUtil
    def processArguments(self, argv, moduleNameVec):
        select = (ctypes.c_char_p * len(argv))()
        for key, item in enumerate(argv):
            select[key] = item.encode('utf-8')
        self.LLVMUtil.processArguments(len(argv), select)
        num =  self.LLVMUtil.getModuleNameVecLen()
        i = 0
        while i < num:
            index = ctypes.c_int(i)
            s = self.LLVMUtil.getModuleNameVecItem(index)
            moduleNameVec.append(str(ctypes.c_char_p(s).value.decode("utf-8")))
            i = i + 1
        print(moduleNameVec)


LLVMUtil = LLVMUtil()