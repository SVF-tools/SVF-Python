#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "SVF-LLVM/LLVMUtil.h"
#include "SVF-LLVM/SVFIRBuilder.h"
#include "Util/CommandLine.h"
#include "Util/Options.h"
#include "Graphs/ICFG.h"  // 需要 include ICFG 头文件

namespace py = pybind11;
using namespace SVF;

class PySVF {
public:
    static SVFIR* analyze(const std::string& bitcodePath) {
        std::vector<std::string> moduleNameVec = { bitcodePath };

        SVFModule* svfModule = LLVMModuleSet::buildSVFModule(moduleNameVec);
        SVFIRBuilder builder(svfModule);
        SVFIR* pag = builder.build();  // 构造 SVFIR

        return pag;  // 直接返回 SVFIR 供 Python 端使用
    }
};

// 绑定 ICFGNode
void bind_icfg(py::module& m) {
    py::class_<ICFGNode>(m, "ICFGNode")
        .def("to_string", [](const ICFGNode& node) {
            std::ostringstream oss;
            oss << node.toString() << "\n";
            return oss.str();
        });
}

// 绑定 ICFG
void bind_icfg_graph(py::module& m) {
    py::class_<ICFG>(m, "ICFG")
        .def("get_nodes", [](const ICFG& icfg) {
            std::vector<ICFGNode*> nodes;
            for (auto& node : icfg) {
                nodes.push_back(node.second);
            }
            return nodes;
        }, py::return_value_policy::reference);
}

// 绑定 SVFIR
void bind_svf(py::module& m) {
    py::class_<SVFIR>(m, "SVFIR")
        .def("get_icfg", [](SVFIR* pag) { return pag->getICFG(); }, py::return_value_policy::reference);

}

PYBIND11_MODULE(pysvf, m) {
    bind_svf(m);
    bind_icfg(m);
    bind_icfg_graph(m);
    m.def("analyze", &PySVF::analyze, py::return_value_policy::reference, "Analyze LLVM bitcode and return SVFIR");
}
