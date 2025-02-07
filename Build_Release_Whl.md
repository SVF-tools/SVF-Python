# Building SVF-Python Release Wheel

## 1. Introduction

This is for SVF maintainers only. If you are interested in building SVF-Python from scratch, please read [Build From Scratch](./Build_From_Scratch.md). 

## 2. For Building SVF-Python
### Pre-requisite

- Python 3.8 - 3.13 (highly recommend using pyenv to maintain multiple Python versions.)
- Pypi API token (for publishing)

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

You should have a pypi API token to publish the library. And make sure the size of whl file is less than 100MB.
