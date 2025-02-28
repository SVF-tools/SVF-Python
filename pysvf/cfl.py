import sys

from . import run_tool

def main():
    run_tool("cfl", sys.argv[1:])

if __name__ == "__main__":
    main()
