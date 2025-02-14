#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "SVF-LLVM/SVFIRBuilder.h"
#include "Util/CommandLine.h"
#include "Graphs/ICFG.h"
#include "SVFIR/SVFType.h"
#include "SVFIR/SVFStatements.h"
#include "MemoryModel/PointerAnalysis.h"
#include "WPA/Andersen.h"

namespace py = pybind11;
using namespace SVF;

class PySVF {
public:
    static SVFIR* analyze(const std::string& bitcodePath) {
        std::vector<std::string> moduleNameVec = { bitcodePath };

        SVFModule* svfModule = LLVMModuleSet::buildSVFModule(moduleNameVec);
        SVFIRBuilder builder(svfModule);
        SVFIR* pag = builder.build();  // TODO: maybe we need to split build() into more steps
        CallGraph* callgraph = AndersenWaveDiff::createAndersenWaveDiff(pag)->getCallGraph();
        builder.updateCallGraph(callgraph);
        pag->getICFG()->updateCallGraph(callgraph);
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
            //get_id
            .def("get_id", &ICFGNode::getId)
            .def("get_fun", &ICFGNode::getFun, py::return_value_policy::reference)
            .def("get_bb", &ICFGNode::getBB, py::return_value_policy::reference)
            .def("get_svf_stmts", &ICFGNode::getSVFStmts, py::return_value_policy::reference)
            // Downcast
            .def("as_fun_entry", [](ICFGNode* node) { return dynamic_cast<FunEntryICFGNode*>(node); }, py::return_value_policy::reference)
            .def("as_fun_exit", [](ICFGNode* node) { return dynamic_cast<FunExitICFGNode*>(node); }, py::return_value_policy::reference)
            .def("as_call", [](ICFGNode* node) { return dynamic_cast<CallICFGNode*>(node); }, py::return_value_policy::reference)
            .def("as_ret", [](ICFGNode* node) { return dynamic_cast<RetICFGNode*>(node); }, py::return_value_policy::reference)
            // Type checking
            .def("is_fun_entry", [](const ICFGNode* node) { return node->getNodeKind() == SVF::ICFGNode::FunEntryBlock; })
            .def("is_fun_exit", [](const ICFGNode* node) { return node->getNodeKind() == SVF::ICFGNode::FunExitBlock; })
            .def("is_call", [](const ICFGNode* node) { return node->getNodeKind() == SVF::ICFGNode::FunCallBlock; })
            .def("is_ret", [](const ICFGNode* node) { return node->getNodeKind() == SVF::ICFGNode::FunRetBlock; })
            // get out edges and get in edges. gather them in a list
            .def("get_out_edges", [](const ICFGNode *node) {
                std::vector<ICFGEdge *> edges;
                for (auto &edge : node->getOutEdges()) {
                    edges.push_back(edge);
                }
                return edges;
            }, py::return_value_policy::reference)
            .def("get_in_edges", [](const ICFGNode *node) {
                std::vector < ICFGEdge * > edges;
                for (auto &edge: node->getInEdges()) {
                    edges.push_back(edge);
                }
                return edges;
            }, py::return_value_policy::reference);

    // IntraICFGNode
    py::class_<IntraICFGNode, ICFGNode>(m, "IntraICFGNode")
            .def("is_ret_inst", &IntraICFGNode::isRetInst);

    // InterICFGNode
    py::class_<InterICFGNode, ICFGNode>(m, "InterICFGNode");

    // FunEntryICFGNode
    py::class_<FunEntryICFGNode, InterICFGNode>(m, "FunEntryICFGNode")
            .def("get_formal_parms", &FunEntryICFGNode::getFormalParms, py::return_value_policy::reference)
            .def("add_formal_parm", &FunEntryICFGNode::addFormalParms);

    // FunExitICFGNode
    py::class_<FunExitICFGNode, InterICFGNode>(m, "FunExitICFGNode")
            .def("get_formal_ret", &FunExitICFGNode::getFormalRet, py::return_value_policy::reference)
            .def("add_formal_ret", &FunExitICFGNode::addFormalRet);

    // CallICFGNode
    py::class_<CallICFGNode, InterICFGNode>(m, "CallICFGNode")
            .def("get_caller", &CallICFGNode::getCaller, py::return_value_policy::reference)
            .def("get_called_function", &CallICFGNode::getCalledFunction, py::return_value_policy::reference)
            .def("get_actual_parms", &CallICFGNode::getActualParms, py::return_value_policy::reference)
            .def("add_actual_parm", &CallICFGNode::addActualParms)
            .def("is_vararg", &CallICFGNode::isVarArg)
            .def("is_virtual_call", &CallICFGNode::isVirtualCall);

    // === RetICFGNode ===
    py::class_<RetICFGNode, InterICFGNode>(m, "RetICFGNode")
            .def("get_actual_ret", &RetICFGNode::getActualRet, py::return_value_policy::reference)
            .def("add_actual_ret", &RetICFGNode::addActualRet);

    // GlobalICFGNode
    py::class_<GlobalICFGNode, ICFGNode>(m, "GlobalICFGNode");
}

void bind_icfg_edge(py::module& m) {
    using namespace SVF;

    py::class_<ICFGEdge, std::shared_ptr<ICFGEdge>>(m, "ICFGEdge")
            .def("to_string", &ICFGEdge::toString)
            .def("is_cfg_edge", &ICFGEdge::isCFGEdge)
            .def("is_call_cfg_edge", &ICFGEdge::isCallCFGEdge)
            .def("is_ret_cfg_edge", &ICFGEdge::isRetCFGEdge)
            .def("is_intra_cfg_edge", &ICFGEdge::isIntraCFGEdge)
            // get inedge and outedge
            .def("get_src", &ICFGEdge::getSrcNode, py::return_value_policy::reference)
            .def("get_dst", &ICFGEdge::getDstNode, py::return_value_policy::reference)
            //downcast
            .def("as_intra_cfg_edge", [](ICFGEdge *edge) { return dynamic_cast<IntraCFGEdge *>(edge); },
                 py::return_value_policy::reference)
            .def("as_call_cfg_edge", [](ICFGEdge *edge) { return dynamic_cast<CallCFGEdge *>(edge); },
                 py::return_value_policy::reference)
            .def("as_ret_cfg_edge", [](ICFGEdge *edge) { return dynamic_cast<RetCFGEdge *>(edge); },
                 py::return_value_policy::reference);

    py::class_<IntraCFGEdge, ICFGEdge, std::shared_ptr<IntraCFGEdge>>(m, "IntraCFGEdge")
            .def("get_condition", &IntraCFGEdge::getCondition, py::return_value_policy::reference)
            .def("get_successor_cond_value", &IntraCFGEdge::getSuccessorCondValue);

    py::class_<CallCFGEdge, ICFGEdge, std::shared_ptr<CallCFGEdge>>(m, "CallCFGEdge")
            .def("get_call_site", &CallCFGEdge::getCallSite, py::return_value_policy::reference)
            .def("get_call_pes", &CallCFGEdge::getCallPEs, py::return_value_policy::reference);

    py::class_<RetCFGEdge, ICFGEdge, std::shared_ptr<RetCFGEdge>>(m, "RetCFGEdge")
            .def("get_call_site", &RetCFGEdge::getCallSite, py::return_value_policy::reference)
            .def("get_ret_pe", &RetCFGEdge::getRetPE, py::return_value_policy::reference);
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
            .def("as_addr_stmt", [](SVFStmt *stmt) { return dynamic_cast<AddrStmt *>(stmt); },
                  py::return_value_policy::reference)
            .def("as_copy_stmt", [](SVFStmt *stmt) { return dynamic_cast<CopyStmt *>(stmt); },
                 py::return_value_policy::reference)
            .def("as_store_stmt", [](SVFStmt *stmt) { return dynamic_cast<StoreStmt *>(stmt); },
                 py::return_value_policy::reference)
            .def("as_load_stmt", [](SVFStmt *stmt) { return dynamic_cast<LoadStmt *>(stmt); },
                 py::return_value_policy::reference)
            .def("as_call_pe", [](SVFStmt *stmt) { return dynamic_cast<CallPE *>(stmt); },
                 py::return_value_policy::reference)
            .def("as_ret_pe", [](SVFStmt *stmt) { return dynamic_cast<RetPE *>(stmt); },
                 py::return_value_policy::reference)
            .def("as_gep_stmt", [](SVFStmt *stmt) { return dynamic_cast<GepStmt *>(stmt); },
                 py::return_value_policy::reference)
            .def("as_phi_stmt", [](SVFStmt *stmt) { return dynamic_cast<PhiStmt *>(stmt); },
                 py::return_value_policy::reference)
            .def("as_select_stmt", [](SVFStmt *stmt) { return dynamic_cast<SelectStmt *>(stmt); },
                 py::return_value_policy::reference)
            .def("as_cmp_stmt", [](SVFStmt* stmt) { return dynamic_cast<CmpStmt*>(stmt); },
                 py::return_value_policy::reference)
            .def("as_binary_op_stmt", [](SVFStmt* stmt) { return dynamic_cast<BinaryOPStmt*>(stmt); },
                 py::return_value_policy::reference)
            .def("as_unary_op_stmt", [](SVFStmt *stmt) { return dynamic_cast<UnaryOPStmt *>(stmt); },
                 py::return_value_policy::reference)
            .def("as_branch_stmt", [](SVFStmt *stmt) { return dynamic_cast<BranchStmt *>(stmt); },
                 py::return_value_policy::reference);

    py::class_<AddrStmt, SVFStmt>(m, "AddrStmt")
            .def("get_lhs_var", &AddrStmt::getLHSVar, py::return_value_policy::reference)
            .def("get_lhs_id", &AddrStmt::getLHSVarID)
            .def("get_rhs_var", &AddrStmt::getRHSVar, py::return_value_policy::reference)
            .def("get_rhs_id", &AddrStmt::getRHSVarID)
            .def("get_arr_size", &AddrStmt::getArrSize, py::return_value_policy::reference);

    py::class_<CopyStmt, SVFStmt>(m, "CopyStmt")
            // TODO: more API from CopyStmt
            .def("get_lhs_var", &CopyStmt::getLHSVar, py::return_value_policy::reference)
            .def("get_lhs_id", &CopyStmt::getLHSVarID)
            .def("get_rhs_var", &CopyStmt::getRHSVar, py::return_value_policy::reference)
            .def("get_rhs_id", &CopyStmt::getRHSVarID);


    py::class_<StoreStmt, SVFStmt>(m, "StoreStmt")
            .def("get_lhs_var", &StoreStmt::getLHSVar, py::return_value_policy::reference)
            .def("get_lhs_id", &StoreStmt::getLHSVarID)
            .def("get_rhs_var", &StoreStmt::getRHSVar, py::return_value_policy::reference)
            .def("get_rhs_id", &StoreStmt::getRHSVarID);


    py::class_<LoadStmt, SVFStmt>(m, "LoadStmt")
            .def("get_lhs_var", &LoadStmt::getLHSVar, py::return_value_policy::reference)
            .def("get_lhs_id", &LoadStmt::getLHSVarID)
            .def("get_rhs_var", &LoadStmt::getRHSVar, py::return_value_policy::reference)
            .def("get_rhs_id", &LoadStmt::getRHSVarID);

    py::class_<CallPE, SVFStmt>(m, "CallPE")
            .def("get_callsite", &CallPE::getCallSite)
            .def("get_lhs_var", &CallPE::getLHSVar, py::return_value_policy::reference)
            .def("get_lhs_id", &CallPE::getLHSVarID)
            .def("get_rhs_var", &CallPE::getRHSVar, py::return_value_policy::reference)
            .def("get_rhs_id", &CallPE::getRHSVarID)
            .def("get_fun_entry_icfg_node", &CallPE::getFunEntryICFGNode, py::return_value_policy::reference);

    py::class_<RetPE, SVFStmt>(m, "RetPE")
            .def("get_callsite", &RetPE::getCallSite)
            .def("get_lhs_var", &RetPE::getLHSVar, py::return_value_policy::reference)
            .def("get_lhs_id", &RetPE::getLHSVarID)
            .def("get_rhs_var", &RetPE::getRHSVar, py::return_value_policy::reference)
            .def("get_rhs_id", &RetPE::getRHSVarID)
            .def("get_fun_exit_icfg_node", &RetPE::getFunExitICFGNode, py::return_value_policy::reference);

    py::class_<GepStmt, SVFStmt>(m, "GepStmt")
            .def("get_lhs_var", &GepStmt::getLHSVar, py::return_value_policy::reference)
            .def("get_lhs_id", &GepStmt::getLHSVarID)
            .def("get_rhs_var", &GepStmt::getRHSVar, py::return_value_policy::reference)
            .def("get_rhs_id", &GepStmt::getRHSVarID)
            .def("is_constant_offset", &GepStmt::isConstantOffset)
            .def("get_constant_offset", &GepStmt::accumulateConstantOffset)
            .def("get_constant_byte_offset", &GepStmt::accumulateConstantByteOffset)
            .def("get_constant_struct_fld_idx", &GepStmt::getConstantStructFldIdx)
            //maybe change a name
            .def("get_offset_var_and_gep_type_pair_vec", &GepStmt::getOffsetVarAndGepTypePairVec, py::return_value_policy::reference);

    py::class_<PhiStmt, SVFStmt>(m, "PhiStmt")
            // TODO: may implement get_op_var and get_op_var_id
            .def("get_res_var", &PhiStmt::getRes, py::return_value_policy::reference)
            .def("get_res_id", &PhiStmt::getResID)
            .def("get_op_icfg_node", [](PhiStmt& stmt, int idx) { return stmt.getOpVar(idx); },
                 py::return_value_policy::reference);

   // TODO: selectStmt

    py::class_<CmpStmt, SVFStmt>(m, "CmpStmt")
            // TODO: Return int, maybe need to think about friendly return value
            .def("get_predicate", &CmpStmt::getPredicate)
            .def("get_res", &CmpStmt::getRes, py::return_value_policy::reference)
            .def("get_res_id", &CmpStmt::getResID)
            // TODO: implement SVFVar
            .def("get_op_var", [](CmpStmt& stmt, int ID) { return stmt.getOpVar(ID); },
                 py::return_value_policy::reference);

    py::class_<BinaryOPStmt, SVFStmt>(m, "BinaryOPStmt")
            //TODO: enum of get_op
            .def("get_op", &BinaryOPStmt::getOpcode)
            .def("get_res", &BinaryOPStmt::getRes, py::return_value_policy::reference)
            .def("get_res_id", &BinaryOPStmt::getResID)
            .def("get_op_var", [](BinaryOPStmt& stmt, int ID) { return stmt.getOpVar(ID); },
                 py::return_value_policy::reference);

    py::class_<UnaryOPStmt, SVFStmt>(m, "UnaryOPStmt")
            .def("get_op", &UnaryOPStmt::getOpcode)
            .def("get_res", &UnaryOPStmt::getRes, py::return_value_policy::reference)
            .def("get_res_id", &UnaryOPStmt::getResID)
            .def("get_op_var", [](UnaryOPStmt& stmt) { return stmt.getOpVar(); },
                 py::return_value_policy::reference);

    py::class_<BranchStmt, SVFStmt>(m, "BranchStmt")
            //std::vector<std::pair<const ICFGNode*, s32_t>> getSuccessors()
            .def("get_successors", &BranchStmt::getSuccessors, py::return_value_policy::reference)
            .def("get_num_successors", &BranchStmt::getNumSuccessors)
            .def("is_conditional", &BranchStmt::isConditional)
            .def("is_unconditional", &BranchStmt::isUnconditional)
            .def("get_condition", &BranchStmt::getCondition, py::return_value_policy::reference)
            .def("get_branch_inst", &BranchStmt::getBranchInst, py::return_value_policy::reference);


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
            }, py::arg("id"), py::return_value_policy::reference)

            .def("get_global_icfg_node", &ICFG::getGlobalICFGNode, py::return_value_policy::reference)
            //void dump(const std::string& file);
            .def("dump", [](ICFG &icfg, std::string file) {
                icfg.dump(file);
            });
}


