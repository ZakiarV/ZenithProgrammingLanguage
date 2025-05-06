from .token import Token
from .op_types import OpType
from .compiler_error import compiler_error


class Simulation:
    def __init__(self, tokens: list[Token], memory: int = 640_000):
        assert len(OpType) == 8, "Update the simulation to support new op types"
        self.tokens = tokens
        self.stack = []
        self.memory_size = memory
        self.memory = bytearray(memory)
        self.memory_pointer = 0

    def simulate(self) -> None:
        for token in self.tokens:
            if token.op_type == OpType.PUSH_INT:
                self.stack.append(int(token.value))
            elif token.op_type == OpType.ADD:
                if len(self.stack) < 2:
                    compiler_error(token.str_pos(), "Simulation", f"Not enough operands for '+' operation at {token.pos}")
                a = self.stack.pop()
                b = self.stack.pop()
                self.stack.append(a + b)
            elif token.op_type == OpType.DUMP:
                if len(self.stack) < 1:
                    compiler_error(token.str_pos(), "Simulation", f"Not enough operands for 'dump' operation at {token.pos}")
                value = self.stack.pop()
                print(value)
            elif token.op_type == OpType.SUB:
                if len(self.stack) < 2:
                    compiler_error(token.str_pos(), "Simulation", f"Not enough operands for '-' operation at {token.pos}")
                a = self.stack.pop()
                b = self.stack.pop()
                self.stack.append(b - a)
            elif token.op_type == OpType.MUL:
                if len(self.stack) < 2:
                    compiler_error(token.str_pos(), "Simulation", f"Not enough operands for '*' operation at {token.pos}")
                a = self.stack.pop()
                b = self.stack.pop()
                self.stack.append(a * b)
            elif token.op_type == OpType.DIV:
                if len(self.stack) < 2:
                    compiler_error(token.str_pos(), "Simulation", f"Not enough operands for '/' operation at {token.pos}")
                a = self.stack.pop()
                b = self.stack.pop()
                if a == 0:
                    raise RuntimeError(f"Division by zero at {token.pos}")
                self.stack.append(b // a)
            elif token.op_type == OpType.MOD:
                if len(self.stack) < 2:
                    compiler_error(token.str_pos(), "Simulation", f"Not enough operands for '%' operation at {token.pos}")
                a = self.stack.pop()
                b = self.stack.pop()
                if a == 0:
                    raise RuntimeError(f"Division by zero at {token.pos}")
                self.stack.append(b % a)
            elif token.op_type == OpType.MEM:
                self.stack.append(self.memory_pointer)
            else:
                raise RuntimeError(f"Unknown operation '{token.op_type}' at {token.pos}")