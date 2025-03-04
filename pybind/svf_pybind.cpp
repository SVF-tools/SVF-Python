//===----- svf_pybind.cpp  pybind for SVF ---------------------//
//
//                     SVF: Static Value-Flow Analysis
//
// Copyright (C) <2013-2017>  <Yulei Sui>
//

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.

// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//===----------------------------------------------------------------------===//

/*
 * svf_pybind.cpp
 *
 *  Created on: 2025-02-27
 *      Author: Jiawei Wang, Yulei Sui
 */

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
    static SVFIR* get_pag(const std::string& bitcodePath) {
        std::vector<std::string> moduleNameVec = { bitcodePath };

        LLVMModuleSet::buildSVFModule(moduleNameVec);
        SVFIRBuilder builder;
        SVFIR* pag = builder.build();  // TODO: maybe we need to split build() into more steps
        CallGraph* callgraph = AndersenWaveDiff::createAndersenWaveDiff(pag)->getCallGraph();
        builder.updateCallGraph(callgraph);
        pag->getICFG()->updateCallGraph(callgraph);
        return pag;  // Now we directly return SVFIR(pag)
    }

    static void release_pag() {
        SVF::LLVMModuleSet::releaseLLVMModuleSet();
        SVF::SVFIR::releaseSVFIR();
    }
};

