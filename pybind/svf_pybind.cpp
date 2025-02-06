#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "SVF-LLVM/LLVMUtil.h"
#include "SVF-LLVM/SVFIRBuilder.h"
#include "Util/CommandLine.h"
#include "Util/Options.h"
#include "Graphs/ICFG.h"
#include "SVFIR/SVFStatements.h"

namespace py = pybind11;
using namespace SVF;

class PySVF {
public:
    static SVFIR* analyze(const std::string& bitcodePath) {
        std::vector<std::string> moduleNameVec = { bitcodePath };

        SVFModule* svfModule = LLVMModuleSet::buildSVFModule(moduleNameVec);
        SVFIRBuilder builder(svfModule);
        SVFIR* pag = builder.build();  // TODO: maybe we need to split build() into more steps

        return pag;  // Now we directly return SVFIR(pag)
    }
};

void bind_icfg_node(py::module& m) {
    py::class_<ICFGNode>(m, "ICFGNode")
            .def("to_string", [](const ICFGNode& node) {
                std::ostringstream oss;
                oss << node.toString() << "\n";
                return oss.str();
            })

            .def("get_fun", &ICFGNode::getFun, py::return_value_policy::reference)

            .def("get_bb", &ICFGNode::getBB, py::return_value_policy::reference)

            // TODO: fetch VFGNodes list related to ICFGNode

            .def("get_svf_stmts", &ICFGNode::getSVFStmts, py::return_value_policy::reference)

            // Downcast to ICFGNode's subtypes
            .def("as_fun_entry", [](ICFGNode* node) {
                return dynamic_cast<FunEntryICFGNode*>(node);
            }, py::return_value_policy::reference)

            .def("as_fun_exit", [](ICFGNode* node) {
                return dynamic_cast<FunExitICFGNode*>(node);
            }, py::return_value_policy::reference)

            .def("as_call", [](ICFGNode* node) {
                return dynamic_cast<CallICFGNode*>(node);
            }, py::return_value_policy::reference)

            .def("as_ret", [](ICFGNode* node) {
                return dynamic_cast<RetICFGNode*>(node);
            }, py::return_value_policy::reference)

            // check whether the node is a ICFGNode's subtype
            .def("is_fun_entry", [](const ICFGNode* node) {
                return node->getNodeKind() == SVF::ICFGNode::FunEntryBlock;
            })

            .def("is_fun_exit", [](const ICFGNode* node) {
                return node->getNodeKind() == SVF::ICFGNode::FunExitBlock;
            })

            .def("is_call", [](const ICFGNode* node) {
                return node->getNodeKind() == SVF::ICFGNode::FunCallBlock;
            })

            .def("is_ret", [](const ICFGNode* node) {
                return node->getNodeKind() == SVF::ICFGNode::FunRetBlock;
            });
}

void bind_svf_stmt(py::module& m) {
    py::class_<SVFStmt>(m, "SVFStmt")
            .def("to_string", [](const SVFStmt& stmt) {
                std::ostringstream oss;
                oss << stmt.toString();
                return oss.str();
            })
            .def("get_edge_id", &SVFStmt::getEdgeID)
            .def("get_icfg_node", &SVFStmt::getICFGNode, py::return_value_policy::reference_internal)
            .def("get_value", &SVFStmt::getValue, py::return_value_policy::reference_internal)
            .def("get_bb", &SVFStmt::getBB, py::return_value_policy::reference_internal)
                    // addr copy store load call ret gep phi select cmp binary unary branch
                    // TODO: may support threadFork threadJoin
            .def("is_addr_stmt", [](const SVFStmt* stmt) { return dynamic_cast<const AddrStmt*>(stmt) != nullptr; })
            .def("is_copy_stmt", [](const SVFStmt* stmt) { return dynamic_cast<const CopyStmt*>(stmt) != nullptr; })
            .def("is_store_stmt", [](const SVFStmt* stmt) { return dynamic_cast<const StoreStmt*>(stmt) != nullptr; })
            .def("is_load_stmt", [](const SVFStmt* stmt) { return dynamic_cast<const LoadStmt*>(stmt) != nullptr; })
            .def("is_call_pe", [](const SVFStmt* stmt) { return dynamic_cast<const CallPE*>(stmt) != nullptr; })
            .def("is_ret_pe", [](const SVFStmt* stmt) { return dynamic_cast<const RetPE*>(stmt) != nullptr; })
            .def("is_gep_stmt", [](const SVFStmt* stmt) {return dynamic_cast<const GepStmt*>(stmt) != nullptr; })
            .def("is_phi_stmt", [](const SVFStmt* stmt) { return dynamic_cast<const PhiStmt*>(stmt) != nullptr; })
            .def("is_select_stmt", [](const SVFStmt* stmt) {
                return dynamic_cast<const SelectStmt*>(stmt) != nullptr;})
            .def("is_cmp_stmt", [](const SVFStmt* stmt) {
                return dynamic_cast<const CmpStmt*>(stmt) != nullptr;})
            .def("is_binary_op_stmt", [](const SVFStmt* stmt) {
                return dynamic_cast<const BinaryOPStmt*>(stmt) != nullptr;})
            .def("is_unary_op_stmt", [](const SVFStmt* stmt) {
                return dynamic_cast<const UnaryOPStmt*>(stmt) != nullptr;})
            .def("is_branch_stmt", [](const SVFStmt* stmt) {
                return dynamic_cast<const BranchStmt*>(stmt) != nullptr;})
             // downcast TODO: more downcast here
            .def("as_cmp_stmt", [](SVFStmt* stmt) { return dynamic_cast<CmpStmt*>(stmt); }, py::return_value_policy::reference);

    py::class_<CmpStmt, SVFStmt>(m, "CmpStmt")  // TODO: Return int, maybe need to think about friendly return value
            .def("get_predicate", &CmpStmt::getPredicate)
            .def("get_res", &CmpStmt::getRes, py::return_value_policy::reference)
            .def("get_res_id", &CmpStmt::getResID)
            .def("get_op_var", [](CmpStmt& stmt, int ID) { return stmt.getOpVar(ID); },
                 py::return_value_policy::reference);

}


// Bind class ICFG
void bind_icfg_graph(py::module& m) {
    py::class_<ICFG>(m, "ICFG")
            .def("get_nodes", [](const ICFG& icfg) {
                std::vector<ICFGNode*> nodes;
                for (auto& node : icfg) {
                    nodes.push_back(node.second);
                }
                return nodes;
            }, py::return_value_policy::reference)

            // getGNode(id)
            .def("get_gnode", [](ICFG& icfg, NodeID id) -> ICFGNode* {
                ICFGNode* node = icfg.getGNode(id);
                if (!node) {
                    throw std::runtime_error("ICFGNode with given ID not found.");
                }
                return node;
            }, py::arg("id"), py::return_value_policy::reference);
}


// Bind SVFIR (PAG)
void bind_svf(py::module& m) {
    py::class_<SVFIR>(m, "SVFIR")
            .def("get_icfg", [](SVFIR* pag) { return pag->getICFG(); }, py::return_value_policy::reference);

}

PYBIND11_MODULE(pysvf, m) {
    bind_svf(m);
    bind_icfg_node(m);
    bind_icfg_graph(m);
    bind_svf_stmt(m);
    m.def("analyze", &PySVF::analyze, py::return_value_policy::reference, "Analyze LLVM bitcode and return SVFIR");
}
