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

# Tool definitions
TOOL_NAMES = {
    "ae": "ae",
    "cfl": "cfl", 
    "dvf": "dvf",
    "llvm2svf": "llvm2svf",
    "mta": "mta",
    "saber": "saber",
    "svf_ex": "svf-ex",
    "wpa": "wpa"
}

# Original run_tool function
def run_tool(tool_name, args):
    tool_path = os.path.join(BIN_DIR, tool_name)

    if not os.path.exists(tool_path):
        print(f"[ERROR] Tool '{tool_name}' not found at {tool_path}")
        sys.exit(1)

    try:
        print(f"[INFO] Running {tool_name} with args {args}")
        result = subprocess.run([tool_path] + args, check=True, text=True, capture_output=True)
        print(f"[INFO] Output:\n{result.stdout}")
        if result.stderr:
            print(f"[INFO] Error:\n{result.stderr}")
        return result.stdout, result.stderr
    except subprocess.CalledProcessError as e:
        print(f"[ERROR] Execution failed: {e}")
        print(f"[ERROR] STDERR:\n{e.stderr}")
        sys.exit(e.returncode)

# Enhanced unified tool function
def run_svf_tool(tool_name, args=None):
    """
    Run a specified SVF tool.
    
    Args:
        tool_name (str): The name of the tool to run.
        args (list, optional): The arguments to pass to the tool. Defaults to sys.argv[1:].
    Returns:
        output (str): The standard output of the tool.
        error (str): The standard error of the tool.
    """
    if args is None:
        args = sys.argv[1:]
    
    if tool_name in TOOL_NAMES:
        output, error = run_tool(TOOL_NAMES[tool_name], args)
    else:
        print(f"[ERROR] Unknown tool: {tool_name}", file=sys.stderr)
        print(f"[INFO] Available tools: {', '.join(TOOL_NAMES.keys())}", file=sys.stderr)
        sys.exit(1)
    return output, error

# Main entry point when module is executed directly
def main():
    """
    Main entry point when the module is executed directly.
    Example: python -m pysvf ae [arguments]
    """
    if len(sys.argv) < 2:
        print("[ERROR] Usage: python -m pysvf <tool_name> [arguments]", file=sys.stderr)
        print(f"[INFO] Available tools: {', '.join(TOOL_NAMES.keys())}", file=sys.stderr)
        sys.exit(1)
    
    tool_name = sys.argv[1]
    args = sys.argv[2:]
    run_svf_tool(tool_name, args)

from .enums import Predicate, OpCode
# Import all the module classes and functions
from .pysvf import (
    releasePAG,
    getPAG,
    getICFG,
    getSVFG,
    getCallGraph,
    getCallGraphSCC,
    getModuleName,
    isExtCall,
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
    SelectStmt,
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
    CallGraph,
    CallGraphSCC,
    SVFG,
    VFGNode,
    VFGEdge,
    DirectSVFGEdge,
    IntraDirSVFGEdge,
    CallDirSVFGEdge,
    RetDirSVFGEdge,
    IndirectSVFGEdge,
    IntraIndSVFGEdge,
    CallIndSVFGEdge,
    RetIndSVFGEdge,
    ThreadMHPIndSVFGEdge,
    StmtVFGNode,
    LoadVFGNode,
    StoreVFGNode,
    ActualINSVFGNode,
    ActualOUTSVFGNode,
    ActualParmVFGNode,
    ActualRetVFGNode,
    AddrVFGNode,
    BinaryOPVFGNode,
    BranchVFGNode,
    CmpVFGNode,
    CopyVFGNode,
    DummyVersionPropSVFGNode,
    FormalINSVFGNode,
    FormalOUTSVFGNode,
    FormalParmVFGNode,
    GepVFGNode,
    InterMSSAPHISVFGNode,
    InterPHIVFGNode,
    IntraDirSVFGEdge,
    IntraIndSVFGEdge,
    IntraMSSAPHISVFGNode,
    IntraPHIVFGNode,
    LoadVFGNode,
    MRSVFGNode,
    MSSAPHISVFGNode,
    PHIVFGNode,
    RetDirSVFGEdge,
    RetIndSVFGEdge,
    VFG,
    AndersenBase,
    PointsTo,
    ConstraintEdge,
    ConstraintNode,
    AddrCGEdge,
    CopyCGEdge,
    StoreCGEdge,
    LoadCGEdge,
    GepCGEdge,
    NormalGepCGEdge,
    VariantGepCGEdge,
    ConstraintGraph,
    IntervalValue,
    AddressValue,
    AbstractState,
    AbstractValue,
    BoundedInt
)

from .pysvf import buildSVFModule as _buildSVFModule
# argument can be a string or a list of strings
def buildSVFModule(args) -> None:
    if isinstance(args, str):
        args = [args]
    # if arg is a list, make sure every element is a string
    if not all(isinstance(arg, str) for arg in args):
        raise ValueError("All arguments must be strings")
    _buildSVFModule(args)


# Enable direct module execution
if __name__ == "__main__":
    main()