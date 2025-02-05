from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext
import os
import subprocess
import platform
import shutil

svf_dir = os.path.abspath(os.path.dirname(__file__))

# 获取 LLVM 头文件和库文件路径
llvm_include_dir = subprocess.check_output(["llvm-config", "--includedir"]).decode("utf-8").strip()
llvm_lib_dir = subprocess.check_output(["llvm-config", "--libdir"]).decode("utf-8").strip()

# macOS 特定设置
if platform.system() == "Darwin":
    z3_lib = "/opt/homebrew/lib/libz3.dylib"
    llvm_lib = "/opt/homebrew/Cellar/llvm@16/16.0.6_1/lib/libLLVM.dylib"  # 不复制它
    force_load_flag = "-Wl,-force_load"
    strip_cmd = "strip -x"  # macOS 下 `strip -x` 只删除符号，不影响动态库可用性
else:
    raise RuntimeError("Only macOS is supported in this setup.py")

lib_dest = os.path.join("pysvf", "libs")
os.makedirs(lib_dest, exist_ok=True)

# 复制静态库 `.a` 文件
a_files = ["libSvfCore.a", "libSvfLLVM.a"]
for a_file in a_files:
    a_src = os.path.join("Release-build/lib", a_file)
    a_dst = os.path.join(lib_dest, a_file)
    if os.path.exists(a_src):
        shutil.copy(a_src, a_dst)
        print(f"Copied {a_src} to {a_dst}")
        # Strip `.a` 文件（减少体积）
        subprocess.run(f"{strip_cmd} {a_dst}", shell=True, check=False)
    else:
        raise FileNotFoundError(f"{a_src} not found!")

# 复制 `libz3.dylib`，但不复制 `libLLVM.dylib`
shutil.copy(z3_lib, os.path.join(lib_dest, "libz3.dylib"))
# Strip `libz3.dylib`
subprocess.run(f"{strip_cmd} {os.path.join(lib_dest, 'libz3.dylib')}", shell=True, check=False)

ext_modules = [
    Pybind11Extension(
        "pysvf",
        ["svf-python/pag_pybind.cpp"],
        include_dirs=[
            os.path.join(svf_dir, "svf/include"),
            os.path.join(svf_dir, "svf-llvm/include"),
            os.path.join(svf_dir, "Release-build/include"),
            llvm_include_dir
        ],
        library_dirs=[lib_dest, llvm_lib_dir],
        libraries=["LLVM", "z3"],  # 仍然链接 libLLVM.dylib，但不复制
        extra_link_args=[
            "-Wl,-rpath,@loader_path/libs",  # 运行时找到 libz3.dylib
            "-Wl,-rpath," + llvm_lib_dir,    # 运行时找到 libLLVM.dylib（外部安装）
            f"{force_load_flag},{os.path.join(lib_dest, 'libSvfCore.a')}",
            f"{force_load_flag},{os.path.join(lib_dest, 'libSvfLLVM.a')}",
            ],
    ),
]

setup(
    name="pysvf",
    version="0.1.3.dev4",
    author="Your Name",
    description="SVF with Python bindings",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    packages=["pysvf"],
    package_data={"pysvf": ["libs/*"]},
    include_package_data=True,
)