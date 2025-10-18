#include <pybind11/pybind11.h>
#include "Util/Options.h"
#include <pybind11/stl.h>
#include "SVF-LLVM/SVFIRBuilder.h"
#include "Graphs/ICFG.h"
#include <pybind11/operators.h>
#include "MTA/MTA.h"


namespace py = pybind11;
using namespace SVF;


void bind_multi_thread_analysis(py::module& m) {
    py::class_<MTA>(m, "MTA", "Multi-Thread Analysis class")
        .def(py::init([](){
            return new MTA();
        }), "Initialize MTA with SVFIR")
        .def("runOnModule", &MTA::runOnModule, py::arg("pag"),
         "Perform multi-thread analysis");
}