void bind_icfg_node(py::module& m) {
    py::class_<ICFGNode>(m, "ICFGNode", "Represents a node in the Interprocedural Control Flow Graph (ICFG)")
            .def("to_string", [](const ICFGNode& node) {
                std::ostringstream oss;
                oss << node.toString() << "\n";
                return oss.str();
            })
            //get_id
            .def("get_id", &ICFGNode::getId, "Get the ID of the ICFG node")
            .def("get_fun", &ICFGNode::getFun, py::return_value_policy::reference, "Get the function that the ICFG node belongs to")
            .def("get_bb", &ICFGNode::getBB, py::return_value_policy::reference, "Get the basic block that the ICFG node belongs to")
            .def("get_svf_stmts", &ICFGNode::getSVFStmts, py::return_value_policy::reference, "Get the SVF statements associated with the ICFG node")
            // Downcast
            .def("as_fun_entry", [](ICFGNode* node) { return SVFUtil::dyn_cast<FunEntryICFGNode>(node); }, py::return_value_policy::reference, "Downcast to FunEntryICFGNode")
            .def("as_fun_exit", [](ICFGNode* node) { return SVFUtil::dyn_cast<FunExitICFGNode>(node); }, py::return_value_policy::reference, "Downcast to FunExitICFGNode")
            .def("as_call", [](ICFGNode* node) { return SVFUtil::dyn_cast<CallICFGNode>(node); }, py::return_value_policy::reference, "Downcast to CallICFGNode")
            .def("as_ret", [](ICFGNode* node) { return SVFUtil::dyn_cast<RetICFGNode>(node); }, py::return_value_policy::reference, "Downcast to RetICFGNode")
            // Type checking
            .def("is_fun_entry", [](const ICFGNode* node) { return node->getNodeKind() == SVF::ICFGNode::FunEntryBlock; }, "Check if the ICFG node is a function entry node")
            .def("is_fun_exit", [](const ICFGNode* node) { return node->getNodeKind() == SVF::ICFGNode::FunExitBlock; }, "Check if the ICFG node is a function exit node")
            .def("is_call", [](const ICFGNode* node) { return node->getNodeKind() == SVF::ICFGNode::FunCallBlock; }, "Check if the ICFG node is a function call node")
            .def("is_ret", [](const ICFGNode* node) { return node->getNodeKind() == SVF::ICFGNode::FunRetBlock; }, "Check if the ICFG node is a function return node")
            // get out edges and get in edges. gather them in a list
            .def("get_out_edges", [](const ICFGNode *node) {
                std::vector<ICFGEdge *> edges;
                for (auto &edge : node->getOutEdges()) {
                    edges.push_back(edge);
                }
                return edges;
            }, py::return_value_policy::reference, "Get the out edges of the ICFG node")
            .def("get_in_edges", [](const ICFGNode *node) {
                std::vector < ICFGEdge * > edges;
                for (auto &edge: node->getInEdges()) {
                    edges.push_back(edge);
                }
                return edges;
            }, py::return_value_policy::reference, "Get the in edges of the ICFG node");

    // IntraICFGNode
    py::class_<IntraICFGNode, ICFGNode>(m, "IntraICFGNode")
            .def("is_ret_inst", &IntraICFGNode::isRetInst, "Check if the ICFG node is a return instruction");

    // InterICFGNode
    py::class_<InterICFGNode, ICFGNode>(m, "InterICFGNode");

    // FunEntryICFGNode
    py::class_<FunEntryICFGNode, InterICFGNode>(m, "FunEntryICFGNode")
            .def("get_formal_parms", &FunEntryICFGNode::getFormalParms, py::return_value_policy::reference,
                 "Get the formal parameters of the function")
            .def("add_formal_parm", &FunEntryICFGNode::addFormalParms, "Add a formal parameter to the function");

    // FunExitICFGNode
    py::class_<FunExitICFGNode, InterICFGNode>(m, "FunExitICFGNode")
            .def("get_formal_ret", &FunExitICFGNode::getFormalRet, py::return_value_policy::reference, "Get the formal return value of the function")
            .def("add_formal_ret", &FunExitICFGNode::addFormalRet, "Add a formal return value to the function");

    // CallICFGNode
    py::class_<CallICFGNode, InterICFGNode>(m, "CallICFGNode")
            .def("get_caller", &CallICFGNode::getCaller, py::return_value_policy::reference, "Get the caller function")
            .def("get_called_function", &CallICFGNode::getCalledFunction, py::return_value_policy::reference,
                 "Get the called function")
            .def("get_actual_parms", &CallICFGNode::getActualParms, py::return_value_policy::reference,
                 "Get the actual parameters of the call")
            .def("add_actual_parm", &CallICFGNode::addActualParms, "Add an actual parameter to the call")
            .def("is_vararg", &CallICFGNode::isVarArg, "Check if the call is a vararg call")
            .def("is_virtual_call", &CallICFGNode::isVirtualCall, "Check if the call is a virtual call");

    // === RetICFGNode ===
    py::class_<RetICFGNode, InterICFGNode>(m, "RetICFGNode")
            .def("get_actual_ret", &RetICFGNode::getActualRet, py::return_value_policy::reference, "Get the actual return value")
            .def("add_actual_ret", &RetICFGNode::addActualRet, "Add an actual return value");

    // GlobalICFGNode
    py::class_<GlobalICFGNode, ICFGNode>(m, "GlobalICFGNode");
}

