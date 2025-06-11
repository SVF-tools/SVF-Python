"""
SVF IR and CodeGraph
-------------------

SVF: Static Value-Flow Analysis

Copyright (C) <2013->  <Yulei Sui>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

CodeGraphs including PAG, ICFG and ConstraintGraph
"""

import pysvf
from pysvf import *
import sys


if len(sys.argv) < 2:
    print("Usage: python SVFIR.py [options] <bitcode_file>")
    sys.exit(1)

# Get the pag(SVFIR) from the bitcode file
sys.argv.extend([
    "-model-arrays=true",
    "-pre-field-sensitive=false",
    "-model-consts=true",
    "-stat=false"
])

# Skip the python script name and use the rest of the arguments
svf_arguments = sys.argv[1:]
pysvf.buildSVFModule(svf_arguments)
pag = pysvf.getPAG()
pag.dump(pag.getModuleIdentifier() + ".pag");

# Get the control flow graph (CFG) from the pag
icfg = pag.getICFG()
icfg.dump(pag.getModuleIdentifier() + ".icfg");

ander = pysvf.AndersenBase(pag)
ander.initialize()
consCG = ander.getConstraintGraph()
consCG.dump(pag.getModuleIdentifier() + ".consG");

print ("\n\n####Printing ICFG (Interprocedural Control-Flow Graph)###\n\n")
for id, node in icfg:
    print("{}\n\n".format(node.toString()))

print ("\n\n####Printing PAG (Program Assignment Graph)#######\n\n")
for id, node in pag:
    print("{}\n\n".format(node.toString()))

print ("\n\n#############Printing Constraint Graph##########\n\n")
for id, node in consCG:
    print("{}\n\n".format(node.toString()))
