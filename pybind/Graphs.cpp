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


void bind_icfg_node(py::module& m) {
    py::class_<ICFGNode>(m, "ICFGNode", "Represents a node in the Interprocedural Control Flow Graph (ICFG)")
            .def("toString", [](const ICFGNode* node) {
                if (!node) {
                    return std::string("nullptr");
                }
                std::ostringstream oss;
                oss << node->toString() << "\n";
                return oss.str();
            })
            .def("__str__", [](const ICFGNode* node) {
                if (!node) {
                    return std::string("nullptr");
                }
                std::ostringstream oss;
                oss << node->toString() << "\n";
                return oss.str();
            })
            .def("__repr__", [](const ICFGNode* node) {
                if (!node) {
                    return std::string("nullptr");
                }
                std::ostringstream oss;
                oss << node->toString() << "\n";
                return oss.str();
            })
            //get_id
            .def("getId", &ICFGNode::getId, "Get the ID of the ICFG node")
            .def("getFun", &ICFGNode::getFun, py::return_value_policy::reference, "Get the function that the ICFG node belongs to")
            .def("getBB", &ICFGNode::getBB, py::return_value_policy::reference, "Get the basic block that the ICFG node belongs to")
            .def("getSourceLoc", &ICFGNode::getSourceLoc, py::return_value_policy::reference, "Get the source location of the ICFG node")
            .def("getSVFStmts", &ICFGNode::getSVFStmts, py::return_value_policy::reference, "Get the SVF statements associated with the ICFG node")
            // Downcast
            .def("asFunEntry", [](ICFGNode* node) { return SVFUtil::dyn_cast<FunEntryICFGNode>(node); }, py::return_value_policy::reference, "Downcast to FunEntryICFGNode")
            .def("asFunExit", [](ICFGNode* node) { return SVFUtil::dyn_cast<FunExitICFGNode>(node); }, py::return_value_policy::reference, "Downcast to FunExitICFGNode")
            .def("asCall", [](ICFGNode* node) { return SVFUtil::dyn_cast<CallICFGNode>(node); }, py::return_value_policy::reference, "Downcast to CallICFGNode")
            .def("asRet", [](ICFGNode* node) { return SVFUtil::dyn_cast<RetICFGNode>(node); }, py::return_value_policy::reference, "Downcast to RetICFGNode")
            // Type checking
            .def("isFunEntry", [](const ICFGNode* node) { return node->getNodeKind() == SVF::ICFGNode::FunEntryBlock; }, "Check if the ICFG node is a function entry node")
            .def("isFunExit", [](const ICFGNode* node) { return node->getNodeKind() == SVF::ICFGNode::FunExitBlock; }, "Check if the ICFG node is a function exit node")
            .def("isCall", [](const ICFGNode* node) { return node->getNodeKind() == SVF::ICFGNode::FunCallBlock; }, "Check if the ICFG node is a function call node")
            .def("isRet", [](const ICFGNode* node) { return node->getNodeKind() == SVF::ICFGNode::FunRetBlock; }, "Check if the ICFG node is a function return node")
            // get out edges and get in edges. gather them in a list
            .def("getOutEdges", [](const ICFGNode *node) {
                std::vector<ICFGEdge *> edges;
                for (auto &edge : node->getOutEdges()) {
                    edges.push_back(edge);
                }
                return edges;
            }, py::return_value_policy::reference, "Get the out edges of the ICFG node")
            .def("getInEdges", [](const ICFGNode *node) {
                std::vector < ICFGEdge * > edges;
                for (auto &edge: node->getInEdges()) {
                    edges.push_back(edge);
                }
                return edges;
            }, py::return_value_policy::reference, "Get the in edges of the ICFG node");

    // IntraICFGNode
    py::class_<IntraICFGNode, ICFGNode>(m, "IntraICFGNode")
            .def("isRetInst", &IntraICFGNode::isRetInst, "Check if the ICFG node is a return instruction");

    // InterICFGNode
    py::class_<InterICFGNode, ICFGNode>(m, "InterICFGNode");

    // FunEntryICFGNode
    py::class_<FunEntryICFGNode, InterICFGNode>(m, "FunEntryICFGNode")
            .def("getFormalParms", &FunEntryICFGNode::getFormalParms, py::return_value_policy::reference,
                 "Get the formal parameters of the function")
            .def("addFormalParm", &FunEntryICFGNode::addFormalParms, "Add a formal parameter to the function");

    // FunExitICFGNode
    py::class_<FunExitICFGNode, InterICFGNode>(m, "FunExitICFGNode")
            .def("getFormalRet", &FunExitICFGNode::getFormalRet, py::return_value_policy::reference, "Get the formal return value of the function")
            .def("addFormalRet", &FunExitICFGNode::addFormalRet, "Add a formal return value to the function");

    // CallICFGNode
    py::class_<CallICFGNode, InterICFGNode>(m, "CallICFGNode")
            .def("getCaller", &CallICFGNode::getCaller, py::return_value_policy::reference, "Get the caller function")
            .def("getCalledFunction", &CallICFGNode::getCalledFunction, py::return_value_policy::reference,
                 "Get the called function")
            .def("getActualParms", &CallICFGNode::getActualParms, py::return_value_policy::reference,
                 "Get the actual parameters of the call")
            .def("getArgument", &CallICFGNode::getArgument, py::arg("idx"),  py::return_value_policy::reference,
                 "Get the argument of the call")
            .def("addActualParms", &CallICFGNode::addActualParms, "Add an actual parameter to the call")
            .def("isVarArg", &CallICFGNode::isVarArg, "Check if the call is a vararg call")
            .def("isVirtualCall", &CallICFGNode::isVirtualCall, "Check if the call is a virtual call")
            .def("getRetICFGNode", &CallICFGNode::getRetICFGNode, py::return_value_policy::reference, "Get the return node");

    // === RetICFGNode ===
    py::class_<RetICFGNode, InterICFGNode>(m, "RetICFGNode")
            .def("getActualRet", &RetICFGNode::getActualRet, py::return_value_policy::reference, "Get the actual return value")
            .def("addActualRet", &RetICFGNode::addActualRet, "Add an actual return value")
            .def("getCallICFGNode", &RetICFGNode::getCallICFGNode, py::return_value_policy::reference, "Get the call node") ;

    // GlobalICFGNode
    py::class_<GlobalICFGNode, ICFGNode>(m, "GlobalICFGNode");
}

