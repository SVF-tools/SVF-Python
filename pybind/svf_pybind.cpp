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
#include "Util/Options.h"
#include <pybind11/stl.h>
#include "SVF-LLVM/SVFIRBuilder.h"
#include "Graphs/ICFG.h"
#include "SVFIR/SVFType.h"
#include "SVFIR/SVFStatements.h"
#include "MemoryModel/PointerAnalysis.h"
#include "WPA/Andersen.h"

namespace py = pybind11;
using namespace SVF;

class PySVF {
static SVFIR* currentSVFIR;
static SVFG* currentSVFG;
static CallGraph* currentCallGraph;
static ICFG* currentICFG;
static std::string lastAnalyzedModule;

public:
    static SVFIR* get_pag(std::string bitcodePath, bool buildSVFG = false) {
        std::vector<std::string> moduleNameVec = { bitcodePath };
        Options::UsePreCompFieldSensitive.setValue(false);
        Options::ModelConsts.setValue(true);
        Options::ModelArrays.setValue(true);
        LLVMModuleSet::buildSVFModule(moduleNameVec);
        SVFIRBuilder builder;
        SVFIR* pag = builder.build();
        AndersenWaveDiff* ander = AndersenWaveDiff::createAndersenWaveDiff(pag);
        CallGraph* callgraph = ander->getCallGraph();
        builder.updateCallGraph(callgraph);
        pag->getICFG()->updateCallGraph(callgraph);
        

        currentSVFIR = pag;
        currentCallGraph = callgraph;
        currentICFG = pag->getICFG();
        if (buildSVFG) {
            SVFGBuilder* svfgBuilder = new SVFGBuilder(pag);
            SVFG* svfg = svfgBuilder->buildFullSVFG(ander);
            currentSVFG = svfg;
        }
        lastAnalyzedModule = bitcodePath;

        return pag;  // Now we directly return SVFIR(pag)
    }

    static void release_pag() {
        SVF::LLVMModuleSet::releaseLLVMModuleSet();
        SVF::SVFIR::releaseSVFIR();
        NodeIDAllocator::unset();
    }

    static SVFIR* get_current_pag() {
        return currentSVFIR;
    }
    static SVFG* get_current_svfg() {
        return currentSVFG;
    }
    static CallGraph* get_current_call_graph() {
        return currentCallGraph;
    }
    static ICFG* get_current_icfg() {
        return currentICFG;
    }
    static std::string get_last_analyzed_module() {
        return lastAnalyzedModule;
    }
};

