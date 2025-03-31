# Pysvf: the Python bindings of SVF

## 1. Introduction
This is a Python binding of the [SVF](https://github.com/SVF-tools/SVF). 

## 2. Install SVF-Python

### Pre-requisite

- Python 3.8 - 3.11

### Install

install pysvf by pip
```angular2html
python3 -m pip install  -i https://test.pypi.org/simple/ pysvf
```

### Documentation

The documentation of pysvf is available at [Wiki](https://github.com/SVF-tools/SVF-Python/wiki) page. 

- Pysvf API reference: [Link](https://github.com/SVF-tools/SVF-Python/wiki/PySVF-API)
- Install Pysvf from Scratch: [Link](https://github.com/SVF-tools/SVF-Python/wiki/Install-Pysvf-from-Scratch) 



### Tutorial

In `tutorial` folder, there are some examples to show how to use pysvf. Before running the examples, you need to install Jupyter Notebook first.

```angular2html
python3 -m pip install jupyter
```

- [PAG](./demo/pag.ipynb): This example shows how to use pysvf to generate Program Assignment Graph (PAG, SVFIR) from LLVM BC files, and how to manipulate PAGNode(SVFVar).
- [ICFG](./demo/icfg.ipynb): This example shows how to use pysvf to generate Interprocedural Control Flow Graph (ICFG) and how to manipulate ICFGNode, ICFGEdge and their sub-classes.
- [SVFG](./demo/svfg.ipynb): This example shows how to use pysvf to generate Static Value Flow Graph (SVFG) and how to manipulate SVFGNode, SVFGEdge and their sub-classes.