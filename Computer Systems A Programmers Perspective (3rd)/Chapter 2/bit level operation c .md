# 2.1.7 Bit-Level Operations in C

## C-ரோட bitwise operators

```
|  → OR
&  → AND
~  → NOT
^  → XOR

Apply to: any integral type
  char, short, int, long, unsigned variants எல்லாம்
```

---

## Book-ரோட exact examples — `char` type (1 byte = 8 bits)

### Example 1: `~0x41`

```
0x41 → binary:
  4    1
  0100 0001
  = [01000001]

~[01000001]:
  every bit flip:
  [10111110]

[10111110] → hex:
  1011 1110
  B    E
  = 0xBE
```

### Example 2: `~0x00`

```
0x00 → binary:
  0    0
  0000 0000
  = [00000000]

~[00000000]:
  every bit flip:
  [11111111]

[11111111] → hex:
  1111 1111
  F    F
  = 0xFF

Intuition: ~0 = all ones = maximum unsigned byte value (255)
```

### Example 3: `0x69 & 0x55`

```
0x69 → [01101001]
0x55 → [01010101]

  0 1 1 0 1 0 0 1   (0x69)
& 0 1 0 1 0 1 0 1   (0x55)
─────────────────
  0 1 0 0 0 0 0 1   (result)

Position by position (AND = both must be 1):
  0&0=0  1&1=1  1&0=0  0&1=0  1&0=0  0&1=0  0&0=0  1&1=1

[01000001] → hex:
  0100 0001
  4    1
  = 0x41
```

### Example 4: `0x69 | 0x55`

```
0x69 → [01101001]
0x55 → [01010101]

  0 1 1 0 1 0 0 1   (0x69)
| 0 1 0 1 0 1 0 1   (0x55)
─────────────────
  0 1 1 1 1 1 0 1   (result)

Position by position (OR = either can be 1):
  0|0=0  1|1=1  1|0=1  0|1=1  1|0=1  0|1=1  0|0=0  1|1=1

[01111101] → hex:
  0111 1101
  7    D
  = 0x7D
```

---

## All 4 examples — summary table (book exact)

```
C expression    Binary expression              Binary result   Hex result
─────────────────────────────────────────────────────────────────────────
~0x41           ~[01000001]                    [10111110]      0xBE
~0x00           ~[00000000]                    [11111111]      0xFF
0x69 & 0x55     [01101001] & [01010101]        [01000001]      0x41
0x69 | 0x55     [01101001] | [01010101]        [01111101]      0x7D
```

---

## Book-ரோட method — best way to compute

> "The best way to determine the effect of a bit-level expression is to expand the hexadecimal arguments to their binary representations, perform the operations in binary, and then convert back to hexadecimal."

```
Step 1: Hex → Binary (4 bits per hex digit)
Step 2: Apply operation bit by bit
Step 3: Binary → Hex (group 4 bits at a time)
```

---

## Practical uses — bit operations real applications

**Masking — specific bits extract பண்றது:**

```c
// Lower 4 bits extract
x & 0x0F    // 0x0F = 00001111
            // upper 4 bits zero out, lower 4 preserved

// Example:
0x69 & 0x0F:
  0110 1001
& 0000 1111
───────────
  0000 1001 = 0x09  (lower nibble of 0x69)
```

**Setting bits:**

```c
// Set bit 3
x | 0x08    // 0x08 = 00001000
            // bit 3 becomes 1, others unchanged
```

**Clearing bits:**

```c
// Clear bit 3
x & ~0x08   // ~0x08 = 11110111
            // bit 3 becomes 0, others unchanged
```

**Toggling bits:**

```c
// Toggle bit 3
x ^ 0x08    // bit 3 flips, others unchanged
```

அடுத்து 2.1.8 (Logical Operations in C) போகலாமா?