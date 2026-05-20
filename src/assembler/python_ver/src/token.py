from dataclasses import dataclass


@dataclass
class Token:
    lexeme: str
    token_type: str
    global_offset: int
    line_number: int
    column_start: int
    column_end: int