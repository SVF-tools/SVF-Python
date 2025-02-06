# Pysvf: the Python bindings of SVF

## Introduction

This is a Python binding of the [SVF](https://github.com/SVF-tools/SVF).

## For Building SVF-Python
### Pre-requisite

- Python 3.x (highly recommend using pyenv to maintain multiple Python versions


Then you should install pyenv to maintain multiple Python versions. You can follow the following steps to install pyenv.
### Install pyenv

1. **Install dependencies** (ensure you have the necessary build tools):

   ```bash
   # macOS
   brew update
   brew install openssl readline sqlite3 xz zlib tcl-tk
   ```

2. **Install pyenv**:

   ```bash
   brew install pyenv
   ```

3. **Configure your shell environment**:

   Add the following to your shell configuration file (e.g., `~/.bashrc` or `~/.zshrc`):

   ```bash
   export PYENV_ROOT="$HOME/.pyenv"
   export PATH="$PYENV_ROOT/bin:$PATH"
   eval "$(pyenv init --path)"
   eval "$(pyenv init -)"
   ```

   Then reload your configuration file:

   ```bash
   source ~/.bashrc  # or source ~/.zshrc
   ```

### Install multiple Python versions

1. **List available Python versions**:

   ```bash
   pyenv install --list
   ```

2. **Install the required Python versions (3.8 to 3.13)**:

   ```bash
   pyenv install 3.8.12
   pyenv install 3.9.7
   pyenv install 3.10.2
   pyenv install 3.11.5
   pyenv install 3.12.0
   pyenv install 3.13.0
   ```

This will set up `pyenv` and install Python versions 3.8 through 3.13. You can then use your script to build your project for each version.



### Installation

Make sure you have installed the Pybind.

```angular2html
python3 -m pip install pybind11 setuptools wheel
```

Then clone the submodules and build the SVF-Python. (set depth 1 to speed up the clone

```angular2html
git clone https://github.com/SVF-tools/SVF-Python.git
cd SVF-Python
git submodule update --init --recursive --depth 1
```

###  Build the Wheels for multiple Python versions

Then you should replace the cmake variables in the following command.
```angular2html
bash build_whl_mac.sh
```

Then you can install the wheel by pip. The wheel file is in the `dist` folder. You should check the file name first.
```angular2html
python3 -m pip install dist/****.whl
```

### Publish pysvf library to Pypi
You can upload to test pypi by the following command.
```angular2html
python3 -m twine upload --repository testpypi dist/* --verbose
```

If test is ok, you can upload to the real pypi by the following command.
```angular2html
python3 -m twine upload dist/* --verbose
```

## For SVF-Python Users

### Pre-requisite

- Python 3.x
- LLVM 16

For Mac User, you can install LLVM 16 by the following command.
```angular2html
brew install llvm@16
```

### Installation and Running

If you want to use the pysvf from pypi, you can run the following command. (If you installed the wheel from the above steps, you can ignore it.)
```angular2html
python3 -m pip install  -i https://test.pypi.org/simple/ pysvf
```

Then you can use the pysvf in your python3 code.
```angular2html
cd test_cases
python3 test.py
```

You should see a bunch of output from ICFGNode from the bitcodes.

