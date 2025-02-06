
# Source folder
SOURCE_DIR=$PWD
VERSION="0.1.3.dev6"

# git submodule clone, but depth = 1
git submodule update --init --recursive --depth 1


# get pybind path
python3 -m venv venv
source venv/bin/activate
python3 -m pip install pybind11
PYBIND_DIR=$(python3 -m pybind11 --cmakedir)

# Install llvm (now only support MacOS)
sysOS=$(uname)
if [[ $sysOS == "Darwin" ]]
then
  cd $SOURCE_DIR/SVF-npm/SVF-osx/
  source ../llvm_install.sh
  cd $SOURCE_DIR
  mkdir Release-build
  cd Release-build
  cmake -DSVF_DIR=$SOURCE_DIR/SVF-npm/SVF-osx/ -DLLVM_DIR=$LLVM_DIR -DZ3_DIR=$Z3_DIR -DCMAKE_PREFIX_PATH=$PYBIND_DIR ..
  make -j4
  install_name_tool -change /opt/homebrew/opt/z3/lib/libz3.4.13.dylib @rpath/libz3.dylib lib

fi
cd $SOURCE_DIR


echo "SVF_DIR=$SOURCE_DIR/SVF-npm/SVF-osx/"
echo "LLVM_DIR=$LLVM_DIR"
echo "Z3_DIR=$Z3_DIR"
SVF_DIR="$SOURCE_DIR/SVF-npm/SVF-osx/"

PYENV_ROOT="/Users/z5489735/.pyenv/versions/"
PROJECT_DIR=$(pwd)


mkdir -p dist

for PYTHON_DIR in ${PYENV_ROOT}*/bin/python*; do
    PYTHON_BIN="${PYTHON_DIR}"
    if [[ -x "$PYTHON_BIN" ]]; then
        rm -rf build pysvf/SVF pysvf/z3.obj
        $PYTHON_BIN -m pip install -U pip setuptools wheel pybind11
        SVF_DIR=$SVF_DIR LLVM_DIR=$LLVM_DIR Z3_DIR=$Z3_DIR VERSION=$VERSION $PYTHON_BIN setup_mac.py bdist_wheel --plat-name macosx_11_0_arm64

    fi
done