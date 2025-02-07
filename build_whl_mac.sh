
# Source folder
SOURCE_DIR=$PWD
VERSION="0.1.3.dev7"

# git submodule clone, but depth = 1
git clone https://github.com/SVF-tools/SVF-npm SVF-npm --depth 1


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
  # if Release-build exists, remove it
  if [ -d "Release-build" ]; then
    rm -rf Release-build
  fi
  mkdir Release-build
  cd Release-build
  cmake -DSVF_DIR=$SOURCE_DIR/SVF-npm/SVF-osx/ -DLLVM_DIR=$LLVM_DIR -DZ3_DIR=$Z3_DIR -DCMAKE_PREFIX_PATH=$PYBIND_DIR ..
  make -j4
fi

cd $SOURCE_DIR


echo "SVF_DIR=$SOURCE_DIR/SVF-npm/SVF-osx/"
echo "LLVM_DIR=$LLVM_DIR"
echo "Z3_DIR=$Z3_DIR"
SVF_DIR="$SOURCE_DIR/SVF-npm/SVF-osx/"

PYENV_ROOT="/Users/z5489735/.pyenv"
PROJECT_DIR=$(pwd)


mkdir -p dist

# 定义 Python 版本列表
PYTHON_VERSIONS=("3.10.13" "3.11.5"  "3.12.1"  "3.12.6"  "3.13.0"  "3.8.16"  "3.9.18")

# 遍历每个版本
for VERSION in "${PYTHON_VERSIONS[@]}"; do
    PYTHON_BIN="$PYENV_ROOT/versions/$VERSION/bin/python"
    echo "Building for $PYTHON_BIN"
    if [ -f "$PYTHON_BIN" ] && [ -x "$PYTHON_BIN" ]; then
        echo "Building for $PYTHON_BIN"

        # 清理构建目录
        rm -rf build pysvf/SVF pysvf/z3.obj

        # 更新 pip 和安装依赖
        "$PYTHON_BIN" -m pip install -U pip setuptools wheel pybind11

        # 设置环境变量并执行构建
        SVF_DIR=$SVF_DIR LLVM_DIR=$LLVM_DIR Z3_DIR=$Z3_DIR VERSION=$VERSION BUILD_TYPE="Release" "$PYTHON_BIN" setup_mac.py bdist_wheel --plat-name macosx_11_0_arm64
    else
        echo "Python binary not found or not executable for version $VERSION"
    fi
done