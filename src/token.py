from typing import Optional
from .op_types import OpType
from .types import Types
from dataclasses import dataclass


@dataclass
class Token:
    """
    A class representing a token in a programming language.

    Attributes:
        type (str): The type of the token (e.g., 'keyword', 'identifier', 'literal').
        value (str): The value of the token (e.g., 'if', 'x', '42').
        pos (tuple[str, int, int]): The position of the token in the source code,
            represented as a tuple of (filename, line number, column number).
            The filename is a string, and the line and column numbers are integers.
    """

    op_type: OpType
    value: str
    value_type: Optional[Types]
    pos: tuple[str, int, int]

    @property
    def str_pos(self) -> str:
        """
        Returns the position of the token as a formatted string.

        Returns:
            str: The position of the token in the format "filename:line:column".
        """
        return f"{self.pos[0]}:{self.pos[1]}:{self.pos[2]}"
    
    def __str__(self) -> str:
        """
        Returns a string representation of the token.

        Returns:
            str: A string representation of the token in the format
                "Token(type=type, value=value, pos=pos)".
        """
        if self.value_type is not None:
            return f"Token(type={self.op_type}, value={self.value}, value_type={self.value_type}, pos={self.str_pos})"
        return f"Token(type={self.op_type}, value={self.value}, pos={self.str_pos})"
    
    def __repr__(self) -> str:
        """
        Returns a string representation of the token for debugging.

        Returns:
            str: A string representation of the token in the format
                "Token(type=type, value=value, pos=pos)".
        """
        return self.__str__()