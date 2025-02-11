# setup_linux.py

from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension
import os
import sys
import subprocess
import shutil

# ------------------------------------------------------------------------------
# 0) Basic config and environment variables
# ------------------------------------------------------------------------------
svf_python_dir = os.path.abspath(os.path.dirname(__file__))

# The following calls assume 'llvm-config' is in PATH (or specify full path).
# If needed, you can also parse env vars (e.g., LLVM_DIR) to locate llvm-config.
llvm_dir = os.getenv("LLVM_DIR", "")
llvm_config_dir = os.path.join(llvm_dir, "bin", "llvm-config")
llvm_include_dir = subprocess.check_output([llvm_config_dir, "--includedir"]).decode("utf-8").strip()
llvm_lib_dir     = subprocess.check_output([llvm_config_dir, "--libdir"]).decode("utf-8").strip()

z3_dir  = os.getenv("Z3_DIR", "")
SVF_DIR = os.getenv("SVF_DIR", "")
VERSION = os.getenv("VERSION", "0.1.0")
BUILD_TYPE= os.getenv("BUILD_TYPE", "Release")

if not z3_dir or not llvm_dir or not SVF_DIR:
    raise RuntimeError(
        "Please set Z3_DIR, LLVM_DIR, and SVF_DIR environment variables "
        "before running setup. e.g.:\n"
        "  export Z3_DIR=/path/to/z3\n"
        "  export LLVM_DIR=/path/to/llvm\n"
        "  export SVF_DIR=/path/to/SVF\n"
    )

# ------------------------------------------------------------------------------
# 1) Force-load flags on Linux vs macOS
#    On Linux, the equivalent of mac's '-Wl,-force_load' is using
#    '-Wl,--whole-archive' and '-Wl,--no-whole-archive' around the library.
#    We'll define a small helper here for convenience.
# ------------------------------------------------------------------------------
force_load_args = []

def whole_archive_args(static_lib_path: str):
    """Return the linker args to forcibly link all symbols in a static library."""
    return [
        "-Wl,--whole-archive",
        static_lib_path,
        "-Wl,--no-whole-archive"
    ]

# ------------------------------------------------------------------------------
# 2) Copy SVF + Z3 into your Python package tree if needed
#    (similar to the macOS approach)
# ------------------------------------------------------------------------------
dst = os.path.join(svf_python_dir, "pysvf")
svf_dst = os.path.join(dst, "SVF")

if os.path.exists(svf_dst):
    shutil.rmtree(svf_dst)
shutil.copytree(SVF_DIR, svf_dst, dirs_exist_ok=True)

z3_dst = os.path.join(svf_dst, "z3.obj")
if z3_dir != z3_dst:
    # Copy or merge your Z3 folder similarly
    if os.path.exists(z3_dst):
        shutil.rmtree(z3_dst)
    shutil.copytree(z3_dir, z3_dst, dirs_exist_ok=True)

# ------------------------------------------------------------------------------
# 3) Build extension modules
#    - RPATH on Linux typically uses '$ORIGIN' instead of '@loader_path' (macOS).
#    - We'll create extension modules for Release/Debug as needed.
# ------------------------------------------------------------------------------
if BUILD_TYPE == "Release":
    # example: library artifacts might be in "Release-build/lib"
    svf_lib_dir = os.path.join(svf_dst, "Release-build", "lib")

    extra_link_args = [
        # Adjust RPATH so the .so can find the needed libs in the 'SVF' folder
        "-Wl,-rpath,$ORIGIN/SVF/Release-build/lib",
        "-Wl,-rpath,$ORIGIN/SVF/z3.obj/lib",
        # If you have a separate "llvm-16.0.0.obj" or similar, handle that as needed:
        "-Wl,-rpath,$ORIGIN/SVF/llvm-16.0.0.obj/lib",
    ]
    # forcibly link some static libs so that all symbols are included
    # (if your project requires that). This emulates macOS's -Wl,-force_load.
    svf_core_path  = os.path.join(svf_lib_dir, "libSvfCore.a")
    svf_llvm_path  = os.path.join(svf_lib_dir, "libSvfLLVM.a")
    extra_link_args += whole_archive_args(svf_core_path)
    extra_link_args += whole_archive_args(svf_llvm_path)

    ext_modules = [
        Pybind11Extension(
            "pysvf.pysvf",
            sources=["pybind/svf_pybind.cpp"],   # your pybind code
            include_dirs=[
                os.path.join(svf_dst, "Release-build", "include"),
                llvm_include_dir
            ],
            library_dirs=[os.path.join(z3_dst, "lib"), llvm_lib_dir],
            libraries=["LLVM", "z3"],  # link to your needed libraries
            extra_link_args=extra_link_args,
        ),
    ]

    setup(
        name="pysvf",
        version=VERSION,
        author="Your Name",
        description="SVF with Python bindings (Linux Release build)",
        packages=find_packages(),
        ext_modules=ext_modules,
        zip_safe=False,
        include_package_data=True,
    )

else:
    # Debug build as an example
    svf_lib_dir = os.path.join(svf_dst, "Release-build", "lib")  # or "Debug-build/lib", adjust as you see fit
    dst = os.path.join(svf_python_dir, "pysvf")
    # Copy all files in SVF_DIR to dst (including SVF_DIR itself)
    svf_dst = os.path.join(dst, "SVF")
    shutil.copytree(SVF_DIR, svf_dst, dirs_exist_ok=True, ignore=lambda directory, files: ['.git'] if '.git' in files else [])
    # Copy z3.obj
    shutil.copytree(os.path.join(SVF_DIR, "z3.obj"), os.path.join(svf_dst, "z3.obj"), dirs_exist_ok=True)
    extra_link_args = [
        "-Wl,-rpath,$ORIGIN/SVF/Release-build/lib",
        "-Wl,-rpath,$ORIGIN/SVF/z3.obj/lib",
    ]
    # forcibly link static libs
    svf_core_path  = os.path.join(svf_lib_dir, "libSvfCore.a")
    svf_llvm_path  = os.path.join(svf_lib_dir, "libSvfLLVM.a")
    extra_link_args += whole_archive_args(svf_core_path)
    extra_link_args += whole_archive_args(svf_llvm_path)

    print("llvm_lib_dir: ", llvm_lib_dir)
    ext_modules = [
        Pybind11Extension(
            "pysvf.pysvf",
            sources=["pybind/svf_pybind.cpp"],
            include_dirs=[
                os.path.join(svf_dst, "svf/include"),
                os.path.join(svf_dst, "svf-llvm/include"),
                os.path.join(svf_dst, "Release-build", "include"),
                llvm_include_dir
            ],
            library_dirs=[os.path.join(z3_dst, "bin"), llvm_lib_dir],
            libraries=["z3", "LLVM", "LLVM-16"],
            extra_link_args=extra_link_args,
        ),
    ]

    setup(
        name="pysvf",
        version=VERSION,
        author="Your Name",
        description="SVF with Python bindings (Linux Debug build)",
        packages=find_packages(),
        ext_modules=ext_modules,
        zip_safe=False,
        package_data={"pysvf": ["SVF/Release-build/**/*", "SVF/z3.obj/**/*"]},
        include_package_data=True,
    )