void bind_icfg_node(py::module& m) {
    py::class_<ICFGNode>(m, "ICFGNode", "Represents a node in the Interprocedural Control Flow Graph (ICFG)")
            .def("to_string", [](const ICFGNode& node) {
                std::ostringstream oss;
                oss << node.toString() << "\n";
                return oss.str();
            })
            .def("__str__", [](const ICFGNode& node) {
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
            .def("is_virtual_call", &CallICFGNode::isVirtualCall, "Check if the call is a virtual call")
            .def("get_ret_node", &CallICFGNode::getRetICFGNode, py::return_value_policy::reference, "Get the return node");

    // === RetICFGNode ===
    py::class_<RetICFGNode, InterICFGNode>(m, "RetICFGNode")
            .def("get_actual_ret", &RetICFGNode::getActualRet, py::return_value_policy::reference, "Get the actual return value")
            .def("add_actual_ret", &RetICFGNode::addActualRet, "Add an actual return value")
            .def("get_call_node", &RetICFGNode::getCallICFGNode, py::return_value_policy::reference, "Get the call node") ;

    // GlobalICFGNode
    py::class_<GlobalICFGNode, ICFGNode>(m, "GlobalICFGNode");
}

void bind_icfg_edge(py::module& m) {
    using namespace SVF;

    py::class_<ICFGEdge, std::shared_ptr<ICFGEdge>>(m, "ICFGEdge")
            .def("to_string", &ICFGEdge::toString, "Get the string representation of the ICFG edge")
            .def("__str__", &ICFGEdge::toString, "Get the string representation of the ICFG edge")
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
            // constructor  (const ICFGNode*, const ICFGNode*), const cast and create a new instance
            .def(py::init([](const ICFGNode *src, const ICFGNode *dst) {
                return std::make_shared<IntraCFGEdge>(const_cast<ICFGNode*>(src), const_cast<ICFGNode*>(dst));
            }))
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
            .def("__str__", [](const SVFStmt& stmt) {
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
                 "Get the offset variable and GEP type pair vector of the GEP statement")
            .def("get_src_pointee_type", [](GepStmt& stmt) { return stmt.getAccessPath().gepSrcPointeeType(); },
                 py::return_value_policy::reference);

    py::class_<PhiStmt, SVFStmt>(m, "PhiStmt")
            // TODO: may implement get_op_var and get_op_var_id
            .def("get_res_var", &PhiStmt::getRes, py::return_value_policy::reference)
            .def("get_res_id", &PhiStmt::getResID)
            .def("get_op_icfg_node", [](PhiStmt& stmt, int idx) { return stmt.getOpICFGNode(idx); },
                 py::return_value_policy::reference)
            .def("get_op_var", [](PhiStmt& stmt, int ID) { return stmt.getOpVar(ID); },
                 py::return_value_policy::reference)
                 //get varnums
            .def("get_op_var_num", &PhiStmt::getOpVarNum, "Get the number of operands of the phi statement");

   // TODO: selectStmt

    py::class_<CmpStmt, SVFStmt>(m, "CmpStmt")
            // TODO: Return int, maybe need to think about friendly return value
            .def("get_predicate", &CmpStmt::getPredicate)
            .def("get_res", &CmpStmt::getRes, py::return_value_policy::reference)
            .def("get_res_id", &CmpStmt::getResID)
            // TODO: implement SVFVar
            .def("get_op_var", [](CmpStmt& stmt, int ID) { return stmt.getOpVar(ID); },
                 py::return_value_policy::reference)
            .def("get_op_var_num", &CmpStmt::getOpVarNum, "Get the number of operands of the phi statement");

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
            .def("get_base_object", [&](SVFIR* pag, NodeID id) { //TODO: get_base_obj
                const BaseObjVar* baseObj = pag->getBaseObject(id);
                if (!baseObj) {
                    throw std::runtime_error("Base object with given ID not found.");
                }
                return baseObj;
            }, py::arg("id"), py::return_value_policy::reference)
            .def("get_pag_node_num", &SVFIR::getPAGNodeNum)
            .def("get_gnode", [](SVFIR* pag, NodeID id) {
                const SVF::PAGNode* node = pag->getGNode(id);
                if (!node) {
                    throw std::runtime_error("PAGNode with given ID not found.");
                }
                return node;
            }, py::arg("id"), py::return_value_policy::reference)
            .def("get_gep_obj_var", [](SVFIR* pag, NodeID id, const APOffset& offset) {
                NodeID gepObjVarID = pag->getGepObjVar(id, offset);
                return gepObjVarID;
            }, py::arg("id"), py::arg("offset"), py::return_value_policy::reference)
            //u32_t getNumOfFlattenElements(const SVFType* T);
            .def("get_num_of_flatten_elements", [](SVFIR* pag, const SVFType* T) {
                return pag->getNumOfFlattenElements(T);
            }, py::arg("T"), py::return_value_policy::reference)
            //u32_t getFlattenedElemIdx(const SVFType* T, u32_t origId);
            .def("get_flattened_elem_idx", [](SVFIR* pag, const SVFType* T, u32_t origId) {
                return pag->getFlattenedElemIdx(T, origId);
            }, py::arg("T"), py::arg("origId"), py::return_value_policy::reference);
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
            // Type checking methods (like isa<>)
            .def("is_val_var", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::ValVar>(node);
            })
            .def("is_obj_var", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::ObjVar>(node);
            })
            .def("is_gep_val_var", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::GepValVar>(node);
            })
            .def("is_gep_obj_var", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::GepObjVar>(node);
            })
            .def("is_fun_obj_var", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::FunObjVar>(node);
            })
            .def("is_fun_val_var", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::FunValVar>(node);
            })
            .def("is_arg_val_var", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::ArgValVar>(node);
            })
            .def("is_ret_val_var", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::RetValPN>(node);
            })
            .def("is_dummy_val_var", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::DummyValVar>(node);
            })
            .def("is_dummy_obj_var", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::DummyObjVar>(node);
            })
            // Type casting methods (like dynamic_cast<>)
            .def("as_val_var", [](SVF::SVFVar* node) -> SVF::ValVar* {
                return SVFUtil::dyn_cast<SVF::ValVar>(node);
            }, py::return_value_policy::reference)
            .def("as_obj_var", [](SVF::SVFVar* node) -> SVF::ObjVar* {
                return SVFUtil::dyn_cast<SVF::ObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_gep_val_var", [](SVF::SVFVar* node) -> SVF::GepValVar* {
                return SVFUtil::dyn_cast<SVF::GepValVar>(node);
            }, py::return_value_policy::reference)
            .def("as_gep_obj_var", [](SVF::SVFVar* node) -> SVF::GepObjVar* {
                return SVFUtil::dyn_cast<SVF::GepObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_fun_obj_var", [](SVF::SVFVar* node) -> SVF::FunObjVar* {
                return SVFUtil::dyn_cast<SVF::FunObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_fun_val_var", [](SVF::SVFVar* node) -> SVF::FunValVar* {
                return SVFUtil::dyn_cast<SVF::FunValVar>(node);
            }, py::return_value_policy::reference)
            .def("as_arg_val_var", [](SVF::SVFVar* node) -> SVF::ArgValVar* {
                return SVFUtil::dyn_cast<SVF::ArgValVar>(node);
            }, py::return_value_policy::reference)
            .def("as_ret_val_var", [](SVF::SVFVar* node) -> SVF::RetValPN* {
                return SVFUtil::dyn_cast<SVF::RetValPN>(node);
            }, py::return_value_policy::reference)
            .def("as_dummy_val_var", [](SVF::SVFVar* node) -> SVF::DummyValVar* {
                return SVFUtil::dyn_cast<SVF::DummyValVar>(node);
            }, py::return_value_policy::reference)
            .def("as_dummy_obj_var", [](SVF::SVFVar* node) -> SVF::DummyObjVar* {
                return SVFUtil::dyn_cast<SVF::DummyObjVar>(node);
            }, py::return_value_policy::reference)
            .def("to_string", &SVF::SVFVar::toString)
            .def("__str__", &SVF::SVFVar::toString);

    py::class_<SVF::ValVar, SVF::SVFVar>(m, "ValVar")
            // For ValVar conversion functions
            .def("is_fun_val_var", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::FunValVar>(node);
            })
            .def("as_fun_val_var", [](SVF::ValVar* node) -> SVF::FunValVar* {
                return SVFUtil::dyn_cast<SVF::FunValVar>(node);
            }, py::return_value_policy::reference)

            .def("is_arg_val_var", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::ArgValVar>(node);
            })
            .def("as_arg_val_var", [](SVF::ValVar* node) -> SVF::ArgValVar* {
                return SVFUtil::dyn_cast<SVF::ArgValVar>(node);
            }, py::return_value_policy::reference)

            .def("is_global_val_var", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::GlobalValVar>(node);
            })
            .def("as_global_val_var", [](SVF::ValVar* node) -> SVF::GlobalValVar* {
                return SVFUtil::dyn_cast<SVF::GlobalValVar>(node);
            }, py::return_value_policy::reference)

            .def("is_const_agg_val_var", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstAggValVar>(node);
            })
            .def("as_const_agg_val_var", [](SVF::ValVar* node) -> SVF::ConstAggValVar* {
                return SVFUtil::dyn_cast<SVF::ConstAggValVar>(node);
            }, py::return_value_policy::reference)

            .def("is_const_data_val_var", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstDataValVar>(node);
            })
            .def("as_const_data_val_var", [](SVF::ValVar* node) -> SVF::ConstDataValVar* {
                return SVFUtil::dyn_cast<SVF::ConstDataValVar>(node);
            }, py::return_value_policy::reference)

            .def("is_const_fp_val_var", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstFPValVar>(node);
            })
            .def("as_const_fp_val_var", [](SVF::ValVar* node) -> SVF::ConstFPValVar* {
                return SVFUtil::dyn_cast<SVF::ConstFPValVar>(node);
            }, py::return_value_policy::reference)

            .def("is_const_int_val_var", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstIntValVar>(node);
            })
            .def("as_const_int_val_var", [](SVF::ValVar* node) -> SVF::ConstIntValVar* {
                return SVFUtil::dyn_cast<SVF::ConstIntValVar>(node);
            }, py::return_value_policy::reference)

            .def("is_const_null_ptr_val_var", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstNullPtrValVar>(node);
            })
            .def("as_const_null_ptr_val_var", [](SVF::ValVar* node) -> SVF::ConstNullPtrValVar* {
                return SVFUtil::dyn_cast<SVF::ConstNullPtrValVar>(node);
            }, py::return_value_policy::reference)

            .def("is_black_hole_val_var", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::BlackHoleValVar>(node);
            })
            .def("as_black_hole_val_var", [](SVF::ValVar* node) -> SVF::BlackHoleValVar* {
                return SVFUtil::dyn_cast<SVF::BlackHoleValVar>(node);
            }, py::return_value_policy::reference)

            .def("is_gep_val_var", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::GepValVar>(node);
            })
            .def("as_gep_val_var", [](SVF::ValVar* node) -> SVF::GepValVar* {
                return SVFUtil::dyn_cast<SVF::GepValVar>(node);
            }, py::return_value_policy::reference)

            .def("is_ret_val_pn", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::RetValPN>(node);
            })
            .def("as_ret_val_pn", [](SVF::ValVar* node) -> SVF::RetValPN* {
                return SVFUtil::dyn_cast<SVF::RetValPN>(node);
            }, py::return_value_policy::reference)

            .def("is_var_arg_val_pn", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::VarArgValPN>(node);
            })
            .def("as_var_arg_val_pn", [](SVF::ValVar* node) -> SVF::VarArgValPN* {
                return SVFUtil::dyn_cast<SVF::VarArgValPN>(node);
            }, py::return_value_policy::reference)

            .def("is_dummy_val_var", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::DummyValVar>(node);
            })
            .def("as_dummy_val_var", [](SVF::ValVar* node) -> SVF::DummyValVar* {
                return SVFUtil::dyn_cast<SVF::DummyValVar>(node);
            }, py::return_value_policy::reference)
            .def("get_icfg_node", &SVF::ValVar::getICFGNode, py::return_value_policy::reference)
            .def("get_value_name", &SVF::ValVar::getValueName)
            .def("get_function", &SVF::ValVar::getFunction, py::return_value_policy::reference)
            .def("to_string", &SVF::ValVar::toString);

    py::class_<SVF::ObjVar, SVF::SVFVar>(m, "ObjVar")
            .def("get_value_name", &SVF::ObjVar::getValueName)
                    // For ObjVar conversion functions
            .def("is_obj_var", [](SVF::SVFVar* node) -> bool {
                return SVFUtil::isa<SVF::ObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_obj_var", [](SVF::SVFVar* node) -> SVF::ObjVar* {
                return SVFUtil::dyn_cast<SVF::ObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_base_obj_var", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::BaseObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_base_obj_var", [](SVF::ObjVar* node) -> SVF::BaseObjVar* {
                return SVFUtil::dyn_cast<SVF::BaseObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_gep_obj_var", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::GepObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_gep_obj_var", [](SVF::ObjVar* node) -> SVF::GepObjVar* {
                return SVFUtil::dyn_cast<SVF::GepObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_fun_obj_var", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::FunObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_fun_obj_var", [](SVF::ObjVar* node) -> SVF::FunObjVar* {
                return SVFUtil::dyn_cast<SVF::FunObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_global_obj_var", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::GlobalObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_global_obj_var", [](SVF::ObjVar* node) -> SVF::GlobalObjVar* {
                return SVFUtil::dyn_cast<SVF::GlobalObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_heap_obj_var", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::HeapObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_heap_obj_var", [](SVF::ObjVar* node) -> SVF::HeapObjVar* {
                return SVFUtil::dyn_cast<SVF::HeapObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_stack_obj_var", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::StackObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_stack_obj_var", [](SVF::ObjVar* node) -> SVF::StackObjVar* {
                return SVFUtil::dyn_cast<SVF::StackObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_const_agg_obj_var", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstAggObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_const_agg_obj_var", [](SVF::ObjVar* node) -> SVF::ConstAggObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstAggObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_const_data_obj_var", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstDataObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_const_data_obj_var", [](SVF::ObjVar* node) -> SVF::ConstDataObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstDataObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_const_fp_obj_var", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstFPObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_const_fp_obj_var", [](SVF::ObjVar* node) -> SVF::ConstFPObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstFPObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_const_int_obj_var", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstIntObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_const_int_obj_var", [](SVF::ObjVar* node) -> SVF::ConstIntObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstIntObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_const_null_ptr_obj_var", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstNullPtrObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_const_null_ptr_obj_var", [](SVF::ObjVar* node) -> SVF::ConstNullPtrObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstNullPtrObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_dummy_obj_var", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::DummyObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_dummy_obj_var", [](SVF::ObjVar* node) -> SVF::DummyObjVar* {
                return SVFUtil::dyn_cast<SVF::DummyObjVar>(node);
            }, py::return_value_policy::reference)
            .def("to_string", &SVF::ObjVar::toString);

    py::class_<SVF::ArgValVar, SVF::ValVar>(m, "ArgValVar")
            .def("get_function", &SVF::ArgValVar::getFunction, py::return_value_policy::reference)
            .def("get_parent", &SVF::ArgValVar::getParent, py::return_value_policy::reference)
            .def("get_arg_no", &SVF::ArgValVar::getArgNo)
            .def("is_pointer", &SVF::ArgValVar::isPointer)
            // For ArgValVar conversion functions
            .def("is_arg_val_var", [](SVF::SVFVar* node) -> bool {
                return SVFUtil::isa<SVF::ArgValVar>(node);
            }, py::return_value_policy::reference)
            .def("as_arg_val_var", [](SVF::ValVar* node) -> SVF::ArgValVar* {
                return SVFUtil::dyn_cast<SVF::ArgValVar>(node);
            }, py::return_value_policy::reference)
            .def("get_arg_no", [](SVF::ArgValVar* node) -> u32_t {
                return node->getArgNo();
            }, py::return_value_policy::reference)
            .def("get_parent", [](SVF::ArgValVar* node) -> const SVF::FunObjVar* {
                return node->getParent();
            }, py::return_value_policy::reference)
            .def("is_arg_of_uncalled_function", [](SVF::ArgValVar* node) -> bool {
                return node->isArgOfUncalledFunction();
            }, py::return_value_policy::reference)
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
            .def("get_byte_size_of_obj", &SVF::BaseObjVar::getByteSizeOfObj)
            .def("get_icfg_node", &SVF::BaseObjVar::getICFGNode, py::return_value_policy::reference)
            .def("get_value_name", &SVF::BaseObjVar::getValueName)
            .def("to_string", &SVF::BaseObjVar::toString)
            .def("get_id", &SVF::BaseObjVar::getId)
            .def("get_type", &SVF::BaseObjVar::getType, py::return_value_policy::reference)
            .def("get_num_of_elements", &SVF::BaseObjVar::getNumOfElements)
            .def("set_num_of_elements", &SVF::BaseObjVar::setNumOfElements)
            .def("get_max_field_offset_limit", &SVF::BaseObjVar::getMaxFieldOffsetLimit)
            .def("is_field_insensitive", &SVF::BaseObjVar::isFieldInsensitive)
            .def("set_field_insensitive", &SVF::BaseObjVar::setFieldInsensitive)
            .def("set_field_sensitive", &SVF::BaseObjVar::setFieldSensitive)
            .def("is_black_hole_obj", &SVF::BaseObjVar::isBlackHoleObj)
            .def("get_byte_size_of_obj", &SVF::BaseObjVar::getByteSizeOfObj)
            .def("is_constant_byte_size", &SVF::BaseObjVar::isConstantByteSize)
            .def("is_function", &SVF::BaseObjVar::isFunction)
            .def("is_global_obj", &SVF::BaseObjVar::isGlobalObj)
            .def("is_static_obj", &SVF::BaseObjVar::isStaticObj)
            .def("is_stack", &SVF::BaseObjVar::isStack)
            .def("is_heap", &SVF::BaseObjVar::isHeap)
            .def("is_struct", &SVF::BaseObjVar::isStruct)
            .def("is_array", &SVF::BaseObjVar::isArray)
            .def("is_var_struct", &SVF::BaseObjVar::isVarStruct)
            .def("is_var_array", &SVF::BaseObjVar::isVarArray)
            .def("is_constant_struct", &SVF::BaseObjVar::isConstantStruct)
            .def("is_constant_array", &SVF::BaseObjVar::isConstantArray)
            .def("is_const_data_or_const_global", &SVF::BaseObjVar::isConstDataOrConstGlobal)
            .def("is_const_data_or_agg_data", &SVF::BaseObjVar::isConstDataOrAggData)
            .def("get_function", &SVF::BaseObjVar::getFunction, py::return_value_policy::reference)
            .def("get_base_mem_obj", &SVF::BaseObjVar::getBaseMemObj, py::return_value_policy::reference)
            .def("is_base_obj_var", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::BaseObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_base_obj_var", [](SVF::ObjVar* node) -> SVF::BaseObjVar* {
                return SVFUtil::dyn_cast<SVF::BaseObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_gep_obj_var", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::GepObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_gep_obj_var", [](SVF::ObjVar* node) -> SVF::GepObjVar* {
                return SVFUtil::dyn_cast<SVF::GepObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_fun_obj_var", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::FunObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_fun_obj_var", [](SVF::BaseObjVar* node) -> SVF::FunObjVar* {
                return SVFUtil::dyn_cast<SVF::FunObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_global_obj_var", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::GlobalObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_global_obj_var", [](SVF::BaseObjVar* node) -> SVF::GlobalObjVar* {
                return SVFUtil::dyn_cast<SVF::GlobalObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_heap_obj_var", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::HeapObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_heap_obj_var", [](SVF::BaseObjVar* node) -> SVF::HeapObjVar* {
                return SVFUtil::dyn_cast<SVF::HeapObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_stack_obj_var", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::StackObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_stack_obj_var", [](SVF::BaseObjVar* node) -> SVF::StackObjVar* {
                return SVFUtil::dyn_cast<SVF::StackObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_const_agg_obj_var", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstAggObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_const_agg_obj_var", [](SVF::BaseObjVar* node) -> SVF::ConstAggObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstAggObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_const_data_obj_var", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstDataObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_const_data_obj_var", [](SVF::BaseObjVar* node) -> SVF::ConstDataObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstDataObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_const_fp_obj_var", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstFPObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_const_fp_obj_var", [](SVF::BaseObjVar* node) -> SVF::ConstFPObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstFPObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_const_int_obj_var", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstIntObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_const_int_obj_var", [](SVF::BaseObjVar* node) -> SVF::ConstIntObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstIntObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_const_null_ptr_obj_var", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstNullPtrObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_const_null_ptr_obj_var", [](SVF::BaseObjVar* node) -> SVF::ConstNullPtrObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstNullPtrObjVar>(node);
            }, py::return_value_policy::reference)
            .def("is_dummy_obj_var", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::DummyObjVar>(node);
            }, py::return_value_policy::reference)
            .def("as_dummy_obj_var", [](SVF::BaseObjVar* node) -> SVF::DummyObjVar* {
                return SVFUtil::dyn_cast<SVF::DummyObjVar>(node);
            }, py::return_value_policy::reference);

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
            .def("to_string", &SVF::FunObjVar::toString)

            .def("dominates", [](SVF::FunObjVar* node, SVF::SVFBasicBlock* bbKey, SVF::SVFBasicBlock* bbValue) -> bool {
                return node->dominate(bbKey, bbValue);
            })
            .def("post_dominates", [](SVF::FunObjVar* node, SVF::SVFBasicBlock* bbKey, SVF::SVFBasicBlock* bbValue) -> bool {
                return node->postDominate(bbKey, bbValue);
            });

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
        .def("get_src", &CallGraphEdge::getSrcNode, py::return_value_policy::reference, "Get the source node")
        .def("get_dst", &CallGraphEdge::getDstNode, py::return_value_policy::reference, "Get the destination node")
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
        .def("dump", [](CallGraph &icfg, std::string file) {
            icfg.dump(file);
        })
        .def("view", &CallGraph::view, "View the call graph");
}

// Add this to svf_pybind.cpp
void bind_basic_block(py::module& m) {
    py::class_<SVF::SVFBasicBlock>(m, "SVFBasicBlock")
            .def("get_id", &SVF::SVFBasicBlock::getId)
            .def("get_name", &SVF::SVFBasicBlock::getName)
                    // Access ICFGNodes in the basic block
            .def("get_icfg_node_list", &SVF::SVFBasicBlock::getICFGNodeList, py::return_value_policy::reference)
            .def("front", &SVF::SVFBasicBlock::front, py::return_value_policy::reference)
            .def("back", &SVF::SVFBasicBlock::back, py::return_value_policy::reference)
                    // Iteration support
            .def("__iter__", [](const SVF::SVFBasicBlock& bb) {
                return py::make_iterator(bb.begin(), bb.end());
            }, py::keep_alive<0, 1>())
                    // Function/parent information
            .def("get_parent", &SVF::SVFBasicBlock::getParent, py::return_value_policy::reference)
            .def("get_function", &SVF::SVFBasicBlock::getFunction, py::return_value_policy::reference)
                    // CFG navigation
            .def("get_successors", &SVF::SVFBasicBlock::getSuccessors, py::return_value_policy::reference)
            .def("get_predecessors", &SVF::SVFBasicBlock::getPredecessors, py::return_value_policy::reference)
            .def("get_num_successors", &SVF::SVFBasicBlock::getNumSuccessors)
                    // Position queries
            .def("get_bb_successor_pos", static_cast<u32_t (SVF::SVFBasicBlock::*)(const SVF::SVFBasicBlock*) const>(&SVF::SVFBasicBlock::getBBSuccessorPos))
            .def("get_bb_predecessor_pos", static_cast<u32_t (SVF::SVFBasicBlock::*)(const SVF::SVFBasicBlock*) const>(&SVF::SVFBasicBlock::getBBPredecessorPos))
                    // String representation
            .def("__repr__", &SVF::SVFBasicBlock::toString)
            .def("__str__", &SVF::SVFBasicBlock::toString);
}


// Bind SVFGEdge classes
void bind_svfg_edge(py::module& m) {
    // Base VFGEdge class
    py::class_<VFGEdge, std::shared_ptr<VFGEdge>>(m, "VFGEdge", "Base class for Value-Flow Graph edges")
        .def("to_string", &VFGEdge::toString, "Get the string representation of the edge")
        .def("__str__", &VFGEdge::toString, "Get the string representation of the edge")
        .def("is_direct_vfg_edge", &VFGEdge::isDirectVFGEdge, "Check if this is a direct VFG edge")
        .def("is_indirect_vfg_edge", &VFGEdge::isIndirectVFGEdge, "Check if this is an indirect VFG edge")
        .def("is_call_vfg_edge", &VFGEdge::isCallVFGEdge, "Check if this is a call VFG edge")
        .def("is_ret_vfg_edge", &VFGEdge::isRetVFGEdge, "Check if this is a return VFG edge")
        .def("is_call_direct_vfg_edge", &VFGEdge::isCallDirectVFGEdge, "Check if this is a direct call VFG edge")
        .def("is_ret_direct_vfg_edge", &VFGEdge::isRetDirectVFGEdge, "Check if this is a direct return VFG edge")
        .def("is_call_indirect_vfg_edge", &VFGEdge::isCallIndirectVFGEdge, "Check if this is an indirect call VFG edge")
        .def("is_ret_indirect_vfg_edge", &VFGEdge::isRetIndirectVFGEdge, "Check if this is an indirect return VFG edge")
        .def("is_intra_vfg_edge", &VFGEdge::isIntraVFGEdge, "Check if this is an intra-procedural VFG edge")
        .def("is_thread_mhp_indirect_vfg_edge", &VFGEdge::isThreadMHPIndirectVFGEdge, "Check if this is a thread MHP indirect VFG edge")
        .def("get_src", [](const VFGEdge *edge) { return edge->getSrcNode(); }, py::return_value_policy::reference, "Get the source node")
        .def("get_dst", [](const VFGEdge *edge) { return edge->getDstNode(); }, py::return_value_policy::reference, "Get the destination node")
        .def("as_direct_svfg_edge", [](VFGEdge *edge) -> DirectSVFGEdge* {
            if (DirectSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<DirectSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to DirectSVFGEdge")
        .def("as_indirect_svfg_edge", [](VFGEdge *edge) -> IndirectSVFGEdge* {
            if (IndirectSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<IndirectSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to IndirectSVFGEdge")
        .def("as_call_dir_svfg_edge", [](VFGEdge *edge) -> CallDirSVFGEdge* {
            if (CallDirSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<CallDirSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to CallDirVFGEdge")
        .def("as_ret_dir_svfg_edge", [](VFGEdge *edge) -> RetDirSVFGEdge* {
            if (RetDirSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<RetDirSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to RetDirSVFGEdge")
        .def("as_call_ind_svfg_edge", [](VFGEdge *edge) -> CallIndSVFGEdge* {
            if (CallIndSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<CallIndSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to CallIndSVFGEdge")
        .def("as_ret_ind_svfg_edge", [](VFGEdge *edge) -> RetIndSVFGEdge* {
            if (RetIndSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<RetIndSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to RetIndSVFGEdge");


    // DirectSVFGEdge - direct value flow edge
    py::class_<DirectSVFGEdge, VFGEdge, std::shared_ptr<DirectSVFGEdge>>(m, "DirectSVFGEdge", "Direct SVF Graph Edge")
        .def("to_string", &DirectSVFGEdge::toString, "Get the string representation of the edge")
        .def("as_intra_dir_svfg_edge", [](DirectSVFGEdge *edge) -> IntraDirSVFGEdge* {
            if (IntraDirSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<IntraDirSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to IntraDirSVFGEdge")
        .def("as_call_dir_svfg_edge", [](DirectSVFGEdge *edge) -> CallDirSVFGEdge* {
            if (CallDirSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<CallDirSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to CallDirSVFGEdge")
        .def("as_ret_dir_svfg_edge", [](DirectSVFGEdge *edge) -> RetDirSVFGEdge* {
            if (RetDirSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<RetDirSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to RetDirSVFGEdge");

    // IntraDirSVFGEdge - Intraprocedural direct SVF edge
    py::class_<IntraDirSVFGEdge, DirectSVFGEdge, std::shared_ptr<IntraDirSVFGEdge>>(m, "IntraDirSVFGEdge", "Intraprocedural direct SVF edge")
        .def("to_string", &IntraDirSVFGEdge::toString, "Get the string representation of the edge");

    // CallDirSVFGEdge - Call direct SVF edge
    py::class_<CallDirSVFGEdge, DirectSVFGEdge, std::shared_ptr<CallDirSVFGEdge>>(m, "CallDirSVFGEdge", "Call direct SVF edge")
        .def("get_call_site_id", &CallDirSVFGEdge::getCallSiteId, "Get the call site ID")
        .def("to_string", &CallDirSVFGEdge::toString, "Get the string representation of the edge");

    // RetDirSVFGEdge - Return direct SVF edge
    py::class_<RetDirSVFGEdge, DirectSVFGEdge, std::shared_ptr<RetDirSVFGEdge>>(m, "RetDirSVFGEdge", "Return direct SVF edge")
        .def("get_call_site_id", &RetDirSVFGEdge::getCallSiteId, "Get the call site ID")
        .def("to_string", &RetDirSVFGEdge::toString, "Get the string representation of the edge");

    // IndirectSVFGEdge - Indirect value flow edge
    py::class_<IndirectSVFGEdge, VFGEdge, std::shared_ptr<IndirectSVFGEdge>>(m, "IndirectSVFGEdge", "Indirect SVF Graph Edge")
        .def("get_points_to", &IndirectSVFGEdge::getPointsTo, py::return_value_policy::reference, "Get the points-to set")
        .def("add_points_to", &IndirectSVFGEdge::addPointsTo, "Add to the points-to set")
        .def("to_string", &IndirectSVFGEdge::toString, "Get the string representation of the edge")
        .def("as_intra_ind_svfg_edge", [](IndirectSVFGEdge *edge) -> IntraIndSVFGEdge* {
            if (IntraIndSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<IntraIndSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to IntraIndSVFGEdge")
        .def("as_call_ind_svfg_edge", [](IndirectSVFGEdge *edge) -> CallIndSVFGEdge* {
            if (CallIndSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<CallIndSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to CallIndSVFGEdge")
        .def("as_ret_ind_svfg_edge", [](IndirectSVFGEdge *edge) -> RetIndSVFGEdge* {
            if (RetIndSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<RetIndSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to RetIndSVFGEdge")
        .def("as_thread_mhp_ind_svfg_edge", [](IndirectSVFGEdge *edge) -> ThreadMHPIndSVFGEdge* {
            if (ThreadMHPIndSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<ThreadMHPIndSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to ThreadMHPIndSVFGEdge");

    // IntraIndSVFGEdge - Intraprocedural indirect SVF edge
    py::class_<IntraIndSVFGEdge, IndirectSVFGEdge, std::shared_ptr<IntraIndSVFGEdge>>(m, "IntraIndSVFGEdge", "Intraprocedural indirect SVF edge")
        .def("to_string", &IntraIndSVFGEdge::toString, "Get the string representation of the edge");

    // CallIndSVFGEdge - Call indirect SVF edge
    py::class_<CallIndSVFGEdge, IndirectSVFGEdge, std::shared_ptr<CallIndSVFGEdge>>(m, "CallIndSVFGEdge", "Call indirect SVF edge")
        .def("get_call_site_id", &CallIndSVFGEdge::getCallSiteId, "Get the call site ID")
        .def("to_string", &CallIndSVFGEdge::toString, "Get the string representation of the edge");

    // RetIndSVFGEdge - Return indirect SVF edge
    py::class_<RetIndSVFGEdge, IndirectSVFGEdge, std::shared_ptr<RetIndSVFGEdge>>(m, "RetIndSVFGEdge", "Return indirect SVF edge")
        .def("get_call_site_id", &RetIndSVFGEdge::getCallSiteId, "Get the call site ID")
        .def("to_string", &RetIndSVFGEdge::toString, "Get the string representation of the edge");

    // ThreadMHPIndSVFGEdge - Thread MHP indirect SVF edge
    py::class_<ThreadMHPIndSVFGEdge, IndirectSVFGEdge, std::shared_ptr<ThreadMHPIndSVFGEdge>>(m, "ThreadMHPIndSVFGEdge", "Thread MHP indirect SVF edge")
        .def("to_string", &ThreadMHPIndSVFGEdge::toString, "Get the string representation of the edge");
}


// Bind VFGNode classes
void bind_vfg_node(py::module& m) {
    // Base VFGNode class
    py::class_<VFGNode, std::shared_ptr<VFGNode>>(m, "VFGNode", "Base class for Value-Flow Graph nodes")
        .def("to_string", &VFGNode::toString, "Get the string representation of the node")
        .def("__str__", &VFGNode::toString, "Get the string representation of the node")
        .def("get_id", &VFGNode::getId, "Get the ID of this VFG node")
        .def("get_value", &VFGNode::getValue, py::return_value_policy::reference, "Get the SVF variable value")
        .def("get_icfg_node", &VFGNode::getICFGNode, py::return_value_policy::reference, "Get the corresponding ICFG node")
        .def("get_def_svf_vars", &VFGNode::getDefSVFVars, "Get the defined SVF variables")
        .def("get_out_edges", [](const VFGNode *node) {
            std::vector<VFGEdge *> edges;
            for (auto it = node->OutEdgeBegin(); it != node->OutEdgeEnd(); ++it) {
                edges.push_back(*it);
            }
            return edges;
        }, py::return_value_policy::reference, "Get the out edges of the VFG node")
        .def("get_in_edges", [](const VFGNode *node) {
            std::vector<VFGEdge *> edges;
            for (auto it = node->InEdgeBegin(); it != node->InEdgeEnd(); ++it) {
                edges.push_back(*it);
            }
            return edges;
        }, py::return_value_policy::reference, "Get the in edges of the VFG node")
        // Type checking methods
        .def("is_stmt_vfg_node", [](const VFGNode* node) { return SVFUtil::isa<StmtVFGNode>(node); }, "Check if this is a statement VFG node")
        .def("is_phi_vfg_node", [](const VFGNode* node) { return SVFUtil::isa<PHIVFGNode>(node); }, "Check if this is a phi VFG node")
        .def("is_argument_vfg_node", [](const VFGNode* node) { return SVFUtil::isa<ArgumentVFGNode>(node); }, "Check if this is an argument VFG node")
        .def("is_cmp_vfg_node", [](const VFGNode* node) { return SVFUtil::isa<CmpVFGNode>(node); }, "Check if this is a compare VFG node")
        .def("is_binary_op_vfg_node", [](const VFGNode* node) { return SVFUtil::isa<BinaryOPVFGNode>(node); }, "Check if this is a binary operation VFG node")
        .def("is_unary_op_vfg_node", [](const VFGNode* node) { return SVFUtil::isa<UnaryOPVFGNode>(node); }, "Check if this is a unary operation VFG node")
        .def("is_branch_vfg_node", [](const VFGNode* node) { return SVFUtil::isa<BranchVFGNode>(node); }, "Check if this is a branch VFG node")
        // Downcasting methods
        .def("as_stmt_vfg_node", [](VFGNode* node) { return SVFUtil::dyn_cast<StmtVFGNode>(node); }, py::return_value_policy::reference, "Downcast to StmtVFGNode")
        .def("as_phi_vfg_node", [](VFGNode* node) { return SVFUtil::dyn_cast<PHIVFGNode>(node); }, py::return_value_policy::reference, "Downcast to PHIVFGNode")
        .def("as_argument_vfg_node", [](VFGNode* node) { return SVFUtil::dyn_cast<ArgumentVFGNode>(node); }, py::return_value_policy::reference, "Downcast to ArgumentVFGNode")
        .def("as_cmp_vfg_node", [](VFGNode* node) { return SVFUtil::dyn_cast<CmpVFGNode>(node); }, py::return_value_policy::reference, "Downcast to CmpVFGNode")
        .def("as_binary_op_vfg_node", [](VFGNode* node) { return SVFUtil::dyn_cast<BinaryOPVFGNode>(node); }, py::return_value_policy::reference, "Downcast to BinaryOPVFGNode")
        .def("as_unary_op_vfg_node", [](VFGNode* node) { return SVFUtil::dyn_cast<UnaryOPVFGNode>(node); }, py::return_value_policy::reference, "Downcast to UnaryOPVFGNode")
        .def("as_branch_vfg_node", [](VFGNode* node) { return SVFUtil::dyn_cast<BranchVFGNode>(node); }, py::return_value_policy::reference, "Downcast to BranchVFGNode");

    // StmtVFGNode - Statement VFG node
    py::class_<StmtVFGNode, VFGNode, std::shared_ptr<StmtVFGNode>>(m, "StmtVFGNode", "Statement VFG node")
        .def("get_value", &StmtVFGNode::getValue, py::return_value_policy::reference, "Get the SVF variable value")
        .def("to_string", &StmtVFGNode::toString, "Get string representation")
        // Type checking
        .def("is_load_vfg_node", [](const StmtVFGNode* node) { return SVFUtil::isa<LoadVFGNode>(node); }, "Check if this is a load VFG node")
        .def("is_store_vfg_node", [](const StmtVFGNode* node) { return SVFUtil::isa<StoreVFGNode>(node); }, "Check if this is a store VFG node") 
        .def("is_copy_vfg_node", [](const StmtVFGNode* node) { return SVFUtil::isa<CopyVFGNode>(node); }, "Check if this is a copy VFG node")
        .def("is_gep_vfg_node", [](const StmtVFGNode* node) { return SVFUtil::isa<GepVFGNode>(node); }, "Check if this is a gep VFG node")
        .def("is_addr_vfg_node", [](const StmtVFGNode* node) { return SVFUtil::isa<AddrVFGNode>(node); }, "Check if this is an addr VFG node")
        // Downcasting
        .def("as_load_vfg_node", [](StmtVFGNode* node) { return SVFUtil::dyn_cast<LoadVFGNode>(node); }, py::return_value_policy::reference, "Downcast to LoadVFGNode")
        .def("as_store_vfg_node", [](StmtVFGNode* node) { return SVFUtil::dyn_cast<StoreVFGNode>(node); }, py::return_value_policy::reference, "Downcast to StoreVFGNode")
        .def("as_copy_vfg_node", [](StmtVFGNode* node) { return SVFUtil::dyn_cast<CopyVFGNode>(node); }, py::return_value_policy::reference, "Downcast to CopyVFGNode")
        .def("as_gep_vfg_node", [](StmtVFGNode* node) { return SVFUtil::dyn_cast<GepVFGNode>(node); }, py::return_value_policy::reference, "Downcast to GepVFGNode")
        .def("as_addr_vfg_node", [](StmtVFGNode* node) { return SVFUtil::dyn_cast<AddrVFGNode>(node); }, py::return_value_policy::reference, "Downcast to AddrVFGNode");

    // LoadVFGNode - Load statement VFG node
    py::class_<LoadVFGNode, StmtVFGNode, std::shared_ptr<LoadVFGNode>>(m, "LoadVFGNode", "Load VFG node")
        .def("get_def_svf_vars", &LoadVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("to_string", &LoadVFGNode::toString, "Get string representation");

    // StoreVFGNode - Store statement VFG node
    py::class_<StoreVFGNode, StmtVFGNode, std::shared_ptr<StoreVFGNode>>(m, "StoreVFGNode", "Store VFG node")
        .def("get_def_svf_vars", &StoreVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("to_string", &StoreVFGNode::toString, "Get string representation");

    // CopyVFGNode - Copy statement VFG node
    py::class_<CopyVFGNode, StmtVFGNode, std::shared_ptr<CopyVFGNode>>(m, "CopyVFGNode", "Copy VFG node")
        .def("get_def_svf_vars", &CopyVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("to_string", &CopyVFGNode::toString, "Get string representation");

    // GepVFGNode - GetElementPtr VFG node
    py::class_<GepVFGNode, StmtVFGNode, std::shared_ptr<GepVFGNode>>(m, "GepVFGNode", "Gep VFG node")
        .def("get_def_svf_vars", &GepVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("to_string", &GepVFGNode::toString, "Get string representation");

    // AddrVFGNode - Address-of VFG node
    py::class_<AddrVFGNode, StmtVFGNode, std::shared_ptr<AddrVFGNode>>(m, "AddrVFGNode", "Address VFG node")
        .def("get_def_svf_vars", &AddrVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("to_string", &AddrVFGNode::toString, "Get string representation");

    // PHIVFGNode - PHI VFG node
    py::class_<PHIVFGNode, VFGNode, std::shared_ptr<PHIVFGNode>>(m, "PHIVFGNode", "PHI VFG node")
        .def("get_def_svf_vars", &PHIVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("get_value", &PHIVFGNode::getValue, py::return_value_policy::reference, "Get result SVF variable")
        .def("to_string", &PHIVFGNode::toString, "Get string representation")
        // Type checking
        .def("is_intra_phi_vfg_node", [](const PHIVFGNode* node) { return SVFUtil::isa<IntraPHIVFGNode>(node); }, "Check if this is an intra-procedural PHI VFG node")
        .def("is_inter_phi_vfg_node", [](const PHIVFGNode* node) { return SVFUtil::isa<InterPHIVFGNode>(node); }, "Check if this is an inter-procedural PHI VFG node")
        // Downcasting
        .def("as_intra_phi_vfg_node", [](PHIVFGNode* node) { return SVFUtil::dyn_cast<IntraPHIVFGNode>(node); }, py::return_value_policy::reference, "Downcast to IntraPHIVFGNode")
        .def("as_inter_phi_vfg_node", [](PHIVFGNode* node) { return SVFUtil::dyn_cast<InterPHIVFGNode>(node); }, py::return_value_policy::reference, "Downcast to InterPHIVFGNode");

    // IntraPHIVFGNode - Intra-procedural PHI VFG node
    py::class_<IntraPHIVFGNode, PHIVFGNode, std::shared_ptr<IntraPHIVFGNode>>(m, "IntraPHIVFGNode", "Intra-procedural PHI VFG node")
        .def("to_string", &IntraPHIVFGNode::toString, "Get string representation");

    // InterPHIVFGNode - Inter-procedural PHI VFG node
    py::class_<InterPHIVFGNode, PHIVFGNode, std::shared_ptr<InterPHIVFGNode>>(m, "InterPHIVFGNode", "Inter-procedural PHI VFG node")
        .def("to_string", &InterPHIVFGNode::toString, "Get string representation");

    // ArgumentVFGNode - Base class for argument VFG nodes
    py::class_<ArgumentVFGNode, VFGNode, std::shared_ptr<ArgumentVFGNode>>(m, "ArgumentVFGNode", "Argument VFG node")
        .def("get_value", &ArgumentVFGNode::getValue, py::return_value_policy::reference, "Get parameter SVF variable")
        .def("to_string", &ArgumentVFGNode::toString, "Get string representation")
        // Type checking
        .def("is_actual_parm_vfg_node", [](const ArgumentVFGNode* node) { return SVFUtil::isa<ActualParmVFGNode>(node); }, "Check if this is an actual parameter VFG node")
        .def("is_formal_parm_vfg_node", [](const ArgumentVFGNode* node) { return SVFUtil::isa<FormalParmVFGNode>(node); }, "Check if this is a formal parameter VFG node")
        .def("is_actual_ret_vfg_node", [](const ArgumentVFGNode* node) { return SVFUtil::isa<ActualRetVFGNode>(node); }, "Check if this is an actual return VFG node")
        .def("is_formal_ret_vfg_node", [](const ArgumentVFGNode* node) { return SVFUtil::isa<FormalRetVFGNode>(node); }, "Check if this is a formal return VFG node")
        // Downcasting
        .def("as_actual_parm_vfg_node", [](ArgumentVFGNode* node) { return SVFUtil::dyn_cast<ActualParmVFGNode>(node); }, py::return_value_policy::reference, "Downcast to ActualParmVFGNode")
        .def("as_formal_parm_vfg_node", [](ArgumentVFGNode* node) { return SVFUtil::dyn_cast<FormalParmVFGNode>(node); }, py::return_value_policy::reference, "Downcast to FormalParmVFGNode")
        .def("as_actual_ret_vfg_node", [](ArgumentVFGNode* node) { return SVFUtil::dyn_cast<ActualRetVFGNode>(node); }, py::return_value_policy::reference, "Downcast to ActualRetVFGNode")
        .def("as_formal_ret_vfg_node", [](ArgumentVFGNode* node) { return SVFUtil::dyn_cast<FormalRetVFGNode>(node); }, py::return_value_policy::reference, "Downcast to FormalRetVFGNode");

    // ActualParmVFGNode - Actual parameter VFG node
    py::class_<ActualParmVFGNode, ArgumentVFGNode, std::shared_ptr<ActualParmVFGNode>>(m, "ActualParmVFGNode", "Actual parameter VFG node")
        .def("get_def_svf_vars", &ActualParmVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("to_string", &ActualParmVFGNode::toString, "Get string representation");

    // FormalParmVFGNode - Formal parameter VFG node
    py::class_<FormalParmVFGNode, ArgumentVFGNode, std::shared_ptr<FormalParmVFGNode>>(m, "FormalParmVFGNode", "Formal parameter VFG node")
        .def("get_def_svf_vars", &FormalParmVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("to_string", &FormalParmVFGNode::toString, "Get string representation");

    // ActualRetVFGNode - Actual return VFG node
    py::class_<ActualRetVFGNode, ArgumentVFGNode, std::shared_ptr<ActualRetVFGNode>>(m, "ActualRetVFGNode", "Actual return VFG node")
        .def("get_def_svf_vars", &ActualRetVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("to_string", &ActualRetVFGNode::toString, "Get string representation");

    // FormalRetVFGNode - Formal return VFG node
    py::class_<FormalRetVFGNode, ArgumentVFGNode, std::shared_ptr<FormalRetVFGNode>>(m, "FormalRetVFGNode", "Formal return VFG node")
        .def("get_def_svf_vars", &FormalRetVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("to_string", &FormalRetVFGNode::toString, "Get string representation");

    // CmpVFGNode - Compare operation VFG node
    py::class_<CmpVFGNode, VFGNode, std::shared_ptr<CmpVFGNode>>(m, "CmpVFGNode", "Compare VFG node")
        .def("get_def_svf_vars", &CmpVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("get_value", &CmpVFGNode::getValue, py::return_value_policy::reference, "Get result SVF variable")
        .def("to_string", &CmpVFGNode::toString, "Get string representation");

    // BinaryOPVFGNode - Binary operation VFG node
    py::class_<BinaryOPVFGNode, VFGNode, std::shared_ptr<BinaryOPVFGNode>>(m, "BinaryOPVFGNode", "Binary operation VFG node")
        .def("get_def_svf_vars", &BinaryOPVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("get_value", &BinaryOPVFGNode::getValue, py::return_value_policy::reference, "Get result SVF variable")
        .def("to_string", &BinaryOPVFGNode::toString, "Get string representation");

    // UnaryOPVFGNode - Unary operation VFG node
    py::class_<UnaryOPVFGNode, VFGNode, std::shared_ptr<UnaryOPVFGNode>>(m, "UnaryOPVFGNode", "Unary operation VFG node")
        .def("get_def_svf_vars", &UnaryOPVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("to_string", &UnaryOPVFGNode::toString, "Get string representation");

    // BranchVFGNode - Branch VFG node
    py::class_<BranchVFGNode, VFGNode, std::shared_ptr<BranchVFGNode>>(m, "BranchVFGNode", "Branch VFG node")
        .def("get_def_svf_vars", &BranchVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("to_string", &BranchVFGNode::toString, "Get string representation");
}

// Bind SVFGNode classes
void bind_svfg_node(py::module& m) {
    // MRSVFGNode - Memory region VFGNode (for address-taken objects)
    py::class_<MRSVFGNode, VFGNode, std::shared_ptr<MRSVFGNode>>(m, "MRSVFGNode", "Memory region VFG node")
        .def("get_points_to", &MRSVFGNode::getPointsTo, py::return_value_policy::reference, "Get the points-to set of the memory region")
        .def("get_def_svf_vars", &MRSVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("to_string", &MRSVFGNode::toString, "Get string representation")
        // Type checking
        .def("is_formal_in_svfg_node", [](const MRSVFGNode* node) { return SVFUtil::isa<FormalINSVFGNode>(node); }, "Check if this is a formal-in SVFG node")
        .def("is_formal_out_svfg_node", [](const MRSVFGNode* node) { return SVFUtil::isa<FormalOUTSVFGNode>(node); }, "Check if this is a formal-out SVFG node")
        .def("is_actual_in_svfg_node", [](const MRSVFGNode* node) { return SVFUtil::isa<ActualINSVFGNode>(node); }, "Check if this is an actual-in SVFG node")
        .def("is_actual_out_svfg_node", [](const MRSVFGNode* node) { return SVFUtil::isa<ActualOUTSVFGNode>(node); }, "Check if this is an actual-out SVFG node")
        .def("is_mssaphi_svfg_node", [](const MRSVFGNode* node) { return SVFUtil::isa<MSSAPHISVFGNode>(node); }, "Check if this is an MSSA phi SVFG node")
        // Downcasting
        .def("as_formal_in_svfg_node", [](MRSVFGNode* node) { return SVFUtil::dyn_cast<FormalINSVFGNode>(node); }, py::return_value_policy::reference, "Downcast to FormalINSVFGNode")
        .def("as_formal_out_svfg_node", [](MRSVFGNode* node) { return SVFUtil::dyn_cast<FormalOUTSVFGNode>(node); }, py::return_value_policy::reference, "Downcast to FormalOUTSVFGNode")
        .def("as_actual_in_svfg_node", [](MRSVFGNode* node) { return SVFUtil::dyn_cast<ActualINSVFGNode>(node); }, py::return_value_policy::reference, "Downcast to ActualINSVFGNode")
        .def("as_actual_out_svfg_node", [](MRSVFGNode* node) { return SVFUtil::dyn_cast<ActualOUTSVFGNode>(node); }, py::return_value_policy::reference, "Downcast to ActualOUTSVFGNode")
        .def("as_mssaphi_svfg_node", [](MRSVFGNode* node) { return SVFUtil::dyn_cast<MSSAPHISVFGNode>(node); }, py::return_value_policy::reference, "Downcast to MSSAPHISVFGNode");
    
    // FormalINSVFGNode - SVFG Node for entry chi node (address-taken variables)
    py::class_<FormalINSVFGNode, MRSVFGNode, std::shared_ptr<FormalINSVFGNode>>(m, "FormalINSVFGNode", "Formal-in SVFG node (entry chi)")
        .def("get_mr_ver", &FormalINSVFGNode::getMRVer, py::return_value_policy::reference, "Get the memory region version")
        .def("get_fun_entry_node", &FormalINSVFGNode::getFunEntryNode, py::return_value_policy::reference, "Get the function entry ICFG node")
        .def("to_string", &FormalINSVFGNode::toString, "Get string representation");
    
    // FormalOUTSVFGNode - SVFG Node for return mu node (address-taken variables)
    py::class_<FormalOUTSVFGNode, MRSVFGNode, std::shared_ptr<FormalOUTSVFGNode>>(m, "FormalOUTSVFGNode", "Formal-out SVFG node (return mu)")
        .def("get_mr_ver", &FormalOUTSVFGNode::getMRVer, py::return_value_policy::reference, "Get the memory region version")
        .def("get_fun_exit_node", &FormalOUTSVFGNode::getFunExitNode, py::return_value_policy::reference, "Get the function exit ICFG node")
        .def("to_string", &FormalOUTSVFGNode::toString, "Get string representation");
    
    // ActualINSVFGNode - SVFG Node for callsite mu node (address-taken variables)
    py::class_<ActualINSVFGNode, MRSVFGNode, std::shared_ptr<ActualINSVFGNode>>(m, "ActualINSVFGNode", "Actual-in SVFG node (callsite mu)")
        .def("get_mr_ver", &ActualINSVFGNode::getMRVer, py::return_value_policy::reference, "Get the memory region version")
        .def("get_callsite", &ActualINSVFGNode::getCallSite, py::return_value_policy::reference, "Get the call site")
        .def("to_string", &ActualINSVFGNode::toString, "Get string representation");
    
    // ActualOUTSVFGNode - SVFG Node for callsite chi node (address-taken variables)
    py::class_<ActualOUTSVFGNode, MRSVFGNode, std::shared_ptr<ActualOUTSVFGNode>>(m, "ActualOUTSVFGNode", "Actual-out SVFG node (callsite chi)")
        .def("get_mr_ver", &ActualOUTSVFGNode::getMRVer, py::return_value_policy::reference, "Get the memory region version")
        .def("get_callsite", &ActualOUTSVFGNode::getCallSite, py::return_value_policy::reference, "Get the call site")
        .def("to_string", &ActualOUTSVFGNode::toString, "Get string representation");
    
    // MSSAPHISVFGNode - SVFG Node for memory SSA phi nodes
    py::class_<MSSAPHISVFGNode, MRSVFGNode, std::shared_ptr<MSSAPHISVFGNode>>(m, "MSSAPHISVFGNode", "Memory SSA PHI SVFG node")
        .def("to_string", &MSSAPHISVFGNode::toString, "Get string representation")
        // Type checking
        .def("is_intra_mssaphi_svfg_node", [](const MSSAPHISVFGNode* node) { return SVFUtil::isa<IntraMSSAPHISVFGNode>(node); }, "Check if this is an intra-procedural MSSA phi SVFG node")
        .def("is_inter_mssaphi_svfg_node", [](const MSSAPHISVFGNode* node) { return SVFUtil::isa<InterMSSAPHISVFGNode>(node); }, "Check if this is an inter-procedural MSSA phi SVFG node")
        // Downcasting
        .def("as_intra_mssaphi_svfg_node", [](MSSAPHISVFGNode* node) { return SVFUtil::dyn_cast<IntraMSSAPHISVFGNode>(node); }, py::return_value_policy::reference, "Downcast to IntraMSSAPHISVFGNode")
        .def("as_inter_mssaphi_svfg_node", [](MSSAPHISVFGNode* node) { return SVFUtil::dyn_cast<InterMSSAPHISVFGNode>(node); }, py::return_value_policy::reference, "Downcast to InterMSSAPHISVFGNode");
    
    // IntraMSSAPHISVFGNode - Intra-procedural MSSA PHI node
    py::class_<IntraMSSAPHISVFGNode, MSSAPHISVFGNode, std::shared_ptr<IntraMSSAPHISVFGNode>>(m, "IntraMSSAPHISVFGNode", "Intra-procedural Memory SSA PHI SVFG node")
        .def("to_string", &IntraMSSAPHISVFGNode::toString, "Get string representation");
    
    // InterMSSAPHISVFGNode - Inter-procedural MSSA PHI node (formalIN/ActualOUT)
    py::class_<InterMSSAPHISVFGNode, MSSAPHISVFGNode, std::shared_ptr<InterMSSAPHISVFGNode>>(m, "InterMSSAPHISVFGNode", "Inter-procedural Memory SSA PHI SVFG node")
        .def("get_fun", &InterMSSAPHISVFGNode::getFun, py::return_value_policy::reference, "Get the function")
        .def("to_string", &InterMSSAPHISVFGNode::toString, "Get string representation");
    
    // DummyVersionPropSVFGNode - Dummy node for object/version propagation
    py::class_<DummyVersionPropSVFGNode, VFGNode, std::shared_ptr<DummyVersionPropSVFGNode>>(m, "DummyVersionPropSVFGNode", "Dummy version propagation SVFG node")
        .def("get_def_svf_vars", &DummyVersionPropSVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("get_object", [](const DummyVersionPropSVFGNode& node) { return node.getObject(); }, "Get the object ID")
        .def("get_version", [](const DummyVersionPropSVFGNode& node) { return node.getVersion(); }, "Get the version");
}


// Bind VFG class
void bind_vfg(py::module& m) {
    py::class_<VFG>(m, "VFG", "Value Flow Graph")
        .def("get_nodes", [](VFG& vfg) {
            std::vector<VFGNode*> nodes;
            for (auto& node : vfg) {
                nodes.push_back(node.second);
            }
            return nodes;
        }, py::return_value_policy::reference, "Get all nodes in the VFG")
        .def("get_gnode", [](VFG& vfg, NodeID id) -> VFGNode* {
            VFGNode* node = vfg.getGNode(id);
            if (!node) {
                throw std::runtime_error("VFGNode with given ID not found.");
            }
            return node;
        }, py::arg("id"), py::return_value_policy::reference, "Get a VFG node by ID")
        .def("view", &VFG::view, "View the VFG")
        .def("get_node_num", &VFG::getTotalNodeNum, "Get the total number of nodes")
        .def("get_edge_num", &VFG::getTotalEdgeNum, "Get the total number of edges");
}

// Bind SVFG class
void bind_svfg(py::module& m) {
    py::class_<SVFG, VFG>(m, "SVFG", "Sparse Value Flow Graph")
        // SVFG specific methods
        .def("get_def_svfg_node", [](SVFG& svfg, const SVFVar* val) { return svfg.getDefSVFGNode(val);}, py::arg("val"), py::return_value_policy::reference, "Get all the definition SVFG nodes of a SVF variable")
        .def("get_actual_out_svfg_nodes", [](SVFG& svfg, const CallICFGNode* cs) { return svfg.getActualOUTSVFGNodes(cs); }, py::arg("cs"), py::return_value_policy::reference, "Get the ActualOUT SVFG nodes for a given call site")
        .def("get_actual_in_svfg_nodes", [](SVFG& svfg, const CallICFGNode* cs) { return svfg.getActualINSVFGNodes(cs); }, py::arg("cs"), py::return_value_policy::reference, "Get the ActualIN SVFG nodes for a given call site")
        .def("get_formal_out_svfg_nodes", [](SVFG& svfg, const FunObjVar* fun) { return svfg.getFormalOUTSVFGNodes(fun); }, py::arg("fun"), py::return_value_policy::reference, "Get the FormalOUT SVFG nodes for a given function")
        .def("get_formal_in_svfg_nodes", [](SVFG& svfg, const FunObjVar* fun) { return svfg.getFormalINSVFGNodes(fun); }, py::arg("fun"), py::return_value_policy::reference, "Get the FormalIN SVFG nodes for a given function")
        .def("dump", [](SVFG &icfg, std::string file) {
            icfg.dump(file);
        })
        .def("view", &SVFG::view, "View the SVFG")
        // has methods
        .def("has_actual_out_svfg_nodes", [](SVFG& svfg, const CallICFGNode* cs) { return svfg.hasActualOUTSVFGNodes(cs); }, py::arg("cs"), "Check if there are ActualOUT SVFG nodes for a given call site")
        .def("has_actual_in_svfg_nodes", [](SVFG& svfg, const CallICFGNode* cs) { return svfg.getActualINSVFGNodes(cs); }, py::arg("cs"), "Check if there are ActualIN SVFG nodes for a given call site")
        .def("has_formal_out_svfg_nodes", [](SVFG& svfg, const FunObjVar* fun) { return svfg.getFormalOUTSVFGNodes(fun); }, py::arg("fun"), "Check if there are FormalOUT SVFG nodes for a given function")
        .def("has_formal_in_svfg_nodes", [](SVFG& svfg, const FunObjVar* fun) { return svfg.getFormalINSVFGNodes(fun); }, py::arg("fun"), "Check if there are FormalIN SVFG nodes for a given function");
}

SVFIR* PySVF::currentSVFIR = nullptr;
CallGraph* PySVF::currentCallGraph = nullptr;
SVFG* PySVF::currentSVFG = nullptr;
ICFG* PySVF::currentICFG = nullptr;
std::string PySVF::lastAnalyzedModule = "";

PYBIND11_MODULE(pysvf, m) {
    bind_svf(m);
    bind_icfg_node(m);
    bind_icfg_edge(m);
    bind_icfg_graph(m);
    bind_svf_stmt(m);
    bind_svf_var(m);
    bind_svf_type(m);
    m.def("get_pag", &PySVF::get_pag, 
        py::arg("bitcode_path"),  // Name the first parameter
        py::arg("build_svfg") = false,  // Name the second parameter with default value
        py::return_value_policy::reference, 
        "Analyze LLVM bitcode and return SVFIR");
    m.def("release_pag", &PySVF::release_pag, "Release SVFIR and LLVMModuleSet");
    m.def("get_icfg", &PySVF::get_current_icfg, py::return_value_policy::reference, "Get the interprocedural control flow graph");
    m.def("get_callgraph", &PySVF::get_current_call_graph, py::return_value_policy::reference, "Get the call graph");
    m.def("get_svfg", &PySVF::get_current_svfg, py::return_value_policy::reference, "Get the sparse value flow graph");
    m.def("get_module_name", &PySVF::get_last_analyzed_module, "Get the name of the last analyzed module");
    bind_callgraph_node(m);
    bind_callgraph_edge(m);
    bind_basic_block(m);
    bind_callgraph(m);
    bind_svfg_edge(m);
    bind_vfg_node(m);
    bind_svfg_node(m);
    bind_vfg(m);   
    bind_svfg(m);    
}
