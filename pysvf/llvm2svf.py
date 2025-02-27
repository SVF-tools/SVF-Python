from . import run_tool
import sys

def main():
    run_tool("llvm2svf", sys.argv[1:])

if __name__ == "__main__":
    main()