// Bind SVFIR (PAG)
void bind_svf(py::module& m) {
    py::class_<SVFIR>(m, "SVFIR")
            .def("get_icfg", [](SVFIR* pag) { return pag->getICFG(); }, py::return_value_policy::reference)
            .def("get_call_sites", [](SVFIR *pag) {
                // move  pag->getCallSiteSet() to vector
                std::vector<const CallICFGNode*> callSites;
                for (auto &callSite : pag->getCallSiteSet()) {
                    callSites.push_back(callSite);
                }
                return callSites;
                }, py::return_value_policy::reference);

}

// Bind SVFVar
void bind_svf_var(py::module &m) {
    py::class_<SVF::SVFVar>(m, "SVFVar")
            .def("get_name", &SVF::SVFVar::getName)
            .def("get_id", &SVF::SVFVar::getId)
            .def("is_pointer", &SVF::SVFVar::isPointer)
            .def("is_const_data_or_agg_data_but_not_null_ptr", &SVF::SVFVar::isConstDataOrAggDataButNotNullPtr)
            .def("is_isolated_node", &SVF::SVFVar::isIsolatedNode)
            .def("get_value_name", &SVF::SVFVar::getValueName)
            .def("get_function", &SVF::SVFVar::getFunction, py::return_value_policy::reference)
            .def("ptr_in_uncalled_function", &SVF::SVFVar::ptrInUncalledFunction)
            .def("is_const_data_or_agg_data", &SVF::SVFVar::isConstDataOrAggData)
            .def("to_string", &SVF::SVFVar::toString);

    py::class_<SVF::ValVar, SVF::SVFVar>(m, "ValVar")
            .def("get_icfg_node", &SVF::ValVar::getICFGNode, py::return_value_policy::reference)
            .def("get_value_name", &SVF::ValVar::getValueName)
            .def("get_function", &SVF::ValVar::getFunction, py::return_value_policy::reference)
            .def("to_string", &SVF::ValVar::toString);

    py::class_<SVF::ObjVar, SVF::SVFVar>(m, "ObjVar")
            .def("get_value_name", &SVF::ObjVar::getValueName)
            .def("to_string", &SVF::ObjVar::toString);

    py::class_<SVF::ArgValVar, SVF::ValVar>(m, "ArgValVar")
            .def("get_function", &SVF::ArgValVar::getFunction, py::return_value_policy::reference)
            .def("get_parent", &SVF::ArgValVar::getParent, py::return_value_policy::reference)
            .def("get_arg_no", &SVF::ArgValVar::getArgNo)
            .def("is_pointer", &SVF::ArgValVar::isPointer)
            .def("to_string", &SVF::ArgValVar::toString);

    py::class_<SVF::GepValVar, SVF::ValVar>(m, "GepValVar")
            .def("get_constant_field_idx", &SVF::GepValVar::getConstantFieldIdx)
            .def("get_base_node", &SVF::GepValVar::getBaseNode, py::return_value_policy::reference)
            .def("get_value_name", &SVF::GepValVar::getValueName)
            .def("is_pointer", &SVF::GepValVar::isPointer)
            .def("get_type", &SVF::GepValVar::getType, py::return_value_policy::reference)
            .def("get_function", &SVF::GepValVar::getFunction, py::return_value_policy::reference)
            .def("to_string", &SVF::GepValVar::toString);


    //// BaseObjVar
    py::class_<SVF::BaseObjVar, SVF::ObjVar>(m, "BaseObjVar")
            .def("get_type", &SVF::BaseObjVar::getType, py::return_value_policy::reference)
            .def("get_byte_size_of_obj", &SVF::BaseObjVar::getByteSizeOfObj);

    /// GepObjVar
    py::class_<SVF::GepObjVar, SVF::ObjVar>(m, "GepObjVar")
            .def("get_constant_field_idx", &SVF::GepObjVar::getConstantFieldIdx)
            .def("get_base_node", &SVF::GepObjVar::getBaseNode, py::return_value_policy::reference)
            .def("get_type", &SVF::GepObjVar::getType, py::return_value_policy::reference)
            .def("get_value_name", &SVF::GepObjVar::getValueName)
            .def("is_pointer", &SVF::GepObjVar::isPointer)
            .def("to_string", &SVF::GepObjVar::toString);

    /// HeapObjVar
    py::class_<SVF::HeapObjVar, SVF::BaseObjVar>(m, "HeapObjVar");

    /// StackObjVar
    py::class_<SVF::StackObjVar, SVF::BaseObjVar>(m, "StackObjVar");



    py::class_<SVF::FunObjVar, SVF::BaseObjVar>(m, "FunObjVar")
            .def("is_declaration", &SVF::FunObjVar::isDeclaration)
            .def("is_intrinsic", &SVF::FunObjVar::isIntrinsic)
            .def("has_address_taken", &SVF::FunObjVar::hasAddressTaken)
            .def("is_vararg", &SVF::FunObjVar::isVarArg)
            .def("is_uncalled_function", &SVF::FunObjVar::isUncalledFunction)
            .def("has_return", &SVF::FunObjVar::hasReturn)
            .def("get_function_type", &SVF::FunObjVar::getFunctionType, py::return_value_policy::reference)
            .def("get_return_type", &SVF::FunObjVar::getReturnType, py::return_value_policy::reference)
            .def("to_string", &SVF::FunObjVar::toString);

    py::class_<SVF::FunValVar, SVF::ValVar>(m, "FunValVar")
            .def("get_function", &SVF::FunValVar::getFunction, py::return_value_policy::reference)
            .def("is_pointer", &SVF::FunValVar::isPointer)
            .def("to_string", &SVF::FunValVar::toString);


    /// GlobalValVar
    py::class_<SVF::GlobalValVar, SVF::ValVar>(m, "GlobalValVar");

    ///ConstAggValVar
    py::class_<SVF::ConstAggValVar, SVF::ValVar>(m, "ConstAggValVar");

    ///ConstDataValVar
    py::class_<SVF::ConstDataValVar, SVF::ValVar>(m, "ConstDataValVar");

    ///BlackHoleValVar
    py::class_<SVF::BlackHoleValVar, SVF::ValVar>(m, "BlackHoleValVar");

    //// ConstFPValVar
    py::class_<SVF::ConstFPValVar, SVF::ConstDataValVar>(m, "ConstFPValVar")
            .def("get_fp_value", &SVF::ConstFPValVar::getFPValue);

    //// ConstIntValVar
    py::class_<SVF::ConstIntValVar, SVF::ConstDataValVar>(m, "ConstIntValVar")
            .def("get_sext_value", &SVF::ConstIntValVar::getSExtValue)
            .def("get_zext_value", &SVF::ConstIntValVar::getZExtValue);

    ///ConstNullPtrValVar
    py::class_<SVF::ConstNullPtrValVar, SVF::ConstDataValVar>(m, "ConstNullPtrValVar");

    ///GlobalObjVar
    py::class_<SVF::GlobalObjVar, SVF::BaseObjVar>(m, "GlobalObjVar");

    ///ConstAggObjVar
    py::class_<SVF::ConstAggObjVar, SVF::BaseObjVar>(m, "ConstAggObjVar");

    ///ConstDataObjVar
    py::class_<SVF::ConstDataObjVar, SVF::BaseObjVar>(m, "ConstDataObjVar");

    //// ConstFPObjVar
    py::class_<SVF::ConstFPObjVar, SVF::ConstDataObjVar>(m, "ConstFPObjVar")
            .def("get_fp_value", &SVF::ConstFPObjVar::getFPValue);


    //// ConstIntObjVar
    py::class_<SVF::ConstIntObjVar, SVF::ConstDataObjVar>(m, "ConstIntObjVar")
            .def("get_sext_value", &SVF::ConstIntObjVar::getSExtValue)
            .def("get_zext_value", &SVF::ConstIntObjVar::getZExtValue);


    ///ConstNullPtrObjVar
    py::class_<SVF::ConstNullPtrObjVar, SVF::ConstDataObjVar>(m, "ConstNullPtrObjVar");

    //// RetValPN
    //inline const FunObjVar* getCallGraphNode() const;
    py::class_<SVF::RetValPN, SVF::ValVar>(m, "RetValPN")
            .def("get_function", &SVF::RetValPN::getFunction, py::return_value_policy::reference);

    ///VarArgValPN
    py::class_<SVF::VarArgValPN, SVF::ValVar>(m, "VarArgValPN")
            .def("get_function", &SVF::VarArgValPN::getFunction, py::return_value_policy::reference);

    ///DummyValVar
    py::class_<SVF::DummyValVar, SVF::ValVar>(m, "DummyValVar");
    ///DummyObjVar
    py::class_<SVF::DummyObjVar, SVF::BaseObjVar>(m, "DummyObjVar");
}


