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

void bind_abstract_state(py::module& m) {

    py::class_<BoundedInt>(m, "BoundedInt")
        .def(py::init([](int64_t val) {
            return new BoundedInt(val);
        }), py::arg("val"))
        .def(py::init([](int64_t val, bool isInf) {
            return new BoundedInt(val, isInf);
        }), py::arg("val"), py::arg("isInf"))

        .def("getNumeral", &BoundedInt::getNumeral)
        .def_property_readonly("is_infinity", &BoundedInt::is_infinity)
        .def_property_readonly("is_plus_infinity", &BoundedInt::is_plus_infinity)
        .def_property_readonly("is_minus_infinity", &BoundedInt::is_minus_infinity)
        .def_property_readonly("is_zero", &BoundedInt::is_zero)
        .def("__str__", [](const BoundedInt& self) { return self.to_string(); })
        .def("__repr__", [](const BoundedInt& self) {
            return "<BoundedInt " + self.to_string() + ">";
        })
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * py::self)
        .def(py::self / py::self)
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self < py::self)
        .def(py::self <= py::self)
        .def(py::self > py::self)
        .def(py::self >= py::self)
        .def("__int__", [](const BoundedInt& self) {
            return static_cast<int64_t>(self.getIntNumeral());
        })
        .def("__neg__", [](const BoundedInt& self) { return -self; })
        .def("__le__", [](const BoundedInt& self, int64_t other) {
            return self <= BoundedInt(other);
        })
        .def("__ge__", [](const BoundedInt& self, int64_t other) {
            return self >= BoundedInt(other);
        })
        .def("__lt__", [](const BoundedInt& self, int64_t other) {
            return self < BoundedInt(other);
        })
        .def("__gt__", [](const BoundedInt& self, int64_t other) {
            return self > BoundedInt(other);
        })
        .def("__eq__", [](const BoundedInt& self, int64_t other) {
            return self == BoundedInt(other);
        })
        .def("__ne__", [](const BoundedInt& self, int64_t other) {
            return self != BoundedInt(other);
        })
        .def_property_readonly("is_plus_infinity", &BoundedInt::is_plus_infinity)
        .def_static("plus_infinity", &BoundedInt::plus_infinity)
        .def_property_readonly("is_minus_infinity", &BoundedInt::is_minus_infinity)
        .def_static("minus_infinity", &BoundedInt::minus_infinity);


    py::class_<IntervalValue, std::unique_ptr<IntervalValue>>(m, "IntervalValue", "Interval Value")
        // Constructors
        .def(py::init([]() {
            return new IntervalValue();
        }))
        .def(py::init([](int64_t val) {
            return new IntervalValue(static_cast<s64_t>(val));
        }), py::arg("val"))

        .def(py::init([](py::object lb, py::object ub) {
            auto to_bounded_int = [](py::object obj) -> BoundedInt {
                if (py::isinstance<BoundedInt>(obj)) {
                    return obj.cast<BoundedInt>();
                } else if (py::isinstance<py::int_>(obj)) {
                    return BoundedInt(obj.cast<int64_t>());
                } else {
                    throw std::invalid_argument("Expected int or BoundedInt");
                }
            };

            return new IntervalValue(to_bounded_int(lb), to_bounded_int(ub));
        }), py::arg("lb"), py::arg("ub"))

        // Equality
        .def("__eq__", [](const IntervalValue &self, const IntervalValue &other) {
            return self.equals(other);
        })
        .def("__ne__", [](const IntervalValue &self, const IntervalValue &other) {
            return !self.equals(other);
        })

        .def("clone", [](const IntervalValue &self) {
            return std::make_unique<IntervalValue>(self);
        }, py::return_value_policy::move)

        // Arithmetic operators
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * py::self)
        .def(py::self / py::self)
        .def(py::self % py::self)

        // Bitwise operators
        .def(py::self << py::self)
        .def(py::self >> py::self)
        .def(py::self & py::self)
        .def(py::self | py::self)
        .def(py::self ^ py::self)

        // Comparison operators (return IntervalValue)
        .def("__lt__", [](const IntervalValue &self, const IntervalValue &other) {
            return self < other;
        })
        .def("__le__", [](const IntervalValue &self, const IntervalValue &other) {
            return self <= other;
        })
        .def("__gt__", [](const IntervalValue &self, const IntervalValue &other) {
            return self > other;
        })
        .def("__ge__", [](const IntervalValue &self, const IntervalValue &other) {
            return self >= other;
        })

        // Methods
        .def("isBottom", &IntervalValue::isBottom)
        .def("isTop", &IntervalValue::isTop)
        .def("is_numeral", &IntervalValue::is_numeral)
        .def("is_zero", &IntervalValue::is_zero)
        .def("is_real", &IntervalValue::is_real)
        .def("is_int", &IntervalValue::is_int)
        .def("equals", &IntervalValue::equals, py::arg("other"))
        .def("eq_interval", [](const IntervalValue &self, const IntervalValue &other) {
                return self.operator==(other);
            }, py::arg("other"))
        .def("ne_interval", [](const IntervalValue &self, const IntervalValue &other) {
                return self.operator!=(other);
            }, py::arg("other"))
        .def("getNumeral", &IntervalValue::getNumeral)
        .def("getIntNumeral", &IntervalValue::getIntNumeral)
        .def("getRealNumeral", &IntervalValue::getRealNumeral)
        .def("containedWithin", &IntervalValue::containedWithin)
        .def("contain", &IntervalValue::contain)
        .def("leq", &IntervalValue::leq)
        .def("geq", &IntervalValue::geq)
        .def("set_to_bottom", &IntervalValue::set_to_bottom)
        .def("set_to_top", &IntervalValue::set_to_top)
        .def("toString", &IntervalValue::toString)
        .def("lb", &IntervalValue::lb)
        .def("ub", &IntervalValue::ub)

        .def("join_with", &IntervalValue::join_with, py::arg("other"))
        .def("meet_with", &IntervalValue::meet_with, py::arg("other"))
        .def("widen_with", &IntervalValue::widen_with, py::arg("other"))
        .def("narrow_with", &IntervalValue::narrow_with, py::arg("other"))

        .def("clone",[](const IntervalValue &self) {
            return std::make_unique<IntervalValue>(self);
        }, py::return_value_policy::move)
        // Class methods for top/bottom
        .def_static("top", &IntervalValue::top)
        .def_static("bottom", &IntervalValue::bottom)

        // __repr__ for Python string representation
        .def("__repr__", [](const IntervalValue &iv) {
            return iv.toString();
        });

    py::class_<AddressValue, std::unique_ptr<AddressValue>>(m, "AddressValue", "Address Value Set")
        .def(py::init<u32_t>(), py::arg("val"))
        .def(py::init<const Set<u32_t>&>(), py::arg("vals"))

        .def("__eq__", [](const AddressValue &self, const AddressValue &other) {
            return self.equals(other);
        })
        .def("__ne__", [](const AddressValue &self, const AddressValue &other) {
            return !self.equals(other);
        })

        .def("__iter__", [](AddressValue &self) {
            return py::make_iterator(self.begin(), self.end());
        }, py::keep_alive<0, 1>())

        .def("__len__", &AddressValue::size)
        .def("__contains__", [](const AddressValue &self, u32_t val) {
            return self.contains(val);
        }, py::arg("val"))

        .def("insert", [](AddressValue &self, u32_t val) {
            return self.insert(val).second;
        }, py::arg("addr"))

        .def("clone", [](const AddressValue &self) {
            return std::make_unique<AddressValue>(self);
        }, py::return_value_policy::move)

        .def("contains", &AddressValue::contains, py::arg("addr"))
        .def("empty", &AddressValue::empty)
        .def("size", &AddressValue::size)
        .def("isBottom", &AddressValue::isBottom)

        .def("join_with", &AddressValue::join_with)
        .def("meet_with", &AddressValue::meet_with)
        .def("hasIntersect", &AddressValue::hasIntersect)

        .def("getVals", &AddressValue::getVals, py::return_value_policy::reference_internal)
        .def("setVals", &AddressValue::setVals, py::arg("vals"))

        .def_static("getVirtualMemAddress", &AddressValue::getVirtualMemAddress, py::arg("idx"))
        .def_static("isVirtualMemAddress", &AddressValue::isVirtualMemAddress, py::arg("val"))

        .def("__str__", [](const AddressValue &av) {
            return av.toString();
        })
        .def("__repr__", [](const AddressValue &av) {
            return "<AddressValue: " + av.toString() + ">";
        });


    py::class_<AbstractValue, std::unique_ptr<AbstractValue>>(m, "AbstractValue")
        .def(py::init<>())
        .def(py::init<const IntervalValue&>())
        .def(py::init<const AddressValue&>())
        .def("isInterval", &AbstractValue::isInterval)
        .def("isAddr", &AbstractValue::isAddr)
        .def("getInterval", py::overload_cast<>(&AbstractValue::getInterval, py::const_))
        .def("getAddrs", py::overload_cast<>(&AbstractValue::getAddrs, py::const_))
        .def("getInterval", py::overload_cast<>(&AbstractValue::getInterval), py::return_value_policy::reference)
        .def("getAddrs", py::overload_cast<>(&AbstractValue::getAddrs), py::return_value_policy::reference)
        .def("equals", &AbstractValue::equals)
            // === join_with overloads ===
        .def("join_with", [](AbstractValue &self, const AbstractValue &other) {
            self.join_with(other);
        })
        .def("join_with", [](AbstractValue &self, const IntervalValue &ival) {
            self.join_with(AbstractValue(ival));
        })
        .def("join_with", [](AbstractValue &self, const AddressValue &aval) {
            self.join_with(AbstractValue(aval));
        })

        // === meet_with overloads ===
        .def("meet_with", [](AbstractValue &self, const AbstractValue &other) {
            self.meet_with(other);
        })
        .def("meet_with", [](AbstractValue &self, const IntervalValue &ival) {
            self.meet_with(AbstractValue(ival));
        })
        .def("meet_with", [](AbstractValue &self, const AddressValue &aval) {
            self.meet_with(AbstractValue(aval));
        })

        // === widen_with overloads ===
        .def("widen_with", [](AbstractValue &self, const AbstractValue &other) {
            self.widen_with(other);
        })
        .def("widen_with", [](AbstractValue &self, const IntervalValue &ival) {
            self.widen_with(AbstractValue(ival));
        })
        .def("widen_with", [](AbstractValue &self, const AddressValue &aval) {
            self.widen_with(AbstractValue(aval));
        })

        // === narrow_with overloads ===
        .def("narrow_with", [](AbstractValue &self, const AbstractValue &other) {
            self.narrow_with(other);
        })
        .def("narrow_with", [](AbstractValue &self, const IntervalValue &ival) {
            self.narrow_with(AbstractValue(ival));
        })
        .def("narrow_with", [](AbstractValue &self, const AddressValue &aval) {
            self.narrow_with(AbstractValue(aval));
        })
		.def("clone", [](const AbstractValue &self) {
            return std::make_unique<AbstractValue>(self);  // clone
        }, py::return_value_policy::move)
        .def("__eq__", [](const AbstractValue& a, const AbstractValue& b) {
            return a.equals(b);
        })
        .def("__str__", &AbstractValue::toString);



    py::class_<AbstractState, std::unique_ptr<AbstractState>>(m, "AbstractState")
        // Constructor
        .def(py::init<>())

        // Getter & Setter for variables via get_var/set_var
        .def("getVar", [](AbstractState& self, u32_t varId) -> AbstractValue& {
            return self[varId];
        }, py::arg("varId"), py::return_value_policy::reference)

        .def("setVar", [](AbstractState& self, u32_t varId, const AbstractValue& val) {
            self[varId] = val;
        }, py::arg("varId"), py::arg("val"))

        // Store value to memory address
        .def("store", &AbstractState::store,
             py::arg("addr"), py::arg("val"))

        // Load value from memory address
        .def("load", &AbstractState::load,
             py::arg("addr"), py::return_value_policy::reference)

        .def("storeValue", &AbstractState::storeValue,
             py::arg("varId"), py::arg("val"))

        .def("loadValue", &AbstractState::loadValue,
            py::arg("varId"), py::return_value_policy::reference)

        // Equality comparison
        .def("equals", &AbstractState::equals,
             py::arg("other"))

        .def("__eq__", [](AbstractState& self, AbstractState& other) {
            return self.operator==(other);
        }, py::arg("other"))

        .def("__ne__", [](AbstractState& self, AbstractState& other) {
            return self.operator!=(other);
        }, py::arg("other"))

        // Abstract operations
        .def("joinWith", &AbstractState::joinWith, py::arg("other"))
        .def("meetWith", &AbstractState::meetWith, py::arg("other"))
        .def("widening", &AbstractState::widening, py::arg("other"))
        .def("narrowing", &AbstractState::narrowing, py::arg("other"))
        .def("getIDFromAddr", &AbstractState::getIDFromAddr, py::arg("addr"))

        // Static utilities for address handling
        .def_static("isVirtualMemAddress", &AbstractState::isVirtualMemAddress, py::arg("val"))
        .def_static("getVirtualMemAddress", &AbstractState::getVirtualMemAddress, py::arg("idx"))
        .def_static("isNullMem", &AbstractState::isNullMem, py::arg("addr"))
        .def_static("isInvalidMem", &AbstractState::isInvalidMem, py::arg("addr"))

        // State management
        .def("clear", &AbstractState::clear)
        .def("printAbstractState", &AbstractState::printAbstractState)
        .def("__str__", &AbstractState::toString)

        // Pythonic operator[]
        .def("__getitem__", [](AbstractState& self, u32_t varId) -> AbstractValue& {
            return self[varId];
        }, py::arg("varId"), py::return_value_policy::reference)

        .def("__setitem__", [](AbstractState& self, u32_t varId, py::object val) {
            if (py::isinstance<AbstractValue>(val)) {
                self[varId] = val.cast<AbstractValue>();
            }
            else if (py::isinstance<IntervalValue>(val)) {
                self[varId] = AbstractValue(val.cast<IntervalValue>());
            }
            else if (py::isinstance<AddressValue>(val)) {
                self[varId] = AbstractValue(val.cast<AddressValue>());
            }
            else if (py::isinstance<py::int_>(val)) {
                self[varId] = AbstractValue(IntervalValue(val.cast<s64_t>()));
            }
            else {
                throw std::invalid_argument("Unsupported type for AbstractState assignment.");
            }
        })

        // Access to internal maps
        .def("getVarToVal", &AbstractState::getVarToVal, py::return_value_policy::reference)
        .def("getLocToVal", &AbstractState::getLocToVal, py::return_value_policy::reference)
        .def("printAbstractState", &AbstractState::printAbstractState)
        .def("clone", [](const AbstractState &self) {
            return std::make_unique<AbstractState>(self);  // clone
         }, py::return_value_policy::move)
        .def("bottom", &AbstractState::bottom)
        .def("top", &AbstractState::top)
        .def("getGepObjAddrs", &AbstractState::getGepObjAddrs, py::arg("ptr"), py::arg("offset"))
        .def("getElementIndex", &AbstractState::getElementIndex, py::arg("gep"))
        .def("getByteOffset", &AbstractState::getByteOffset, py::arg("gep"))
        .def("loadValue", &AbstractState::loadValue, py::arg("var_id"))
        .def("storeValue", &AbstractState::storeValue, py::arg("var_id"), py::arg("val"))
        .def("getPointeeElement", &AbstractState::getPointeeElement, py::arg("var_id"), py::return_value_policy::reference)
        .def("inVarToValTable", &AbstractState::inVarToValTable, py::arg("var_id"))
        .def("inVarToAddrsTable", &AbstractState::inVarToAddrsTable, py::arg("var_id"))
        .def("getGepObjAddrs", &AbstractState::getGepObjAddrs, py::arg("var_id"), py::arg("offset"))
        .def_static("isCmpBranchFeasible", [](SVFIR* svfir, const CmpStmt* cmpStmt, s64_t succ, AbstractState& as) {
            Map<s32_t, s32_t> _reverse_predicate = {
                {CmpStmt::Predicate::FCMP_OEQ, CmpStmt::Predicate::FCMP_ONE}, // == -> !=
                {CmpStmt::Predicate::FCMP_UEQ, CmpStmt::Predicate::FCMP_UNE}, // == -> !=
                {CmpStmt::Predicate::FCMP_OGT, CmpStmt::Predicate::FCMP_OLE}, // > -> <=
                {CmpStmt::Predicate::FCMP_OGE, CmpStmt::Predicate::FCMP_OLT}, // >= -> <
                {CmpStmt::Predicate::FCMP_OLT, CmpStmt::Predicate::FCMP_OGE}, // < -> >=
                {CmpStmt::Predicate::FCMP_OLE, CmpStmt::Predicate::FCMP_OGT}, // <= -> >
                {CmpStmt::Predicate::FCMP_ONE, CmpStmt::Predicate::FCMP_OEQ}, // != -> ==
                {CmpStmt::Predicate::FCMP_UNE, CmpStmt::Predicate::FCMP_UEQ}, // != -> ==
                {CmpStmt::Predicate::ICMP_EQ, CmpStmt::Predicate::ICMP_NE}, // == -> !=
                {CmpStmt::Predicate::ICMP_NE, CmpStmt::Predicate::ICMP_EQ}, // != -> ==
                {CmpStmt::Predicate::ICMP_UGT, CmpStmt::Predicate::ICMP_ULE}, // > -> <=
                {CmpStmt::Predicate::ICMP_ULT, CmpStmt::Predicate::ICMP_UGE}, // < -> >=
                {CmpStmt::Predicate::ICMP_UGE, CmpStmt::Predicate::ICMP_ULT}, // >= -> <
                {CmpStmt::Predicate::ICMP_SGT, CmpStmt::Predicate::ICMP_SLE}, // > -> <=
                {CmpStmt::Predicate::ICMP_SLT, CmpStmt::Predicate::ICMP_SGE}, // < -> >=
                {CmpStmt::Predicate::ICMP_SGE, CmpStmt::Predicate::ICMP_SLT}, // >= -> <
            };

            Map<s32_t, s32_t> _switch_lhsrhs_predicate = {
                {CmpStmt::Predicate::FCMP_OEQ, CmpStmt::Predicate::FCMP_OEQ}, // == -> ==
                {CmpStmt::Predicate::FCMP_UEQ, CmpStmt::Predicate::FCMP_UEQ}, // == -> ==
                {CmpStmt::Predicate::FCMP_OGT, CmpStmt::Predicate::FCMP_OLT}, // > -> <
                {CmpStmt::Predicate::FCMP_OGE, CmpStmt::Predicate::FCMP_OLE}, // >= -> <=
                {CmpStmt::Predicate::FCMP_OLT, CmpStmt::Predicate::FCMP_OGT}, // < -> >
                {CmpStmt::Predicate::FCMP_OLE, CmpStmt::Predicate::FCMP_OGE}, // <= -> >=
                {CmpStmt::Predicate::FCMP_ONE, CmpStmt::Predicate::FCMP_ONE}, // != -> !=
                {CmpStmt::Predicate::FCMP_UNE, CmpStmt::Predicate::FCMP_UNE}, // != -> !=
                {CmpStmt::Predicate::ICMP_EQ, CmpStmt::Predicate::ICMP_EQ}, // == -> ==
                {CmpStmt::Predicate::ICMP_NE, CmpStmt::Predicate::ICMP_NE}, // != -> !=
                {CmpStmt::Predicate::ICMP_UGT, CmpStmt::Predicate::ICMP_ULT}, // > -> <
                {CmpStmt::Predicate::ICMP_ULT, CmpStmt::Predicate::ICMP_UGT}, // < -> >
                {CmpStmt::Predicate::ICMP_UGE, CmpStmt::Predicate::ICMP_ULE}, // >= -> <=
                {CmpStmt::Predicate::ICMP_SGT, CmpStmt::Predicate::ICMP_SLT}, // > -> <
                {CmpStmt::Predicate::ICMP_SLT, CmpStmt::Predicate::ICMP_SGT}, // < -> >
                {CmpStmt::Predicate::ICMP_SGE, CmpStmt::Predicate::ICMP_SLE}, // >= -> <=
            };

            AbstractState new_es = as;
            // get cmp stmt's op0, op1, and predicate
            NodeID op0 = cmpStmt->getOpVarID(0);
            NodeID op1 = cmpStmt->getOpVarID(1);
            NodeID res_id = cmpStmt->getResID();
            s32_t predicate = cmpStmt->getPredicate();

            // if op0 or op1 is undefined, return;
            // skip address compare
            if (new_es.inVarToAddrsTable(op0) || new_es.inVarToAddrsTable(op1)) {
                as = new_es;
                return true;
            }
            // get '%1 = load i32 s', and load inst may not exist
            auto getLoadOp = [](SVFVar* opVar) -> const LoadStmt* {
                if (!opVar->getInEdges().empty()) {
                    SVFStmt* loadVar0InStmt = *opVar->getInEdges().begin();
                    if (const LoadStmt* loadStmt = SVFUtil::dyn_cast<LoadStmt>(loadVar0InStmt)) {
                        return loadStmt;
                    }
                    else if (const CopyStmt* copyStmt = SVFUtil::dyn_cast<CopyStmt>(loadVar0InStmt)) {
                        if (!copyStmt->getRHSVar()->getInEdges().empty()) {
                            SVFStmt* loadVar0InStmt2 = *opVar->getInEdges().begin();
                            if (const LoadStmt* loadStmt = SVFUtil::dyn_cast<LoadStmt>(loadVar0InStmt2)) {
                                return loadStmt;
                            }
                        }
                    }
                }
                return nullptr;
            };
            const LoadStmt* load_op0 = getLoadOp(svfir->getGNode(op0));
            const LoadStmt* load_op1 = getLoadOp(svfir->getGNode(op1));

            // for const X const, we may get concrete resVal instantly
            // for var X const, we may get [0,1] if the intersection of var and const is not empty set
            IntervalValue resVal = new_es[res_id].getInterval();
            resVal.meet_with(IntervalValue((s64_t)succ, succ));
            // If Var X const generates bottom value, it means this branch path is not feasible.
            if (resVal.isBottom()) {
                return false;
            }

            bool b0 = new_es[op0].getInterval().is_numeral();
            bool b1 = new_es[op1].getInterval().is_numeral();

            // if const X var, we should reverse op0 and op1.
            if (b0 && !b1) {
                std::swap(op0, op1);
                std::swap(load_op0, load_op1);
                predicate = _switch_lhsrhs_predicate[predicate];
            }
            else {
                // if var X var, we cannot preset the branch condition to infer the intervals of var0,var1
                if (!b0 && !b1) {
                    as = new_es;
                    return true;
                }
                // if const X const, we can instantly get the resVal
                else if (b0 && b1) {
                    as = new_es;
                    return true;
                }
            }
            // if cmp is 'var X const == false', we should reverse predicate 'var X' const == true'
            // X' is reverse predicate of X
            if (succ == 0) {
                predicate = _reverse_predicate[predicate];
            }
            else {
            }
            // change interval range according to the compare predicate
            AddressValue addrs;
            if (load_op0 && new_es.inVarToAddrsTable(load_op0->getRHSVarID()))
                addrs = new_es[load_op0->getRHSVarID()].getAddrs();

            IntervalValue &lhs = new_es[op0].getInterval(), &rhs = new_es[op1].getInterval();
            switch (predicate) {
            case CmpStmt::Predicate::ICMP_EQ:
            case CmpStmt::Predicate::FCMP_OEQ:
            case CmpStmt::Predicate::FCMP_UEQ: {
                // Var == Const, so [var.lb, var.ub].meet_with(const)
                lhs.meet_with(rhs);
                break;
            }
            case CmpStmt::Predicate::ICMP_NE:
            case CmpStmt::Predicate::FCMP_ONE:
            case CmpStmt::Predicate::FCMP_UNE:
                // Compliment set
                break;
            case CmpStmt::Predicate::ICMP_UGT:
            case CmpStmt::Predicate::ICMP_SGT:
            case CmpStmt::Predicate::FCMP_OGT:
            case CmpStmt::Predicate::FCMP_UGT:
                // Var > Const, so [var.lb, var.ub].meet_with([Const+1, +INF])
                lhs.meet_with(IntervalValue(rhs.lb() + 1, IntervalValue::plus_infinity()));
                break;
            case CmpStmt::Predicate::ICMP_UGE:
            case CmpStmt::Predicate::ICMP_SGE:
            case CmpStmt::Predicate::FCMP_OGE:
            case CmpStmt::Predicate::FCMP_UGE: {
                // Var >= Const, so [var.lb, var.ub].meet_with([Const, +INF])
                lhs.meet_with(IntervalValue(rhs.lb(), IntervalValue::plus_infinity()));
                break;
            }
            case CmpStmt::Predicate::ICMP_ULT:
            case CmpStmt::Predicate::ICMP_SLT:
            case CmpStmt::Predicate::FCMP_OLT:
            case CmpStmt::Predicate::FCMP_ULT: {
                // Var < Const, so [var.lb, var.ub].meet_with([-INF, const.ub-1])
                lhs.meet_with(IntervalValue(IntervalValue::minus_infinity(), rhs.ub() - 1));
                break;
            }
            case CmpStmt::Predicate::ICMP_ULE:
            case CmpStmt::Predicate::ICMP_SLE:
            case CmpStmt::Predicate::FCMP_OLE:
            case CmpStmt::Predicate::FCMP_ULE: {
                // Var <= Const, so [var.lb, var.ub].meet_with([-INF, const.ub])
                lhs.meet_with(IntervalValue(IntervalValue::minus_infinity(), rhs.ub()));
                break;
            }
            case CmpStmt::Predicate::FCMP_FALSE: break;
            case CmpStmt::Predicate::FCMP_TRUE: break;
            default: assert(false && "implement this part"); abort();
            }

            for (const auto& addr : addrs) {
                NodeID objId = as.getIDFromAddr(addr);
                if (new_es.inAddrToValTable(objId)) {
                    switch (predicate) {
                    case CmpStmt::Predicate::ICMP_EQ:
                    case CmpStmt::Predicate::FCMP_OEQ:
                    case CmpStmt::Predicate::FCMP_UEQ: {
                        new_es.load(addr).meet_with(rhs);
                        break;
                    }
                    case CmpStmt::Predicate::ICMP_NE:
                    case CmpStmt::Predicate::FCMP_ONE:
                    case CmpStmt::Predicate::FCMP_UNE:
                        // Compliment set
                        break;
                    case CmpStmt::Predicate::ICMP_UGT:
                    case CmpStmt::Predicate::ICMP_SGT:
                    case CmpStmt::Predicate::FCMP_OGT:
                    case CmpStmt::Predicate::FCMP_UGT:
                        // Var > Const, so [var.lb, var.ub].meet_with([Const+1, +INF])
                        new_es.load(addr).meet_with(IntervalValue(rhs.lb() + 1, IntervalValue::plus_infinity()));
                        break;
                    case CmpStmt::Predicate::ICMP_UGE:
                    case CmpStmt::Predicate::ICMP_SGE:
                    case CmpStmt::Predicate::FCMP_OGE:
                    case CmpStmt::Predicate::FCMP_UGE: {
                        // Var >= Const, so [var.lb, var.ub].meet_with([Const, +INF])
                        new_es.load(addr).meet_with(IntervalValue(rhs.lb(), IntervalValue::plus_infinity()));
                        break;
                    }
                    case CmpStmt::Predicate::ICMP_ULT:
                    case CmpStmt::Predicate::ICMP_SLT:
                    case CmpStmt::Predicate::FCMP_OLT:
                    case CmpStmt::Predicate::FCMP_ULT: {
                        // Var < Const, so [var.lb, var.ub].meet_with([-INF, const.ub-1])
                        new_es.load(addr).meet_with(IntervalValue(IntervalValue::minus_infinity(), rhs.ub() - 1));
                        break;
                    }
                    case CmpStmt::Predicate::ICMP_ULE:
                    case CmpStmt::Predicate::ICMP_SLE:
                    case CmpStmt::Predicate::FCMP_OLE:
                    case CmpStmt::Predicate::FCMP_ULE: {
                        // Var <= Const, so [var.lb, var.ub].meet_with([-INF, const.ub])
                        new_es.load(addr).meet_with(IntervalValue(IntervalValue::minus_infinity(), rhs.ub()));
                        break;
                    }
                    case CmpStmt::Predicate::FCMP_FALSE: break;
                    case CmpStmt::Predicate::FCMP_TRUE: break;
                    default: assert(false && "implement this part"); abort();
                    }
                }
            }

            as = new_es;
            return true;
        }, py::arg("pag"), py::arg("cmpStmt"), py::arg("succ"), py::arg("as"))
        .def_static("isSwitchBranchFeasible", [](SVFIR* svfir, const SVFVar* var, s64_t succ, AbstractState& as) {
            Map<s32_t, s32_t> _reverse_predicate = {
                {CmpStmt::Predicate::FCMP_OEQ, CmpStmt::Predicate::FCMP_ONE}, // == -> !=
                {CmpStmt::Predicate::FCMP_UEQ, CmpStmt::Predicate::FCMP_UNE}, // == -> !=
                {CmpStmt::Predicate::FCMP_OGT, CmpStmt::Predicate::FCMP_OLE}, // > -> <=
                {CmpStmt::Predicate::FCMP_OGE, CmpStmt::Predicate::FCMP_OLT}, // >= -> <
                {CmpStmt::Predicate::FCMP_OLT, CmpStmt::Predicate::FCMP_OGE}, // < -> >=
                {CmpStmt::Predicate::FCMP_OLE, CmpStmt::Predicate::FCMP_OGT}, // <= -> >
                {CmpStmt::Predicate::FCMP_ONE, CmpStmt::Predicate::FCMP_OEQ}, // != -> ==
                {CmpStmt::Predicate::FCMP_UNE, CmpStmt::Predicate::FCMP_UEQ}, // != -> ==
                {CmpStmt::Predicate::ICMP_EQ, CmpStmt::Predicate::ICMP_NE}, // == -> !=
                {CmpStmt::Predicate::ICMP_NE, CmpStmt::Predicate::ICMP_EQ}, // != -> ==
                {CmpStmt::Predicate::ICMP_UGT, CmpStmt::Predicate::ICMP_ULE}, // > -> <=
                {CmpStmt::Predicate::ICMP_ULT, CmpStmt::Predicate::ICMP_UGE}, // < -> >=
                {CmpStmt::Predicate::ICMP_UGE, CmpStmt::Predicate::ICMP_ULT}, // >= -> <
                {CmpStmt::Predicate::ICMP_SGT, CmpStmt::Predicate::ICMP_SLE}, // > -> <=
                {CmpStmt::Predicate::ICMP_SLT, CmpStmt::Predicate::ICMP_SGE}, // < -> >=
                {CmpStmt::Predicate::ICMP_SGE, CmpStmt::Predicate::ICMP_SLT}, // >= -> <
            };

            Map<s32_t, s32_t> _switch_lhsrhs_predicate = {
                {CmpStmt::Predicate::FCMP_OEQ, CmpStmt::Predicate::FCMP_OEQ}, // == -> ==
                {CmpStmt::Predicate::FCMP_UEQ, CmpStmt::Predicate::FCMP_UEQ}, // == -> ==
                {CmpStmt::Predicate::FCMP_OGT, CmpStmt::Predicate::FCMP_OLT}, // > -> <
                {CmpStmt::Predicate::FCMP_OGE, CmpStmt::Predicate::FCMP_OLE}, // >= -> <=
                {CmpStmt::Predicate::FCMP_OLT, CmpStmt::Predicate::FCMP_OGT}, // < -> >
                {CmpStmt::Predicate::FCMP_OLE, CmpStmt::Predicate::FCMP_OGE}, // <= -> >=
                {CmpStmt::Predicate::FCMP_ONE, CmpStmt::Predicate::FCMP_ONE}, // != -> !=
                {CmpStmt::Predicate::FCMP_UNE, CmpStmt::Predicate::FCMP_UNE}, // != -> !=
                {CmpStmt::Predicate::ICMP_EQ, CmpStmt::Predicate::ICMP_EQ}, // == -> ==
                {CmpStmt::Predicate::ICMP_NE, CmpStmt::Predicate::ICMP_NE}, // != -> !=
                {CmpStmt::Predicate::ICMP_UGT, CmpStmt::Predicate::ICMP_ULT}, // > -> <
                {CmpStmt::Predicate::ICMP_ULT, CmpStmt::Predicate::ICMP_UGT}, // < -> >
                {CmpStmt::Predicate::ICMP_UGE, CmpStmt::Predicate::ICMP_ULE}, // >= -> <=
                {CmpStmt::Predicate::ICMP_SGT, CmpStmt::Predicate::ICMP_SLT}, // > -> <
                {CmpStmt::Predicate::ICMP_SLT, CmpStmt::Predicate::ICMP_SGT}, // < -> >
                {CmpStmt::Predicate::ICMP_SGE, CmpStmt::Predicate::ICMP_SLE}, // >= -> <=
            };

            AbstractState new_es = as;
            IntervalValue& switch_cond = new_es[var->getId()].getInterval();
            s64_t value = succ;
            FIFOWorkList<const SVFStmt*> workList;
            for (SVFStmt* cmpVarInStmt : var->getInEdges()) {
                workList.push(cmpVarInStmt);
            }
            switch_cond.meet_with(IntervalValue(value, value));
            if (switch_cond.isBottom()) {
                return false;
            }
            while (!workList.empty()) {
                const SVFStmt* stmt = workList.pop();
                if (SVFUtil::isa<CopyStmt>(stmt)) {
                    IntervalValue& copy_cond = new_es[var->getId()].getInterval();
                    copy_cond.meet_with(IntervalValue(value, value));
                }
                else if (const LoadStmt* load = SVFUtil::dyn_cast<LoadStmt>(stmt)) {
                    if (new_es.inVarToAddrsTable(load->getRHSVarID())) {
                        AddressValue& addrs = new_es[load->getRHSVarID()].getAddrs();
                        for (const auto& addr : addrs) {
                            NodeID objId = as.getIDFromAddr(addr);
                            if (new_es.inAddrToValTable(objId)) {
                                new_es.load(addr).meet_with(switch_cond);
                            }
                        }
                    }
                }
            }
            as = new_es;
            return true;
        }, py::arg("svfir"), py::arg("var"), py::arg("succ"), py::arg("as"));
}

