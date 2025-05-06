from .op_types import OpType
from .token import Token
from .types import Types
from .compiler_error import compiler_error


class Lexer:
    def __init__(self, file_name: str):
        assert len(OpType) == 9, "Update the lexer to support new op types"
        assert file_name.endswith(".zenith"), "File must be a .zenith file"
        self.file_name = file_name
        self.tokens = []
        self.ops = {
            "+": OpType.ADD,
            "dump": OpType.DUMP,
            "-": OpType.SUB,
            "*": OpType.MUL,
            "/": OpType.DIV,
            "%": OpType.MOD,
            "mem": OpType.MEM,
            "dup": OpType.DUP,
        }
        self.includes = set()

    def tokenize(self) -> tuple[list[Token], set[str]]:
        with open(self.file_name, 'r') as file:
            source_code = file.readlines()

        for line_number, line in enumerate(source_code):
            if line.startswith("include"):
                parts = line.split("\"")
                if len(parts) != 3:
                    compiler_error(f"{self.file_name}:{line_number + 1}:{1}", "Lexer", "Invalid include statement. Expected format: include \"filename\"")
                filename = parts[1]
                if filename not in self.includes:
                    lex = Lexer(filename)
                    self.tokens.extend(lex.tokenize())
                    self.includes.add(filename)
            else:
                words = line.split()
                for word in words:
                    if word in self.ops:
                        token = Token(op_type=self.ops[word], value=word, value_type=None, pos=(self.file_name, line_number + 1, line.index(word)))
                        self.tokens.append(token)
                    elif word.isdigit():
                        token = Token(op_type=OpType.PUSH_INT, value=word, value_type=Types.INT, pos=(self.file_name, line_number + 1, line.index(word)))
                        self.tokens.append(token)
                    elif word.startswith("0x") and all(c in "0123456789abcdefABCDEF" for c in word[2:]):
                        token = Token(op_type=OpType.PUSH_INT, value=word, value_type=Types.INT, pos=(self.file_name, line_number + 1, line.index(word)))
                        self.tokens.append(token)
                    else:
                        compiler_error(f"{self.file_name}:{line_number + 1}:{line.index(word) + 1}", "Lexer", f"Unknown token '{word}'")
        return self.tokens