// Bind SVFType
void bind_svf_type(py::module& m) {
    py::class_<SVFType>(m, "SVFType")
            .def("get_kind", &SVFType::getKind)
            .def("get_byte_size", &SVFType::getByteSize)
            .def("is_pointer_ty", &SVFType::isPointerTy)
            .def("is_array_ty", &SVFType::isArrayTy)
            .def("is_struct_ty", &SVFType::isStructTy)
            .def("is_single_value_ty", &SVFType::isSingleValueType)
            .def("to_string", &SVFType::toString);


    py::class_<SVFPointerType, SVFType>(m, "SVFPointerType")
            .def("print", &SVFPointerType::print);

    py::class_<SVFIntegerType, SVFType>(m, "SVFIntegerType")
            .def("print", &SVFIntegerType::print)
            .def("set_sign_and_width", &SVFIntegerType::setSignAndWidth)
            .def("is_signed", &SVFIntegerType::isSigned);

    py::class_<SVFFunctionType, SVFType>(m, "SVFFunctionType")
            .def("print", &SVFFunctionType::print)
            .def("get_return_type", &SVFFunctionType::getReturnType, py::return_value_policy::reference);

    py::class_<SVFStructType, SVFType>(m, "SVFStructType")
            .def("print", &SVFStructType::print)
            .def("get_name", &SVFStructType::getName)
            .def("set_name", py::overload_cast<const std::string&>(&SVFStructType::setName))
            .def("set_name", py::overload_cast<std::string&&>(&SVFStructType::setName));

    py::class_<SVFArrayType, SVFType>(m, "SVFArrayType")
            .def("print", &SVFArrayType::print)
            .def("get_type_of_element", &SVFArrayType::getTypeOfElement, py::return_value_policy::reference)
            .def("set_type_of_element", &SVFArrayType::setTypeOfElement)
            .def("set_num_of_element", &SVFArrayType::setNumOfElement);

    py::class_<SVFOtherType, SVFType>(m, "SVFOtherType")
            .def("print", &SVFOtherType::print)
            .def("get_repr", &SVFOtherType::getRepr)
            .def("set_repr", py::overload_cast<const std::string&>(&SVFOtherType::setRepr))
            .def("set_repr", py::overload_cast<std::string&&>(&SVFOtherType::setRepr));

    // Downcasting utilities for polymorphic types
    m.def("as_pointer_type", [](SVFType* type) { return dynamic_cast<SVFPointerType*>(type); }, py::return_value_policy::reference);
    m.def("as_integer_type", [](SVFType* type) { return dynamic_cast<SVFIntegerType*>(type); }, py::return_value_policy::reference);
    m.def("as_function_type", [](SVFType* type) { return dynamic_cast<SVFFunctionType*>(type); }, py::return_value_policy::reference);
    m.def("as_struct_type", [](SVFType* type) { return dynamic_cast<SVFStructType*>(type); }, py::return_value_policy::reference);
    m.def("as_array_type", [](SVFType* type) { return dynamic_cast<SVFArrayType*>(type); }, py::return_value_policy::reference);
    m.def("as_other_type", [](SVFType* type) { return dynamic_cast<SVFOtherType*>(type); }, py::return_value_policy::reference);
}

