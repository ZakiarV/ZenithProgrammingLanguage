# ModRM Byte Encoding

## Structure
The ModRM byte is a 1-byte encoding of the ModR/M byte. The structure is as follows:

```
  7   6   5   4   3   2   1   0
+---+---+---+---+---+---+---+---+
|  Mod  |    Reg    |    R/M    |
+---+---+---+---+---+---+---+---+
```

## Mod
The Mod field is a 2-bit field that specifies the register or memory operand.
- 00: Register indirect addressing mode (except when R/M = 101, which indicates a direct memory address).
- 01: Register indirect addressing mode with an 8-bit displacement.
- 10: Register indirect addressing mode with a 32-bit displacement.
- 11: Register addressing mode (both operands are registers).


## Reg
The Reg field is a 3-bit field that specifies the register operand. The specific register is determined by 
the opcode and the value of the Reg field.
- 000: AL, AX, EAX, MM0, XMM0
- 001: CL, CX, ECX, MM1, XMM1
- 010: DL, DX, EDX, MM2, XMM2
- 011: BL, BX, EBX, MM3, XMM3
- 100: AH, SP, ESP, MM4, XMM4
- 101: CH, BP, EBP, MM5, XMM5
- 110: DH, SI, ESI, MM6, XMM6
- 111: BH, DI, EDI, MM7, XMM7


## R/M
The R/M field is a 3-bit field that specifies the memory operand.
- 000: [EAX]
- 001: [ECX]
- 010: [EDX]
- 011: [EBX]
- 100: SIB byte follows (used for complex addressing modes)
- 101: [disp32] (used for direct memory addressing)
- 110: [ESI]
- 111: [EDI]