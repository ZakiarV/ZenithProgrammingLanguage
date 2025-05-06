from .token import Token
from .op_types import OpType


class Compiler:
    def __init__(self, tokens: list[Token], file_name: str):
        assert len(OpType) == 7, "Update the compiler to support new op types"
        self.tokens = tokens
        self.file_name = file_name

    def compile(self) -> None:
        with open(self.file_name, 'w') as file:
            file.write("section .text\n")
            file.write("global _start\n")
            file.write("dump:\n")
            file.write("\tmov r9, -3689348814741910323\n") 
            file.write("\tsub rsp, 40\n") 
            file.write("\tmov BYTE [rsp+31], 10\n") 
            file.write("\tlea rcx, [rsp+30]\n") 
            file.write(".L2:\n") 
            file.write("\tmov rax, rdi\n") 
            file.write("\tlea r8, [rsp+32]\n") 
            file.write("\tmul r9\n") 
            file.write("\tmov rax, rdi\n") 
            file.write("\tsub r8, rcx\n") 
            file.write("\tshr rdx, 3\n") 
            file.write("\tlea rsi, [rdx+rdx*4]\n") 
            file.write("\tadd rsi, rsi\n") 
            file.write("\tsub rax, rsi\n") 
            file.write("\tadd eax, 48\n") 
            file.write("\tmov BYTE [rcx], al\n") 
            file.write("\tmov rax, rdi\n") 
            file.write("\tmov rdi, rdx\n") 
            file.write("\tmov rdx, rcx\n") 
            file.write("\tsub rcx, 1\n") 
            file.write("\tcmp rax, 9\n") 
            file.write("\tja  .L2\n") 
            file.write("\tlea rax, [rsp+32]\n") 
            file.write("\tmov edi, 1\n") 
            file.write("\tsub rdx, rax\n") 
            file.write("\txor eax, eax\n") 
            file.write("\tlea rsi, [rsp+32+rdx]\n") 
            file.write("\tmov rdx, r8\n") 
            file.write("\tmov rax, 1\n") 
            file.write("\tsyscall\n") 
            file.write("\tadd rsp, 40\n") 
            file.write("\tret\n") 

            file.write("_start:\n")
            for i ,token in enumerate(self.tokens):
                if token.op_type == OpType.PUSH_INT:
                    file.write(f"instruction_{i}:\n")
                    file.write(f"    ;; -- PUSH_INT {token.value} --\n")
                    file.write(f"    push {token.value}\n")
                elif token.op_type == OpType.ADD:
                    file.write(f"instruction_{i}:\n")
                    file.write(f"   ;; -- PLUS --\n")
                    file.write("    pop rax\n")
                    file.write("    pop rbx\n")
                    file.write("    add rax, rbx\n")
                    file.write("    push rax\n")
                elif token.op_type == OpType.DUMP:
                    file.write(f"instruction_{i}:\n")
                    file.write(f"   ;; -- DUMP --\n")
                    file.write("    pop rdi\n")
                    file.write("    call dump\n")
                elif token.op_type == OpType.SUB:
                    file.write(f"instruction_{i}:\n")
                    file.write(f"   ;; -- SUB --\n")
                    file.write("    pop rbx\n")
                    file.write("    pop rax\n")
                    file.write("    sub rax, rbx\n")
                    file.write("    push rax\n")
                elif token.op_type == OpType.MUL:
                    file.write(f"instruction_{i}:\n")
                    file.write(f"   ;; -- MUL --\n")
                    file.write("    pop rbx\n")
                    file.write("    pop rax\n")
                    file.write("    mul rbx\n")
                    file.write("    push rax\n")
                elif token.op_type == OpType.DIV:
                    file.write(f"instruction_{i}:\n")
                    file.write(f"   ;; -- DIV --\n")
                    file.write("    pop rbx\n")
                    file.write("    pop rax\n")
                    file.write("    xor rdx, rdx\n")
                    file.write("    div rbx\n")
                    file.write("    push rax\n")
                elif token.op_type == OpType.MOD:
                    file.write(f"instruction_{i}:\n")
                    file.write(f"   ;; -- MOD --\n")
                    file.write("    pop rbx\n")
                    file.write("    pop rax\n")
                    file.write("    xor rdx, rdx\n")
                    file.write("    div rbx\n")
                    file.write("    push rdx\n")
            file.write(f"instruction_{len(self.tokens)}:\n")
            file.write("    ; -- Exit --\n")
            file.write("    mov rax, 60\n")
            file.write("    mov rdi, 0\n")
            file.write("    syscall\n")

