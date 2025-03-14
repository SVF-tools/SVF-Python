# Pysvf: the Python bindings of SVF

## 1. Introduction
This is a Python binding of the [SVF](https://github.com/SVF-tools/SVF). 

- If you are interested in using SVF by Python, please install the SVF-Python by the following command in (Chapter 2. Install SVF-Python).

- If you are interested in building SVF-Python from scratch, please read [Build From Scratch](./Build_From_Scratch.md). If you cannot use
  the pre-built wheels, you can follow this guide.

- If you are publishing a new release of SVF-Python, please read [Build Release Whl](./Build_Release_Whl.md). [For SVF maintainers only]

## 2. Install SVF-Python

### Pre-requisite

- Python 3.8 - 3.11

### Install

install pysvf by pip
```angular2html
python3 -m pip install  -i https://test.pypi.org/simple/ pysvf
```

### Demo

Then you can use the pysvf in your python3 code.
```angular2html
git clone https://github.com/SVF-tools/SVF-Python.git
cd SVF-Python/test_cases
python3 test.py
```

Then you should see a bunch of output from ICFGNode from the bitcodes.
```angular2html
.... [Other output
CallICFGNode45 {fun: main{ "ln": 21, "cl": 5, "fl": "src/ae_assert_tests/BASIC_array_2d_0-0.c" }}
   call void @svf_assert(i1 noundef zeroext %cmp), !dbg !47 CallICFGNode: { "ln": 21, "cl": 5, "fl": "src/ae_assert_tests/BASIC_array_2d_0-0.c" }

RetICFGNode46 {fun: main{ "ln": 21, "cl": 5, "fl": "src/ae_assert_tests/BASIC_array_2d_0-0.c" }}
   call void @svf_assert(i1 noundef zeroext %cmp), !dbg !47 RetICFGNode: { "ln": 21, "cl": 5, "fl": "src/ae_assert_tests/BASIC_array_2d_0-0.c" }

IntraICFGNode47 {fun: main{ "ln": 22, "cl": 5, "fl": "src/ae_assert_tests/BASIC_array_2d_0-0.c" }}
   ret i32 0, !dbg !48 { "ln": 22, "cl": 5, "fl": "src/ae_assert_tests/BASIC_array_2d_0-0.c" }

```