#include <pybind11/pybind11.h>
#include "Util/Options.h"
#include <pybind11/stl.h>
#include "SVF-LLVM/SVFIRBuilder.h"
#include "Graphs/ICFG.h"
#include "SVFIR/SVFType.h"
#include "SVFIR/SVFStatements.h"
#include "MemoryModel/PointerAnalysis.h"
#include "WPA/Andersen.h"
#include "WPA/Steensgaard.h"
#include "AE/Core/AbstractState.h"
#include <pybind11/operators.h>


namespace py = pybind11;
using namespace SVF;

void bind_andersen_base(py::module& m) {
    class PublicAndersen : public AndersenBase {
        public:
            using AndersenBase::AndersenBase;  // 继承构造函数
        
            using AndersenBase::pushIntoWorklist;
            using AndersenBase::popFromWorklist;
            using AndersenBase::isWorklistEmpty;
            using AndersenBase::unionPts;
            using AndersenBase::addPts;
            using AndersenBase::getPts;
            using AndersenBase::analyze;
            using AndersenBase::initWorklist;
            using AndersenBase::finalize;
            using AndersenBase::initialize;
            using AndersenBase::updateCallGraph;
            using AndersenBase::getIndirectCallsites;
            using AndersenBase::getConstraintGraph;
            using AndersenBase::alias;
            bool addCopyEdge(NodeID src, NodeID dst) override {
                assert(false && "You cannot call AndersenBase::addCopyEdge");
                return false;
            }
    };

    py::enum_<AliasResult>(m, "AliasResult")
        .value("NoAlias", AliasResult::NoAlias)
        .value("MayAlias", AliasResult::MayAlias)
        .value("MustAlias", AliasResult::MustAlias)
        .value("PartialAlias", AliasResult::PartialAlias)
        .export_values();

    py::class_<PointerAnalysis, std::shared_ptr<PointerAnalysis>>(m, "PointerAnalysis", "PTA");
    py::class_<BVDataPTAImpl, std::shared_ptr<BVDataPTAImpl>, PointerAnalysis>(m, "BVDataPTAImpl", "BVDataPTAImpl");
    // WPAConstraintSolver has a protected constructor and is not exposed to Python directly.
    // Skip binding it to avoid needing to expose its protected constructor.
    // AndersenBase still inherits BVDataPTAImpl (which in turn derives from PointerAnalysis),
    // but we can not upcast AndersenBase to PointerAnalysis,
    // because pybind can not handle the share_ptr cast correctly.
    py::class_<AndersenBase, std::shared_ptr<AndersenBase>, BVDataPTAImpl>(m, "AndersenBase_", "AndersenBase");
    py::class_<PublicAndersen, std::shared_ptr<PublicAndersen>, AndersenBase>(m, "AndersenBase", "Anderson's analysis base class")
        .def(py::init([](SVFIR* svfir) {
            return std::make_shared<PublicAndersen>(svfir);
        }))
        .def("analyze", &PublicAndersen::analyze, "Analysis entry")
        .def("initialize", &PublicAndersen::initialize, "Initialize the analysis")
        .def("initWorklist", &PublicAndersen::initWorklist, "Initialize the worklist")
        .def("updateCallGraph", [](PublicAndersen& base) {
            return base.updateCallGraph(base.getIndirectCallsites());
        }, py::return_value_policy::reference, "Update the call graph with the given call sites")
        .def("pushIntoWorklist", [](PublicAndersen& base, NodeID id) {
            base.pushIntoWorklist(id);
        }, py::arg("node"), "Push a node into the worklist")
        .def("finalize", &PublicAndersen::finalize, "Finalize the analysis")
        .def("addPts",[](PublicAndersen& base, NodeID id, NodeID ptd) {
            return base.addPts(id, ptd);
        }, py::arg("id"), py::arg("ptd"), "Add points-to information")
        .def("getConstraintGraph", &PublicAndersen::getConstraintGraph, py::return_value_policy::reference, "Get the constraint graph")
        .def("unionPts", [](PublicAndersen& base, NodeID id, NodeID ptd) {
            return base.unionPts(id, ptd);
        }, py::arg("id"), py::arg("ptd"), "Union points-to 1 information")
        .def("unionPts2", [](PublicAndersen& base, NodeID id, const PointsTo& ptd) {
            return base.unionPts(id, ptd);
        }, py::arg("id"), py::arg("ptd"), "Union points-to 2 information")
        .def("dumpPts", [](PublicAndersen& base, NodeID id, const PointsTo& pts) {
            base.dumpPts(id, pts);
        }, py::arg("id"), py::arg("pts"), "Dump points-to information")
        .def("alias", [](PublicAndersen& base, NodeID id1, NodeID id2) {
            AliasResult res =  base.alias(id1, id2);
            std::cout << "Alias result: " << res << std::endl;
            return res;
        }, py::arg("id1"), py::arg("id2"), "Check if two nodes are aliases")
        .def("isWorklistEmpty", &PublicAndersen::isWorklistEmpty, "Check if the worklist is empty")
        .def("popFromWorklist", [](PublicAndersen& base) {
            return base.popFromWorklist();
        }, "Pop a node from the worklist")
        .def("getPts", [](PublicAndersen& base, NodeID id) {
            return base.getPts(id);
        }, py::arg("id"), py::return_value_policy::reference, "Get points-to information for a given ID");

    py::class_<Andersen, std::shared_ptr<Andersen>, AndersenBase>(m, "Andersen", "Andersen's pts");
    py::class_<AndersenWaveDiff, std::shared_ptr<AndersenWaveDiff>, Andersen>(m, "AndersenWaveDiff", "AndersenWaveDiff Pointer Analysis")
        .def(py::init([](SVFIR *svfir){
            return std::make_shared<AndersenWaveDiff>(svfir);
        }))
        .def("analyze", &AndersenWaveDiff::analyze, "Analysis entry");

    py::class_<Steensgaard, std::shared_ptr<Steensgaard>, AndersenBase>(m, "Steensgaard", "Steensgaard's pts")
        .def(py::init([](SVFIR *svfir){
            return std::make_shared<Steensgaard>(svfir);
        }))
        .def("analyze", &Steensgaard::analyze, "Analysis entry");
}
