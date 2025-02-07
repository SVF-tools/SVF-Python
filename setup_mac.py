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

if z3_dir == "" or llvm_dir == "" or SVF_DIR == "":
    raise RuntimeError("Please set Z3_DIR, LLVM_DIR or SVF_DIR environment variable")

# dst
dst = os.path.join(svf_dir, "pysvf")
# Copy all files in SVF_DIR to dst (including SVF_DIR itself)
svf_dst = os.path.join(dst, "SVF")
shutil.copytree(SVF_DIR, svf_dst, dirs_exist_ok=True)

# Copy all file in Z3_DIR to dst (including Z3_DIR itself)
z3_dst = os.path.join(dst, "SVF/z3.obj")
shutil.copytree(z3_dir, z3_dst, dirs_exist_ok=True)

# we do not copy LLVM because it is too huge

force_load_flag = "-Wl,-force_load"

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

def get_site_packages_path():
    if hasattr(site, 'getsitepackages'):
        return site.getsitepackages()[0]  # 全局 Python
    else:
        return os.path.join(sys.prefix, "lib", f"python{sys.version_info.major}.{sys.version_info.minor}", "site-packages")  # `venv`


class CustomInstall(install):
    def run(self):
        # 先执行默认安装流程
        install.run(self)
        if "bdist_wheel" in sys.argv:
            print("Skipping installation script during bdist_wheel build")
            return

        # 获取 `site-packages` 目录路径
        site_packages_path = get_site_packages_path()
        pysvf_path = os.path.join(site_packages_path, "pysvf")

        print(f"Running installation script inside {pysvf_path}")

        # 定义 Bash 脚本，确保在 `site-packages/mypackage/` 目录下执行
        bash_script = f"""
           cd {pysvf_path}
           # if exist  {pysvf_path}/SVF, rm it
           if [ -d "SVF" ]; then rm -rf SVF; fi
           git clone https://github.com/SVF-tools/SVF-npm.git --depth 1
           cd SVF-npm/SVF-osx/Release-build/
           bash ../../llvm_install.sh
           cd {pysvf_path}
           mv SVF-npm/SVF-osx SVF
           rm -rf SVF-npm
        """

        print("Running shell script to install dependencies...")
        process = subprocess.run(["/bin/bash", "-c", bash_script], check=True, text=True, capture_output=True)

        if process.returncode == 0:
            print("Bash script executed successfully.")
        else:
            print("Bash script execution failed.")
            exit(1)

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
        "install": CustomInstall
    },
    include_package_data=True,
)