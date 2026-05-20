from enum import IntEnum


class TokenType(IntEnum):
    INSTRUCTION = 0
    REGISTER = 1
    INT_LITERAL = 2
    CHAR_LITERAL = 3
    STRING_LITERAL = 4
    LABEL = 5
    DIRECTIVE = 6
    LBRACKET = 7
    RBRACKET = 8
    PLUS = 9
    MINUS = 10
    STAR = 11
    COLON = 12
    COMMA = 13
    
