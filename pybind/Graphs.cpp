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
            .def("to_string", [](const ICFGNode* node) {
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
            .def("get_arg", &CallICFGNode::getArgument, py::arg("idx"),  py::return_value_policy::reference,
                 "Get the argument of the call")
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
            .def("to_string", [](const ICFGEdge* edge) {
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
            .def("get_fun_entry_icfg_node", &ICFG::getFunEntryICFGNode, py::return_value_policy::reference, py::arg("fun_obj"))

            .def("get_global_icfg_node", &ICFG::getGlobalICFGNode, py::return_value_policy::reference)
            //void dump(const std::string& file);
            .def("dump", [](ICFG &icfg, std::string file) {
                icfg.dump(file);
            });
}

void bind_callgraph_node(py::module& m) {
    py::class_<CallGraphNode>(m, "CallGraphNode", "Represents a node in the Call Graph")
        .def("to_string", [](const CallGraphNode* node) {
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
        .def("to_string", [](const CallGraphEdge* edge) {
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
        .def("get_gnode", [](CallGraph& callg, NodeID id) -> CallGraphNode* {
            CallGraphNode* node = callg.getGNode(id);
            if (!node) {
                throw std::runtime_error("CallGraphNode with given ID not found.");
            }
            return node;
        }, py::arg("id"), py::return_value_policy::reference, "Get a VFG node by ID")
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
        .def("to_string", [](const VFGEdge* edge) {
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
    py::class_<IntraDirSVFGEdge, DirectSVFGEdge, std::shared_ptr<IntraDirSVFGEdge>>(m, "IntraDirSVFGEdge", "Intraprocedural direct SVF edge");

    // CallDirSVFGEdge - Call direct SVF edge
    py::class_<CallDirSVFGEdge, DirectSVFGEdge, std::shared_ptr<CallDirSVFGEdge>>(m, "CallDirSVFGEdge", "Call direct SVF edge")
        .def("get_call_site_id", &CallDirSVFGEdge::getCallSiteId, "Get the call site ID");

    // RetDirSVFGEdge - Return direct SVF edge
    py::class_<RetDirSVFGEdge, DirectSVFGEdge, std::shared_ptr<RetDirSVFGEdge>>(m, "RetDirSVFGEdge", "Return direct SVF edge")
        .def("get_call_site_id", &RetDirSVFGEdge::getCallSiteId, "Get the call site ID");

    // IndirectSVFGEdge - Indirect value flow edge
    py::class_<IndirectSVFGEdge, VFGEdge, std::shared_ptr<IndirectSVFGEdge>>(m, "IndirectSVFGEdge", "Indirect SVF Graph Edge")
        .def("get_points_to", &IndirectSVFGEdge::getPointsTo, py::return_value_policy::reference, "Get the points-to set")
        .def("add_points_to", &IndirectSVFGEdge::addPointsTo, "Add to the points-to set")
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
    py::class_<IntraIndSVFGEdge, IndirectSVFGEdge, std::shared_ptr<IntraIndSVFGEdge>>(m, "IntraIndSVFGEdge", "Intraprocedural indirect SVF edge");

    // CallIndSVFGEdge - Call indirect SVF edge
    py::class_<CallIndSVFGEdge, IndirectSVFGEdge, std::shared_ptr<CallIndSVFGEdge>>(m, "CallIndSVFGEdge", "Call indirect SVF edge")
        .def("get_call_site_id", &CallIndSVFGEdge::getCallSiteId, "Get the call site ID");

    // RetIndSVFGEdge - Return indirect SVF edge
    py::class_<RetIndSVFGEdge, IndirectSVFGEdge, std::shared_ptr<RetIndSVFGEdge>>(m, "RetIndSVFGEdge", "Return indirect SVF edge")
        .def("get_call_site_id", &RetIndSVFGEdge::getCallSiteId, "Get the call site ID");

    // ThreadMHPIndSVFGEdge - Thread MHP indirect SVF edge
    py::class_<ThreadMHPIndSVFGEdge, IndirectSVFGEdge, std::shared_ptr<ThreadMHPIndSVFGEdge>>(m, "ThreadMHPIndSVFGEdge", "Thread MHP indirect SVF edge");
}


// Bind VFGNode classes
void bind_vfg_node(py::module& m) {
    // Base VFGNode class
    py::class_<VFGNode, std::shared_ptr<VFGNode>>(m, "VFGNode", "Base class for Value-Flow Graph nodes")
        .def("to_string", [](const VFGNode* node) {
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
        .def("get_def_svf_vars", &LoadVFGNode::getDefSVFVars, "Get defined SVF variables");

    // StoreVFGNode - Store statement VFG node
    py::class_<StoreVFGNode, StmtVFGNode, std::shared_ptr<StoreVFGNode>>(m, "StoreVFGNode", "Store VFG node")
        .def("get_def_svf_vars", &StoreVFGNode::getDefSVFVars, "Get defined SVF variables");

    // CopyVFGNode - Copy statement VFG node
    py::class_<CopyVFGNode, StmtVFGNode, std::shared_ptr<CopyVFGNode>>(m, "CopyVFGNode", "Copy VFG node")
        .def("get_def_svf_vars", &CopyVFGNode::getDefSVFVars, "Get defined SVF variables");

    // GepVFGNode - GetElementPtr VFG node
    py::class_<GepVFGNode, StmtVFGNode, std::shared_ptr<GepVFGNode>>(m, "GepVFGNode", "Gep VFG node")
        .def("get_def_svf_vars", &GepVFGNode::getDefSVFVars, "Get defined SVF variables");

    // AddrVFGNode - Address-of VFG node
    py::class_<AddrVFGNode, StmtVFGNode, std::shared_ptr<AddrVFGNode>>(m, "AddrVFGNode", "Address VFG node")
        .def("get_def_svf_vars", &AddrVFGNode::getDefSVFVars, "Get defined SVF variables");

    // PHIVFGNode - PHI VFG node
    py::class_<PHIVFGNode, VFGNode, std::shared_ptr<PHIVFGNode>>(m, "PHIVFGNode", "PHI VFG node")
        .def("get_def_svf_vars", &PHIVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("get_value", &PHIVFGNode::getValue, py::return_value_policy::reference, "Get result SVF variable")
        // Type checking
        .def("is_intra_phi_vfg_node", [](const PHIVFGNode* node) { return SVFUtil::isa<IntraPHIVFGNode>(node); }, "Check if this is an intra-procedural PHI VFG node")
        .def("is_inter_phi_vfg_node", [](const PHIVFGNode* node) { return SVFUtil::isa<InterPHIVFGNode>(node); }, "Check if this is an inter-procedural PHI VFG node")
        // Downcasting
        .def("as_intra_phi_vfg_node", [](PHIVFGNode* node) { return SVFUtil::dyn_cast<IntraPHIVFGNode>(node); }, py::return_value_policy::reference, "Downcast to IntraPHIVFGNode")
        .def("as_inter_phi_vfg_node", [](PHIVFGNode* node) { return SVFUtil::dyn_cast<InterPHIVFGNode>(node); }, py::return_value_policy::reference, "Downcast to InterPHIVFGNode");

    // IntraPHIVFGNode - Intra-procedural PHI VFG node
    py::class_<IntraPHIVFGNode, PHIVFGNode, std::shared_ptr<IntraPHIVFGNode>>(m, "IntraPHIVFGNode", "Intra-procedural PHI VFG node");

    // InterPHIVFGNode - Inter-procedural PHI VFG node
    py::class_<InterPHIVFGNode, PHIVFGNode, std::shared_ptr<InterPHIVFGNode>>(m, "InterPHIVFGNode", "Inter-procedural PHI VFG node");

    // ArgumentVFGNode - Base class for argument VFG nodes
    py::class_<ArgumentVFGNode, VFGNode, std::shared_ptr<ArgumentVFGNode>>(m, "ArgumentVFGNode", "Argument VFG node")
        .def("get_value", &ArgumentVFGNode::getValue, py::return_value_policy::reference, "Get parameter SVF variable")
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
        .def("get_def_svf_vars", &ActualParmVFGNode::getDefSVFVars, "Get defined SVF variables");

    // FormalParmVFGNode - Formal parameter VFG node
    py::class_<FormalParmVFGNode, ArgumentVFGNode, std::shared_ptr<FormalParmVFGNode>>(m, "FormalParmVFGNode", "Formal parameter VFG node")
        .def("get_def_svf_vars", &FormalParmVFGNode::getDefSVFVars, "Get defined SVF variables");

    // ActualRetVFGNode - Actual return VFG node
    py::class_<ActualRetVFGNode, ArgumentVFGNode, std::shared_ptr<ActualRetVFGNode>>(m, "ActualRetVFGNode", "Actual return VFG node")
        .def("get_def_svf_vars", &ActualRetVFGNode::getDefSVFVars, "Get defined SVF variables");

    // FormalRetVFGNode - Formal return VFG node
    py::class_<FormalRetVFGNode, ArgumentVFGNode, std::shared_ptr<FormalRetVFGNode>>(m, "FormalRetVFGNode", "Formal return VFG node")
        .def("get_def_svf_vars", &FormalRetVFGNode::getDefSVFVars, "Get defined SVF variables");

    // CmpVFGNode - Compare operation VFG node
    py::class_<CmpVFGNode, VFGNode, std::shared_ptr<CmpVFGNode>>(m, "CmpVFGNode", "Compare VFG node")
        .def("get_def_svf_vars", &CmpVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("get_value", &CmpVFGNode::getValue, py::return_value_policy::reference, "Get result SVF variable");

    // BinaryOPVFGNode - Binary operation VFG node
    py::class_<BinaryOPVFGNode, VFGNode, std::shared_ptr<BinaryOPVFGNode>>(m, "BinaryOPVFGNode", "Binary operation VFG node")
        .def("get_def_svf_vars", &BinaryOPVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("get_value", &BinaryOPVFGNode::getValue, py::return_value_policy::reference, "Get result SVF variable");

    // UnaryOPVFGNode - Unary operation VFG node
    py::class_<UnaryOPVFGNode, VFGNode, std::shared_ptr<UnaryOPVFGNode>>(m, "UnaryOPVFGNode", "Unary operation VFG node")
        .def("get_def_svf_vars", &UnaryOPVFGNode::getDefSVFVars, "Get defined SVF variables");

    // BranchVFGNode - Branch VFG node
    py::class_<BranchVFGNode, VFGNode, std::shared_ptr<BranchVFGNode>>(m, "BranchVFGNode", "Branch VFG node")
        .def("get_def_svf_vars", &BranchVFGNode::getDefSVFVars, "Get defined SVF variables");
}

// Bind SVFGNode classes
void bind_svfg_node(py::module& m) {
    // MRSVFGNode - Memory region VFGNode (for address-taken objects)
    py::class_<MRSVFGNode, VFGNode, std::shared_ptr<MRSVFGNode>>(m, "MRSVFGNode", "Memory region VFG node")
        .def("get_points_to", &MRSVFGNode::getPointsTo, py::return_value_policy::reference, "Get the points-to set of the memory region")
        .def("get_def_svf_vars", &MRSVFGNode::getDefSVFVars, "Get defined SVF variables")
        .def("to_string", [](const MRSVFGNode* node) {
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
        .def("get_fun_entry_node", &FormalINSVFGNode::getFunEntryNode, py::return_value_policy::reference, "Get the function entry ICFG node");
    
    // FormalOUTSVFGNode - SVFG Node for return mu node (address-taken variables)
    py::class_<FormalOUTSVFGNode, MRSVFGNode, std::shared_ptr<FormalOUTSVFGNode>>(m, "FormalOUTSVFGNode", "Formal-out SVFG node (return mu)")
        .def("get_mr_ver", &FormalOUTSVFGNode::getMRVer, py::return_value_policy::reference, "Get the memory region version")
        .def("get_fun_exit_node", &FormalOUTSVFGNode::getFunExitNode, py::return_value_policy::reference, "Get the function exit ICFG node");
    
    // ActualINSVFGNode - SVFG Node for callsite mu node (address-taken variables)
    py::class_<ActualINSVFGNode, MRSVFGNode, std::shared_ptr<ActualINSVFGNode>>(m, "ActualINSVFGNode", "Actual-in SVFG node (callsite mu)")
        .def("get_mr_ver", &ActualINSVFGNode::getMRVer, py::return_value_policy::reference, "Get the memory region version")
        .def("get_callsite", &ActualINSVFGNode::getCallSite, py::return_value_policy::reference, "Get the call site");
    
    // ActualOUTSVFGNode - SVFG Node for callsite chi node (address-taken variables)
    py::class_<ActualOUTSVFGNode, MRSVFGNode, std::shared_ptr<ActualOUTSVFGNode>>(m, "ActualOUTSVFGNode", "Actual-out SVFG node (callsite chi)")
        .def("get_mr_ver", &ActualOUTSVFGNode::getMRVer, py::return_value_policy::reference, "Get the memory region version")
        .def("get_callsite", &ActualOUTSVFGNode::getCallSite, py::return_value_policy::reference, "Get the call site");
    
    // MSSAPHISVFGNode - SVFG Node for memory SSA phi nodes
    py::class_<MSSAPHISVFGNode, MRSVFGNode, std::shared_ptr<MSSAPHISVFGNode>>(m, "MSSAPHISVFGNode", "Memory SSA PHI SVFG node")
        // Type checking
        .def("is_intra_mssaphi_svfg_node", [](const MSSAPHISVFGNode* node) { return SVFUtil::isa<IntraMSSAPHISVFGNode>(node); }, "Check if this is an intra-procedural MSSA phi SVFG node")
        .def("is_inter_mssaphi_svfg_node", [](const MSSAPHISVFGNode* node) { return SVFUtil::isa<InterMSSAPHISVFGNode>(node); }, "Check if this is an inter-procedural MSSA phi SVFG node")
        // Downcasting
        .def("as_intra_mssaphi_svfg_node", [](MSSAPHISVFGNode* node) { return SVFUtil::dyn_cast<IntraMSSAPHISVFGNode>(node); }, py::return_value_policy::reference, "Downcast to IntraMSSAPHISVFGNode")
        .def("as_inter_mssaphi_svfg_node", [](MSSAPHISVFGNode* node) { return SVFUtil::dyn_cast<InterMSSAPHISVFGNode>(node); }, py::return_value_policy::reference, "Downcast to InterMSSAPHISVFGNode");
    
    // IntraMSSAPHISVFGNode - Intra-procedural MSSA PHI node
    py::class_<IntraMSSAPHISVFGNode, MSSAPHISVFGNode, std::shared_ptr<IntraMSSAPHISVFGNode>>(m, "IntraMSSAPHISVFGNode", "Intra-procedural Memory SSA PHI SVFG node");
    
    // InterMSSAPHISVFGNode - Inter-procedural MSSA PHI node (formalIN/ActualOUT)
    py::class_<InterMSSAPHISVFGNode, MSSAPHISVFGNode, std::shared_ptr<InterMSSAPHISVFGNode>>(m, "InterMSSAPHISVFGNode", "Inter-procedural Memory SSA PHI SVFG node")
        .def("get_fun", &InterMSSAPHISVFGNode::getFun, py::return_value_policy::reference, "Get the function");
    
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

void bind_constraint_graph(py::module& m) {
    // ConstraintEdge (base class)
    py::class_<ConstraintEdge>(m, "ConstraintEdge", "Constraint edge")
        .def("get_src", &ConstraintEdge::getSrcNode, py::return_value_policy::reference)
        .def("get_dst", &ConstraintEdge::getDstNode, py::return_value_policy::reference)
        .def("get_src_id", &ConstraintEdge::getSrcID)
        .def("get_dst_id", &ConstraintEdge::getDstID)
        .def("as_addr_cg_edge", [](ConstraintEdge* edge) { return SVFUtil::dyn_cast<AddrCGEdge>(edge); }, py::return_value_policy::reference)
        .def("as_copy_cg_edge", [](ConstraintEdge* edge) { return SVFUtil::dyn_cast<CopyCGEdge>(edge); }, py::return_value_policy::reference)
        .def("as_store_cg_edge", [](ConstraintEdge* edge) { return SVFUtil::dyn_cast<StoreCGEdge>(edge); }, py::return_value_policy::reference)
        .def("as_load_cg_edge", [](ConstraintEdge* edge) { return SVFUtil::dyn_cast<LoadCGEdge>(edge); }, py::return_value_policy::reference)
        .def("as_gep_cg_edge", [](ConstraintEdge* edge) { return SVFUtil::dyn_cast<GepCGEdge>(edge); }, py::return_value_policy::reference)
        .def("as_normal_gep_cg_edge", [](ConstraintEdge* edge) { return SVFUtil::dyn_cast<NormalGepCGEdge>(edge); }, py::return_value_policy::reference)
        .def("as_variant_gep_cg_edge", [](ConstraintEdge* edge) { return SVFUtil::dyn_cast<VariantGepCGEdge>(edge); }, py::return_value_policy::reference);

    // Subclasses of ConstraintEdge
    py::class_<AddrCGEdge, ConstraintEdge>(m, "AddrCGEdge");
    py::class_<CopyCGEdge, ConstraintEdge>(m, "CopyCGEdge");
    py::class_<StoreCGEdge, ConstraintEdge>(m, "StoreCGEdge");
    py::class_<LoadCGEdge, ConstraintEdge>(m, "LoadCGEdge");
    // GepCGEdge is abstract; don't instantiate directly
    py::class_<GepCGEdge, ConstraintEdge>(m, "GepCGEdge")
        .def("as_normal_gep_cg_edge", [](GepCGEdge* edge) { return SVFUtil::dyn_cast<NormalGepCGEdge>(edge); }, py::return_value_policy::reference)
        .def("as_variant_gep_cg_edge", [](GepCGEdge* edge) { return SVFUtil::dyn_cast<VariantGepCGEdge>(edge); }, py::return_value_policy::reference);

    // NormalGepCGEdge
    py::class_<NormalGepCGEdge, GepCGEdge>(m, "NormalGepCGEdge")
        .def("get_access_path", &NormalGepCGEdge::getAccessPath, py::return_value_policy::reference)
        .def("get_constant_field_idx", &NormalGepCGEdge::getConstantFieldIdx);

    // VariantGepCGEdge
    py::class_<VariantGepCGEdge, GepCGEdge>(m, "VariantGepCGEdge");


    py::class_<ConstraintNode>(m, "ConstraintNode", "Constraint node")
        .def("to_string", &ConstraintNode::toString)
        .def("__str__", &ConstraintNode::toString)
        .def("__repr__", &ConstraintNode::toString)
        .def("get_in_edges", &ConstraintNode::getInEdges, py::return_value_policy::reference)
        .def("get_out_edges", &ConstraintNode::getOutEdges, py::return_value_policy::reference)
        .def("get_direct_in_edges", &ConstraintNode::getDirectInEdges, py::return_value_policy::reference)
        .def("get_direct_out_edges", &ConstraintNode::getDirectOutEdges, py::return_value_policy::reference)
        .def("get_copy_in_edges", &ConstraintNode::getCopyInEdges, py::return_value_policy::reference)
        .def("get_copy_out_edges", &ConstraintNode::getCopyOutEdges, py::return_value_policy::reference)
        .def("get_gep_in_edges", &ConstraintNode::getGepInEdges, py::return_value_policy::reference)
        .def("get_gep_out_edges", &ConstraintNode::getGepOutEdges, py::return_value_policy::reference)
        .def("get_load_in_edges", &ConstraintNode::getLoadInEdges, py::return_value_policy::reference)
        .def("get_load_out_edges", &ConstraintNode::getLoadOutEdges, py::return_value_policy::reference)
        .def("get_store_in_edges", &ConstraintNode::getStoreInEdges, py::return_value_policy::reference)
        .def("get_store_out_edges", &ConstraintNode::getStoreOutEdges, py::return_value_policy::reference)
        .def("get_addr_in_edges", &ConstraintNode::getAddrInEdges, py::return_value_policy::reference)
        .def("get_addr_out_edges", &ConstraintNode::getAddrOutEdges, py::return_value_policy::reference);
    // ConstraintGraph itself
    py::class_<ConstraintGraph>(m, "ConstraintGraph")
        .def("get_nodes", [](ConstraintGraph& graph) {
            std::vector<ConstraintNode*> nodes;
            for (auto& node : graph) {
                nodes.push_back(node.second);
            }
            return nodes;
        }, py::return_value_policy::reference)
        .def("get_gnode", [](ConstraintGraph& graph, NodeID id) -> ConstraintNode* {
            ConstraintNode* node = graph.getGNode(id);
            if (!node) {
                throw std::runtime_error("ConstraintsNode with given ID not found.");
            }
            return node;
        }, py::arg("id"), py::return_value_policy::reference)
        .def("get_constraint_node", [](ConstraintGraph& graph, NodeID id) -> ConstraintNode* {
            ConstraintNode* node = graph.getConstraintNode(id);
            if (!node) {
                throw std::runtime_error("ConstraintsNode with given ID not found.");
            }
            return node;
        }, py::arg("id"), py::return_value_policy::reference)
        .def("add_copy_edge", [](ConstraintGraph& graph, NodeID src, NodeID dst) -> CopyCGEdge* {
            std::cout << "Pybind Adding copy edge from " << src << " to " << dst << std::endl;
            return graph.addCopyCGEdge(src, dst);
        }, py::arg("src"), py::arg("dst"),
           py::return_value_policy::reference_internal)
        .def("get_gep_obj_var", [](ConstraintGraph& graph, NodeID id, const APOffset& ap) 
        { return graph.getGepObjVar(id, ap); }, py::arg("id"), py::arg("offset"), py::return_value_policy::reference);
}