void bind_icfg_edge(py::module& m) {
    using namespace SVF;

    py::class_<ICFGEdge, std::shared_ptr<ICFGEdge>>(m, "ICFGEdge")
            .def("toString", [](const ICFGEdge* edge) {
                if (!edge) {
                    return std::string("nullptr");
                }
                std::ostringstream oss;
                if (edge->getSrcNode() == nullptr) {
                    oss << "nullptr";
                } else {
                    oss << edge->getSrcNode()->toString() << " -> ";
                }

                if (edge->getDstNode() == nullptr) {
                    oss << "nullptr";
                } else {
                    oss << edge->getDstNode()->toString();
                }
                return oss.str();
            })
            .def("__str__", [](const ICFGEdge* edge) {
                if (!edge) {
                    return std::string("nullptr");
                }
                std::ostringstream oss;
                if (edge->getSrcNode() == nullptr) {
                    oss << "nullptr";
                } else {
                    oss << edge->getSrcNode()->toString() << " -> ";
                }

                if (edge->getDstNode() == nullptr) {
                    oss << "nullptr";
                } else {
                    oss << edge->getDstNode()->toString();
                }
                return oss.str();
            })
            .def("__repr__", [](const ICFGEdge* edge) {
                if (!edge) {
                    return std::string("nullptr");
                }
                std::ostringstream oss;
                if (edge->getSrcNode() == nullptr) {
                    oss << "nullptr";
                } else {
                    oss << edge->getSrcNode()->toString() << " -> ";
                }

                if (edge->getDstNode() == nullptr) {
                    oss << "nullptr";
                } else {
                    oss << edge->getDstNode()->toString();
                }
                return oss.str();
            })
            .def("isCFGEdge", &ICFGEdge::isCFGEdge, "Check if the edge is a CFG edge")
            .def("isCallCFGEdge", &ICFGEdge::isCallCFGEdge, "Check if the edge is a call CFG edge")
            .def("isRetCFGEdge", &ICFGEdge::isRetCFGEdge, "Check if the edge is a return CFG edge")
            .def("isIntraCFGEdge", &ICFGEdge::isIntraCFGEdge, "Check if the edge is an intra CFG edge")
            // get inedge and outedge
            .def("getSrcNode", &ICFGEdge::getSrcNode, py::return_value_policy::reference, "Get the source node of the edge")
            .def("getDstNode", &ICFGEdge::getDstNode, py::return_value_policy::reference, "Get the destination node of the edge")
            //downcast
            .def("asIntraCFGEdge", [](ICFGEdge *edge) { return SVFUtil::dyn_cast<IntraCFGEdge>(edge); },
                 py::return_value_policy::reference, "Downcast to IntraCFGEdge")
            .def("asCallCFGEdge", [](ICFGEdge *edge) { return SVFUtil::dyn_cast<CallCFGEdge>(edge); },
                 py::return_value_policy::reference, "Downcast to CallCFGEdge")
            .def("asRetCFGEdge", [](ICFGEdge *edge) { return SVFUtil::dyn_cast<RetCFGEdge>(edge); },
                 py::return_value_policy::reference, "Downcast to RetCFGEdge");

    py::class_<IntraCFGEdge, ICFGEdge, std::shared_ptr<IntraCFGEdge>>(m, "IntraCFGEdge")
            // constructor  (const ICFGNode*, const ICFGNode*), const cast and create a new instance
            .def(py::init([](const ICFGNode *src, const ICFGNode *dst) {
                return std::make_shared<IntraCFGEdge>(const_cast<ICFGNode*>(src), const_cast<ICFGNode*>(dst));
            }))
            .def("getCondition", &IntraCFGEdge::getCondition, py::return_value_policy::reference,
                 "Get the condition of the edge")
            .def("getSuccessorCondValue", &IntraCFGEdge::getSuccessorCondValue, py::return_value_policy::reference,
                 "Get the successor condition value");

    py::class_<CallCFGEdge, ICFGEdge, std::shared_ptr<CallCFGEdge>>(m, "CallCFGEdge")
            .def("getCallSite", &CallCFGEdge::getCallSite, py::return_value_policy::reference, "Get the call site")
            .def("getCallPEs", &CallCFGEdge::getCallPEs, py::return_value_policy::reference, "Get the call PEs");

    py::class_<RetCFGEdge, ICFGEdge, std::shared_ptr<RetCFGEdge>>(m, "RetCFGEdge")
            .def("getCallSite", &RetCFGEdge::getCallSite, py::return_value_policy::reference, "Get the call site")
            .def("getRetPE", &RetCFGEdge::getRetPE, py::return_value_policy::reference, "Get the return PE");
}



// Bind class ICFG
void bind_icfg_graph(py::module& m) {
    py::class_<ICFG>(m, "ICFG")
            .def("getNodes", [](const ICFG& icfg) {
                std::vector<ICFGNode*> nodes;
                for (auto& node : icfg) {
                    nodes.push_back(node.second);
                }
                return nodes;
            }, py::return_value_policy::reference)

            // getGNode(id)
            .def("getGNode", [](ICFG& icfg, NodeID id) -> ICFGNode* {
                ICFGNode* node = icfg.getGNode(id);
                if (!node) {
                    throw std::runtime_error("ICFGNode with given ID not found.");
                }
                return node;
            }, py::arg("id"), py::return_value_policy::reference)
            .def("getFunEntryICFGNode", &ICFG::getFunEntryICFGNode, py::return_value_policy::reference, py::arg("fun_obj"))
            .def("__iter__", [](ICFG* icfg) {
                return py::make_iterator(icfg->begin(), icfg->end());
            }, py::keep_alive<0, 1>(), "Iterate over the ICFG nodes")

            .def("getGlobalICFGNode", &ICFG::getGlobalICFGNode, py::return_value_policy::reference)
            //void dump(const std::string& file);
            .def("dump", [](ICFG &icfg, std::string file) {
                icfg.dump(file);
            });
}

void bind_callgraph_node(py::module& m) {
    py::class_<CallGraphNode>(m, "CallGraphNode", "Represents a node in the Call Graph")
        .def("toString", [](const CallGraphNode* node) {
            if (!node) {
                return std::string("nullptr");
            }
            std::ostringstream oss;
            oss << node->toString();
            return oss.str();
        }, "Get the string representation of the CallGraph node")
        .def("__str__", [](const CallGraphNode* node) {
            if (!node) {
                return std::string("nullptr");
            }
            std::ostringstream oss;
            oss << node->toString();
            return oss.str();
        }, "Get the string representation of the CallGraph node")
        .def("__repr__", [](const CallGraphNode* node) {
            if (!node) {
                return std::string("nullptr");
            }
            std::ostringstream oss;
            oss << node->toString();
            return oss.str();
        }, "Get the string representation of the CallGraph node")
        .def("getId", &CallGraphNode::getId, "Get the ID of the CallGraph node")
        .def("getFunction", &CallGraphNode::getFunction, py::return_value_policy::reference, "Get the function of this call node")
        .def("getName", &CallGraphNode::getName, "Get the name of the function")
        .def("isReachableFromProgEntry", &CallGraphNode::isReachableFromProgEntry, "Check if this function can be reached from main")
        // Get out edges and in edges
        .def("getOutEdges", [](const CallGraphNode *node) {
            std::vector<CallGraphEdge *> edges;
            for (auto it = node->OutEdgeBegin(); it != node->OutEdgeEnd(); ++it) {
                edges.push_back(*it);
            }
            return edges;
        }, py::return_value_policy::reference, "Get the out edges of the CallGraph node")
        .def("getInEdges", [](const CallGraphNode *node) {
            std::vector<CallGraphEdge *> edges;
            for (auto it = node->InEdgeBegin(); it != node->InEdgeEnd(); ++it) {
                edges.push_back(*it);
            }
            return edges;
        }, py::return_value_policy::reference, "Get the in edges of the CallGraph node");
}

void bind_callgraph_edge(py::module& m) {
    py::class_<CallGraphEdge>(m, "CallGraphEdge", "Represents an edge in the Call Graph")
        .def("toString", [](const CallGraphEdge* edge) {
            if (!edge) {
                return std::string("nullptr");
            }
            std::ostringstream oss;
            if (edge->getSrcNode() == nullptr) {
                oss << "nullptr";
            } else {
                oss << edge->getSrcNode()->toString() << " -> ";
            }

            if (edge->getDstNode() == nullptr) {
                oss << "nullptr";
            } else {
                oss << edge->getDstNode()->toString();
            }
            return oss.str();
        }, "Get the string representation of the CallGraph edge")
        .def("__str__", [](const CallGraphEdge* edge) {
            if (!edge) {
                return std::string("nullptr");
            }
            std::ostringstream oss;
            if (edge->getSrcNode() == nullptr) {
                oss << "nullptr";
            } else {
                oss << edge->getSrcNode()->toString() << " -> ";
            }

            if (edge->getDstNode() == nullptr) {
                oss << "nullptr";
            } else {
                oss << edge->getDstNode()->toString();
            }
            return oss.str();
        }, "Get the string representation of the CallGraph edge")
        .def("__repr__", [](const CallGraphEdge* edge) {
            if (!edge) {
                return std::string("nullptr");
            }
            std::ostringstream oss;
            if (edge->getSrcNode() == nullptr) {
                oss << "nullptr";
            } else {
                oss << edge->getSrcNode()->toString() << " -> ";
            }

            if (edge->getDstNode() == nullptr) {
                oss << "nullptr";
            } else {
                oss << edge->getDstNode()->toString();
            }
            return oss.str();
        }, "Get the string representation of the CallGraph edge")
        .def("getCallSiteID", &CallGraphEdge::getCallSiteID, "Get the call site ID")
        .def("isDirectCallEdge", &CallGraphEdge::isDirectCallEdge, "Check if this is a direct call edge")
        .def("isIndirectCallEdge", &CallGraphEdge::isIndirectCallEdge, "Check if this is an indirect call edge")
        .def("getDirectCalls", [](const CallGraphEdge& edge) {
            return edge.getDirectCalls();
        }, py::return_value_policy::reference, "Get the direct calls")
        .def("getIndirectCalls", [](const CallGraphEdge& edge) {
            return edge.getIndirectCalls();
        }, py::return_value_policy::reference, "Get the indirect calls")
        // Get source and destination nodes
        .def("getSrcNode", &CallGraphEdge::getSrcNode, py::return_value_policy::reference, "Get the source node")
        .def("getDstNode", &CallGraphEdge::getDstNode, py::return_value_policy::reference, "Get the destination node")
        .def("getSrcID", &CallGraphEdge::getSrcID, "Get the source node ID")
        .def("getDstID", &CallGraphEdge::getDstID, "Get the destination node ID");
}

