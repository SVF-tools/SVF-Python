## 0. Install npm, zlib, unzip, cmake, gcc, nodejs... (if you machine does not have these libs)
### Install on Linux
```
sudo apt-get install zlib1g-dev unzip cmake gcc g++ nodejs libtinfo5 python3 npm pybind11
```
### Install on MacOS
```
brew install zlib1g-dev unzip cmake gcc g++ nodejs libtinfo5 python3 npm pybind11
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
cd {desired_download_path}/node_modules/svf-tools
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
## 5. Analyze a bc file using svf-ex.py
```
clang -S -c -g -fno-discard-value-names -emit-llvm example.c -o example.ll
python3 python_svfModule.py example.ll
```