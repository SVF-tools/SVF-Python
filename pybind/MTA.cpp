#include <pybind11/pybind11.h>
#include "Util/Options.h"
#include <pybind11/stl.h>
#include "SVF-LLVM/SVFIRBuilder.h"
#include "Graphs/ICFG.h"
#include <pybind11/operators.h>
#include "MTA/TCT.h"
#include "MTA/MTA.h"
#include "MTA/MHP.h"
#include "MTA/LockAnalysis.h"
#include "Util/Casting.h"
#include "WPA/Andersen.h"


namespace py = pybind11;
using namespace SVF;


void bind_multi_thread_analysis(py::module& m) {
    class PublicMTA: public MTA {
    public:
        using MTA::MTA;
        using MTA::runOnModule;
    };

    py::class_<PublicMTA, std::shared_ptr<PublicMTA>>(m, "MTA", "Multi-Thread Analysis class")
        .def(py::init([](){
                return std::make_shared<PublicMTA>();
            }), "Initialize MTA with SVFIR")
        .def("runOnModule", &PublicMTA::runOnModule, py::arg("pag"),
         "Perform multi-thread analysis")
        .def("detect", &PublicMTA::detect, "Detect data race");

    py::class_<MHP, std::shared_ptr<MHP>>(m, "MHP", "May-Happen-in-Parallel analysis class")
        .def(py::init([](std::shared_ptr<TCT> tct){
                return std::make_shared<MHP>(tct.get());
            }),
            py::arg("tct"), "Initialize MHP analysis",
            py::keep_alive<2, 1>())
        .def("analyze", &MHP::analyze, "Analyze entry")
        .def("mayHappenInParallelInst", &MHP::mayHappenInParallelInst,
            py::arg("node1"), py::arg("node2"),
            "Check if two ICFG nodes may happen in parallel");

    py::class_<LockAnalysis, std::shared_ptr<LockAnalysis>>(m, "LockAnalysis", "Lock Set Analysis class")
        .def(py::init([](std::shared_ptr<TCT> tct){
                return std::make_shared<LockAnalysis>(tct.get());
            ;})
            , py::arg("tct"), "Initialize Lock Set analysis",
            py::keep_alive<2, 1>())
        .def("analyze", &LockAnalysis::analyze, "Analysis entry")
        .def("isProtectedByCommandLock", &LockAnalysis::isProtectedByCommonLock,
            py::arg("node1"), py::arg("node2"),
            "Check if two ICFG nodes are protected by common locks");

    py::class_<TCT, std::shared_ptr<TCT>>(m, "TCT", "Thread Creation Tree class")
        .def(py::init([](std::shared_ptr<AndersenBase> pta){
                return std::make_shared<TCT>(SVFUtil::dyn_cast<PointerAnalysis>(pta.get()));
            }), py::arg("pa"), "Initialize Thread Creation Tree with Pointer Analysis",
            py::keep_alive<2,1>())
        .def(py::init([](std::shared_ptr<AndersenWaveDiff> pta){
                return std::make_shared<TCT>(SVFUtil::dyn_cast<PointerAnalysis>(pta.get()));
            }), py::arg("pa"), "Initialize Thread Creation Tree with Pointer Analysis",
            py::keep_alive<2,1>())
//        .def(py::init([](std::shared_ptr<PointerAnalysis> pta){
//                if (pta == nullptr || pta.get() == nullptr) { throw std::runtime_error("TCT::__init__: received null PointerAnalysis"); }
//                return std::make_shared<TCT>(pta.get());
//            }), py::arg("pa"), "Initialize Thread Creation Tree with Pointer Analysis",
//            py::keep_alive<2,1>())
        .def("getThreadCallGraph", &TCT::getThreadCallGraph, "Get the thread call graph");
}
