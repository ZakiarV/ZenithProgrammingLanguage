from .token import Token
from .op_types import OpType
from .compiler_error import compiler_error


class Parser:
    def __init__(self, tokens: list[Token]):
        assert len(OpType) == 9, "Update the parser to support new op types"
        self.tokens = tokens

    def parse(self) -> list[Token]:
        for token in self.tokens:
            pass

        return self.tokens