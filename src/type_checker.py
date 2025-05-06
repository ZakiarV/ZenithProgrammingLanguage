from .op_types import OpType
from .token import Token
from .types import Types
from .compiler_error import compiler_error


class TypeChecker:
    def __init__(self, tokens: list[Token]):
        assert len(OpType) == 7, "Update the type checker to support new op types"
        assert len(Types) == 1, "Update the type checker to support new types"
        self.tokens = tokens
        self.stack = []

    def check_types(self) -> None:
        for token in self.tokens:
            if token.op_type == OpType.PUSH_INT:
                self.stack.append(Types.INT)
            elif token.op_type == OpType.ADD:
                if len(self.stack) < 2:
                    compiler_error(token.str_pos(), "TypeChecker", f"Not enough operands for '+' operation at {token.pos}")
                if self.stack[-1] != Types.INT or self.stack[-2] != Types.INT:
                    compiler_error(token.str_pos(), "TypeChecker", f"Invalid types for '+' operation at {token.pos}. Expected INT, got {self.stack[-1].name} and {self.stack[-2].name}")
                self.stack.pop()
                self.stack.pop()
                self.stack.append(Types.INT)
            elif token.op_type == OpType.DUMP:
                if len(self.stack) < 1:
                    compiler_error(token.str_pos(), "TypeChecker", f"Not enough operands for 'dump' operation at {token.pos}")
                if self.stack[-1] != Types.INT:
                    compiler_error(token.str_pos(), "TypeChecker", f"Invalid type for 'dump' operation at {token.pos}. Expected INT, got {self.stack[-1].name}")
                self.stack.pop()
            elif token.op_type == OpType.SUB:
                if len(self.stack) < 2:
                    compiler_error(token.str_pos(), "TypeChecker", f"Not enough operands for '-' operation at {token.pos}")
                if self.stack[-1] != Types.INT or self.stack[-2] != Types.INT:
                    compiler_error(token.str_pos(), "TypeChecker", f"Invalid types for '-' operation at {token.pos}. Expected INT, got {self.stack[-1].name} and {self.stack[-2].name}")
                self.stack.pop()
                self.stack.pop()
                self.stack.append(Types.INT)
            elif token.op_type == OpType.MUL:
                if len(self.stack) < 2:
                    compiler_error(token.str_pos(), "TypeChecker", f"Not enough operands for '*' operation at {token.pos}")
                if self.stack[-1] != Types.INT or self.stack[-2] != Types.INT:
                    compiler_error(token.str_pos(), "TypeChecker", f"Invalid types for '*' operation at {token.pos}. Expected INT, got {self.stack[-1].name} and {self.stack[-2].name}")
                self.stack.pop()
                self.stack.pop()
                self.stack.append(Types.INT)
            elif token.op_type == OpType.DIV:
                if len(self.stack) < 2:
                    compiler_error(token.str_pos(), "TypeChecker", f"Not enough operands for '/' operation at {token.pos}")
                if self.stack[-1] != Types.INT or self.stack[-2] != Types.INT:
                    compiler_error(token.str_pos(), "TypeChecker", f"Invalid types for '/' operation at {token.pos}. Expected INT, got {self.stack[-1].name} and {self.stack[-2].name}")
                self.stack.pop()
                self.stack.pop()
                self.stack.append(Types.INT)
            elif token.op_type == OpType.MOD:
                if len(self.stack) < 2:
                    compiler_error(token.str_pos(), "TypeChecker", f"Not enough operands for '%' operation at {token.pos}")
                if self.stack[-1] != Types.INT or self.stack[-2] != Types.INT:
                    compiler_error(token.str_pos(), "TypeChecker", f"Invalid types for '%' operation at {token.pos}. Expected INT, got {self.stack[-1].name} and {self.stack[-2].name}")
                self.stack.pop()
                self.stack.pop()
                self.stack.append(Types.INT)
            else:
                compiler_error(token.str_pos(), "TypeChecker", f"Unknown operation '{token.op_type}' at {token.pos}")
