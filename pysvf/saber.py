import sys
print("[DEBUG] saber.py is executing!")

from . import run_tool

def main():
    print("[DEBUG] Inside main() of saber.py")
    run_tool("saber", sys.argv[1:])

if __name__ == "__main__":
    print("[DEBUG] __name__ == '__main__', calling main()")
    main()
