# Pysvf: the Python bindings of SVF

## 1. Introduction
This is a Python binding of the [SVF](https://github.com/SVF-tools/SVF). 

- If you are interested in using SVF by Python, please install the SVF-Python by the following command in Chapter 2.

- If you are interested in building SVF-Python from scratch, please read [Build From Scratch](./Build_From_Scratch.md). If you cannot use
  the pre-built wheels, you can follow this guide.

- If you are publishing a new release of SVF-Python, please read [Build Release Whl](./Build_Release_Whl.md). [For SVF maintainers only]

## 2. Install SVF-Python

### Pre-requisite

- Python 3.8 - 3.13

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

If you are using pysvf for the first time after installation. you may see the following output.  
```angular2html
SVF/Z3/LLVM dependencies not found.
Running dependency installation process...
Starting SVF/Z3/LLVM dependency installation...
[1/5] Checking and removing existing SVF directory...
[2/5] Cloning SVF repository...
....[Dependency configuration output]
[4/5] Building SVF...
[5/5] Finalizing installation...
SVF_DIR: ~/.pyenv/versions/3.11.5/lib/python3.11/site-packages/pysvf/SVF
LLVM_DIR: ~/.pyenv/versions/3.11.5/lib/python3.11/site-packages/pysvf/SVF/llvm-16.0.0.obj
Z3_DIR: ~/.pyenv/versions/3.11.5/lib/python3.11/site-packages/pysvf/SVF/z3.obj
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