void bind_icfg_edge(py::module& m) {
    using namespace SVF;

    py::class_<ICFGEdge, std::shared_ptr<ICFGEdge>>(m, "ICFGEdge")
            .def("to_string", &ICFGEdge::toString, "Get the string representation of the ICFG edge")
            .def("is_cfg_edge", &ICFGEdge::isCFGEdge, "Check if the edge is a CFG edge")
            .def("is_call_cfg_edge", &ICFGEdge::isCallCFGEdge, "Check if the edge is a call CFG edge")
            .def("is_ret_cfg_edge", &ICFGEdge::isRetCFGEdge, "Check if the edge is a return CFG edge")
            .def("is_intra_cfg_edge", &ICFGEdge::isIntraCFGEdge, "Check if the edge is an intra CFG edge")
            // get inedge and outedge
            .def("get_src", &ICFGEdge::getSrcNode, py::return_value_policy::reference, "Get the source node of the edge")
            .def("get_dst", &ICFGEdge::getDstNode, py::return_value_policy::reference, "Get the destination node of the edge")
            //downcast
            .def("as_intra_cfg_edge", [](ICFGEdge *edge) { return SVFUtil::dyn_cast<IntraCFGEdge>(edge); },
                 py::return_value_policy::reference, "Downcast to IntraCFGEdge")
            .def("as_call_cfg_edge", [](ICFGEdge *edge) { return SVFUtil::dyn_cast<CallCFGEdge>(edge); },
                 py::return_value_policy::reference, "Downcast to CallCFGEdge")
            .def("as_ret_cfg_edge", [](ICFGEdge *edge) { return SVFUtil::dyn_cast<RetCFGEdge>(edge); },
                 py::return_value_policy::reference, "Downcast to RetCFGEdge");

    py::class_<IntraCFGEdge, ICFGEdge, std::shared_ptr<IntraCFGEdge>>(m, "IntraCFGEdge")
            .def("get_condition", &IntraCFGEdge::getCondition, py::return_value_policy::reference,
                 "Get the condition of the edge")
            .def("get_successor_cond_value", &IntraCFGEdge::getSuccessorCondValue, py::return_value_policy::reference,
                 "Get the successor condition value");

    py::class_<CallCFGEdge, ICFGEdge, std::shared_ptr<CallCFGEdge>>(m, "CallCFGEdge")
            .def("get_call_site", &CallCFGEdge::getCallSite, py::return_value_policy::reference, "Get the call site")
            .def("get_call_pes", &CallCFGEdge::getCallPEs, py::return_value_policy::reference, "Get the call PEs");

    py::class_<RetCFGEdge, ICFGEdge, std::shared_ptr<RetCFGEdge>>(m, "RetCFGEdge")
            .def("get_call_site", &RetCFGEdge::getCallSite, py::return_value_policy::reference, "Get the call site")
            .def("get_ret_pe", &RetCFGEdge::getRetPE, py::return_value_policy::reference, "Get the return PE");
}



