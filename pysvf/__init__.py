import os
import sys
import subprocess

# Get the current script directory
CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))

# Set key paths
EXTAPI_BC_PATH = os.path.join(CURRENT_DIR, "SVF/Release-build/lib/", "extapi.bc")
SVF_DIR = os.path.join(CURRENT_DIR, "SVF")
Z3_DIR = os.path.join(CURRENT_DIR, "SVF/z3.obj")
LLVM_DIR = os.path.join(CURRENT_DIR, "SVF/llvm-16.0.0.obj")
BIN_DIR = os.path.join(SVF_DIR, "Release-build", "bin")

# Set environment variables
os.environ["Z3_DIR"] = Z3_DIR
os.environ["SVF_DIR"] = SVF_DIR
os.environ["LLVM_DIR"] = LLVM_DIR
os.environ["SVF_EXTAPI_DIR"] = os.path.join(SVF_DIR, "Release-build/lib")

# Debugging print statements
print(f"SVF_DIR: {SVF_DIR}")
print(f"BIN_DIR: {BIN_DIR}")

def run_tool(tool_name, args):
    tool_path = os.path.join(BIN_DIR, tool_name)

    if not os.path.exists(tool_path):
        print(f"[ERROR] Tool '{tool_name}' not found at {tool_path}")
        sys.exit(1)

    try:
        print(f"[INFO] Running {tool_name} with args {args}")
        result = subprocess.run([tool_path] + args, check=True, text=True, capture_output=True)
        print(f"[INFO] Output:\n{result.stdout}")
    except subprocess.CalledProcessError as e:
        print(f"[ERROR] Execution failed: {e}")
        print(f"[ERROR] STDERR:\n{e.stderr}")
        sys.exit(e.returncode)

from .pysvf import (
    get_pag,
    release_pag,
    ICFGNode,
    IntraICFGNode,
    InterICFGNode,
    FunEntryICFGNode,
    FunExitICFGNode,
    CallICFGNode,
    RetICFGNode,
    GlobalICFGNode,
    ICFGEdge,
    IntraCFGEdge,
    CallCFGEdge,
    RetCFGEdge,
    SVFStmt,
    AddrStmt,
    CopyStmt,
    StoreStmt,
    LoadStmt,
    CallPE,
    RetPE,
    GepStmt,
    PhiStmt,
    CmpStmt,
    BinaryOPStmt,
    UnaryOPStmt,
    BranchStmt,
    ICFG,
    SVFIR,
    SVFVar,
    ValVar,
    ObjVar,
    ArgValVar,
    GepValVar,
    BaseObjVar,
    GepObjVar,
    HeapObjVar,
    StackObjVar,
    FunObjVar,
    FunValVar,
    GlobalValVar,
    ConstAggValVar,
    ConstDataValVar,
    BlackHoleValVar,
    ConstFPValVar,
    ConstIntValVar,
    ConstNullPtrValVar,
    GlobalObjVar,
    ConstAggObjVar,
    ConstDataObjVar,
    ConstFPObjVar,
    ConstIntObjVar,
    ConstNullPtrObjVar,
    RetValPN,
    VarArgValPN,
    DummyValVar,
    DummyObjVar,
    SVFType,
    SVFPointerType,
    SVFIntegerType,
    SVFFunctionType,
    SVFStructType,
    SVFArrayType,
    SVFOtherType,
    CallGraphNode,
    CallGraphEdge,
    CallGraph
)
from .enums import Predicate, OpCode
