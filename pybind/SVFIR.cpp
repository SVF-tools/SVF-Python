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
            .def("toString", [](const SVFStmt* stmt) {
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
            .def("getEdgeId", &SVFStmt::getEdgeID, "Get the ID of the SVF statement")
            .def("getICFGNode", &SVFStmt::getICFGNode, py::return_value_policy::reference_internal, "Get the ICFG node that the SVF statement belongs to")
            .def("getValue", &SVFStmt::getValue, py::return_value_policy::reference_internal, "Get the value of the SVF statement")
            .def("getBB", &SVFStmt::getBB, py::return_value_policy::reference_internal, "Get the basic block that the SVF statement belongs to")
                    // addr copy store load call ret gep phi select cmp binary unary branch
                    // TODO: may support threadFork threadJoin
            .def("isAddrStmt", [](const SVFStmt* stmt) { return SVFUtil::dyn_cast<const AddrStmt>(stmt) != nullptr; }, "Check if the SVF statement is an address statement")
            .def("isCopyStmt", [](const SVFStmt* stmt) { return SVFUtil::dyn_cast<const CopyStmt>(stmt) != nullptr; }, "Check if the SVF statement is a copy statement")
            .def("isStoreStmt", [](const SVFStmt* stmt) { return SVFUtil::dyn_cast<const StoreStmt>(stmt) != nullptr; }, "Check if the SVF statement is a store statement")
            .def("isLoadStmt", [](const SVFStmt* stmt) { return SVFUtil::dyn_cast<const LoadStmt>(stmt) != nullptr; }, "Check if the SVF statement is a load statement")
            .def("isCallPE", [](const SVFStmt* stmt) { return SVFUtil::dyn_cast<const CallPE>(stmt) != nullptr; }, "Check if the SVF statement is a call PE")
            .def("isRetPE", [](const SVFStmt* stmt) { return SVFUtil::dyn_cast<const RetPE>(stmt) != nullptr; }, "Check if the SVF statement is a return PE")
            .def("isGepStmt", [](const SVFStmt* stmt) {return SVFUtil::dyn_cast<const GepStmt>(stmt) != nullptr; }, "Check if the SVF statement is a GEP statement")
            .def("isPhiStmt", [](const SVFStmt* stmt) { return SVFUtil::dyn_cast<const PhiStmt>(stmt) != nullptr; },
                 "Check if the SVF statement is a phi statement")
            .def("isSelectStmt", [](const SVFStmt* stmt) {
                return SVFUtil::dyn_cast<const SelectStmt>(stmt) != nullptr;}, "Check if the SVF statement is a select statement")
            .def("isCmpStmt", [](const SVFStmt* stmt) {
                return SVFUtil::dyn_cast<const CmpStmt>(stmt) != nullptr;}, "Check if the SVF statement is a compare statement")
            .def("isBinaryOpStmt", [](const SVFStmt* stmt) {
                return SVFUtil::dyn_cast<const BinaryOPStmt>(stmt) != nullptr;}, "Check if the SVF statement is a binary operation statement")
            .def("isUnaryOpStmt", [](const SVFStmt* stmt) {
                return SVFUtil::dyn_cast<const UnaryOPStmt>(stmt) != nullptr;}, "Check if the SVF statement is a unary operation statement")
            .def("isBranchStmt", [](const SVFStmt* stmt) {
                return SVFUtil::dyn_cast<const BranchStmt>(stmt) != nullptr;}, "Check if the SVF statement is a branch statement")
             // downcast TODO: more downcast here
            .def("asAddrStmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<AddrStmt>(stmt); },
                  py::return_value_policy::reference, "Downcast the SVF statement to an address statement")
            .def("asCopyStmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<CopyStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a copy statement")
            .def("asStoreStmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<StoreStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a store statement")
            .def("asLoadStmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<LoadStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a load statement")
            .def("asCallPE", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<CallPE>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a call PE")
            .def("asRetPE", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<RetPE>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a return PE")
            .def("asGepStmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<GepStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a GEP statement")
            .def("asPhiStmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<PhiStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a phi statement")
            .def("asSelectStmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<SelectStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a select statement")
            .def("asCmpStmt", [](SVFStmt* stmt) { return SVFUtil::dyn_cast<CmpStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a compare statement")
            .def("asBinaryOpStmt", [](SVFStmt* stmt) { return SVFUtil::dyn_cast<BinaryOPStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a binary operation statement")
            .def("asUnaryOpStmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<UnaryOPStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a unary operation statement")
            .def("asBranchStmt", [](SVFStmt *stmt) { return SVFUtil::dyn_cast<BranchStmt>(stmt); },
                 py::return_value_policy::reference, "Downcast the SVF statement to a branch statement");

    py::class_<AssignStmt, SVFStmt>(m, "AssignStmt")
            .def("getLHSVar", &AssignStmt::getLHSVar, py::return_value_policy::reference, "Get the LHS variable of the assignment statement")
            .def("getLHSVarID", &AssignStmt::getLHSVarID, "Get the ID of the LHS variable of the assignment statement")
            .def("getRHSVar", &AssignStmt::getRHSVar, py::return_value_policy::reference, "Get the RHS variable of the assignment statement")
            .def("getRHSVarID", &AssignStmt::getRHSVarID, "Get the ID of the RHS variable of the assignment statement");

    py::class_<AddrStmt, AssignStmt>(m, "AddrStmt")
            .def("getArrSize", &AddrStmt::getArrSize, py::return_value_policy::reference, "Get the array size of the address statement");

    py::class_<CopyStmt, AssignStmt>(m, "CopyStmt")
            .def("getCopyKind", &CopyStmt::getCopyKind, "Get the copy kind of the copy statement")
            .def("isBitcast", &CopyStmt::isBitCast, "Check if the copy statement is a bitcast")
            .def("isValueCopy", &CopyStmt::isValueCopy, "Check if the copy statement is a value copy")
            .def("isInt2Ptr", &CopyStmt::isInt2Ptr, "Check if the copy statement is an int2ptr")
            .def("isPtr2Int", &CopyStmt::isPtr2Int, "Check if the copy statement is a ptr2int")
            .def("isZext", &CopyStmt::isZext, "Check if the copy statement is a zext")
            .def("isSext", &CopyStmt::isSext, "Check if the copy statement is a sext");

    py::class_<StoreStmt, AssignStmt>(m, "StoreStmt");


    py::class_<LoadStmt, AssignStmt>(m, "LoadStmt");

    py::class_<CallPE, AssignStmt>(m, "CallPE")
            .def("getCallSite", &CallPE::getCallSite, "Get the call site")
            .def("getFunEntryICFGNode", &CallPE::getFunEntryICFGNode, py::return_value_policy::reference,
                 "Get the function entry ICFG node");

    py::class_<RetPE, AssignStmt>(m, "RetPE")
            .def("getCallSite", &RetPE::getCallSite, "Get the call site")
            .def("getFunExitICFGNode", &RetPE::getFunExitICFGNode, py::return_value_policy::reference,
                 "Get the function exit ICFG node");


    py::class_<GepStmt, AssignStmt>(m, "GepStmt")
            .def("isConstantOffset", &GepStmt::isConstantOffset, "Check if the GEP statement has a constant offset")
            .def("getConstantOffset", &GepStmt::accumulateConstantOffset, "Get the constant offset of the GEP statement")
            .def("getConstantByteOffset", &GepStmt::accumulateConstantByteOffset,
                 "Get the constant byte offset of the GEP statement")
            .def("getConstantStructFldIdx", &GepStmt::getConstantStructFldIdx,
                      "Get the constant struct field index of the GEP statement")
            .def("getOffsetVarAndGepTypePairVec", &GepStmt::getOffsetVarAndGepTypePairVec, py::return_value_policy::reference,
                 "Get the offset variable and GEP type pair vector of the GEP statement")
            .def("getSrcPointeeType", [](GepStmt& stmt) { return stmt.getAccessPath().gepSrcPointeeType(); },
                 py::return_value_policy::reference);

    py::class_<MultiOpndStmt, SVFStmt>(m, "MultiOpndStmt")
            .def("getOpVar", [](MultiOpndStmt& stmt, int ID) { return stmt.getOpVar(ID); },
                 py::return_value_policy::reference)
            .def("getOpVarId", &MultiOpndStmt::getOpVarID)
            .def("getOpndVars", &MultiOpndStmt::getOpndVars, py::return_value_policy::reference)
            .def("getResId", &MultiOpndStmt::getResID)
            .def("getRes", &MultiOpndStmt::getRes, py::return_value_policy::reference)
            .def("getOpVarNum", &MultiOpndStmt::getOpVarNum, "Get the number of operands of the statement")
            .def("__iter__", [](MultiOpndStmt& stmt) {
                return py::make_iterator(stmt.opVarBegin(), stmt.opVerEnd());
            }, py::keep_alive<0, 1>()); // Keep the iterator alive while iterating

    py::class_<PhiStmt, MultiOpndStmt>(m, "PhiStmt")
            // TODO: may implement get_op_var and get_op_var_id
            .def("getOpICFGNode", [](PhiStmt& stmt, int idx) { return stmt.getOpICFGNode(idx); },
                 py::return_value_policy::reference)
            .def("isFunRetPhi", &PhiStmt::isFunctionRetPhi, "Check if the phi statement is a function return phi");

    // TODO: selectStmt
    py::class_<SelectStmt, MultiOpndStmt>(m, "SelectStmt")
            .def("getCondition", &SelectStmt::getCondition, py::return_value_policy::reference)
            .def("getTrueValue", &SelectStmt::getTrueValue, py::return_value_policy::reference)
            .def("getFalseValue", &SelectStmt::getFalseValue, py::return_value_policy::reference);

    py::class_<CmpStmt, MultiOpndStmt>(m, "CmpStmt")
            // TODO: Return int, maybe need to think about friendly return value
            .def("getPredicate", &CmpStmt::getPredicate);

    py::class_<BinaryOPStmt, MultiOpndStmt>(m, "BinaryOPStmt")
            .def("getOpcode", &BinaryOPStmt::getOpcode);

    py::class_<UnaryOPStmt, SVFStmt>(m, "UnaryOPStmt")
            .def("getOp", &UnaryOPStmt::getOpcode)
            .def("getOpcode", &UnaryOPStmt::getOpcode)
            .def("getOpVar", &UnaryOPStmt::getOpVar, py::return_value_policy::reference)
            .def("getRes", &UnaryOPStmt::getRes, py::return_value_policy::reference)
            .def("getResVar", &UnaryOPStmt::getRes, py::return_value_policy::reference)
            .def("getOpVarId", &UnaryOPStmt::getOpVarID)
            .def("getResId", &UnaryOPStmt::getResID);

    py::class_<BranchStmt, SVFStmt>(m, "BranchStmt")
            //std::vector<std::pair<const ICFGNode*, s32_t>> getSuccessors()
            .def("getSuccessors", &BranchStmt::getSuccessors, py::return_value_policy::reference)
            .def("getNumSuccessors", &BranchStmt::getNumSuccessors)
            .def("isConditional", &BranchStmt::isConditional)
            .def("isUnconditional", &BranchStmt::isUnconditional)
            .def("getCondition", &BranchStmt::getCondition, py::return_value_policy::reference)
            .def("getBranchInst", &BranchStmt::getBranchInst, py::return_value_policy::reference);
}


// Bind SVFIR (PAG)
void bind_svf(py::module& m) {
    py::class_<SVFIR>(m, "SVFIR")
            .def("getICFG", [](SVFIR* pag) { return pag->getICFG(); }, py::return_value_policy::reference)
            .def("getCallGraph", [](SVFIR* pag) { return pag->getCallGraph(); }, py::return_value_policy::reference)
            .def("getCallSites", [](SVFIR *pag) {
                // move  pag->getCallSiteSet() to vector
                std::vector<const CallICFGNode*> callSites;
                for (auto &callSite : pag->getCallSiteSet()) {
                    callSites.push_back(callSite);
                }
                return callSites;
                }, py::return_value_policy::reference)
            .def("getBaseObject", [&](SVFIR* pag, NodeID id) { //TODO: get_base_obj
                const BaseObjVar* baseObj = pag->getBaseObject(id);
                if (!baseObj) {
                    throw std::runtime_error("Base object with given ID not found.");
                }
                return baseObj;
            }, py::arg("id"), py::return_value_policy::reference)
            .def("getPAGNodeNum", &SVFIR::getPAGNodeNum)
            .def("getGNode", [](SVFIR* pag, NodeID id) {
                const SVF::PAGNode* node = pag->getGNode(id);
                if (!node) {
                    throw std::runtime_error("PAGNode with given ID not found.");
                }
                return node;
            }, py::arg("id"), py::return_value_policy::reference)
            .def("__iter__", [](SVFIR* pag) {
                return py::make_iterator(pag->begin(), pag->end());
            }, py::keep_alive<0, 1>(), "Iterate over the PAG nodes")
            .def("getGepObjVar", [](SVFIR* pag, NodeID id, const APOffset& offset) {
                NodeID gepObjVarID = pag->getGepObjVar(id, offset);
                return gepObjVarID;
            }, py::arg("id"), py::arg("offset"), py::return_value_policy::reference)
            //u32_t getNumOfFlattenElements(const SVFType* T);
            .def("getNumOfFlattenElements", [](SVFIR* pag, const SVFType* T) {
                return pag->getNumOfFlattenElements(T);
            }, py::arg("T"), py::return_value_policy::reference)
            //u32_t getFlattenedElemIdx(const SVFType* T, u32_t origId);
            .def("getFlattenedElemIdx", [](SVFIR* pag, const SVFType* T, u32_t origId) {
                return pag->getFlattenedElemIdx(T, origId);
            }, py::arg("T"), py::arg("origId"), py::return_value_policy::reference)
            .def("getFunObjVar", [](SVFIR* pag, std::string funName) {
                const FunObjVar* funObj = pag->getFunObjVar(funName);
                if (!funObj) {
                    throw std::runtime_error("Function object with given ID not found.");
                }
                return funObj;
            }, py::arg("funName"), py::return_value_policy::reference)
            .def("getFunRet", [](SVFIR* pag, const FunObjVar* funObj) {
                auto retVal = pag->getFunRet(funObj);
                if (!retVal) {
                    throw std::runtime_error("Return value with given ID not found.");
                }
                return retVal;
            }, py::arg("funObj"), py::return_value_policy::reference)
            .def("getModuleIdentifier", &SVFIR::getModuleIdentifier)
            .def("dump", [](SVFIR* pag, std::string file) {
                pag->dump(file);
            }, py::arg("file"));
}

// Bind SVFVar
void bind_svf_var(py::module &m) {
    py::class_<SVF::SVFVar>(m, "SVFVar")
            .def("getName", &SVF::SVFVar::getName)
            .def("getId", &SVF::SVFVar::getId)
            .def("isPointer", &SVF::SVFVar::isPointer)
            .def("isConstDataOrAggDataButNotNullPtr", &SVF::SVFVar::isConstDataOrAggDataButNotNullPtr)
            .def("isIsolatedNode", &SVF::SVFVar::isIsolatedNode)
            .def("getValueName", &SVF::SVFVar::getValueName)
            .def("getFunction", &SVF::SVFVar::getFunction, py::return_value_policy::reference)
            .def("ptrInUncalledFunction", &SVF::SVFVar::ptrInUncalledFunction)
            .def("isConstDataOrAggData", &SVF::SVFVar::isConstDataOrAggData)
            // Type checking methods (like isa<>)
            .def("isValVar", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::ValVar>(node);
            })
            .def("isObjVar", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::ObjVar>(node);
            })
            .def("isGepValVar", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::GepValVar>(node);
            })
            .def("isGepObjVar", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::GepObjVar>(node);
            })
            .def("isFunObjVar", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::FunObjVar>(node);
            })
            .def("isFunValVar", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::FunValVar>(node);
            })
            .def("isArgValVar", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::ArgValVar>(node);
            })
            .def("isRetValVar", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::RetValPN>(node);
            })
            .def("isDummyValVar", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::DummyValVar>(node);
            })
            .def("isDummyObjVar", [](const SVF::SVFVar* node) {
                return SVFUtil::isa<SVF::DummyObjVar>(node);
            })
            // Type casting methods (like dynamic_cast<>)
            .def("asValVar", [](SVF::SVFVar* node) -> SVF::ValVar* {
                return SVFUtil::dyn_cast<SVF::ValVar>(node);
            }, py::return_value_policy::reference)
            .def("asObjVar", [](SVF::SVFVar* node) -> SVF::ObjVar* {
                return SVFUtil::dyn_cast<SVF::ObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asGepValVar", [](SVF::SVFVar* node) -> SVF::GepValVar* {
                return SVFUtil::dyn_cast<SVF::GepValVar>(node);
            }, py::return_value_policy::reference)
            .def("asGepObjVar", [](SVF::SVFVar* node) -> SVF::GepObjVar* {
                return SVFUtil::dyn_cast<SVF::GepObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asFunObjVar", [](SVF::SVFVar* node) -> SVF::FunObjVar* {
                return SVFUtil::dyn_cast<SVF::FunObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asFunValVar", [](SVF::SVFVar* node) -> SVF::FunValVar* {
                return SVFUtil::dyn_cast<SVF::FunValVar>(node);
            }, py::return_value_policy::reference)
            .def("asArgValVar", [](SVF::SVFVar* node) -> SVF::ArgValVar* {
                return SVFUtil::dyn_cast<SVF::ArgValVar>(node);
            }, py::return_value_policy::reference)
            .def("asRetValVar", [](SVF::SVFVar* node) -> SVF::RetValPN* {
                return SVFUtil::dyn_cast<SVF::RetValPN>(node);
            }, py::return_value_policy::reference)
            .def("asDummyValVar", [](SVF::SVFVar* node) -> SVF::DummyValVar* {
                return SVFUtil::dyn_cast<SVF::DummyValVar>(node);
            }, py::return_value_policy::reference)
            .def("asDummyObjVar", [](SVF::SVFVar* node) -> SVF::DummyObjVar* {
                return SVFUtil::dyn_cast<SVF::DummyObjVar>(node);
            }, py::return_value_policy::reference)
            .def("getInEdges", [](SVF::SVFVar* node) {
                std::vector<const SVFStmt*> inEdges;
                for (auto& edge : node->getInEdges()) {
                    inEdges.push_back(edge);
                }
                return inEdges;
            }, py::return_value_policy::reference)
            .def("getOutEdges", [](SVF::SVFVar* node) {
                std::vector<const SVFStmt*> outEdges;
                for (auto& edge : node->getOutEdges()) {
                    outEdges.push_back(edge);
                }
                return outEdges;
            }, py::return_value_policy::reference)
            .def("toString", [](SVF::SVFVar* node) {
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
            .def("isFunValVar", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::FunValVar>(node);
            })
            .def("asFunValVar", [](SVF::ValVar* node) -> SVF::FunValVar* {
                return SVFUtil::dyn_cast<SVF::FunValVar>(node);
            }, py::return_value_policy::reference)

            .def("isArgValVar", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::ArgValVar>(node);
            })
            .def("asArgValVar", [](SVF::ValVar* node) -> SVF::ArgValVar* {
                return SVFUtil::dyn_cast<SVF::ArgValVar>(node);
            }, py::return_value_policy::reference)

            .def("isGlobalValVar", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::GlobalValVar>(node);
            })
            .def("asGlobalValVar", [](SVF::ValVar* node) -> SVF::GlobalValVar* {
                return SVFUtil::dyn_cast<SVF::GlobalValVar>(node);
            }, py::return_value_policy::reference)

            .def("isConstAggValVar", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstAggValVar>(node);
            })
            .def("asConstAggValVar", [](SVF::ValVar* node) -> SVF::ConstAggValVar* {
                return SVFUtil::dyn_cast<SVF::ConstAggValVar>(node);
            }, py::return_value_policy::reference)

            .def("isConstDataValVar", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstDataValVar>(node);
            })
            .def("asConstDataValVar", [](SVF::ValVar* node) -> SVF::ConstDataValVar* {
                return SVFUtil::dyn_cast<SVF::ConstDataValVar>(node);
            }, py::return_value_policy::reference)

            .def("isConstFPValVar", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstFPValVar>(node);
            })
            .def("asConstFPValVar", [](SVF::ValVar* node) -> SVF::ConstFPValVar* {
                return SVFUtil::dyn_cast<SVF::ConstFPValVar>(node);
            }, py::return_value_policy::reference)

            .def("isConstIntValVar", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstIntValVar>(node);
            })
            .def("asConstIntValVar", [](SVF::ValVar* node) -> SVF::ConstIntValVar* {
                return SVFUtil::dyn_cast<SVF::ConstIntValVar>(node);
            }, py::return_value_policy::reference)

            .def("isConstNullPtrValVar", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstNullPtrValVar>(node);
            })
            .def("asConstNullPtrValVar", [](SVF::ValVar* node) -> SVF::ConstNullPtrValVar* {
                return SVFUtil::dyn_cast<SVF::ConstNullPtrValVar>(node);
            }, py::return_value_policy::reference)

            .def("isBlackHoleValVar", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::BlackHoleValVar>(node);
            })
            .def("asBlackHoleValVar", [](SVF::ValVar* node) -> SVF::BlackHoleValVar* {
                return SVFUtil::dyn_cast<SVF::BlackHoleValVar>(node);
            }, py::return_value_policy::reference)

            .def("isGepValVar", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::GepValVar>(node);
            })
            .def("asGepValVar", [](SVF::ValVar* node) -> SVF::GepValVar* {
                return SVFUtil::dyn_cast<SVF::GepValVar>(node);
            }, py::return_value_policy::reference)

            .def("isRetValVar", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::RetValPN>(node);
            })
            .def("asRetValVar", [](SVF::ValVar* node) -> SVF::RetValPN* {
                return SVFUtil::dyn_cast<SVF::RetValPN>(node);
            }, py::return_value_policy::reference)

            .def("isVarArgValPN", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::VarArgValPN>(node);
            })
            .def("asVarArgValPN", [](SVF::ValVar* node) -> SVF::VarArgValPN* {
                return SVFUtil::dyn_cast<SVF::VarArgValPN>(node);
            }, py::return_value_policy::reference)
            .def("isRetValPN", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::RetValPN>(node);
            })
            .def("asRetValPN",[](SVF::ValVar* node) -> SVF::RetValPN* {
                return SVFUtil::dyn_cast<SVF::RetValPN>(node);
            }, py::return_value_policy::reference)
            .def("isVarArgValPN", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::VarArgValPN>(node);
            })
            .def("asVarArgValPN",[](SVF::ValVar* node) -> SVF::VarArgValPN* {
                return SVFUtil::dyn_cast<SVF::VarArgValPN>(node);
            }, py::return_value_policy::reference)
            .def("isDummyValVar", [](SVF::ValVar* node) -> bool {
                return SVFUtil::isa<SVF::DummyValVar>(node);
            })
            .def("asDummyValVar", [](SVF::ValVar* node) -> SVF::DummyValVar* {
                return SVFUtil::dyn_cast<SVF::DummyValVar>(node);
            }, py::return_value_policy::reference)
            .def("getICFGNode", &SVF::ValVar::getICFGNode, py::return_value_policy::reference)
            .def("getValueName", &SVF::ValVar::getValueName)
            .def("getFunction", &SVF::ValVar::getFunction, py::return_value_policy::reference)
            .def("getType", &SVF::GlobalValVar::getType, py::return_value_policy::reference);

    py::class_<SVF::ObjVar, SVF::SVFVar>(m, "ObjVar")
            .def("getValueName", &SVF::ObjVar::getValueName)
                    // For ObjVar conversion functions
            .def("isObjVar", [](SVF::SVFVar* node) -> bool {
                return SVFUtil::isa<SVF::ObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asObjVar", [](SVF::SVFVar* node) -> SVF::ObjVar* {
                return SVFUtil::dyn_cast<SVF::ObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isBaseObjVar", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::BaseObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asBaseObjVar", [](SVF::ObjVar* node) -> SVF::BaseObjVar* {
                return SVFUtil::dyn_cast<SVF::BaseObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isGepObjVar", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::GepObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asGepObjVar", [](SVF::ObjVar* node) -> SVF::GepObjVar* {
                return SVFUtil::dyn_cast<SVF::GepObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isFunObjVar", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::FunObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asFunObjVar", [](SVF::ObjVar* node) -> SVF::FunObjVar* {
                return SVFUtil::dyn_cast<SVF::FunObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isGlobalObjVar", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::GlobalObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asGlobalObjVar", [](SVF::ObjVar* node) -> SVF::GlobalObjVar* {
                return SVFUtil::dyn_cast<SVF::GlobalObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isHeapObjVar", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::HeapObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asHeapObjVar", [](SVF::ObjVar* node) -> SVF::HeapObjVar* {
                return SVFUtil::dyn_cast<SVF::HeapObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isStackObjVar", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::StackObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asStackObjVar", [](SVF::ObjVar* node) -> SVF::StackObjVar* {
                return SVFUtil::dyn_cast<SVF::StackObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isConstAggObjVar", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstAggObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asConstAggObjVar", [](SVF::ObjVar* node) -> SVF::ConstAggObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstAggObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isConstDataObjVar", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstDataObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asConstDataObjVar", [](SVF::ObjVar* node) -> SVF::ConstDataObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstDataObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isConstFPObjVar", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstFPObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asConstFPObjVar", [](SVF::ObjVar* node) -> SVF::ConstFPObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstFPObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isConstIntObjVar", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstIntObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asConstIntObjVar", [](SVF::ObjVar* node) -> SVF::ConstIntObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstIntObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isConstNullPtrObjVar", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstNullPtrObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asConstNullPtrObjVar", [](SVF::ObjVar* node) -> SVF::ConstNullPtrObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstNullPtrObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isDummyObjVar", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::DummyObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asDummyObjVar", [](SVF::ObjVar* node) -> SVF::DummyObjVar* {
                return SVFUtil::dyn_cast<SVF::DummyObjVar>(node);
            }, py::return_value_policy::reference);

    py::class_<SVF::ArgValVar, SVF::ValVar>(m, "ArgValVar")
            .def("getFunction", &SVF::ArgValVar::getFunction, py::return_value_policy::reference)
            .def("getParent", &SVF::ArgValVar::getParent, py::return_value_policy::reference)
            .def("getArgNo", &SVF::ArgValVar::getArgNo)
            .def("isPointer", &SVF::ArgValVar::isPointer)
            // For ArgValVar conversion functions
            .def("getArgNo", [](SVF::ArgValVar* node) -> u32_t {
                return node->getArgNo();
            }, py::return_value_policy::reference)
            .def("getParent", [](SVF::ArgValVar* node) -> const SVF::FunObjVar* {
                return node->getParent();
            }, py::return_value_policy::reference)
            .def("isArgOfUncalledFunction", [](SVF::ArgValVar* node) -> bool {
                return node->isArgOfUncalledFunction();
            }, py::return_value_policy::reference);

    py::class_<SVF::GepValVar, SVF::ValVar>(m, "GepValVar")
            .def("getConstantFieldIdx", &SVF::GepValVar::getConstantFieldIdx)
            .def("getBaseNode", &SVF::GepValVar::getBaseNode, py::return_value_policy::reference)
            .def("getValueName", &SVF::GepValVar::getValueName)
            .def("isPointer", &SVF::GepValVar::isPointer)
            .def("getType", &SVF::GepValVar::getType, py::return_value_policy::reference)
            .def("getFunction", &SVF::GepValVar::getFunction, py::return_value_policy::reference);


    //// BaseObjVar
    py::class_<SVF::BaseObjVar, SVF::ObjVar>(m, "BaseObjVar")
            .def("getType", &SVF::BaseObjVar::getType, py::return_value_policy::reference)
            .def("getByteSizeOfObj", &SVF::BaseObjVar::getByteSizeOfObj)
            .def("getICFGNode", &SVF::BaseObjVar::getICFGNode, py::return_value_policy::reference)
            .def("getValueName", &SVF::BaseObjVar::getValueName)
            .def("getId", &SVF::BaseObjVar::getId)
            .def("getType", &SVF::BaseObjVar::getType, py::return_value_policy::reference)
            .def("getNumOfElements", &SVF::BaseObjVar::getNumOfElements)
            .def("setNumOfElements", &SVF::BaseObjVar::setNumOfElements)
            .def("getMaxFieldOffsetLimit", &SVF::BaseObjVar::getMaxFieldOffsetLimit)
            .def("isFieldInsensitive", &SVF::BaseObjVar::isFieldInsensitive)
            .def("setFieldInsensitive", &SVF::BaseObjVar::setFieldInsensitive)
            .def("setFieldSensitive", &SVF::BaseObjVar::setFieldSensitive)
            .def("isBlackHoleObj", &SVF::BaseObjVar::isBlackHoleObj)
            .def("getByteSizeOfObj", &SVF::BaseObjVar::getByteSizeOfObj)
            .def("isConstantByteSize", &SVF::BaseObjVar::isConstantByteSize)
            .def("isFunction", &SVF::BaseObjVar::isFunction)
            .def("isGlobalObj", &SVF::BaseObjVar::isGlobalObj)
            .def("isStaticObj", &SVF::BaseObjVar::isStaticObj)
            .def("isStack", &SVF::BaseObjVar::isStack)
            .def("isHeap", &SVF::BaseObjVar::isHeap)
            .def("isStruct", &SVF::BaseObjVar::isStruct)
            .def("isArray", &SVF::BaseObjVar::isArray)
            .def("isVarStruct", &SVF::BaseObjVar::isVarStruct)
            .def("isVarArray", &SVF::BaseObjVar::isVarArray)
            .def("isConstantStruct", &SVF::BaseObjVar::isConstantStruct)
            .def("isConstantArray", &SVF::BaseObjVar::isConstantArray)
            .def("isConstDataOrConstGlobal", &SVF::BaseObjVar::isConstDataOrConstGlobal)
            .def("isConstDataOrAggData", &SVF::BaseObjVar::isConstDataOrAggData)
            .def("getFunction", &SVF::BaseObjVar::getFunction, py::return_value_policy::reference)
            .def("getBaseMemObj", &SVF::BaseObjVar::getBaseMemObj, py::return_value_policy::reference)
            .def("isBaseObjVar", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::BaseObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asBaseObjVar", [](SVF::ObjVar* node) -> SVF::BaseObjVar* {
                return SVFUtil::dyn_cast<SVF::BaseObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isGepObjVar", [](SVF::ObjVar* node) -> bool {
                return SVFUtil::isa<SVF::GepObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asGepObjVar", [](SVF::ObjVar* node) -> SVF::GepObjVar* {
                return SVFUtil::dyn_cast<SVF::GepObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isFunObjVar", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::FunObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asFunObjVar", [](SVF::BaseObjVar* node) -> SVF::FunObjVar* {
                return SVFUtil::dyn_cast<SVF::FunObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isGlobalObjVar", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::GlobalObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asGlobalObjVar", [](SVF::BaseObjVar* node) -> SVF::GlobalObjVar* {
                return SVFUtil::dyn_cast<SVF::GlobalObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isHeapObjVar", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::HeapObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asHeapObjVar", [](SVF::BaseObjVar* node) -> SVF::HeapObjVar* {
                return SVFUtil::dyn_cast<SVF::HeapObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isStackObjVar", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::StackObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asStackObjVar", [](SVF::BaseObjVar* node) -> SVF::StackObjVar* {
                return SVFUtil::dyn_cast<SVF::StackObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isConstAggObjVar", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstAggObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asConstAggObjVar", [](SVF::BaseObjVar* node) -> SVF::ConstAggObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstAggObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isConstDataObjVar", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstDataObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asConstDataObjVar", [](SVF::BaseObjVar* node) -> SVF::ConstDataObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstDataObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isConstFPObjVar", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstFPObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asConstFPObjVar", [](SVF::BaseObjVar* node) -> SVF::ConstFPObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstFPObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isConstIntObjVar", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstIntObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asConstIntObjVar", [](SVF::BaseObjVar* node) -> SVF::ConstIntObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstIntObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isConstNullPtrObjVar", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::ConstNullPtrObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asConstNullPtrObjVar", [](SVF::BaseObjVar* node) -> SVF::ConstNullPtrObjVar* {
                return SVFUtil::dyn_cast<SVF::ConstNullPtrObjVar>(node);
            }, py::return_value_policy::reference)
            .def("isDummyObjVar", [](SVF::BaseObjVar* node) -> bool {
                return SVFUtil::isa<SVF::DummyObjVar>(node);
            }, py::return_value_policy::reference)
            .def("asDummyObjVar", [](SVF::BaseObjVar* node) -> SVF::DummyObjVar* {
                return SVFUtil::dyn_cast<SVF::DummyObjVar>(node);
            }, py::return_value_policy::reference);

    /// GepObjVar
    py::class_<SVF::GepObjVar, SVF::ObjVar>(m, "GepObjVar")
            .def("getConstantFieldIdx", &SVF::GepObjVar::getConstantFieldIdx)
            .def("getBaseNode", &SVF::GepObjVar::getBaseNode, py::return_value_policy::reference)
            .def("getType", &SVF::GepObjVar::getType, py::return_value_policy::reference)
            .def("getValueName", &SVF::GepObjVar::getValueName)
            .def("isPointer", &SVF::GepObjVar::isPointer);

    /// HeapObjVar
    py::class_<SVF::HeapObjVar, SVF::BaseObjVar>(m, "HeapObjVar");

    /// StackObjVar
    py::class_<SVF::StackObjVar, SVF::BaseObjVar>(m, "StackObjVar");



    py::class_<SVF::FunObjVar, SVF::BaseObjVar>(m, "FunObjVar")
            .def("isDeclaration", &SVF::FunObjVar::isDeclaration)
            .def("isIntrinsic", &SVF::FunObjVar::isIntrinsic)
            .def("hasAddressTaken", &SVF::FunObjVar::hasAddressTaken)
            .def("isVarArg", &SVF::FunObjVar::isVarArg)
            .def("isUncalledFunction", &SVF::FunObjVar::isUncalledFunction)
            .def("hasReturn", &SVF::FunObjVar::hasReturn)
            .def("getFunctionType", &SVF::FunObjVar::getFunctionType, py::return_value_policy::reference)
            .def("getReturnType", &SVF::FunObjVar::getReturnType, py::return_value_policy::reference)
            .def("arg_size", &SVF::FunObjVar::arg_size)
            .def("getArg", &SVF::FunObjVar::getArg, py::arg("index"), py::return_value_policy::reference)

            .def("dominates", [](SVF::FunObjVar* node, SVF::SVFBasicBlock* bbKey, SVF::SVFBasicBlock* bbValue) -> bool {
                return node->dominate(bbKey, bbValue);
            })
            .def("postDominate", [](SVF::FunObjVar* node, SVF::SVFBasicBlock* bbKey, SVF::SVFBasicBlock* bbValue) -> bool {
                return node->postDominate(bbKey, bbValue);
            });

    py::class_<SVF::FunValVar, SVF::ValVar>(m, "FunValVar")
            .def("getFunction", &SVF::FunValVar::getFunction, py::return_value_policy::reference)
            .def("isPointer", &SVF::FunValVar::isPointer);


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
            .def("getFPValue", &SVF::ConstFPValVar::getFPValue);

    //// ConstIntValVar
    py::class_<SVF::ConstIntValVar, SVF::ConstDataValVar>(m, "ConstIntValVar")
            .def("getSExtValue", &SVF::ConstIntValVar::getSExtValue)
            .def("getZExtValue", &SVF::ConstIntValVar::getZExtValue);

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
            .def("getFPValue", &SVF::ConstFPObjVar::getFPValue);


    //// ConstIntObjVar
    py::class_<SVF::ConstIntObjVar, SVF::ConstDataObjVar>(m, "ConstIntObjVar")
            .def("getSExtValue", &SVF::ConstIntObjVar::getSExtValue)
            .def("getZExtValue", &SVF::ConstIntObjVar::getZExtValue);


    ///ConstNullPtrObjVar
    py::class_<SVF::ConstNullPtrObjVar, SVF::ConstDataObjVar>(m, "ConstNullPtrObjVar");

    //// RetValPN
    //inline const FunObjVar* getCallGraphNode() const;
    py::class_<SVF::RetValPN, SVF::ValVar>(m, "RetValPN")
            .def("getFunction", &SVF::RetValPN::getFunction, py::return_value_policy::reference);

    ///VarArgValPN
    py::class_<SVF::VarArgValPN, SVF::ValVar>(m, "VarArgValPN")
            .def("getFunction", &SVF::VarArgValPN::getFunction, py::return_value_policy::reference);

    ///DummyValVar
    py::class_<SVF::DummyValVar, SVF::ValVar>(m, "DummyValVar");
    ///DummyObjVar
    py::class_<SVF::DummyObjVar, SVF::BaseObjVar>(m, "DummyObjVar");
}



// Bind SVFType
void bind_svf_type(py::module& m) {
    py::class_<SVFType>(m, "SVFType")
            .def("getKind", &SVFType::getKind)
            .def("getByteSize", &SVFType::getByteSize)
            .def("isPointerTy", &SVFType::isPointerTy)
            .def("isArrayTy", &SVFType::isArrayTy)
            .def("isStructTy", &SVFType::isStructTy)
            .def("isSingleValueTy", &SVFType::isSingleValueType)
            .def("asPointerType", [](SVFType* type) { return SVFUtil::dyn_cast<SVFPointerType>(type); }, py::return_value_policy::reference)
            .def("asIntegerType", [](SVFType* type) { return SVFUtil::dyn_cast<SVFIntegerType>(type); }, py::return_value_policy::reference)
            .def("asFunctionType", [](SVFType* type) { return SVFUtil::dyn_cast<SVFFunctionType>(type); }, py::return_value_policy::reference)
            .def("asStructType", [](SVFType* type) { return SVFUtil::dyn_cast<SVFStructType>(type); }, py::return_value_policy::reference)
            .def("asArrayType", [](SVFType *type) { return SVFUtil::dyn_cast<SVFArrayType>(type); },py::return_value_policy::reference)
            .def("asOtherType", [](SVFType *type) { return SVFUtil::dyn_cast<SVFOtherType>(type); },py::return_value_policy::reference)
            .def("isPointerType", [](SVFType* type) { return SVFUtil::isa<SVFPointerType>(type); })
            .def("isIntegerType", [](SVFType* type) { return SVFUtil::isa<SVFIntegerType>(type); })
            .def("isFunctionType", [](SVFType* type) { return SVFUtil::isa<SVFFunctionType>(type); })
            .def("isStructType", [](SVFType* type) { return SVFUtil::isa<SVFStructType>(type); })
            .def("isArrayType", [](SVFType* type) { return SVFUtil::isa<SVFArrayType>(type); })
            .def("isOtherType", [](SVFType* type) { return SVFUtil::isa<SVFOtherType>(type); });


    py::class_<SVFPointerType, SVFType>(m, "SVFPointerType")
            .def("print", &SVFPointerType::print);

    py::class_<SVFIntegerType, SVFType>(m, "SVFIntegerType")
            .def("print", &SVFIntegerType::print)
            .def("setSignAndWidth", &SVFIntegerType::setSignAndWidth)
            .def("isSigned", &SVFIntegerType::isSigned);

    py::class_<SVFFunctionType, SVFType>(m, "SVFFunctionType")
            .def("print", &SVFFunctionType::print)
            .def("getReturnType", &SVFFunctionType::getReturnType, py::return_value_policy::reference);

    py::class_<SVFStructType, SVFType>(m, "SVFStructType")
            .def("print", &SVFStructType::print)
            .def("getName", &SVFStructType::getName)
            .def("setName", py::overload_cast<const std::string&>(&SVFStructType::setName))
            .def("setName", py::overload_cast<std::string&&>(&SVFStructType::setName));

    py::class_<SVFArrayType, SVFType>(m, "SVFArrayType")
            .def("print", &SVFArrayType::print)
            .def("getTypeOfElement", &SVFArrayType::getTypeOfElement, py::return_value_policy::reference)
            .def("setTypeOfElement", &SVFArrayType::setTypeOfElement)
            .def("setNumOfElement", &SVFArrayType::setNumOfElement);

    py::class_<SVFOtherType, SVFType>(m, "SVFOtherType")
            .def("print", &SVFOtherType::print)
            .def("getRepr", &SVFOtherType::getRepr)
            .def("setRepr", py::overload_cast<const std::string&>(&SVFOtherType::setRepr))
            .def("setRepr", py::overload_cast<std::string&&>(&SVFOtherType::setRepr));


}
