from enum import IntEnum, auto


class OpType(IntEnum):
    PUSH_INT = auto()

    ADD = auto()
    SUB = auto()
    MUL = auto()
    DIV = auto()
    MOD = auto()

    DUMP = auto()

    MEM = auto()
