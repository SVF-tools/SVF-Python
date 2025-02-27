import sys
print("[DEBUG] ae.py is executing!")

from . import run_tool

def main():
    print("[DEBUG] Inside main() of cfl.py")
    run_tool("cfl", sys.argv[1:])

if __name__ == "__main__":
    print("[DEBUG] __name__ == '__main__', calling main()")
    main()
