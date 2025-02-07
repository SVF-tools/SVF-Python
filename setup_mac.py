from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension, build_ext
import os
import sys
import site
import subprocess
from setuptools import setup, Extension
import shutil
from setuptools.command.install import install

svf_dir = os.path.abspath(os.path.dirname(__file__))

# get llvm include dir and lib dir
llvm_include_dir = subprocess.check_output(["llvm-config", "--includedir"]).decode("utf-8").strip()
llvm_lib_dir = subprocess.check_output(["llvm-config", "--libdir"]).decode("utf-8").strip()

z3_dir = os.getenv("Z3_DIR", "")
llvm_dir = os.getenv("LLVM_DIR", "")
SVF_DIR = os.getenv("SVF_DIR", "")
VERSION = os.getenv("VERSION", "0.1.0")
BUILD_TYPE= os.getenv("BUILD_TYPE", "Release")

if z3_dir == "" or llvm_dir == "" or SVF_DIR == "":
    raise RuntimeError("Please set Z3_DIR, LLVM_DIR or SVF_DIR environment variable")




# we do not copy LLVM because it is too huge

force_load_flag = "-Wl,-force_load"


def get_site_packages_path():
    if hasattr(site, 'getsitepackages'):
        return site.getsitepackages()[0]  # 全局 Python
    else:
        return os.path.join(sys.prefix, "lib", f"python{sys.version_info.major}.{sys.version_info.minor}", "site-packages")  # `venv`


class CustomBuildExt(build_ext):
    def run(self):
        # Run the standard build_ext first
        super().run()

        # Path to the built shared object file
        ext_path = self.get_ext_fullpath('pysvf')

        # If the module is in a subdirectory, adjust the path
        ext_path = os.path.join(os.path.dirname(ext_path), 'pysvf', os.path.basename(ext_path))

        # Modify the library path
        subprocess.check_call([
            'install_name_tool',
            '-change',
            '/opt/homebrew/opt/z3/lib/libz3.4.13.dylib',
            '@rpath/libz3.dylib',
            ext_path
        ])

        subprocess.check_call([
            'install_name_tool',
            '-change',
            '/opt/homebrew/opt/llvm@16/lib/libLLVM.dylib',
            '@rpath/libLLVM.dylib',
            ext_path
        ])

        subprocess.check_call([
            'install_name_tool',
            '-change',
            '/opt/homebrew/opt/llvm@16/lib/libunwind.1.dylib',
            '@rpath/libunwind.1.dylib',
            ext_path
        ])

if BUILD_TYPE == "Release":
    # dst
    dst = os.path.join(svf_dir, "pysvf")
    # Copy all files in SVF_DIR to dst (including SVF_DIR itself)
    svf_dst = os.path.join(dst, "SVF")
    shutil.copytree(SVF_DIR, svf_dst, dirs_exist_ok=True)

    # Copy all file in Z3_DIR to dst (including Z3_DIR itself)
    z3_dst = os.path.join(dst, "SVF/z3.obj")
    shutil.copytree(z3_dir, z3_dst, dirs_exist_ok=True)

    ext_modules = [
        Pybind11Extension(
            "pysvf.pysvf",
            ["pybind/svf_pybind.cpp"],
            include_dirs=[os.path.join(svf_dst, "Release-build/include"), llvm_include_dir],
            library_dirs=[os.path.join(z3_dst, "lib"), llvm_lib_dir],
            libraries=["LLVM", "z3"],
            extra_link_args=[
                "-Wl,-rpath,@loader_path/SVF/Release-build/lib",
                "-Wl,-rpath,@loader_path/SVF/z3.obj/lib",
                "-Wl,-rpath,@loader_path/SVF/llvm-16.0.0.obj/lib",
                f"{force_load_flag},{os.path.join(svf_dst, 'Release-build/lib', 'libSvfCore.a')}",
                f"{force_load_flag},{os.path.join(svf_dst,  'Release-build/lib', 'libSvfLLVM.a')}",
            ],
        ),
    ]
    setup(
        name="pysvf",
        version=VERSION,
        author="Your Name",
        description="SVF with Python bindings",
        packages=find_packages(),
        ext_modules=ext_modules,
        zip_safe=False,
        package_data={},
        cmdclass={
            'build_ext': CustomBuildExt,
        },
        include_package_data=True,
    )
elif BUILD_TYPE == "Debug":
    # dst
    dst = os.path.join(svf_dir, "pysvf")
    # Copy all files in SVF_DIR to dst (including SVF_DIR itself)
    svf_dst = os.path.join(dst, "SVF")
    shutil.copytree(SVF_DIR, svf_dst, dirs_exist_ok=True)

    z3_dst = os.path.join(svf_dst, "z3.obj")

    print("svf_dst", svf_dst)
    ext_modules = [
        Pybind11Extension(
            "pysvf.pysvf",
            ["pybind/svf_pybind.cpp"],
            include_dirs=[os.path.join(svf_dst, "svf/include"), os.path.join(svf_dst, "svf-llvm/include"), os.path.join(svf_dst, "Release-build/include"), llvm_include_dir],
            library_dirs=[os.path.join(z3_dst, "lib"), llvm_lib_dir],
            libraries=["LLVM", "z3"],
            extra_link_args=[
                "-Wl,-rpath,@loader_path/SVF/Release-build/lib",
                "-Wl,-rpath,@loader_path/SVF/z3.obj/lib",
                "-Wl,-rpath,@loader_path/SVF/llvm-16.0.0.obj/lib",
                f"{force_load_flag},{os.path.join(svf_dst, 'Release-build/lib', 'libSvfCore.a')}",
                f"{force_load_flag},{os.path.join(svf_dst,  'Release-build/lib', 'libSvfLLVM.a')}",
            ],
        ),
    ]
    setup(
        name="pysvf",
        version=VERSION,
        author="Your Name",
        description="SVF with Python bindings",
        packages=find_packages(),
        ext_modules=ext_modules,
        zip_safe=False,
        package_data={"pysvf": ["SVF/Release-build/*"]},
        cmdclass={
        },
        include_package_data=True,
    )