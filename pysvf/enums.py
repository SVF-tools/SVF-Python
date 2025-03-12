from enum import IntEnum

class Predicate(IntEnum):
    """Predicate values for compare operations"""

    FCMP_FALSE = 0          # Always false (always folded)
    FCMP_OEQ = 1            # Ordered and equal is true
    FCMP_OGT = 2            # Ordered and greater is true
    FCMP_OGE = 3            # Ordered and greater than or equal is true
    FCMP_OLT = 4            # Ordered and less than is true
    FCMP_OLE = 5            # Ordered and less than or equal is true
    FCMP_ONE = 6            # Ordered and not equal is true
    FCMP_ORD = 7            # Ordered (no NaN) is true
    FCMP_UNO = 8            # Unordered is true
    FCMP_UEQ = 9            # Unordered or equal is true
    FCMP_UGT = 10           # Unordered or greater is true
    FCMP_UGE = 11           # Unordered or greater than or equal is true
    FCMP_ULT = 12           # Unordered or less than is true
    FCMP_ULE = 13           # Unordered or less than or equal is true
    FCMP_UNE = 14           # Unordered or not equal is true
    FCMP_TRUE = 15          # Always true (always folded)
    FIRST_FCMP_PREDICATE = FCMP_FALSE
    LAST_FCMP_PREDICATE = FCMP_TRUE
    BAD_FCMP_PREDICATE = 16

    ICMP_EQ = 32           # Equal
    ICMP_NE = 33           # Not equal
    ICMP_UGT = 34          # Unsigned greater than
    ICMP_UGE = 35          # Unsigned greater than or equal
    ICMP_ULT = 36          # Unsigned less than
    ICMP_ULE = 37          # Unsigned less than or equal
    ICMP_SGT = 38          # Signed greater than
    ICMP_SGE = 39          # Signed greater than or equal
    ICMP_SLT = 40          # Signed less than
    ICMP_SLE = 41          # Signed less than or equal
    FIRST_ICMP_PREDICATE = ICMP_EQ
    LAST_ICMP_PREDICATE = ICMP_SLE
    BAD_ICMP_PREDICATE = 42


class OpCode(IntEnum):
    """Operation codes for binary operations"""

    Add = 13            # Sum of integers
    FAdd = 14           # Sum of floats
    Sub = 15            # Subtraction of integers
    FSub = 16           # Subtraction of floats
    Mul = 17            # Product of integers
    FMul = 18           # Product of floats
    UDiv = 19           # Unsigned division
    SDiv = 20           # Signed division
    FDiv = 21           # Float division
    URem = 22           # Unsigned remainder
    SRem = 23           # Signed remainder
    FRem = 24           # Float remainder
    Shl = 25            # Shift left (logical)
    LShr = 26           # Shift right (logical)
    AShr = 27           # Shift right (arithmetic)
    And = 28            # Logical and
    Or = 29             # Logical or
    Xor = 30            # Logical xor
    FIRST_OPCODE = Add
    LAST_OPCODE = Xor
    BAD_OPCODE = 31