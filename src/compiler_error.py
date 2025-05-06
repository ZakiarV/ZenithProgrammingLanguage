import sys


def compiler_error(position: str, origin: str, message: str) -> None:
    """
    Print a compiler error message and exit the program.

    Args:
        position (str): The position in the source code where the error occurred.
        origin (str): The origin of the error (e.g., file name).
        message (str): The error message to display.
    """
    print(f"{position}: {origin} Error: {message}", file=sys.stderr)
    sys.exit(1)