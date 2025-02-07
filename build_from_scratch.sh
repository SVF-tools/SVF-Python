# Source folder
SOURCE_DIR=$PWD
VERSION="0.1.3.dev7"
PYTHON_BIN=""

# git submodule clone, but depth = 1
git submodule init SVF-npm --depth 1
PYBIND_DIR=$($PYTHON_BIN -m pybind11 --cmakedir)

# Install llvm (now only support MacOS)
sysOS=$(uname)
if [[ $sysOS == "Darwin" ]]
then
  cd $SOURCE_DIR/SVF
  source build.sh
  cd $SOURCE_DIR
  mkdir Release-build
  cd Release-build
  cmake -DSVF_DIR=$SOURCE_DIR/SVF-npm/SVF-osx/ -DLLVM_DIR=$LLVM_DIR -DZ3_DIR=$Z3_DIR -DCMAKE_PREFIX_PATH=$PYBIND_DIR ..
  make -j4
fi


echo "SVF_DIR=$SOURCE_DIR/SVF"
echo "LLVM_DIR=$LLVM_DIR"
echo "Z3_DIR=$Z3_DIR"
rm -rf build pysvf/SVF pysvf/z3.obj
"$PYTHON_BIN" -m pip install -U pip setuptools wheel pybind11
SVF_DIR=$SVF_DIR LLVM_DIR=$LLVM_DIR Z3_DIR=$Z3_DIR VERSION=$VERSION BUILD_TYPE="Debug" "$PYTHON_BIN" setup_mac.py bdist_wheel --plat-name macosx_11_0_arm64



