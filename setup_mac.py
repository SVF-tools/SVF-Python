from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension, build_ext
import os
import subprocess
import platform
import shutil

svf_dir = os.path.abspath(os.path.dirname(__file__))

# get llvm include dir and lib dir
llvm_include_dir = subprocess.check_output(["llvm-config", "--includedir"]).decode("utf-8").strip()
llvm_lib_dir = subprocess.check_output(["llvm-config", "--libdir"]).decode("utf-8").strip()

# macOS specific settings
if platform.system() == "Darwin":
    z3_lib = "z3.obj/bin/libz3.dylib"
    llvm_lib = "/opt/homebrew/Cellar/llvm@16/16.0.6_1/lib/libLLVM.dylib"
    force_load_flag = "-Wl,-force_load"
    strip_cmd = "strip -x"
else:
    raise RuntimeError("Only macOS is supported in this setup.py")

lib_dest = os.path.join("pysvf", "libs")
os.makedirs(lib_dest, exist_ok=True)

# Copy static library `.a` files
a_files = ["libSvfCore.a", "libSvfLLVM.a"]
for a_file in a_files:
    a_src = os.path.join("Release-build/lib", a_file)
    a_dst = os.path.join(lib_dest, a_file)
    if os.path.exists(a_src):
        shutil.copy(a_src, a_dst)
        print(f"Copied {a_src} to {a_dst}")
        # strip .a file (reduce size)
        subprocess.run(f"{strip_cmd} {a_dst}", shell=True, check=False)
    else:
        raise FileNotFoundError(f"{a_src} not found!")

# copy extapi.bc
extapi_bc_src = os.path.join("Release-build/lib", "extapi.bc")
extapi_bc_dst = os.path.join(lib_dest, "extapi.bc")
shutil.copy(extapi_bc_src, extapi_bc_dst)
# Copy `libz3.dylib`, but not `libLLVM.dylib`
shutil.copy(z3_lib, os.path.join(lib_dest, "libz3.dylib"))
# Strip `libz3.dylib`
subprocess.run(f"{strip_cmd} {os.path.join(lib_dest, 'libz3.dylib')}", shell=True, check=False)


ext_modules = [
    Pybind11Extension(
        "pysvf.pysvf",
        ["pybind/svf_pybind.cpp"],
        include_dirs=["SVF/svf/include", "SVF/svf-llvm/include", "Release-build/SVF/include", llvm_include_dir],
        library_dirs=[lib_dest, llvm_lib_dir],
        libraries=["LLVM", "z3"],
        extra_link_args=[
            "-Wl,-rpath,@loader_path/libs",
            "-Wl,-rpath," + llvm_lib_dir,
            f"{force_load_flag},{os.path.join(lib_dest, 'libSvfCore.a')}",
            f"{force_load_flag},{os.path.join(lib_dest, 'libSvfLLVM.a')}",
        ],
    ),
]

setup(
    name="pysvf",
    version="0.1.3.dev5",
    author="Your Name",
    description="SVF with Python bindings",
    packages=find_packages(),
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    package_data={"pysvf": ["libs/*"]},
    include_package_data=True,
)