void bind_callgraph(py::module& m) {
    py::class_<CallGraph>(m, "CallGraph", "Call Graph used internally for various pointer analyses")
        .def("getNodes", [](const CallGraph& cg) {
            std::vector<CallGraphNode*> nodes;
            for (auto& node : cg) {
                nodes.push_back(node.second);
            }
            return nodes;
        }, py::return_value_policy::reference, "Get all nodes in the call graph")
        .def("getGNode", [](CallGraph& callg, NodeID id) -> CallGraphNode* {
            CallGraphNode* node = callg.getGNode(id);
            if (!node) {
                throw std::runtime_error("CallGraphNode with given ID not found.");
            }
            return node;
        }, py::arg("id"), py::return_value_policy::reference, "Get a VFG node by ID")
        .def("getCallGraphNodeByFunObj", [](CallGraph& cg, const FunObjVar* fun) {
            return cg.getCallGraphNode(fun);
        }, py::return_value_policy::reference, "Get a call graph node by function")
        .def("getCallGraphNodeByName", [](CallGraph& cg, const std::string& name) {
            return cg.getCallGraphNode(name);
        }, py::return_value_policy::reference, "Get a call graph node by function name")
        .def("getCallGraphNodeById", [](CallGraph& cg, NodeID id) -> CallGraphNode* {
            CallGraphNode* node = cg.getGNode(id);
            if (!node) {
                throw std::runtime_error("CallGraphNode with given ID not found.");
            }
            return node;
        }, py::arg("id"), py::return_value_policy::reference, "Get a call graph node by ID")
        .def("isReachableBetweenFunctions", &CallGraph::isReachableBetweenFunctions, "Check if there's a path between two functions")
        .def("dump", [](CallGraph &icfg, std::string file) {
            icfg.dump(file);
        })
        .def("view", &CallGraph::view, "View the call graph");
}

// Add this to svf_pybind.cpp
void bind_basic_block(py::module& m) {
    py::class_<SVF::SVFBasicBlock>(m, "SVFBasicBlock")
            .def("getId", &SVF::SVFBasicBlock::getId)
            .def("getName", &SVF::SVFBasicBlock::getName)
                    // Access ICFGNodes in the basic block
            .def("getICFGNodeList", &SVF::SVFBasicBlock::getICFGNodeList, py::return_value_policy::reference)
            .def("front", &SVF::SVFBasicBlock::front, py::return_value_policy::reference)
            .def("back", &SVF::SVFBasicBlock::back, py::return_value_policy::reference)
                    // Iteration support
            .def("__iter__", [](const SVF::SVFBasicBlock& bb) {
                return py::make_iterator(bb.begin(), bb.end());
            }, py::keep_alive<0, 1>())
                    // Function/parent information
            .def("getParent", &SVF::SVFBasicBlock::getParent, py::return_value_policy::reference)
            .def("getFunction", &SVF::SVFBasicBlock::getFunction, py::return_value_policy::reference)
                    // CFG navigation
            .def("getSuccessors", &SVF::SVFBasicBlock::getSuccessors, py::return_value_policy::reference)
            .def("getPredecessors", &SVF::SVFBasicBlock::getPredecessors, py::return_value_policy::reference)
            .def("getNumSuccessors", &SVF::SVFBasicBlock::getNumSuccessors)
                    // Position queries
            .def("getBBSuccessorPos", static_cast<u32_t (SVF::SVFBasicBlock::*)(const SVF::SVFBasicBlock*) const>(&SVF::SVFBasicBlock::getBBSuccessorPos))
            .def("getBBPredecessorPos", static_cast<u32_t (SVF::SVFBasicBlock::*)(const SVF::SVFBasicBlock*) const>(&SVF::SVFBasicBlock::getBBPredecessorPos))
            // String representation __str__ __repr__ should check nullptr
            .def("__repr__", [](const SVF::SVFBasicBlock* bb) {
                if (bb == nullptr) {
                    return std::string("SVFBasicBlock(nullptr)");
                }
                std::ostringstream oss;
                oss << bb->toString();
                return oss.str();
            }, "Get the string representation of the basic block")
            .def("__str__", [](const SVF::SVFBasicBlock* bb) {
                if (bb == nullptr) {
                    return std::string("SVFBasicBlock(nullptr)");
                }
                std::ostringstream oss;
                oss << bb->toString();
                return oss.str();
            }, "Get the string representation of the basic block");
}

