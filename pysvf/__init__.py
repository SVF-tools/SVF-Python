import os
import sys
import subprocess

CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))

EXTAPI_BC_PATH = os.path.join(CURRENT_DIR, "SVF/Release-build/lib/", "extapi.bc")
SVF_DIR = os.path.join(CURRENT_DIR, "SVF")
Z3_DIR = os.path.join(CURRENT_DIR, "SVF/z3.obj")
LLVM_DIR = os.path.join(CURRENT_DIR, "SVF/llvm-16.0.0.obj")

# Set environment variables
os.environ["Z3_DIR"] = Z3_DIR
os.environ["SVF_DIR"] = SVF_DIR
os.environ["LLVM_DIR"] = LLVM_DIR
os.environ["SVF_EXTAPI_DIR"] = os.path.join(SVF_DIR, "Release-build/lib")

def dependencies_exist():
    """Check if SVF dependencies already exist"""
    return os.path.isdir(SVF_DIR) and os.path.isdir(Z3_DIR)

def install_svf():
    """Automatically install SVF dependencies if missing, with failure handling"""
    pysvf_path = CURRENT_DIR

    bash_script = f"""
    echo "Starting SVF/Z3/LLVM dependency installation..."
    
    cd {pysvf_path} || {{ echo "Error: Unable to access {pysvf_path}"; exit 1; }}

    echo "[1/5] Checking and removing existing SVF directory..."
    if [ -d "SVF" ]; then rm -rf SVF; fi || {{ echo "Error: Failed to remove old SVF directory"; exit 1; }}

    echo "[2/5] Cloning SVF repository..."
    git clone https://github.com/SVF-tools/SVF-npm.git --depth 1 || {{ echo "Error: Failed to clone SVF repository"; exit 1; }}

    echo "[3/5] Navigating to build directory..."
    cd SVF-npm/SVF-osx/Release-build/ || {{ echo "Error: Build directory not found"; exit 1; }}

    echo "[4/5] Running LLVM installation script..."
    bash ../../llvm_install.sh || {{ echo "Error: LLVM installation script failed"; exit 1; }}

    echo "[5/5] Finalizing installation..."
    cd {pysvf_path} || {{ echo "Error: Unable to return to {pysvf_path}"; exit 1; }}
    mv SVF-npm/SVF-osx SVF || {{ echo "Error: Failed to move SVF directory"; exit 1; }}
    rm -rf SVF-npm || {{ echo "Error: Failed to clean up SVF-npm directory"; exit 1; }}

    echo "SVF/Z3/LLVM dependencies installed successfully!"
    """

    print("Running dependency installation process...")
    process = subprocess.run(["/bin/bash", "-c", bash_script], text=True, stdout=sys.stdout, stderr=sys.stderr)

    if process.returncode == 0:
        print("SVF/Z3/LLVM dependencies installation completed successfully.")
    else:
        print(f"Installation failed! Error log:\n{process.stderr}")
        print("Please check your internet connection, available disk space, and ensure dependencies can be built.")
        exit(1)

# 1️⃣ **Check if dependencies are already installed**
# if dependencies_exist():
#     print("SVF/Z3/LLVM dependencies exist.")
# else:
#     print("SVF/Z3/LLVM dependencies not found.")
#     install_svf()

def force_update_deps():
    """Force update SVF dependencies by deleting the existing SVF folder and reinstalling"""
    if os.path.exists(SVF_DIR):
        print(f"Removing existing SVF directory: {SVF_DIR}")
        try:
            # Remove the SVF directory
            subprocess.run(["rm", "-rf", SVF_DIR], check=True)
            print("Existing SVF directory removed successfully.")
        except subprocess.CalledProcessError as e:
            print(f"Error: Failed to remove SVF directory: {e}")
            exit(1)
    else:
        print("No existing SVF directory found. Skipping removal.")

    # Call install_svf to reinstall dependencies
    print("Reinstalling SVF dependencies...")
    install_svf()


# 2️⃣ **Import `pysvf` only after dependencies are confirmed**
from .pysvf import *

# Print environment paths for verification
print(f"SVF_DIR: {SVF_DIR}")
print(f"LLVM_DIR: {LLVM_DIR}")
print(f"Z3_DIR: {Z3_DIR}")
__all__ = ["force_update_deps"]
