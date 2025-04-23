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

void bind_svf_stmt(py::module& m) {
    py::class_<SVFStmt>(m, "SVFStmt")
            .def("to_string", [](const SVFStmt* stmt) {
                if (!stmt) {
                    return std::string("nullptr");
                }
                std::ostringstream oss;
                oss << stmt->toString();
                return oss.str();
            }, "Get the string representation of the SVF statement")
            .def("__str__", [](const SVFStmt* stmt) {
                if (!stmt) {
                    return std::string("nullptr");
                }
                std::ostringstream oss;
                oss << stmt->toString();
                return oss.str();
            }, "Get the string representation of the SVF statement")
            .def("__repr__", [](const SVFStmt* stmt) {
                if (!stmt) {
                    return std::string("nullptr");
                }
                std::ostringstream oss;
                oss << stmt->toString();
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

    py::class_<AssignStmt, SVFStmt>(m, "AssignStmt")
            .def("get_lhs_var", &AssignStmt::getLHSVar, py::return_value_policy::reference, "Get the LHS variable of the assignment statement")
            .def("get_lhs_id", &AssignStmt::getLHSVarID, "Get the ID of the LHS variable of the assignment statement")
            .def("get_rhs_var", &AssignStmt::getRHSVar, py::return_value_policy::reference, "Get the RHS variable of the assignment statement")
            .def("get_rhs_id", &AssignStmt::getRHSVarID, "Get the ID of the RHS variable of the assignment statement");

    py::class_<AddrStmt, AssignStmt>(m, "AddrStmt")
            .def("get_arr_size", &AddrStmt::getArrSize, py::return_value_policy::reference, "Get the array size of the address statement");

    py::class_<CopyStmt, AssignStmt>(m, "CopyStmt")
            .def("get_copy_kind", &CopyStmt::getCopyKind, "Get the copy kind of the copy statement")
            .def("is_bitcast", &CopyStmt::isBitCast, "Check if the copy statement is a bitcast")
            .def("is_value_copy", &CopyStmt::isValueCopy, "Check if the copy statement is a value copy")
            .def("is_int2ptr", &CopyStmt::isInt2Ptr, "Check if the copy statement is an int2ptr")
            .def("is_ptr2int", &CopyStmt::isPtr2Int, "Check if the copy statement is a ptr2int")
            .def("is_zext", &CopyStmt::isZext, "Check if the copy statement is a zext")
            .def("is_sext", &CopyStmt::isSext, "Check if the copy statement is a sext");

    py::class_<StoreStmt, AssignStmt>(m, "StoreStmt");


    py::class_<LoadStmt, AssignStmt>(m, "LoadStmt");

    py::class_<CallPE, AssignStmt>(m, "CallPE")
            .def("get_callsite", &CallPE::getCallSite, "Get the call site")
            .def("get_fun_entry_icfg_node", &CallPE::getFunEntryICFGNode, py::return_value_policy::reference,
                 "Get the function entry ICFG node");

    py::class_<RetPE, AssignStmt>(m, "RetPE")
            .def("get_callsite", &RetPE::getCallSite, "Get the call site")
            .def("get_fun_exit_icfg_node", &RetPE::getFunExitICFGNode, py::return_value_policy::reference,
                 "Get the function exit ICFG node");


    py::class_<GepStmt, AssignStmt>(m, "GepStmt")
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

    py::class_<MultiOpndStmt, SVFStmt>(m, "MultiOpndStmt")
            .def("get_op_var", [](MultiOpndStmt& stmt, int ID) { return stmt.getOpVar(ID); },
                 py::return_value_policy::reference)
            .def("get_op_var_id", &MultiOpndStmt::getOpVarID)
            .def("get_opnd_vars", &MultiOpndStmt::getOpndVars, py::return_value_policy::reference)
            .def("get_res_id", &MultiOpndStmt::getResID)
            .def("get_res", &MultiOpndStmt::getRes, py::return_value_policy::reference)
            .def("get_res_var", &MultiOpndStmt::getRes, py::return_value_policy::reference)
            .def("get_op_var_num", &MultiOpndStmt::getOpVarNum, "Get the number of operands of the statement")
            .def("__iter__", [](MultiOpndStmt& stmt) {
                return py::make_iterator(stmt.opVarBegin(), stmt.opVerEnd());
            }, py::keep_alive<0, 1>()); // Keep the iterator alive while iterating

    py::class_<PhiStmt, MultiOpndStmt>(m, "PhiStmt")
            // TODO: may implement get_op_var and get_op_var_id
            .def("get_op_icfg_node", [](PhiStmt& stmt, int idx) { return stmt.getOpICFGNode(idx); },
                 py::return_value_policy::reference)
            .def("is_fun_ret_phi", &PhiStmt::isFunctionRetPhi, "Check if the phi statement is a function return phi");

    // TODO: selectStmt
    py::class_<SelectStmt, MultiOpndStmt>(m, "SelectStmt")
            .def("get_condition", &SelectStmt::getCondition, py::return_value_policy::reference)
            .def("get_true_value", &SelectStmt::getTrueValue, py::return_value_policy::reference)
            .def("get_false_value", &SelectStmt::getFalseValue, py::return_value_policy::reference);

    py::class_<CmpStmt, MultiOpndStmt>(m, "CmpStmt")
            // TODO: Return int, maybe need to think about friendly return value
            .def("get_predicate", &CmpStmt::getPredicate);

    py::class_<BinaryOPStmt, MultiOpndStmt>(m, "BinaryOPStmt")
            //TODO: enum of get_op
            .def("get_op", &BinaryOPStmt::getOpcode)
            .def("get_opcode", &BinaryOPStmt::getOpcode);

    py::class_<UnaryOPStmt, SVFStmt>(m, "UnaryOPStmt")
            .def("get_op", &UnaryOPStmt::getOpcode)
            .def("get_opcode", &UnaryOPStmt::getOpcode)
            .def("get_op_var", &UnaryOPStmt::getOpVar, py::return_value_policy::reference)
            .def("get_res", &UnaryOPStmt::getRes, py::return_value_policy::reference)
            .def("get_res_var", &UnaryOPStmt::getRes, py::return_value_policy::reference)
            .def("get_op_var_id", &UnaryOPStmt::getOpVarID)
            .def("get_res_id", &UnaryOPStmt::getResID);

    py::class_<BranchStmt, SVFStmt>(m, "BranchStmt")
            //std::vector<std::pair<const ICFGNode*, s32_t>> getSuccessors()
            .def("get_successors", &BranchStmt::getSuccessors, py::return_value_policy::reference)
            .def("get_num_successors", &BranchStmt::getNumSuccessors)
            .def("is_conditional", &BranchStmt::isConditional)
            .def("is_unconditional", &BranchStmt::isUnconditional)
            .def("get_condition", &BranchStmt::getCondition, py::return_value_policy::reference)
            .def("get_branch_inst", &BranchStmt::getBranchInst, py::return_value_policy::reference);
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
            .def("__iter__", [](SVFIR* pag) {
                return py::make_iterator(pag->begin(), pag->end());
            }, py::keep_alive<0, 1>(), "Iterate over the PAG nodes")
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
            }, py::arg("T"), py::arg("origId"), py::return_value_policy::reference)
            .def("get_fun_obj_var", [](SVFIR* pag, std::string funName) {
                const FunObjVar* funObj = pag->getFunObjVar(funName);
                if (!funObj) {
                    throw std::runtime_error("Function object with given ID not found.");
                }
                return funObj;
            }, py::arg("funName"), py::return_value_policy::reference)
            .def("get_fun_ret", [](SVFIR* pag, const FunObjVar* funObj) {
                auto retVal = pag->getFunRet(funObj);
                if (!retVal) {
                    throw std::runtime_error("Return value with given ID not found.");
                }
                return retVal;
            }, py::arg("funObj"), py::return_value_policy::reference);
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
            .def("get_in_edges", [](SVF::SVFVar* node) {
                std::vector<const SVFStmt*> inEdges;
                for (auto& edge : node->getInEdges()) {
                    inEdges.push_back(edge);
                }
                return inEdges;
            }, py::return_value_policy::reference)
            .def("get_out_edges", [](SVF::SVFVar* node) {
                std::vector<const SVFStmt*> outEdges;
                for (auto& edge : node->getOutEdges()) {
                    outEdges.push_back(edge);
                }
                return outEdges;
            }, py::return_value_policy::reference)
            .def("to_string", [](SVF::SVFVar* node) {
                if (!node) {
                    return std::string("nullptr");
                }
                std::ostringstream oss;
                oss << node->toString();
                return oss.str();
            }, "Get the string representation of the SVF variable")
            .def("__str__", [](SVF::SVFVar* node) {
                if (!node) {
                    return std::string("nullptr");
                }
                std::ostringstream oss;
                oss << node->toString();
                return oss.str();
            }, "Get the string representation of the SVF variable")
            .def("__repr__", [](SVF::SVFVar* node) {
                if (!node) {
                    return std::string("nullptr");
                }
                std::ostringstream oss;
                oss << node->toString();
                return oss.str();
            }, "Get the string representation of the SVF variable");

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
            .def("get_type", &SVF::GlobalValVar::getType, py::return_value_policy::reference);

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
            }, py::return_value_policy::reference);

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
            }, py::return_value_policy::reference);

    py::class_<SVF::GepValVar, SVF::ValVar>(m, "GepValVar")
            .def("get_constant_field_idx", &SVF::GepValVar::getConstantFieldIdx)
            .def("get_base_node", &SVF::GepValVar::getBaseNode, py::return_value_policy::reference)
            .def("get_value_name", &SVF::GepValVar::getValueName)
            .def("is_pointer", &SVF::GepValVar::isPointer)
            .def("get_type", &SVF::GepValVar::getType, py::return_value_policy::reference)
            .def("get_function", &SVF::GepValVar::getFunction, py::return_value_policy::reference);


    //// BaseObjVar
    py::class_<SVF::BaseObjVar, SVF::ObjVar>(m, "BaseObjVar")
            .def("get_type", &SVF::BaseObjVar::getType, py::return_value_policy::reference)
            .def("get_byte_size_of_obj", &SVF::BaseObjVar::getByteSizeOfObj)
            .def("get_icfg_node", &SVF::BaseObjVar::getICFGNode, py::return_value_policy::reference)
            .def("get_value_name", &SVF::BaseObjVar::getValueName)
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
            .def("is_pointer", &SVF::GepObjVar::isPointer);

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
            .def("arg_size", &SVF::FunObjVar::arg_size)
            .def("get_arg", &SVF::FunObjVar::getArg, py::arg("index"), py::return_value_policy::reference)

            .def("dominates", [](SVF::FunObjVar* node, SVF::SVFBasicBlock* bbKey, SVF::SVFBasicBlock* bbValue) -> bool {
                return node->dominate(bbKey, bbValue);
            })
            .def("post_dominates", [](SVF::FunObjVar* node, SVF::SVFBasicBlock* bbKey, SVF::SVFBasicBlock* bbValue) -> bool {
                return node->postDominate(bbKey, bbValue);
            });

    py::class_<SVF::FunValVar, SVF::ValVar>(m, "FunValVar")
            .def("get_function", &SVF::FunValVar::getFunction, py::return_value_policy::reference)
            .def("is_pointer", &SVF::FunValVar::isPointer);


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
