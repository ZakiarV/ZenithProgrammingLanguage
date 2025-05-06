from src.lexer import Lexer
from src.parser import Parser
from src.type_checker import TypeChecker
from src.simulation import Simulation
from src.compiler import Compiler
import sys
import subprocess


def main():
    run = False
    args = sys.argv[1:]
    if len(args) < 2:
        print("Usage: python main.py <com/sim> <filename>.zenith", file=sys.stderr)
        sys.exit(1)
    if "com" in args:
        args.remove("com")
        mode = "com"
        if "-run" in args:
            args.remove("-run")
            run = True
    elif "sim" in args:
        args.remove("sim")
        mode = "sim"
    else:
        print("Invalid mode. Use 'com' for compilation or 'sim' for simulation.", file=sys.stderr)
        sys.exit(1)
    filename = ""
    for arg in args:
        if arg.endswith(".zenith"):
            filename = arg
            args.remove(arg)
            break
    if not filename:
        print("No .zenith file provided.", file=sys.stderr)
        sys.exit(1)
    if mode not in ["com", "sim"]:
        print("Invalid mode. Use 'com' for compilation or 'sim' for simulation.", file=sys.stderr)
        sys.exit(1)
    if not filename.endswith(".zenith"):
        print("Invalid file type. Please provide a .zenith file.", file=sys.stderr)
        sys.exit(1)
    lexer = Lexer(filename)
    tokens = lexer.tokenize()
    parser = Parser(tokens)
    parsed_tokens = parser.parse()
    type_checker = TypeChecker(parsed_tokens.copy())
    type_checker.check_types()
    if mode == "sim":
        simulation = Simulation(parsed_tokens)
        simulation.simulate()
    elif mode == "com":
        compiler = Compiler(parsed_tokens, filename.split(".")[0] + ".asm")
        compiler.compile()
        print("[CMD] " + "".join(["nasm", "-f", "elf64", filename.split(".")[0] + ".asm", "-o", filename.split(".")[0] + ".o"]))
        subprocess.run(["nasm", "-f", "elf64", filename.split(".")[0] + ".asm", "-o", filename.split(".")[0] + ".o"])
        print("[CMD] " + "".join(["ld", filename.split(".")[0] + ".o", "-o", filename.split(".")[0]]))
        subprocess.run(["ld", filename.split(".")[0] + ".o", "-o", filename.split(".")[0]])
        if run:
            print("[CMD] " + "".join(["./", filename.split(".")[0]]))
            subprocess.run([f"./{filename.split('.')[0]}"])

if __name__ == "__main__":
    main()