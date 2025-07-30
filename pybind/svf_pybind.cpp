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
#include "WPA/Andersen.h"
#include <pybind11/operators.h>
#include "Util/CommandLine.h"


namespace py = pybind11;
using namespace SVF;

class PySVF {
static SVFIR* currentSVFIR;
static SVFG* currentSVFG;
static CallGraph* currentCallGraph;
static ICFG* currentICFG;
static std::string lastAnalyzedModule;
static Andersen::CallGraphSCC* currentCallGraphSCC;

public:
    static void buildSVFModule(std::vector<std::string> options) {
        if (options.size() == 0) {
            std::cout << "No options provided, using default options -h" << std::endl;
            options.push_back("-h");
        }
        // Create an array of char* from the vector of strings
        std::vector<char*> arg_value;
        std::string empty = "";
        arg_value.push_back(&empty[0]);
        for (auto& option : options) {
            arg_value.push_back(&option[0]); // Get a pointer to the first character of the string
        }

        std::vector<std::string> moduleNameVec = OptionBase::parseOptions(
            arg_value.size(),
            arg_value.data(), // Pass the array of char*
            "PySVF",
            "[options] <input-bitcode...>"
        );
        // if options contains [svfg-with-ind-call, opt-svfg, write-svfg, read-svfg]
        bool buildSVFG = false;
        std::set<std::string> svfgOptions = {"-svfg"};
        if (std::any_of(options.begin(), options.end(), [&](const std::string& opt) {
            return svfgOptions.count(opt) > 0;
        })) {
            buildSVFG = true;
        }
        get_pag(moduleNameVec[0], buildSVFG);
    }

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
        Andersen::CallGraphSCC* callGraphScc = ander->getCallGraphSCC();
 	    callGraphScc->find();
        currentCallGraphSCC = callGraphScc;
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
    static Andersen::CallGraphSCC* get_current_call_graph_scc() {
        return currentCallGraphSCC;
    }
    static ICFG* get_current_icfg() {
        return currentICFG;
    }
    static std::string get_last_analyzed_module() {
        return lastAnalyzedModule;
    }

};

void bind_icfg_node(py::module& m);

void bind_icfg_edge(py::module& m);

void bind_icfg_graph(py::module& m);

void bind_svf_stmt(py::module& m);

// Bind SVFIR (PAG)
void bind_svf(py::module& m);
// Bind SVFVar
void bind_svf_var(py::module &m);


// Bind SVFType
void bind_svf_type(py::module& m);
// Add these bindings for CallGraph classes

void bind_callgraph_node(py::module& m);
void bind_callgraph_edge(py::module& m);
void bind_callgraph(py::module& m);

// Add this to svf_pybind.cpp
void bind_basic_block(py::module& m);

// Bind SVFGEdge classes
void bind_svfg_edge(py::module& m);

// Bind VFGNode classes
void bind_vfg_node(py::module& m);
// Bind SVFGNode classes
void bind_svfg_node(py::module& m);

// Bind VFG class
void bind_vfg(py::module& m);
// Bind SVFG class
void bind_svfg(py::module& m);

void bind_andersen_base(py::module& m);


void bind_points_to(py::module& m);

void bind_constraint_graph(py::module& m);

void bind_abstract_state(py::module& m);

SVFIR* PySVF::currentSVFIR = nullptr;
CallGraph* PySVF::currentCallGraph = nullptr;
SVFG* PySVF::currentSVFG = nullptr;
ICFG* PySVF::currentICFG = nullptr;
std::string PySVF::lastAnalyzedModule = "";
Andersen::CallGraphSCC* PySVF::currentCallGraphSCC = nullptr;

PYBIND11_MODULE(pysvf, m) {
    bind_svf(m);
    bind_icfg_node(m);
    bind_icfg_edge(m);
    bind_icfg_graph(m);
    bind_svf_stmt(m);
    bind_svf_var(m);
    bind_svf_type(m);
    m.def("buildSVFModule", &PySVF::buildSVFModule, py::arg("options"), "Build SVF module");
    m.def("getPAG", &PySVF::get_current_pag, py::return_value_policy::reference);
    m.def("releasePAG", &PySVF::release_pag, "Release SVFIR and LLVMModuleSet");
    m.def("getICFG", &PySVF::get_current_icfg, py::return_value_policy::reference, "Get the interprocedural control flow graph");
    m.def("getCallGraph", &PySVF::get_current_call_graph, py::return_value_policy::reference, "Get the call graph");
    m.def("getCallGraphSCC", &PySVF::get_current_call_graph_scc, py::return_value_policy::reference, "Get the call graph SCC");
    m.def("getSVFG", &PySVF::get_current_svfg, py::return_value_policy::reference, "Get the sparse value flow graph");
    m.def("getModuleName", &PySVF::get_last_analyzed_module, "Get the name of the last analyzed module");
    bind_callgraph_node(m);
    bind_callgraph_edge(m);
    bind_basic_block(m);
    bind_callgraph(m);
    bind_svfg_edge(m);
    bind_vfg_node(m);
    bind_svfg_node(m);
    bind_vfg(m);   
    bind_svfg(m);  
    bind_andersen_base(m);  
    bind_points_to(m);
    bind_constraint_graph(m);
    bind_abstract_state(m);

}
