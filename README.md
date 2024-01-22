## 0. Install npm, zlib, unzip, cmake, gcc, nodejs (if you machine does not have these libs)
```
sudo apt-get install zlib1g-dev unzip cmake gcc g++ nodejs libtinfo5 python3
```

## 1. Install SVF and its dependence (LLVM pre-built binary) via npm
```
npm i --silent svf-lib --prefix ${HOME}
```

## 2. Clone repository
```
git clone https://github.com/SVF-tools/SVF-example.git
```
## 3. Install repository
```
cd noce_modules/svf-lib
./llvm_install.sh   
```
```
For mac arm64 system replace two files
node_modules/SVF/Release-build/svf/libSvfCore.a
node_modules/SVF/Release-build/svf-llvm/libSvfLLVM.a
to the given file in MacLib respectively
```
## 4. Setup SVF environment and create python lib
```
source ./env.sh
cmake . && make
```
## 5. Analyze a bc file using svf-ex.py
```
clang -S -c -g -fno-discard-value-names -emit-llvm example.c -o example.ll
python3 ./src/svfpy/svf-ex.py example.ll
```