// Bind SVFGEdge classes
void bind_svfg_edge(py::module& m) {
    // Base VFGEdge class
    py::class_<VFGEdge, std::shared_ptr<VFGEdge>>(m, "VFGEdge", "Base class for Value-Flow Graph edges")
        .def("toString", [](const VFGEdge* edge) {
            if (!edge) {
                return std::string("nullptr");
            }
            std::ostringstream oss;
            if (edge->getSrcNode() == nullptr) {
                oss << "nullptr";
            } else {
                oss << edge->getSrcNode()->toString() << " -> ";
            }

            if (edge->getDstNode() == nullptr) {
                oss << "nullptr";
            } else {
                oss << edge->getDstNode()->toString();
            }
            return oss.str();
        }, "Get the string representation of the edge")
        .def("__str__", [](const VFGEdge* edge) {
            if (!edge) {
                return std::string("nullptr");
            }
            std::ostringstream oss;
            if (edge->getSrcNode() == nullptr) {
                oss << "nullptr";
            } else {
                oss << edge->getSrcNode()->toString() << " -> ";
            }

            if (edge->getDstNode() == nullptr) {
                oss << "nullptr";
            } else {
                oss << edge->getDstNode()->toString();
            }
            return oss.str();
        }, "Get the string representation of the edge")
        .def("__repr__", [](const VFGEdge* edge) {
            if (!edge) {
                return std::string("nullptr");
            }
            std::ostringstream oss;
            if (edge->getSrcNode() == nullptr) {
                oss << "nullptr";
            } else {
                oss << edge->getSrcNode()->toString() << " -> ";
            }

            if (edge->getDstNode() == nullptr) {
                oss << "nullptr";
            } else {
                oss << edge->getDstNode()->toString();
            }
            return oss.str();
        }, "Get the string representation of the edge")
        .def("isDirectVFGEdge", &VFGEdge::isDirectVFGEdge, "Check if this is a direct VFG edge")
        .def("isIndirectVFGEdge", &VFGEdge::isIndirectVFGEdge, "Check if this is an indirect VFG edge")
        .def("isCallVFGEdge", &VFGEdge::isCallVFGEdge, "Check if this is a call VFG edge")
        .def("isRetVFGEdge", &VFGEdge::isRetVFGEdge, "Check if this is a return VFG edge")
        .def("isCallDirectVFGEdge", &VFGEdge::isCallDirectVFGEdge, "Check if this is a direct call VFG edge")
        .def("isRetDirectVFGEdge", &VFGEdge::isRetDirectVFGEdge, "Check if this is a direct return VFG edge")
        .def("isCallIndirectVFGEdge", &VFGEdge::isCallIndirectVFGEdge, "Check if this is an indirect call VFG edge")
        .def("isRetIndirectVFGEdge", &VFGEdge::isRetIndirectVFGEdge, "Check if this is an indirect return VFG edge")
        .def("isIntraVFGEdge", &VFGEdge::isIntraVFGEdge, "Check if this is an intra-procedural VFG edge")
        .def("isThreadMHPIndirectVFGEdge", &VFGEdge::isThreadMHPIndirectVFGEdge, "Check if this is a thread MHP indirect VFG edge")
        .def("getSrcNode", [](const VFGEdge *edge) { return edge->getSrcNode(); }, py::return_value_policy::reference, "Get the source node")
        .def("getDstNode", [](const VFGEdge *edge) { return edge->getDstNode(); }, py::return_value_policy::reference, "Get the destination node")
        .def("asDirectSVFGEdge", [](VFGEdge *edge) -> DirectSVFGEdge* {
            if (DirectSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<DirectSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to DirectSVFGEdge")
        .def("asIndirectSVFGEdge", [](VFGEdge *edge) -> IndirectSVFGEdge* {
            if (IndirectSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<IndirectSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to IndirectSVFGEdge")
        .def("asCallDirSVFGEdge", [](VFGEdge *edge) -> CallDirSVFGEdge* {
            if (CallDirSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<CallDirSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to CallDirVFGEdge")
        .def("asRetDirSVFGEdge", [](VFGEdge *edge) -> RetDirSVFGEdge* {
            if (RetDirSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<RetDirSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to RetDirSVFGEdge")
        .def("asCallIndSVFGEdge", [](VFGEdge *edge) -> CallIndSVFGEdge* {
            if (CallIndSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<CallIndSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to CallIndSVFGEdge")
        .def("asRetIndSVFGEdge", [](VFGEdge *edge) -> RetIndSVFGEdge* {
            if (RetIndSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<RetIndSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to RetIndSVFGEdge")
        .def("asThreadMHPIndSVFGEdge", [](VFGEdge *edge) -> ThreadMHPIndSVFGEdge* {
            if (ThreadMHPIndSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<ThreadMHPIndSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to ThreadMHPIndSVFGEdge");


    // DirectSVFGEdge - direct value flow edge
    py::class_<DirectSVFGEdge, VFGEdge, std::shared_ptr<DirectSVFGEdge>>(m, "DirectSVFGEdge", "Direct SVF Graph Edge")
        .def("asIntraDirSVFGEdge", [](DirectSVFGEdge *edge) -> IntraDirSVFGEdge* {
            if (IntraDirSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<IntraDirSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to IntraDirSVFGEdge")
        .def("asCallDirSVFGEdge", [](DirectSVFGEdge *edge) -> CallDirSVFGEdge* {
            if (CallDirSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<CallDirSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to CallDirSVFGEdge")
        .def("asRetDirSVFGEdge", [](DirectSVFGEdge *edge) -> RetDirSVFGEdge* {
            if (RetDirSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<RetDirSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to RetDirSVFGEdge");

    // IntraDirSVFGEdge - Intraprocedural direct SVF edge
    py::class_<IntraDirSVFGEdge, DirectSVFGEdge, std::shared_ptr<IntraDirSVFGEdge>>(m, "IntraDirSVFGEdge", "Intraprocedural direct SVF edge");

    // CallDirSVFGEdge - Call direct SVF edge
    py::class_<CallDirSVFGEdge, DirectSVFGEdge, std::shared_ptr<CallDirSVFGEdge>>(m, "CallDirSVFGEdge", "Call direct SVF edge")
        .def("getCallSiteId", &CallDirSVFGEdge::getCallSiteId, "Get the call site ID");

    // RetDirSVFGEdge - Return direct SVF edge
    py::class_<RetDirSVFGEdge, DirectSVFGEdge, std::shared_ptr<RetDirSVFGEdge>>(m, "RetDirSVFGEdge", "Return direct SVF edge")
        .def("getCallSiteId", &RetDirSVFGEdge::getCallSiteId, "Get the call site ID");

    // IndirectSVFGEdge - Indirect value flow edge
    py::class_<IndirectSVFGEdge, VFGEdge, std::shared_ptr<IndirectSVFGEdge>>(m, "IndirectSVFGEdge", "Indirect SVF Graph Edge")
        .def("getPointsTo", &IndirectSVFGEdge::getPointsTo, py::return_value_policy::reference, "Get the points-to set")
        .def("addPointsTo", &IndirectSVFGEdge::addPointsTo, "Add to the points-to set")
        .def("asIntraIndSVFGEdge", [](IndirectSVFGEdge *edge) -> IntraIndSVFGEdge* {
            if (IntraIndSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<IntraIndSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to IntraIndSVFGEdge")
        .def("asCallIndSVFGEdge", [](IndirectSVFGEdge *edge) -> CallIndSVFGEdge* {
            if (CallIndSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<CallIndSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to CallIndSVFGEdge")
        .def("asRetIndSVFGEdge", [](IndirectSVFGEdge *edge) -> RetIndSVFGEdge* {
            if (RetIndSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<RetIndSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to RetIndSVFGEdge")
        .def("asThreadMhpIndSVFGEdge", [](IndirectSVFGEdge *edge) -> ThreadMHPIndSVFGEdge* {
            if (ThreadMHPIndSVFGEdge::classof(edge))
                return SVFUtil::dyn_cast<ThreadMHPIndSVFGEdge>(edge);
            return nullptr;
        }, py::return_value_policy::reference, "Downcast to ThreadMHPIndSVFGEdge");

    // IntraIndSVFGEdge - Intraprocedural indirect SVF edge
    py::class_<IntraIndSVFGEdge, IndirectSVFGEdge, std::shared_ptr<IntraIndSVFGEdge>>(m, "IntraIndSVFGEdge", "Intraprocedural indirect SVF edge");

    // CallIndSVFGEdge - Call indirect SVF edge
    py::class_<CallIndSVFGEdge, IndirectSVFGEdge, std::shared_ptr<CallIndSVFGEdge>>(m, "CallIndSVFGEdge", "Call indirect SVF edge")
        .def("getCallSiteId", &CallIndSVFGEdge::getCallSiteId, "Get the call site ID");

    // RetIndSVFGEdge - Return indirect SVF edge
    py::class_<RetIndSVFGEdge, IndirectSVFGEdge, std::shared_ptr<RetIndSVFGEdge>>(m, "RetIndSVFGEdge", "Return indirect SVF edge")
        .def("getCallSiteId", &RetIndSVFGEdge::getCallSiteId, "Get the call site ID");

    // ThreadMHPIndSVFGEdge - Thread MHP indirect SVF edge
    py::class_<ThreadMHPIndSVFGEdge, IndirectSVFGEdge, std::shared_ptr<ThreadMHPIndSVFGEdge>>(m, "ThreadMHPIndSVFGEdge", "Thread MHP indirect SVF edge");
}


// Bind VFGNode classes
void bind_vfg_node(py::module& m) {
    // Base VFGNode class
    py::class_<VFGNode, std::shared_ptr<VFGNode>>(m, "VFGNode", "Base class for Value-Flow Graph nodes")
        .def("toString", [](const VFGNode* node) {
            if (node == nullptr) {
                return std::string("VFGNode(nullptr)");
            }
            std::ostringstream oss;
            oss << node->toString();
            return oss.str();
        }, "Get the string representation of the node")
        .def("__str__", [](const VFGNode* node) {
            if (node == nullptr) {
                return std::string("VFGNode(nullptr)");
            }
            std::ostringstream oss;
            oss << node->toString();
            return oss.str();
        }, "Get the string representation of the node")
        .def("__repr__", [](const VFGNode* node) {
            if (node == nullptr) {
                return std::string("VFGNode(nullptr)");
            }
            std::ostringstream oss;
            oss << node->toString();
            return oss.str();
        }, "Get the string representation of the node")
        .def("getId", &VFGNode::getId, "Get the ID of this VFG node")
        .def("getValue", &VFGNode::getValue, py::return_value_policy::reference, "Get the SVF variable value")
        .def("getICFGNode", &VFGNode::getICFGNode, py::return_value_policy::reference, "Get the corresponding ICFG node")
        .def("getDefSVFVars", &VFGNode::getDefSVFVars, "Get the defined SVF variables")
        .def("getOutEdges", [](const VFGNode *node) {
            std::vector<VFGEdge *> edges;
            for (auto it = node->OutEdgeBegin(); it != node->OutEdgeEnd(); ++it) {
                edges.push_back(*it);
            }
            return edges;
        }, py::return_value_policy::reference, "Get the out edges of the VFG node")
        .def("getInEdges", [](const VFGNode *node) {
            std::vector<VFGEdge *> edges;
            for (auto it = node->InEdgeBegin(); it != node->InEdgeEnd(); ++it) {
                edges.push_back(*it);
            }
            return edges;
        }, py::return_value_policy::reference, "Get the in edges of the VFG node")
        // Type checking methods
        .def("isStmtVFGNode", [](const VFGNode* node) { return SVFUtil::isa<StmtVFGNode>(node); }, "Check if this is a statement VFG node")
        .def("isPhiVFGNode", [](const VFGNode* node) { return SVFUtil::isa<PHIVFGNode>(node); }, "Check if this is a phi VFG node")
        .def("isArgumentVFGNode", [](const VFGNode* node) { return SVFUtil::isa<ArgumentVFGNode>(node); }, "Check if this is an argument VFG node")
        .def("isCmpVFGNode", [](const VFGNode* node) { return SVFUtil::isa<CmpVFGNode>(node); }, "Check if this is a compare VFG node")
        .def("isBinaryOpVFGNode", [](const VFGNode* node) { return SVFUtil::isa<BinaryOPVFGNode>(node); }, "Check if this is a binary operation VFG node")
        .def("isUnaryOpVFGNode", [](const VFGNode* node) { return SVFUtil::isa<UnaryOPVFGNode>(node); }, "Check if this is a unary operation VFG node")
        .def("isBranchVFGNode", [](const VFGNode* node) { return SVFUtil::isa<BranchVFGNode>(node); }, "Check if this is a branch VFG node")
        // Downcasting methods
        .def("asStmtVFGNode", [](VFGNode* node) { return SVFUtil::dyn_cast<StmtVFGNode>(node); }, py::return_value_policy::reference, "Downcast to StmtVFGNode")
        .def("asPhiVFGNode", [](VFGNode* node) { return SVFUtil::dyn_cast<PHIVFGNode>(node); }, py::return_value_policy::reference, "Downcast to PHIVFGNode")
        .def("asArgumentVFGNode", [](VFGNode* node) { return SVFUtil::dyn_cast<ArgumentVFGNode>(node); }, py::return_value_policy::reference, "Downcast to ArgumentVFGNode")
        .def("asCmpVFGNode", [](VFGNode* node) { return SVFUtil::dyn_cast<CmpVFGNode>(node); }, py::return_value_policy::reference, "Downcast to CmpVFGNode")
        .def("asBinaryOpVFGNode", [](VFGNode* node) { return SVFUtil::dyn_cast<BinaryOPVFGNode>(node); }, py::return_value_policy::reference, "Downcast to BinaryOPVFGNode")
        .def("asUnaryOpVFGNode", [](VFGNode* node) { return SVFUtil::dyn_cast<UnaryOPVFGNode>(node); }, py::return_value_policy::reference, "Downcast to UnaryOPVFGNode")
        .def("asBranchVFGNode", [](VFGNode* node) { return SVFUtil::dyn_cast<BranchVFGNode>(node); }, py::return_value_policy::reference, "Downcast to BranchVFGNode");

    // StmtVFGNode - Statement VFG node
    py::class_<StmtVFGNode, VFGNode, std::shared_ptr<StmtVFGNode>>(m, "StmtVFGNode", "Statement VFG node")
        .def("getValue", &StmtVFGNode::getValue, py::return_value_policy::reference, "Get the SVF variable value")
        // Type checking
        .def("isLoadVFGNode", [](const StmtVFGNode* node) { return SVFUtil::isa<LoadVFGNode>(node); }, "Check if this is a load VFG node")
        .def("isStoreVFGNode", [](const StmtVFGNode* node) { return SVFUtil::isa<StoreVFGNode>(node); }, "Check if this is a store VFG node") 
        .def("isCopyVFGNode", [](const StmtVFGNode* node) { return SVFUtil::isa<CopyVFGNode>(node); }, "Check if this is a copy VFG node")
        .def("isGepVFGNode", [](const StmtVFGNode* node) { return SVFUtil::isa<GepVFGNode>(node); }, "Check if this is a gep VFG node")
        .def("isAddrVFGNode", [](const StmtVFGNode* node) { return SVFUtil::isa<AddrVFGNode>(node); }, "Check if this is an addr VFG node")
        // Downcasting
        .def("asLoadVFGNode", [](StmtVFGNode* node) { return SVFUtil::dyn_cast<LoadVFGNode>(node); }, py::return_value_policy::reference, "Downcast to LoadVFGNode")
        .def("asStoreVFGNode", [](StmtVFGNode* node) { return SVFUtil::dyn_cast<StoreVFGNode>(node); }, py::return_value_policy::reference, "Downcast to StoreVFGNode")
        .def("asCopyVFGNode", [](StmtVFGNode* node) { return SVFUtil::dyn_cast<CopyVFGNode>(node); }, py::return_value_policy::reference, "Downcast to CopyVFGNode")
        .def("asGepVFGNode", [](StmtVFGNode* node) { return SVFUtil::dyn_cast<GepVFGNode>(node); }, py::return_value_policy::reference, "Downcast to GepVFGNode")
        .def("asAddrVFGNode", [](StmtVFGNode* node) { return SVFUtil::dyn_cast<AddrVFGNode>(node); }, py::return_value_policy::reference, "Downcast to AddrVFGNode");

    // LoadVFGNode - Load statement VFG node
    py::class_<LoadVFGNode, StmtVFGNode, std::shared_ptr<LoadVFGNode>>(m, "LoadVFGNode", "Load VFG node")
        .def("getDefSVFVars", &LoadVFGNode::getDefSVFVars, "Get defined SVF variables");

    // StoreVFGNode - Store statement VFG node
    py::class_<StoreVFGNode, StmtVFGNode, std::shared_ptr<StoreVFGNode>>(m, "StoreVFGNode", "Store VFG node")
        .def("getDefSVFVars", &StoreVFGNode::getDefSVFVars, "Get defined SVF variables");

    // CopyVFGNode - Copy statement VFG node
    py::class_<CopyVFGNode, StmtVFGNode, std::shared_ptr<CopyVFGNode>>(m, "CopyVFGNode", "Copy VFG node")
        .def("getDefSVFVars", &CopyVFGNode::getDefSVFVars, "Get defined SVF variables");

    // GepVFGNode - GetElementPtr VFG node
    py::class_<GepVFGNode, StmtVFGNode, std::shared_ptr<GepVFGNode>>(m, "GepVFGNode", "Gep VFG node")
        .def("getDefSVFVars", &GepVFGNode::getDefSVFVars, "Get defined SVF variables");

    // AddrVFGNode - Address-of VFG node
    py::class_<AddrVFGNode, StmtVFGNode, std::shared_ptr<AddrVFGNode>>(m, "AddrVFGNode", "Address VFG node")
        .def("getDefSVFVars", &AddrVFGNode::getDefSVFVars, "Get defined SVF variables");

    // PHIVFGNode - PHI VFG node
    py::class_<PHIVFGNode, VFGNode, std::shared_ptr<PHIVFGNode>>(m, "PHIVFGNode", "PHI VFG node")
        .def("getDefSVFVars", &PHIVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("getValue", &PHIVFGNode::getValue, py::return_value_policy::reference, "Get result SVF variable")
        // Type checking
        .def("isIntraPhiVFGNode", [](const PHIVFGNode* node) { return SVFUtil::isa<IntraPHIVFGNode>(node); }, "Check if this is an intra-procedural PHI VFG node")
        .def("isInterPhiVFGNode", [](const PHIVFGNode* node) { return SVFUtil::isa<InterPHIVFGNode>(node); }, "Check if this is an inter-procedural PHI VFG node")
        // Downcasting
        .def("asIntraPhiVFGNode", [](PHIVFGNode* node) { return SVFUtil::dyn_cast<IntraPHIVFGNode>(node); }, py::return_value_policy::reference, "Downcast to IntraPHIVFGNode")
        .def("asInterPhiVFGNode", [](PHIVFGNode* node) { return SVFUtil::dyn_cast<InterPHIVFGNode>(node); }, py::return_value_policy::reference, "Downcast to InterPHIVFGNode");

    // IntraPHIVFGNode - Intra-procedural PHI VFG node
    py::class_<IntraPHIVFGNode, PHIVFGNode, std::shared_ptr<IntraPHIVFGNode>>(m, "IntraPHIVFGNode", "Intra-procedural PHI VFG node");

    // InterPHIVFGNode - Inter-procedural PHI VFG node
    py::class_<InterPHIVFGNode, PHIVFGNode, std::shared_ptr<InterPHIVFGNode>>(m, "InterPHIVFGNode", "Inter-procedural PHI VFG node");

    // ArgumentVFGNode - Base class for argument VFG nodes
    py::class_<ArgumentVFGNode, VFGNode, std::shared_ptr<ArgumentVFGNode>>(m, "ArgumentVFGNode", "Argument VFG node")
        .def("getValue", &ArgumentVFGNode::getValue, py::return_value_policy::reference, "Get parameter SVF variable")
        // Type checking
        .def("isActualParmVFGNode", [](const ArgumentVFGNode* node) { return SVFUtil::isa<ActualParmVFGNode>(node); }, "Check if this is an actual parameter VFG node")
        .def("isFormalParmVFGNode", [](const ArgumentVFGNode* node) { return SVFUtil::isa<FormalParmVFGNode>(node); }, "Check if this is a formal parameter VFG node")
        .def("isActualRetVFGNode", [](const ArgumentVFGNode* node) { return SVFUtil::isa<ActualRetVFGNode>(node); }, "Check if this is an actual return VFG node")
        .def("isFormalRetVFGNode", [](const ArgumentVFGNode* node) { return SVFUtil::isa<FormalRetVFGNode>(node); }, "Check if this is a formal return VFG node")
        // Downcasting
        .def("asActualParmVFGNode", [](ArgumentVFGNode* node) { return SVFUtil::dyn_cast<ActualParmVFGNode>(node); }, py::return_value_policy::reference, "Downcast to ActualParmVFGNode")
        .def("asFormalParmVFGNode", [](ArgumentVFGNode* node) { return SVFUtil::dyn_cast<FormalParmVFGNode>(node); }, py::return_value_policy::reference, "Downcast to FormalParmVFGNode")
        .def("asActualRetVFGNode", [](ArgumentVFGNode* node) { return SVFUtil::dyn_cast<ActualRetVFGNode>(node); }, py::return_value_policy::reference, "Downcast to ActualRetVFGNode")
        .def("asFormalRetVFGNode", [](ArgumentVFGNode* node) { return SVFUtil::dyn_cast<FormalRetVFGNode>(node); }, py::return_value_policy::reference, "Downcast to FormalRetVFGNode");

    // ActualParmVFGNode - Actual parameter VFG node
    py::class_<ActualParmVFGNode, ArgumentVFGNode, std::shared_ptr<ActualParmVFGNode>>(m, "ActualParmVFGNode", "Actual parameter VFG node")
        .def("getDefSVFVars", &ActualParmVFGNode::getDefSVFVars, "Get defined SVF variables");

    // FormalParmVFGNode - Formal parameter VFG node
    py::class_<FormalParmVFGNode, ArgumentVFGNode, std::shared_ptr<FormalParmVFGNode>>(m, "FormalParmVFGNode", "Formal parameter VFG node")
        .def("getDefSVFVars", &FormalParmVFGNode::getDefSVFVars, "Get defined SVF variables");

    // ActualRetVFGNode - Actual return VFG node
    py::class_<ActualRetVFGNode, ArgumentVFGNode, std::shared_ptr<ActualRetVFGNode>>(m, "ActualRetVFGNode", "Actual return VFG node")
        .def("getDefSVFVars", &ActualRetVFGNode::getDefSVFVars, "Get defined SVF variables");

    // FormalRetVFGNode - Formal return VFG node
    py::class_<FormalRetVFGNode, ArgumentVFGNode, std::shared_ptr<FormalRetVFGNode>>(m, "FormalRetVFGNode", "Formal return VFG node")
        .def("getDefSVFVars", &FormalRetVFGNode::getDefSVFVars, "Get defined SVF variables");

    // CmpVFGNode - Compare operation VFG node
    py::class_<CmpVFGNode, VFGNode, std::shared_ptr<CmpVFGNode>>(m, "CmpVFGNode", "Compare VFG node")
        .def("getDefSVFVars", &CmpVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("getValue", &CmpVFGNode::getValue, py::return_value_policy::reference, "Get result SVF variable");

    // BinaryOPVFGNode - Binary operation VFG node
    py::class_<BinaryOPVFGNode, VFGNode, std::shared_ptr<BinaryOPVFGNode>>(m, "BinaryOPVFGNode", "Binary operation VFG node")
        .def("getDefSVFVars", &BinaryOPVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("getValue", &BinaryOPVFGNode::getValue, py::return_value_policy::reference, "Get result SVF variable");

    // UnaryOPVFGNode - Unary operation VFG node
    py::class_<UnaryOPVFGNode, VFGNode, std::shared_ptr<UnaryOPVFGNode>>(m, "UnaryOPVFGNode", "Unary operation VFG node")
        .def("getDefSVFVars", &UnaryOPVFGNode::getDefSVFVars, "Get defined SVF variables");

    // BranchVFGNode - Branch VFG node
    py::class_<BranchVFGNode, VFGNode, std::shared_ptr<BranchVFGNode>>(m, "BranchVFGNode", "Branch VFG node")
        .def("getDefSVFVars", &BranchVFGNode::getDefSVFVars, "Get defined SVF variables");
}

// Bind SVFGNode classes
void bind_svfg_node(py::module& m) {
    // MRSVFGNode - Memory region VFGNode (for address-taken objects)
    py::class_<MRSVFGNode, VFGNode, std::shared_ptr<MRSVFGNode>>(m, "MRSVFGNode", "Memory region VFG node")
        .def("getPointsTo", &MRSVFGNode::getPointsTo, py::return_value_policy::reference, "Get the points-to set of the memory region")
        .def("getDefSVFVars", &MRSVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("toString", [](const MRSVFGNode* node) {
            if (node == nullptr) {
                return std::string("MRSVFGNode(nullptr)");
            }
            std::ostringstream oss;
            oss << node->toString();
            return oss.str();
        }, "Get the string representation of the memory region VFG node")
        .def("__str__", [](const MRSVFGNode* node) {
            if (node == nullptr) {
                return std::string("MRSVFGNode(nullptr)");
            }
            std::ostringstream oss;
            oss << node->toString();
            return oss.str();
        }, "Get the string representation of the memory region VFG node")
        .def("__repr__", [](const MRSVFGNode* node) {
            if (node == nullptr) {
                return std::string("MRSVFGNode(nullptr)");
            }
            std::ostringstream oss;
            oss << node->toString();
            return oss.str();
        }, "Get the string representation of the memory region VFG node")
        // Type checking
        .def("isFormalInSVFGNode", [](const MRSVFGNode* node) { return SVFUtil::isa<FormalINSVFGNode>(node); }, "Check if this is a formal-in SVFG node")
        .def("isFormalOutSVFGNode", [](const MRSVFGNode* node) { return SVFUtil::isa<FormalOUTSVFGNode>(node); }, "Check if this is a formal-out SVFG node")
        .def("isActualInSVFGNode", [](const MRSVFGNode* node) { return SVFUtil::isa<ActualINSVFGNode>(node); }, "Check if this is an actual-in SVFG node")
        .def("isActualOutSVFGNode", [](const MRSVFGNode* node) { return SVFUtil::isa<ActualOUTSVFGNode>(node); }, "Check if this is an actual-out SVFG node")
        .def("isMssaPhiSVFGNode", [](const MRSVFGNode* node) { return SVFUtil::isa<MSSAPHISVFGNode>(node); }, "Check if this is an MSSA phi SVFG node")
        // Downcasting
        .def("asFormalInSVFGNode", [](MRSVFGNode* node) { return SVFUtil::dyn_cast<FormalINSVFGNode>(node); }, py::return_value_policy::reference, "Downcast to FormalINSVFGNode")
        .def("asFormalOutSVFGNode", [](MRSVFGNode* node) { return SVFUtil::dyn_cast<FormalOUTSVFGNode>(node); }, py::return_value_policy::reference, "Downcast to FormalOUTSVFGNode")
        .def("asActualInSVFGNode", [](MRSVFGNode* node) { return SVFUtil::dyn_cast<ActualINSVFGNode>(node); }, py::return_value_policy::reference, "Downcast to ActualINSVFGNode")
        .def("asActualOutSVFGNode", [](MRSVFGNode* node) { return SVFUtil::dyn_cast<ActualOUTSVFGNode>(node); }, py::return_value_policy::reference, "Downcast to ActualOUTSVFGNode")
        .def("asMssaPhiSVFGNode", [](MRSVFGNode* node) { return SVFUtil::dyn_cast<MSSAPHISVFGNode>(node); }, py::return_value_policy::reference, "Downcast to MSSAPHISVFGNode");
    
    // FormalINSVFGNode - SVFG Node for entry chi node (address-taken variables)
    py::class_<FormalINSVFGNode, MRSVFGNode, std::shared_ptr<FormalINSVFGNode>>(m, "FormalINSVFGNode", "Formal-in SVFG node (entry chi)")
        .def("getMRVer", &FormalINSVFGNode::getMRVer, py::return_value_policy::reference, "Get the memory region version")
        .def("getFunEntryNode", &FormalINSVFGNode::getFunEntryNode, py::return_value_policy::reference, "Get the function entry ICFG node");
    
    // FormalOUTSVFGNode - SVFG Node for return mu node (address-taken variables)
    py::class_<FormalOUTSVFGNode, MRSVFGNode, std::shared_ptr<FormalOUTSVFGNode>>(m, "FormalOUTSVFGNode", "Formal-out SVFG node (return mu)")
        .def("getMRVer", &FormalOUTSVFGNode::getMRVer, py::return_value_policy::reference, "Get the memory region version")
        .def("getFunExitNode", &FormalOUTSVFGNode::getFunExitNode, py::return_value_policy::reference, "Get the function exit ICFG node");
    
    // ActualINSVFGNode - SVFG Node for callsite mu node (address-taken variables)
    py::class_<ActualINSVFGNode, MRSVFGNode, std::shared_ptr<ActualINSVFGNode>>(m, "ActualINSVFGNode", "Actual-in SVFG node (callsite mu)")
        .def("getMRVer", &ActualINSVFGNode::getMRVer, py::return_value_policy::reference, "Get the memory region version")
        .def("getCallSite", &ActualINSVFGNode::getCallSite, py::return_value_policy::reference, "Get the call site");
    
    // ActualOUTSVFGNode - SVFG Node for callsite chi node (address-taken variables)
    py::class_<ActualOUTSVFGNode, MRSVFGNode, std::shared_ptr<ActualOUTSVFGNode>>(m, "ActualOUTSVFGNode", "Actual-out SVFG node (callsite chi)")
        .def("getMRVer", &ActualOUTSVFGNode::getMRVer, py::return_value_policy::reference, "Get the memory region version")
        .def("getCallSite", &ActualOUTSVFGNode::getCallSite, py::return_value_policy::reference, "Get the call site");
    
    // MSSAPHISVFGNode - SVFG Node for memory SSA phi nodes
    py::class_<MSSAPHISVFGNode, MRSVFGNode, std::shared_ptr<MSSAPHISVFGNode>>(m, "MSSAPHISVFGNode", "Memory SSA PHI SVFG node")
        // Type checking
        .def("isIntraMssaPhiSVFGNode", [](const MSSAPHISVFGNode* node) { return SVFUtil::isa<IntraMSSAPHISVFGNode>(node); }, "Check if this is an intra-procedural MSSA phi SVFG node")
        .def("isInterMssaPhiSVFGNode", [](const MSSAPHISVFGNode* node) { return SVFUtil::isa<InterMSSAPHISVFGNode>(node); }, "Check if this is an inter-procedural MSSA phi SVFG node")
        // Downcasting
        .def("asIntraMssaPhiSVFGNode", [](MSSAPHISVFGNode* node) { return SVFUtil::dyn_cast<IntraMSSAPHISVFGNode>(node); }, py::return_value_policy::reference, "Downcast to IntraMSSAPHISVFGNode")
        .def("asInterMssaPhiSVFGNode", [](MSSAPHISVFGNode* node) { return SVFUtil::dyn_cast<InterMSSAPHISVFGNode>(node); }, py::return_value_policy::reference, "Downcast to InterMSSAPHISVFGNode");
    
    // IntraMSSAPHISVFGNode - Intra-procedural MSSA PHI node
    py::class_<IntraMSSAPHISVFGNode, MSSAPHISVFGNode, std::shared_ptr<IntraMSSAPHISVFGNode>>(m, "IntraMSSAPHISVFGNode", "Intra-procedural Memory SSA PHI SVFG node");
    
    // InterMSSAPHISVFGNode - Inter-procedural MSSA PHI node (formalIN/ActualOUT)
    py::class_<InterMSSAPHISVFGNode, MSSAPHISVFGNode, std::shared_ptr<InterMSSAPHISVFGNode>>(m, "InterMSSAPHISVFGNode", "Inter-procedural Memory SSA PHI SVFG node")
        .def("getFun", &InterMSSAPHISVFGNode::getFun, py::return_value_policy::reference, "Get the function");
    
    // DummyVersionPropSVFGNode - Dummy node for object/version propagation
    py::class_<DummyVersionPropSVFGNode, VFGNode, std::shared_ptr<DummyVersionPropSVFGNode>>(m, "DummyVersionPropSVFGNode", "Dummy version propagation SVFG node")
        .def("getDefSVFVars", &DummyVersionPropSVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("getObject", [](const DummyVersionPropSVFGNode& node) { return node.getObject(); }, "Get the object ID")
        .def("getVersion", [](const DummyVersionPropSVFGNode& node) { return node.getVersion(); }, "Get the version");
}


// Bind VFG class
void bind_vfg(py::module& m) {
    py::class_<VFG>(m, "VFG", "Value Flow Graph")
        .def("getNodes", [](VFG& vfg) {
            std::vector<VFGNode*> nodes;
            for (auto& node : vfg) {
                nodes.push_back(node.second);
            }
            return nodes;
        }, py::return_value_policy::reference, "Get all nodes in the VFG")
        .def("getGNode", [](VFG& vfg, NodeID id) -> VFGNode* {
            VFGNode* node = vfg.getGNode(id);
            if (!node) {
                throw std::runtime_error("VFGNode with given ID not found.");
            }
            return node;
        }, py::arg("id"), py::return_value_policy::reference, "Get a VFG node by ID")
        .def("view", &VFG::view, "View the VFG")
        .def("getTotalNodeNum", &VFG::getTotalNodeNum, "Get the total number of nodes")
        .def("getTotalEdgeNum", &VFG::getTotalEdgeNum, "Get the total number of edges");
}

// Bind SVFG class
void bind_svfg(py::module& m) {
    py::class_<SVFG, VFG>(m, "SVFG", "Sparse Value Flow Graph")
        // SVFG specific methods
        .def("hasDefSVFGNode", [](SVFG& svfg, const SVFVar* val) { return svfg.hasDefSVFGNode(val);}, py::arg("val"), "Check if there is a definition SVFG node for a given SVF variable")
        .def("getDefSVFGNode", [](SVFG& svfg, const SVFVar* val) { return svfg.getDefSVFGNode(val);}, py::arg("val"), py::return_value_policy::reference, "Get all the definition SVFG nodes of a SVF variable")
        .def("getActualOUTSVFGNodes", [](SVFG& svfg, const CallICFGNode* cs) { return svfg.getActualOUTSVFGNodes(cs); }, py::arg("cs"), py::return_value_policy::reference, "Get the ActualOUT SVFG nodes for a given call site")
        .def("getActualINSVFGNodes", [](SVFG& svfg, const CallICFGNode* cs) { return svfg.getActualINSVFGNodes(cs); }, py::arg("cs"), py::return_value_policy::reference, "Get the ActualIN SVFG nodes for a given call site")
        .def("getFormalOUTSVFGNodes", [](SVFG& svfg, const FunObjVar* fun) { return svfg.getFormalOUTSVFGNodes(fun); }, py::arg("fun"), py::return_value_policy::reference, "Get the FormalOUT SVFG nodes for a given function")
        .def("getFormalINSVFGNodes", [](SVFG& svfg, const FunObjVar* fun) { return svfg.getFormalINSVFGNodes(fun); }, py::arg("fun"), py::return_value_policy::reference, "Get the FormalIN SVFG nodes for a given function")
        .def("dump", [](SVFG &icfg, std::string file) {
            icfg.dump(file);
        })
        .def("view", &SVFG::view, "View the SVFG")
        // has methods
        .def("hasActualOUTSVFGNodes", [](SVFG& svfg, const CallICFGNode* cs) { return svfg.hasActualOUTSVFGNodes(cs); }, py::arg("cs"), "Check if there are ActualOUT SVFG nodes for a given call site")
        .def("hasActualINSVFGNodes", [](SVFG& svfg, const CallICFGNode* cs) { return svfg.hasActualINSVFGNodes(cs); }, py::arg("cs"), "Check if there are ActualIN SVFG nodes for a given call site")
        .def("hasFormalOUTSVFGNodes", [](SVFG& svfg, const FunObjVar* fun) { return svfg.hasFormalOUTSVFGNodes(fun); }, py::arg("fun"), "Check if there are FormalOUT SVFG nodes for a given function")
        .def("hasFormalINSVFGNodes", [](SVFG& svfg, const FunObjVar* fun) { return svfg.hasFormalINSVFGNodes(fun); }, py::arg("fun"), "Check if there are FormalIN SVFG nodes for a given function");
}

void bind_constraint_graph(py::module& m) {
    // ConstraintEdge (base class)
    py::class_<ConstraintEdge>(m, "ConstraintEdge", "Constraint edge")
        .def("getSrcNode", &ConstraintEdge::getSrcNode, py::return_value_policy::reference)
        .def("getDstNode", &ConstraintEdge::getDstNode, py::return_value_policy::reference)
        .def("getSrcID", &ConstraintEdge::getSrcID)
        .def("getDstID", &ConstraintEdge::getDstID)
        .def("isAddrCGEdge", [](ConstraintEdge* edge) { return SVFUtil::isa<AddrCGEdge>(edge); }, "Check if this is an address constraint edge")
        .def("isCopyCGEdge", [](ConstraintEdge* edge) { return SVFUtil::isa<CopyCGEdge>(edge); }, "Check if this is a copy constraint edge")
        .def("isStoreCGEdge", [](ConstraintEdge* edge) { return SVFUtil::isa<StoreCGEdge>(edge); }, "Check if this is a store constraint edge")
        .def("isLoadCGEdge", [](ConstraintEdge* edge) { return SVFUtil::isa<LoadCGEdge>(edge); }, "Check if this is a load constraint edge")
        .def("isGepCGEdge", [](ConstraintEdge* edge) { return SVFUtil::isa<GepCGEdge>(edge); }, "Check if this is a gep constraint edge")
        .def("isNormalGepCGEdge", [](ConstraintEdge* edge) { return SVFUtil::isa<NormalGepCGEdge>(edge); }, "Check if this is a normal gep constraint edge")
        .def("isVariantGepCGEdge", [](ConstraintEdge* edge) { return SVFUtil::isa<VariantGepCGEdge>(edge); }, "Check if this is a variant gep constraint edge")
        .def("asAddrCGEdge", [](ConstraintEdge* edge) { return SVFUtil::dyn_cast<AddrCGEdge>(edge); }, py::return_value_policy::reference)
        .def("asCopyCGEdge", [](ConstraintEdge* edge) { return SVFUtil::dyn_cast<CopyCGEdge>(edge); }, py::return_value_policy::reference)
        .def("asStoreCGEdge", [](ConstraintEdge* edge) { return SVFUtil::dyn_cast<StoreCGEdge>(edge); }, py::return_value_policy::reference)
        .def("asLoadCGEdge", [](ConstraintEdge* edge) { return SVFUtil::dyn_cast<LoadCGEdge>(edge); }, py::return_value_policy::reference)
        .def("asGepCGEdge", [](ConstraintEdge* edge) { return SVFUtil::dyn_cast<GepCGEdge>(edge); }, py::return_value_policy::reference)
        .def("asNormalGepCGEdge", [](ConstraintEdge* edge) { return SVFUtil::dyn_cast<NormalGepCGEdge>(edge); }, py::return_value_policy::reference)
        .def("asVariantGepCGEdge", [](ConstraintEdge* edge) { return SVFUtil::dyn_cast<VariantGepCGEdge>(edge); }, py::return_value_policy::reference);

    // Subclasses of ConstraintEdge
    py::class_<AddrCGEdge, ConstraintEdge>(m, "AddrCGEdge");
    py::class_<CopyCGEdge, ConstraintEdge>(m, "CopyCGEdge");
    py::class_<StoreCGEdge, ConstraintEdge>(m, "StoreCGEdge");
    py::class_<LoadCGEdge, ConstraintEdge>(m, "LoadCGEdge");
    // GepCGEdge is abstract; don't instantiate directly
    py::class_<GepCGEdge, ConstraintEdge>(m, "GepCGEdge")
        .def("asNormalGepCGEdge", [](GepCGEdge* edge) { return SVFUtil::dyn_cast<NormalGepCGEdge>(edge); }, py::return_value_policy::reference)
        .def("asVariantGepCGEdge", [](GepCGEdge* edge) { return SVFUtil::dyn_cast<VariantGepCGEdge>(edge); }, py::return_value_policy::reference);

    // NormalGepCGEdge
    py::class_<NormalGepCGEdge, GepCGEdge>(m, "NormalGepCGEdge")
        .def("getAccessPath", &NormalGepCGEdge::getAccessPath, py::return_value_policy::reference)
        .def("getConstantFieldIdx", &NormalGepCGEdge::getConstantFieldIdx);

    // VariantGepCGEdge
    py::class_<VariantGepCGEdge, GepCGEdge>(m, "VariantGepCGEdge");


    py::class_<ConstraintNode>(m, "ConstraintNode", "Constraint node")
        .def("toString", &ConstraintNode::toString)
        .def("__str__", &ConstraintNode::toString)
        .def("__repr__", &ConstraintNode::toString)
        .def("getId", &ConstraintNode::getId)
        .def("getInEdges", &ConstraintNode::getInEdges, py::return_value_policy::reference)
        .def("getOutEdges", &ConstraintNode::getOutEdges, py::return_value_policy::reference)
        .def("getDirectInEdges", &ConstraintNode::getDirectInEdges, py::return_value_policy::reference)
        .def("getDirectOutEdges", &ConstraintNode::getDirectOutEdges, py::return_value_policy::reference)
        .def("getCopyInEdges", &ConstraintNode::getCopyInEdges, py::return_value_policy::reference)
        .def("getCopyOutEdges", &ConstraintNode::getCopyOutEdges, py::return_value_policy::reference)
        .def("getGepInEdges", &ConstraintNode::getGepInEdges, py::return_value_policy::reference)
        .def("getGepOutEdges", &ConstraintNode::getGepOutEdges, py::return_value_policy::reference)
        .def("getLoadInEdges", &ConstraintNode::getLoadInEdges, py::return_value_policy::reference)
        .def("getLoadOutEdges", &ConstraintNode::getLoadOutEdges, py::return_value_policy::reference)
        .def("getStoreInEdges", &ConstraintNode::getStoreInEdges, py::return_value_policy::reference)
        .def("getStoreOutEdges", &ConstraintNode::getStoreOutEdges, py::return_value_policy::reference)
        .def("getAddrInEdges", &ConstraintNode::getAddrInEdges, py::return_value_policy::reference)
        .def("getAddrOutEdges", &ConstraintNode::getAddrOutEdges, py::return_value_policy::reference);
    // ConstraintGraph itself
    py::class_<ConstraintGraph>(m, "ConstraintGraph")
        .def("__iter__", [](ConstraintGraph* graph) {
            return py::make_iterator(graph->begin(), graph->end());
        }, py::keep_alive<0, 1>(), "Iterate over the ConstraintGraph nodes")
        .def("getNodes", [](ConstraintGraph& graph) {
            std::vector<ConstraintNode*> nodes;
            for (auto& node : graph) {
                nodes.push_back(node.second);
            }
            return nodes;
        }, py::return_value_policy::reference)
        .def("getGNode", [](ConstraintGraph& graph, NodeID id) -> ConstraintNode* {
            ConstraintNode* node = graph.getGNode(id);
            if (!node) {
                throw std::runtime_error("ConstraintsNode with given ID not found.");
            }
            return node;
        }, py::arg("id"), py::return_value_policy::reference)
        .def("getConstraintNode", [](ConstraintGraph& graph, NodeID id) -> ConstraintNode* {
            ConstraintNode* node = graph.getConstraintNode(id);
            if (!node) {
                throw std::runtime_error("ConstraintsNode with given ID not found.");
            }
            return node;
        }, py::arg("id"), py::return_value_policy::reference)
        .def("addCopyCGEdge", [](ConstraintGraph& graph, NodeID src, NodeID dst) -> CopyCGEdge* {
            std::cout << "Pybind Adding copy edge from " << src << " to " << dst << std::endl;
            return graph.addCopyCGEdge(src, dst);
        }, py::arg("src"), py::arg("dst"),
           py::return_value_policy::reference_internal)
        .def("getGepObjVar", [](ConstraintGraph& graph, NodeID id, const APOffset& ap) 
        { return graph.getGepObjVar(id, ap); }, py::arg("id"), py::arg("offset"), py::return_value_policy::reference)
        .def("dump", [](ConstraintGraph& graph, std::string file) {
            graph.dump(file);
        }, py::arg("file"));
}

