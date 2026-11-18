"""Smoke-test PAG bindings against SVF's renamed C++ API.

Run with: python -m unittest discover -s test_cases -p test_pag_api.py -v
"""

import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


class PAGAPITest(unittest.TestCase):
    def run_binding_check(self, code):
        # SVF has process-wide graph and option singletons; isolate each check.
        with tempfile.TemporaryDirectory() as directory:
            bitcode = Path(directory) / "input.ll"
            bitcode.write_text(
                "define i32 @main() {\n"
                "entry:\n"
                "  %x = alloca i32\n"
                "  %p = alloca ptr\n"
                "  store i32 7, ptr %x\n"
                "  store ptr %x, ptr %p\n"
                "  %q = load ptr, ptr %p\n"
                "  %v = load i32, ptr %q\n"
                "  ret i32 %v\n"
                "}\n",
                encoding="ascii",
            )
            result = subprocess.run(
                [sys.executable, "-c", "import pysvf\nimport sys\n" + code, str(bitcode)],
                capture_output=True,
                text=True,
                timeout=60,
            )
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_build_and_release(self):
        self.run_binding_check(
            "pag = pysvf.get_svfir(sys.argv[1])\n"
            "assert isinstance(pag, pysvf.SVFIR)\n"
            "assert pysvf.getPAG() is pag\n"
            "assert pag.getSVFVarNum() > 0\n"
            "assert pag.getICFG() is pysvf.getICFG()\n"
            "assert list(pag)\n"
            "pysvf.releasePAG()\n"
            "assert pysvf.getPAG() is None\n"
            "assert pysvf.getICFG() is None\n"
        )

    def test_pointer_analyses(self):
        for analysis in ("AndersenWaveDiff_WPA", "Steensgaard_WPA"):
            with self.subTest(analysis=analysis):
                self.run_binding_check(
                    "pag = pysvf.get_svfir(sys.argv[1])\n"
                    f"pysvf.run_pta(pysvf.{analysis})\n"
                    "assert pysvf.get_pta() is not None\n"
                    "assert pysvf.getCallGraph() is not None\n"
                    "pysvf.releasePAG()\n"
                )

    def test_svfg(self):
        self.run_binding_check(
            "pysvf.buildSVFModule(['-svfg', sys.argv[1]])\n"
            "assert isinstance(pysvf.getPAG(), pysvf.SVFIR)\n"
            "assert pysvf.getSVFG() is not None\n"
            "pysvf.releasePAG()\n"
        )


if __name__ == "__main__":
    unittest.main()
