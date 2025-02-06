# Pysvf: the Python bindings of SVF

## Introduction

This is a Python binding of the [SVF](https://github.com/SVF-tools/SVF).

## For Building SVF-Python
### Pre-requisite

- Python 3.x
- You can compile SVF from the source code
- 


### Installation

Make sure you have installed the Pybind.

```angular2html
pip install pybind11 setuptools wheel
```

Then clone the submodules and build the SVF-Python.

```angular2html
git clone https://github.com/SVF-tools/SVF-Python.git
cd SVF-Python
git submodule update --init --recursive
```

Then cmake and build the SVF-Python.
We should make sure the some environment variables are set correctly.

- `LLVM_DIR`: same to the `LLVM_DIR` in SVF
- `CMAKE_PREFIX_PATH`: You should try to find out the pybind installation dir and get its cmake toolkit folder. You can run `python -m pybind11 --cmakedir`. For example, the path maybe `~/.pyenv/versions/3.12.6/lib/python3.12/site-packages/pybind11/share/cmake/pybind11`

Then you should replace the cmake variables in the following command.
```angular2html
mkdir Release-build
cd Release-build
cmake -DLLVM_DIR=/path/to/LLVM_DIR -DCMAKE_PREFIX_PATH=/path/to/pybind11/cmake/ ..
```

Build the SVF-Python.

```angular2html
make -j20
```

Then run the setup tools to build pip wheel.
```angular2html
cd ..
python setup.py bdist_wheel
```

Then you can install the wheel by pip. The wheel file is in the `dist` folder. You should check the file name first.
```angular2html
pip install dist/****.whl
```

### Release to Pypi
You can upload to test pypi by the following command.
```angular2html
python -m twine upload --repository testpypi dist/* --verbose
```

If test is ok, you can upload to the real pypi by the following command.
```angular2html
python -m twine upload dist/* --verbose
```

## For SVF-Python Users

If you want to use the pysvf from pypi, you can run the following command. (If you installed the wheel from the above steps, you can ignore it.)
```angular2html
python3 -m pip install  -i https://test.pypi.org/simple/ pysvf
```

Then you can use the pysvf in your python code.
```angular2html
cd test_cases
python test.py
```

You should see a bunch of output from ICFGNode from the bitcodes.

