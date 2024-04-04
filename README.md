## 0. Install prerequisite libs
### Install on Linux
```
sudo apt-get install zlib1g-dev unzip cmake gcc g++ nodejs libtinfo5 python3 npm python3-pybind11
```
### Install on MacOS
```
brew install zlib-ng unzip cmake gcc nodejs npm pybind11
```
### Install on Conda Environment
```
conda install zlib-ng unzip cmake nodejs pybind11
```

## 1. Install SVF and its dependence (LLVM pre-built binary) via npm
```
npm i --silent svf-tools --prefix {desired_download_path} 
```
### Example
```
npm i --silent svf-tools --prefix /Users/ABB/Documents/SVF/ 
```

## 2. Build SVF
```
cd {desired_download_path}/node_modules/SVF
source ./build.sh
```

## 3. Clone SVF-Python repository
```
git clone https://github.com/SVF-tools/SVF-Python
```

## 4. Setup SVF environment and create python lib
```
cd SVF-Python
source ./env.sh
cmake . && make
```
## 5. Analyse a LLVM IR file using binded SVF python modules
```
clang -S -c -g -fno-discard-value-names -emit-llvm example.c -o example.ll
```
### AE:
```
python3 python_modules/python_aeModule.py example.ll
```
### CFL:
```
python3 python_modules/python_cflModule.py example.ll
```
### DDA:
```
python3 python_modules/python_ddaModule.py example.ll
```
### LLVM2SVF:
```
python3 python_modules/python_llvm2svfModule.py example.ll
```
### MTA:
```
python3 python_modules/python_mtaModule.py example.ll
```
### SABER:
```
python3 python_modules/python_saberModule.py example.ll
```
### WPA:
```
python3 python_modules/python_wpaModule.py -ander -svfg example.ll
```
#### Flags can be customised accordingly for WPA