void bind_svf_value(py::module& m) {

    py::class_<SVFLLVMValue, std::shared_ptr<SVFLLVMValue>>(m, "SVFLLVMValue")
            .def("get_kind", &SVFLLVMValue::getKind)
            .def("get_type", &SVFLLVMValue::getType, py::return_value_policy::reference)
            .def("get_name", &SVFLLVMValue::getName)
            .def("set_name", &SVFLLVMValue::setName)
            .def("get_source_loc", &SVFLLVMValue::getSourceLoc)
            .def("set_source_loc", &SVFLLVMValue::setSourceLoc)
            .def("to_string", &SVFLLVMValue::toString);

    py::class_<SVFFunction, SVFLLVMValue, std::shared_ptr<SVFFunction>>(m, "SVFFunction")
            .def("is_declaration", &SVFFunction::isDeclaration)
            .def("is_intrinsic", &SVFFunction::isIntrinsic)
            .def("has_address_taken", &SVFFunction::hasAddressTaken)
            .def("get_function_type", &SVFFunction::getFunctionType, py::return_value_policy::reference)
            .def("get_return_type", &SVFFunction::getReturnType, py::return_value_policy::reference)
            .def("arg_size", &SVFFunction::arg_size)
            .def("get_arg", &SVFFunction::getArg, py::return_value_policy::reference)
            .def("get_entry_block", &SVFFunction::getEntryBlock, py::return_value_policy::reference)
            .def("get_exit_bb", &SVFFunction::getExitBB, py::return_value_policy::reference)
            .def("has_loop_info", &SVFFunction::hasLoopInfo)
            .def("get_loop_info", &SVFFunction::getLoopInfo, py::return_value_policy::reference)
            .def("dominate", &SVFFunction::dominate)
            .def("post_dominate", &SVFFunction::postDominate);
    /// TODO: add more subclass
}



PYBIND11_MODULE(pysvf, m) {
    bind_svf(m);
    bind_icfg_node(m);
    bind_icfg_edge(m);
    bind_icfg_graph(m);
    bind_svf_stmt(m);
    bind_svf_var(m);
    bind_svf_type(m);
    bind_svf_value(m);
    m.def("analyze", &PySVF::analyze, py::return_value_policy::reference, "Analyze LLVM bitcode and return SVFIR");
}
