#include <pybind11/pybind11.h>
#include "Util/Options.h"
#include <pybind11/stl.h>
#include "SVF-LLVM/SVFIRBuilder.h"
#include "Graphs/ICFG.h"
#include "SVFIR/SVFType.h"
#include "SVFIR/SVFStatements.h"
#include "MemoryModel/PointerAnalysis.h"
#include "WPA/Andersen.h"
#include "AE/Core/AbstractState.h"
#include <pybind11/operators.h>


namespace py = pybind11;
using namespace SVF;

void bind_points_to(py::module& m) {
    py::class_<PointsTo, std::shared_ptr<PointsTo>>(m, "PointsTo", "Points-to set")
        .def(py::init<>())  // 默认构造
        .def("set", [](PointsTo& pts, NodeID id) {
            pts.set(id);
        }, py::arg("id"), "Set a node ID in the points-to set")
        .def("__iter__", [](PointsTo &self) {
            return py::make_iterator(self.begin(), self.end());
        }, py::keep_alive<0, 1>())
        .def("__str__", [](const PointsTo &self) {
            std::ostringstream oss;
            for (NodeID id : self) {
                oss << id << " ";
            }
            return oss.str();
        }, "Get the string representation of the points-to set");
}
