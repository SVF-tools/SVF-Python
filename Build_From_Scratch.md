# Building SVF-Python From Scratch 

## 1. Introduction

If you cannot use the pre-built wheels because of running environment, network connection, or other reasons, you can follow this guide to build SVF-Python from scratch.

## 2. Build SVF-Python

### Pre-requisite

- Python 3.8 - 3.13

### Build

You can build SVF-Python by the following commands. If you want to rebuild it, please clean the cache `build` `dist` `pysvf.egg-info` `pysvf/SVF` directories. `rm -rf build dist pysvf.egg-info pysvf/SVF`.

```angular2html
git clone https://github.com/SVF-tools/SVF-Python.git
cd SVF-Python
git clone https://github.com/SVF-tools/SVF.git SVF --depth 1
bash build_from_scratch.sh
```

### Install

If building is successful, you can install the wheel by pip. The wheel file is in the `dist` folder. You should check the file name first.

```angular2html
# please check the whl file name under dist folder
python3 -m pip install dist/****.whl
```

### Demo

Then you can use the pysvf in your python3 code.
```angular2html
cd test_cases
python3 test.py
```

Unlike the pre-built wheels, it has no dependency installation process. You should see a bunch of output from ICFGNode from the bitcodes.
```angular2html
.... [Other output
CallICFGNode45 {fun: main{ "ln": 21, "cl": 5, "fl": "src/ae_assert_tests/BASIC_array_2d_0-0.c" }}
   call void @svf_assert(i1 noundef zeroext %cmp), !dbg !47 CallICFGNode: { "ln": 21, "cl": 5, "fl": "src/ae_assert_tests/BASIC_array_2d_0-0.c" }

RetICFGNode46 {fun: main{ "ln": 21, "cl": 5, "fl": "src/ae_assert_tests/BASIC_array_2d_0-0.c" }}
   call void @svf_assert(i1 noundef zeroext %cmp), !dbg !47 RetICFGNode: { "ln": 21, "cl": 5, "fl": "src/ae_assert_tests/BASIC_array_2d_0-0.c" }

IntraICFGNode47 {fun: main{ "ln": 22, "cl": 5, "fl": "src/ae_assert_tests/BASIC_array_2d_0-0.c" }}
   ret i32 0, !dbg !48 { "ln": 22, "cl": 5, "fl": "src/ae_assert_tests/BASIC_array_2d_0-0.c" }

```


