# SIB Byte

## Structure
```aiignore
  7   6   5   4   3   2   1   0
+---+---+---+---+---+---+---+---+
| Scale |   Index   |   Base    |
+---+---+---+---+---+---+---+---+
```
## Fields
- `Scale` - Scale factor
- `Index` - Index register
- `Base` - Base register


## Scale
- 00 - 1
- 01 - 2
- 10 - 4
- 11 - 8

## Index
- 000: [EAX]
- 001: [ECX]
- 010: [EDX]
- 011: [EBX]
- 100: No index (used when the SIB byte is present but no index register is used)
- 101: [EBP] (used when the SIB byte is present and the base register is not used)
- 110: [ESI]
- 111: [EDI]


## Base
- 000: [EAX]
- 001: [ECX]
- 010: [EDX]
- 011: [EBX]
- 100: [ESP]
- 101: [EBP] (used when the SIB byte is present and the base register is not used)
- 110: [ESI]
- 111: [EDI]