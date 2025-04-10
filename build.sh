
SVF_DIR="$SVF_DIR"
LLVM_DIR="$LLVM_DIR"
Z3_DIR="$Z3_DIR"

# if SVF_IR and LLVM_DIR and Z3_DIR not define
if [ -z "$SVF_DIR" ] || [ -z "$LLVM_DIR" ] || [ -z "$Z3_DIR" ]; then
    echo "Please define SVF_DIR, LLVM_DIR, and Z3_DIR."
    exit 1
fi

PYTHON_VERSIONS=("3.8" "3.9" "3.10" "3.11" "3.12" "3.13")

for version in "${PYTHON_VERSIONS[@]}"; do
    rm -rf build
    PYTHON_EXEC="python${version}"
    # if PYTHON_EXEC not exist
    if [ ! -x "$(command -v $PYTHON_EXEC)" ]; then
        echo "Python ${version} not found. Please install pyenv, and install the PYTHON_VERSIONS: ${version}. Then set it as global."
        continue
    fi

    $PYTHON_EXEC -m pip install -U pip pybind11 setuptools wheel

    PYBIND11_DIR=$(${PYTHON_EXEC} -m pybind11 --cmakedir)

    SVF_DIR=${SVF_DIR} LLVM_DIR=${LLVM_DIR} Z3_DIR=${Z3_DIR} PYBIND11_DIR=${PYBIND11_DIR}  ${PYTHON_EXEC} -m build --wheel
    if [ $? -ne 0 ]; then
        echo "Building for Python ${version} failed."
    else
        echo "Successfully built for Python ${version}."
    fi
done