void bind_svf_stmt(py::module& m) {
    py::class_<SVFStmt>(m, "SVFStmt")
            .def("to_string", [](const SVFStmt& stmt) {
                std::ostringstream oss;
                oss << stmt.toString();
                return oss.str();
            }, "Get the string representation of the SVF statement")
            .def("get_edge_id", &SVFStmt::getEdgeID, "Get the ID of the SVF statement")
            .def("get_icfg_node", &SVFStmt::getICFGNode, py::return_value_policy::reference_internal, "Get the ICFG node that the SVF statement belongs to")
            .def("get_value", &SVFStmt::getValue, py::return_value_policy::reference_internal, "Get the value of the SVF statement")
            .def("get_bb", &SVFStmt::getBB, py::return_value_policy::reference_internal, "Get the basic block that the SVF statement belongs to")
                    // addr copy store load call ret gep phi select cmp binary unary branch
                    // TODO: may support threadFork threadJoin
            .def("is_addr_stmt", [](const SVFStmt* stmt) { return SVFUtil::dyn_cast<const AddrStmt>(stmt) != nullptr; }, "Check if the SVF statement is an address statement")
            .def("is_copy_stmt", [](const SVFStmt* stmt) { return SVFUtil::dyn_cast<const CopyStmt>(stmt) != nullptr; }, "Check if the SVF statement is a copy statement")
            .def("is_store_stmt", [](const SVFStmt* stmt) { return SVFUtil::dyn_cast<const StoreStmt>(stmt) != nullptr; }, "Check if the SVF statement is a store statement")
            .def("is_load_stmt", [](const SVFStmt* stmt) { return SVFUtil::dyn_cast<const LoadStmt>(stmt) != nullptr; }, "Check if the SVF statement is a load statement")
            .def("is_call_pe", [](const SVFStmt* stmt) { return SVFUtil::dyn_cast<const CallPE>(stmt) != nullptr; }, "Check if the SVF statement is a call PE")
            .def("is_ret_pe", [](const SVFStmt* stmt) { return SVFUtil::dyn_cast<const RetPE>(stmt) != nullptr; }, "Check if the SVF statement is a return PE")
            .def("is_gep_stmt", [](const SVFStmt* stmt) {return SVFUtil::dyn_cast<const GepStmt>(stmt) != nullptr; }, "Check if the SVF statement is a GEP statement")
            .def("is_phi_stmt", [](const SVFStmt* stmt) { return SVFUtil::dyn_cast<const PhiStmt>(stmt) != nullptr; },
                 "Check if the SVF statement is a phi statement")
            .def("is_select_stmt", [](const SVFStmt* stmt) {
                return SVFUtil::dyn_cast<const SelectStmt>(stmt) != nullptr;}, "Check if the SVF statement is a select statement")
            .def("is_cmp_stmt", [](const SVFStmt* stmt) {
                return SVFUtil::dyn_cast<const CmpStmt>(stmt) != nullptr;}, "Check if the SVF statement is a compare statement")
            .def("is_binary_op_stmt", [](const SVFStmt* stmt) {
                return SVFUtil::dyn_cast<const BinaryOPStmt>(stmt) != nullptr;}, "Check if the SVF statement is a binary operation statement")
            .def("is_unary_op_stmt", [](const SVFStmt* stmt) {
                return SVFUtil::dyn_cast<const UnaryOPStmt>(stmt) != nullptr;}, "Check if the SVF statement is a unary operation statement")
            .def("is_branch_stmt", [](const SVFStmt* stmt) {
                return SVFUtil::dyn_cast<const BranchStmt>(stmt) != nullptr;}, "Check if the SVF statement is a branch statement")
             // downcast TODO: more downcast here
            .def("as_addr_stmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<AddrStmt>(stmt); },
                  py::return_value_policy::reference, "Downcast the SVF statement to an address statement")
            .def("as_copy_stmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<CopyStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a copy statement")
            .def("as_store_stmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<StoreStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a store statement")
            .def("as_load_stmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<LoadStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a load statement")
            .def("as_call_pe", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<CallPE>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a call PE")
            .def("as_ret_pe", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<RetPE>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a return PE")
            .def("as_gep_stmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<GepStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a GEP statement")
            .def("as_phi_stmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<PhiStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a phi statement")
            .def("as_select_stmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<SelectStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a select statement")
            .def("as_cmp_stmt", [](SVFStmt* stmt) { return SVFUtil::dyn_cast<CmpStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a compare statement")
            .def("as_binary_op_stmt", [](SVFStmt* stmt) { return SVFUtil::dyn_cast<BinaryOPStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a binary operation statement")
            .def("as_unary_op_stmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<UnaryOPStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a unary operation statement")
            .def("as_branch_stmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<BranchStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a branch statement");

    py::class_<AddrStmt, SVFStmt>(m, "AddrStmt")
            .def("get_lhs_var", &AddrStmt::getLHSVar, py::return_value_policy::reference, "Get the LHS variable of the address statement")
            .def("get_lhs_id", &AddrStmt::getLHSVarID, "Get the ID of the LHS variable of the address statement")
            .def("get_rhs_var", &AddrStmt::getRHSVar, py::return_value_policy::reference, "Get the RHS variable of the address statement")
            .def("get_rhs_id", &AddrStmt::getRHSVarID, "Get the ID of the RHS variable of the address statement")
            .def("get_arr_size", &AddrStmt::getArrSize, py::return_value_policy::reference, "Get the array size of the address statement");

    py::class_<CopyStmt, SVFStmt>(m, "CopyStmt")
            // TODO: more API from CopyStmt
            .def("get_lhs_var", &CopyStmt::getLHSVar, py::return_value_policy::reference, "Get the LHS variable of the copy statement")
            .def("get_lhs_id", &CopyStmt::getLHSVarID, "Get the ID of the LHS variable of the copy statement")
            .def("get_rhs_var", &CopyStmt::getRHSVar, py::return_value_policy::reference, "Get the RHS variable of the copy statement")
            .def("get_rhs_id", &CopyStmt::getRHSVarID, "Get the ID of the RHS variable of the copy statement");


    py::class_<StoreStmt, SVFStmt>(m, "StoreStmt")
            .def("get_lhs_var", &StoreStmt::getLHSVar, py::return_value_policy::reference, "Get the LHS variable of the store statement")
            .def("get_lhs_id", &StoreStmt::getLHSVarID, "Get the ID of the LHS variable of the store statement")
            .def("get_rhs_var", &StoreStmt::getRHSVar, py::return_value_policy::reference,
                 "Get the RHS variable of the store statement")
            .def("get_rhs_id", &StoreStmt::getRHSVarID, "Get the ID of the RHS variable of the store statement");


    py::class_<LoadStmt, SVFStmt>(m, "LoadStmt")
            .def("get_lhs_var", &LoadStmt::getLHSVar, py::return_value_policy::reference,
                 "Get the LHS variable of the load statement")
            .def("get_lhs_id", &LoadStmt::getLHSVarID, "Get the ID of the LHS variable of the load statement")
            .def("get_rhs_var", &LoadStmt::getRHSVar, py::return_value_policy::reference,
                 "Get the RHS variable of the load statement")
            .def("get_rhs_id", &LoadStmt::getRHSVarID, "Get the ID of the RHS variable of the load statement");

    py::class_<CallPE, SVFStmt>(m, "CallPE")
            .def("get_callsite", &CallPE::getCallSite, "Get the call site")
            .def("get_lhs_var", &CallPE::getLHSVar, py::return_value_policy::reference,
                 "Get the LHS variable of the call PE")
            .def("get_lhs_id", &CallPE::getLHSVarID, "Get the ID of the LHS variable of the call PE")
            .def("get_rhs_var", &CallPE::getRHSVar, py::return_value_policy::reference,
                 "Get the RHS variable of the call PE")
            .def("get_rhs_id", &CallPE::getRHSVarID, "Get the ID of the RHS variable of the call PE")
            .def("get_fun_entry_icfg_node", &CallPE::getFunEntryICFGNode, py::return_value_policy::reference,
                 "Get the function entry ICFG node");

    py::class_<RetPE, SVFStmt>(m, "RetPE")
            .def("get_callsite", &RetPE::getCallSite, "Get the call site")
            .def("get_lhs_var", &RetPE::getLHSVar, py::return_value_policy::reference,
                 "Get the LHS variable of the return PE")
            .def("get_lhs_id", &RetPE::getLHSVarID, "Get the ID of the LHS variable of the return PE")
            .def("get_rhs_var", &RetPE::getRHSVar, py::return_value_policy::reference,
                 "Get the RHS variable of the return PE")
            .def("get_rhs_id", &RetPE::getRHSVarID, "Get the ID of the RHS variable of the return PE")
            .def("get_fun_exit_icfg_node", &RetPE::getFunExitICFGNode, py::return_value_policy::reference,
                 "Get the function exit ICFG node");


    py::class_<GepStmt, SVFStmt>(m, "GepStmt")
            .def("get_lhs_var", &GepStmt::getLHSVar, py::return_value_policy::reference, "Get the LHS variable of the GEP statement")
            .def("get_lhs_id", &GepStmt::getLHSVarID, "Get the ID of the LHS variable of the GEP statement")
            .def("get_rhs_var", &GepStmt::getRHSVar, py::return_value_policy::reference, "Get the RHS variable of the GEP statement")
            .def("get_rhs_id", &GepStmt::getRHSVarID, "Get the ID of the RHS variable of the GEP statement")
            .def("is_constant_offset", &GepStmt::isConstantOffset, "Check if the GEP statement has a constant offset")
            .def("get_constant_offset", &GepStmt::accumulateConstantOffset, "Get the constant offset of the GEP statement")
            .def("get_constant_byte_offset", &GepStmt::accumulateConstantByteOffset,
                 "Get the constant byte offset of the GEP statement")
            .def("get_constant_struct_fld_idx", &GepStmt::getConstantStructFldIdx,
                      "Get the constant struct field index of the GEP statement")
            .def("get_offset_var_and_gep_type_pair_vec", &GepStmt::getOffsetVarAndGepTypePairVec, py::return_value_policy::reference,
                 "Get the offset variable and GEP type pair vector of the GEP statement");

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
            .def("get_call_graph", [](SVFIR* pag) { return pag->getCallGraph(); }, py::return_value_policy::reference)
            .def("get_call_sites", [](SVFIR *pag) {
                // move  pag->getCallSiteSet() to vector
                std::vector<const CallICFGNode*> callSites;
                for (auto &callSite : pag->getCallSiteSet()) {
                    callSites.push_back(callSite);
                }
                return callSites;
                }, py::return_value_policy::reference)
            .def("get_pag_node_num", &SVFIR::getPAGNodeNum);
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
            .def("to_string", &SVFType::toString)
            .def("as_pointer_type", [](SVFType* type) { return SVFUtil::dyn_cast<SVFPointerType>(type); }, py::return_value_policy::reference)
            .def("as_integer_type", [](SVFType* type) { return SVFUtil::dyn_cast<SVFIntegerType>(type); }, py::return_value_policy::reference)
            .def("as_function_type", [](SVFType* type) { return SVFUtil::dyn_cast<SVFFunctionType>(type); }, py::return_value_policy::reference)
            .def("as_struct_type", [](SVFType* type) { return SVFUtil::dyn_cast<SVFStructType>(type); }, py::return_value_policy::reference)
            .def("as_array_type", [](SVFType *type) { return SVFUtil::dyn_cast<SVFArrayType>(type); },py::return_value_policy::reference)
            .def("as_other_type", [](SVFType *type) { return SVFUtil::dyn_cast<SVFOtherType>(type); },py::return_value_policy::reference)
            .def("is_pointer_type", [](SVFType* type) { return SVFUtil::isa<SVFPointerType>(type); })
            .def("is_integer_type", [](SVFType* type) { return SVFUtil::isa<SVFIntegerType>(type); })
            .def("is_function_type", [](SVFType* type) { return SVFUtil::isa<SVFFunctionType>(type); })
            .def("is_struct_type", [](SVFType* type) { return SVFUtil::isa<SVFStructType>(type); })
            .def("is_array_type", [](SVFType* type) { return SVFUtil::isa<SVFArrayType>(type); })
            .def("is_other_type", [](SVFType* type) { return SVFUtil::isa<SVFOtherType>(type); });


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


}

// Add these bindings for CallGraph classes

void bind_callgraph_node(py::module& m) {
    py::class_<CallGraphNode>(m, "CallGraphNode", "Represents a node in the Call Graph")
        .def("to_string", [](const CallGraphNode& node) {
            std::ostringstream oss;
            oss << node.toString();
            return oss.str();
        }, "Get the string representation of the CallGraph node")
        .def("get_id", &CallGraphNode::getId, "Get the ID of the CallGraph node")
        .def("get_function", &CallGraphNode::getFunction, py::return_value_policy::reference, "Get the function of this call node")
        .def("get_name", &CallGraphNode::getName, "Get the name of the function")
        .def("is_reachable_from_prog_entry", &CallGraphNode::isReachableFromProgEntry, "Check if this function can be reached from main")
        // Get out edges and in edges
        .def("get_out_edges", [](const CallGraphNode *node) {
            std::vector<CallGraphEdge *> edges;
            for (auto it = node->OutEdgeBegin(); it != node->OutEdgeEnd(); ++it) {
                edges.push_back(*it);
            }
            return edges;
        }, py::return_value_policy::reference, "Get the out edges of the CallGraph node")
        .def("get_in_edges", [](const CallGraphNode *node) {
            std::vector<CallGraphEdge *> edges;
            for (auto it = node->InEdgeBegin(); it != node->InEdgeEnd(); ++it) {
                edges.push_back(*it);
            }
            return edges;
        }, py::return_value_policy::reference, "Get the in edges of the CallGraph node");
}

void bind_callgraph_edge(py::module& m) {
    py::class_<CallGraphEdge>(m, "CallGraphEdge", "Represents an edge in the Call Graph")
        .def("to_string", [](const CallGraphEdge& edge) {
            std::ostringstream oss;
            oss << edge.toString();
            return oss.str();
        }, "Get the string representation of the CallGraph edge")
        .def("get_call_site_id", &CallGraphEdge::getCallSiteID, "Get the call site ID")
        .def("is_direct_call_edge", &CallGraphEdge::isDirectCallEdge, "Check if this is a direct call edge")
        .def("is_indirect_call_edge", &CallGraphEdge::isIndirectCallEdge, "Check if this is an indirect call edge")
        .def("get_direct_calls", [](const CallGraphEdge& edge) {
            return edge.getDirectCalls();
        }, py::return_value_policy::reference, "Get the direct calls")
        .def("get_indirect_calls", [](const CallGraphEdge& edge) {
            return edge.getIndirectCalls();
        }, py::return_value_policy::reference, "Get the indirect calls")
        // Get source and destination nodes
        .def("get_src_node", &CallGraphEdge::getSrcNode, py::return_value_policy::reference, "Get the source node")
        .def("get_dst_node", &CallGraphEdge::getDstNode, py::return_value_policy::reference, "Get the destination node")
        .def("get_src_id", &CallGraphEdge::getSrcID, "Get the source node ID")
        .def("get_dst_id", &CallGraphEdge::getDstID, "Get the destination node ID");
}

void bind_callgraph(py::module& m) {
    py::class_<CallGraph>(m, "CallGraph", "Call Graph used internally for various pointer analyses")
        .def("get_nodes", [](const CallGraph& cg) {
            std::vector<CallGraphNode*> nodes;
            for (auto& node : cg) {
                nodes.push_back(node.second);
            }
            return nodes;
        }, py::return_value_policy::reference, "Get all nodes in the call graph")
        .def("get_call_graph_node", [](CallGraph& cg, const FunObjVar* fun) {
            return cg.getCallGraphNode(fun);
        }, py::return_value_policy::reference, "Get a call graph node by function")
        .def("get_call_graph_node_by_name", [](CallGraph& cg, const std::string& name) {
            return cg.getCallGraphNode(name);
        }, py::return_value_policy::reference, "Get a call graph node by function name")
        .def("get_call_graph_node_by_id", [](CallGraph& cg, NodeID id) -> CallGraphNode* {
            CallGraphNode* node = cg.getGNode(id);
            if (!node) {
                throw std::runtime_error("CallGraphNode with given ID not found.");
            }
            return node;
        }, py::arg("id"), py::return_value_policy::reference, "Get a call graph node by ID")
        .def("is_reachable_between_functions", &CallGraph::isReachableBetweenFunctions, "Check if there's a path between two functions")
        .def("dump", &CallGraph::dump, "Dump the call graph to a DOT file")
        .def("view", &CallGraph::view, "View the call graph");
}

PYBIND11_MODULE(pysvf, m) {
    bind_svf(m);
    bind_icfg_node(m);
    bind_icfg_edge(m);
    bind_icfg_graph(m);
    bind_svf_stmt(m);
    bind_svf_var(m);
    bind_svf_type(m);
    m.def("get_pag", &PySVF::get_pag, py::return_value_policy::reference, "Analyze LLVM bitcode and return SVFIR");
    m.def("release_pag", &PySVF::release_pag, "Release SVFIR and LLVMModuleSet");
    bind_callgraph_node(m);
    bind_callgraph_edge(m);
    bind_callgraph(m);
}
