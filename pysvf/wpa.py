import sys

from . import run_tool

def main():
    run_tool("wpa", sys.argv[1:])

if __name__ == "__main__":
    main()
