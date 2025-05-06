from .token import Token
from .op_types import OpType


class Simulation:
    def __init__(self, tokens: list[Token]):
        assert len(OpType) == 7, "Update the simulation to support new op types"
        self.tokens = tokens
        self.stack = []

    def simulate(self) -> None:
        for token in self.tokens:
            if token.op_type == OpType.PUSH_INT:
                self.stack.append(int(token.value))
            elif token.op_type == OpType.ADD:
                if len(self.stack) < 2:
                    raise RuntimeError(f"Not enough operands for '+' operation at {token.pos}")
                a = self.stack.pop()
                b = self.stack.pop()
                self.stack.append(a + b)
            elif token.op_type == OpType.DUMP:
                if len(self.stack) < 1:
                    raise RuntimeError(f"Not enough operands for 'dump' operation at {token.pos}")
                value = self.stack.pop()
                print(value)
            elif token.op_type == OpType.SUB:
                if len(self.stack) < 2:
                    raise RuntimeError(f"Not enough operands for '-' operation at {token.pos}")
                a = self.stack.pop()
                b = self.stack.pop()
                self.stack.append(b - a)
            elif token.op_type == OpType.MUL:
                if len(self.stack) < 2:
                    raise RuntimeError(f"Not enough operands for '*' operation at {token.pos}")
                a = self.stack.pop()
                b = self.stack.pop()
                self.stack.append(a * b)
            elif token.op_type == OpType.DIV:
                if len(self.stack) < 2:
                    raise RuntimeError(f"Not enough operands for '/' operation at {token.pos}")
                a = self.stack.pop()
                b = self.stack.pop()
                if a == 0:
                    raise RuntimeError(f"Division by zero at {token.pos}")
                self.stack.append(b // a)
            elif token.op_type == OpType.MOD:
                if len(self.stack) < 2:
                    raise RuntimeError(f"Not enough operands for '%' operation at {token.pos}")
                a = self.stack.pop()
                b = self.stack.pop()
                if a == 0:
                    raise RuntimeError(f"Division by zero at {token.pos}")
                self.stack.append(b % a)
            else:
                raise RuntimeError(f"Unknown operation '{token.op_type}' at {token.pos}")