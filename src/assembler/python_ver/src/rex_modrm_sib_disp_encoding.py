from dataclasses import dataclass


@dataclass
class RexModrmSibDispEncoding:
    rex_w: bool
    rex_r: bool
    rex_x: bool
    rex_b: bool
    modrm_rm: int
    sib: int | None
    disp: bytes



def encode_memory_operand()-> RexModrmSibDispEncoding:
    pass