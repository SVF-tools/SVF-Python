#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "SVF-LLVM/LLVMUtil.h"
#include "SVF-LLVM/SVFIRBuilder.h"
#include "Util/CommandLine.h"
#include "Util/Options.h"
#include "Graphs/ICFG.h"
#include "SVFIR/SVFType.h"
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

// Bind


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

// Bind SVFVar
void bind_svf_var(py::module &m) {
    py::class_<SVFVar>(m, "SVFVar")
            .def("get_name", &SVFVar::getName)
            .def("get_type", &SVFVar::getType)
            // SVFFunction* getFunction()
            .def("get_function", &SVFVar::getFunction, py::return_value_policy::reference)
            .def("get_id", &SVFVar::getId)
            .def("to_string", &SVFVar::toString);
            // TODO: implement the downcast and isa
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
            .def("print", &SVFPointerType::print)
            .def_static("classof", &SVFPointerType::classof);

    py::class_<SVFIntegerType, SVFType>(m, "SVFIntegerType")
            .def("print", &SVFIntegerType::print)
            .def("set_sign_and_width", &SVFIntegerType::setSignAndWidth)
            .def("is_signed", &SVFIntegerType::isSigned)
            .def_static("classof", &SVFIntegerType::classof);

    py::class_<SVFFunctionType, SVFType>(m, "SVFFunctionType")
            .def("print", &SVFFunctionType::print)
            .def("get_return_type", &SVFFunctionType::getReturnType, py::return_value_policy::reference)
            .def_static("classof", &SVFFunctionType::classof);

    py::class_<SVFStructType, SVFType>(m, "SVFStructType")
            .def("print", &SVFStructType::print)
            .def("get_name", &SVFStructType::getName)
            .def("set_name", py::overload_cast<const std::string&>(&SVFStructType::setName))
            .def("set_name", py::overload_cast<std::string&&>(&SVFStructType::setName))
            .def_static("classof", &SVFStructType::classof);

    py::class_<SVFArrayType, SVFType>(m, "SVFArrayType")
            .def("print", &SVFArrayType::print)
            .def("get_type_of_element", &SVFArrayType::getTypeOfElement, py::return_value_policy::reference)
            .def("set_type_of_element", &SVFArrayType::setTypeOfElement)
            .def("set_num_of_element", &SVFArrayType::setNumOfElement)
            .def_static("classof", &SVFArrayType::classof);

    py::class_<SVFOtherType, SVFType>(m, "SVFOtherType")
            .def("print", &SVFOtherType::print)
            .def("get_repr", &SVFOtherType::getRepr)
            .def("set_repr", py::overload_cast<const std::string&>(&SVFOtherType::setRepr))
            .def("set_repr", py::overload_cast<std::string&&>(&SVFOtherType::setRepr))
            .def_static("classof", &SVFOtherType::classof);

    // Downcasting utilities for polymorphic types
    m.def("as_pointer_type", [](SVFType* type) { return dynamic_cast<SVFPointerType*>(type); }, py::return_value_policy::reference);
    m.def("as_integer_type", [](SVFType* type) { return dynamic_cast<SVFIntegerType*>(type); }, py::return_value_policy::reference);
    m.def("as_function_type", [](SVFType* type) { return dynamic_cast<SVFFunctionType*>(type); }, py::return_value_policy::reference);
    m.def("as_struct_type", [](SVFType* type) { return dynamic_cast<SVFStructType*>(type); }, py::return_value_policy::reference);
    m.def("as_array_type", [](SVFType* type) { return dynamic_cast<SVFArrayType*>(type); }, py::return_value_policy::reference);
    m.def("as_other_type", [](SVFType* type) { return dynamic_cast<SVFOtherType*>(type); }, py::return_value_policy::reference);
}


PYBIND11_MODULE(pysvf, m) {
    bind_svf(m);
    bind_icfg_node(m);
    bind_icfg_graph(m);
    bind_svf_stmt(m);
    bind_svf_var(m);
    bind_svf_type(m);
    m.def("analyze", &PySVF::analyze, py::return_value_policy::reference, "Analyze LLVM bitcode and return SVFIR");
}
