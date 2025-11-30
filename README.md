# Pysvf: Python Bindings for SVF


## News
* <b>SVF-Python now supports MTA bindings and more pointer analysis strategies(AndersenWaveDiff, AndersenBase, and Steensgaard bindings). (Thank [JoelYYoung](https://github.com/JoelYYoung) for his help!).  </b>

## 1. Introduction

**Pysvf** is a Python binding for the [SVF (Static Value-Flow)](https://github.com/SVF-tools/SVF), a static analysis framework built on LLVM.

## 2. Installation

Pysvf can be installed in two ways:

### Method 1: Install via pip (Test PyPI)

#### Requirements

- Python 3.8 - 3.11
- OS: Linux X86-64, Linux Arm64, MacOS

#### Install Command

```bash
python3 -m pip install -i https://test.pypi.org/simple/ pysvf
````

### Method 2: Build from Source

Clone the repository and run the build script:

```bash
bash build.sh
```

> This will build the SVF and its depedencies like LLVM and Z3, and set up the Python bindings.

## 3. Documentation

Documentation is available on the [SVF-Python Wiki](https://github.com/SVF-tools/SVF-Python/wiki):

* [Pysvf API Reference](https://github.com/SVF-tools/SVF-Python/wiki/PySVF-API)

## 4. Tutorial

Example notebooks are provided in the `tutorial` folder. Before running them, please install Jupyter Notebook:

```bash
python3 -m pip install jupyter
cd demo
jupyter notebook
```

Then you can click one of the following notebooks.

### Example Notebooks

* [PAG](./demo/pag.ipynb): Generate Program Assignment Graph (PAG / SVFIR) from LLVM BC files and manipulate `PAGNode` (`SVFVar`).
* [ICFG](./demo/icfg.ipynb): Generate Interprocedural Control Flow Graph (ICFG) and manipulate `ICFGNode`, `ICFGEdge`, and subclasses.
* [SVFG](./demo/svfg.ipynb): Generate Static Value Flow Graph (SVFG) and manipulate `SVFGNode`, `SVFGEdge`, and subclasses.

