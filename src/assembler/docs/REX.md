# REX Prefix Byte

## Structure
The REX prefix byte is structured as follows:

```
  7   6   5   4   3   2   1   0
+---+---+---+---+---+---+---+---+
| 0 | 1 | 0 | 0 | W | R | X | B |
+---+---+---+---+---+---+---+---+
```

## W, R, X, B Bits
- **W**: When set to 1, it indicates that the instruction operates on 64-bit operands. When set to 0, it indicates that the instruction operates on 8-bit, 16-bit, or 32-bit operands.
- **R**: When set to 1, it extends the ModR/M reg field by adding 8 to the register number.
- **X**: When set to 1, it extends the SIB index field by adding 8 to the register number.
- **B**: When set to 1, it extends the ModR/M r/m field or the SIB base field by adding 8 to the register number.

## Usage
The REX prefix byte is used to specify the operand size and addressing modes for instructions that support them.
It is typically placed before the opcode byte.
