# 2.1.1 Hexadecimal Notation

## ஏன் Hex?

```
Binary:  11111010000111010011011110110 ← too verbose
Decimal: tedious to convert to/from bits
Hex:     0xFA1D37B  ← compact, easy to convert
```

1 hex digit = exactly 4 bits. அதனால conversion straightforward.

---

## Hex table — memorize பண்ணணும்
![alt text](image.png)

---

## Memory trick — A, C, F முதல்ல memorize

Book சொல்றது: A=10, C=12, F=15 தெரிஞ்சா மீதி B, D, E derive பண்ணலாம்:

```
A = 10  → B = 10+1 = 11
C = 12  → D = 12+1 = 13
F = 15  → E = 15-1 = 14
```

---

## C-ல hex notation

```c
0xFA1D37B   // same as
0xfa1d37b   // same as
0xFa1D37b   // all identical — case insensitive
```

`0x` அல்லது `0X` prefix = hexadecimal constant.

---

## Hex → Binary conversion (book-ரோட exact example)

`0x173A4C` → Binary:

```
Hex:     1      7      3      A      4      C
         ↓      ↓      ↓      ↓      ↓      ↓
Binary: 0001   0111   0011   1010   0100   1100

Result: 000101110011101001001100
```

Rule: ஒவ்வொரு hex digit-உம் **exactly 4 bits**. Direct substitution — no calculation needed.

---

## Binary → Hex conversion (book-ரோட exact example)

`1111001010110110110011` → Hex:

**Step 1:** Right-லிருந்து 4 bits-ஆ group பண்ணு:

```
1111001010110110110011
→ 11 | 1100 | 1010 | 1101 | 1011 | 0011
```

**Step 2:** leftmost group 4-க்கு குறைவா இருந்தா leading zeros add:

```
11 → 0011
```

**Step 3:** ஒவ்வொரு group-உம் hex-ஆ convert:

```
Binary:  0011   1100   1010   1101   1011   0011
Hex:       3      C      A      D      B      3

Result: 0x3CADB3
```

---

## Byte range in hex

```
1 byte = 8 bits
Minimum: 00000000 = 0x00
Maximum: 11111111 = 0xFF = 255

Memory addresses always shown in hex:
  0x7fff1000  ← stack address
  0x00401020  ← code address
  0xFFFFFFFF  ← max 32-bit address
```

---

## Practice — mental conversion shortcuts

```
Powers of 2 → hex மிகவும் easy:

2^4  = 16   = 0x10
2^8  = 256  = 0x100
2^12 = 4096 = 0x1000
2^16 = 65536 = 0x10000

n = power of 2:
  n/16 → how many trailing zeros in hex
  
  2^20 = 0x100000  (5 zeros)
  2^32 = 0x100000000 (8 zeros)
```

```
Common values:
  255  = 0xFF
  256  = 0x100
  4096 = 0x1000   (page size)
  1MB  = 0x100000
  1GB  = 0x40000000
```

அடுத்து 2.1.2 (Data Sizes) போகலாமா?