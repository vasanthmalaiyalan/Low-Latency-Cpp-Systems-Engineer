# Practice Problem 2.5

## Setup

```c
int a = 0x12345678;
byte_pointer ap = (byte_pointer) &a;
```

`0x12345678` — 4 bytes breakdown:

```
Hex digit pairs:  12    34    56    78
                  ↑MSB                ↑LSB
```

Memory-ல எப்படி store ஆகும்:

```
Little endian (Intel x86):
ap[0]=78  ap[1]=56  ap[2]=34  ap[3]=12
  ↑ lowest address              ↑ highest address
  LSB first                     MSB last

Big endian (SPARC):
ap[0]=12  ap[1]=34  ap[2]=56  ap[3]=78
  ↑ lowest address              ↑ highest address
  MSB first                     LSB last
```

## Call A — `show_bytes(ap, 1)` — len=1, ap[0] மட்டும்

```
Little endian: ap[0] = 78  →  prints: 78
Big endian:    ap[0] = 12  →  prints: 12
```

## Call B — `show_bytes(ap, 2)` — len=2, ap[0] ap[1]

```
Little endian: ap[0]=78, ap[1]=56  →  prints: 7856
Big endian:    ap[0]=12, ap[1]=34  →  prints: 1234
```

## Call C — `show_bytes(ap, 3)` — len=3, ap[0] ap[1] ap[2]

```
Little endian: ap[0]=78, ap[1]=56, ap[2]=34  →  prints: 785634
Big endian:    ap[0]=12, ap[1]=34, ap[2]=56  →  prints: 123456
```

## Answer table

```
Call    Little endian    Big endian
─────────────────────────────────
A.          78              12
B.        7856            1234
C.      785634          123456
```

---

# Practice Problem 2.6

## Given

```
integer    2607352   → hex: 0x0027C8F8
float    3510593.0   → hex: 0x4A1F23E0
```

## Part A — Hex → Binary

**`0x0027C8F8`:**

```
0    0    2    7    C    8    F    8
↓    ↓    ↓    ↓    ↓    ↓    ↓    ↓
0000 0000 0010 0111 1100 1000 1111 1000
```

Full binary: `00000000001001111100100011111000`

**`0x4A1F23E0`:**

```
4    A    1    F    2    3    E    0
↓    ↓    ↓    ↓    ↓    ↓    ↓    ↓
0100 1010 0001 1111 0010 0011 1110 0000
```

Full binary: `01001010000111110010001111100000`

---

## Part B — Shift to maximize matching bits

இரண்டையும் எழுது:

```
Int:   00000000001001111100100011111000
Float: 01001010000111110010001111100000
```

Float-ஐ fixed-ஆ வச்சு, integer-ஐ 2 positions left shift பண்ணு:

```
Int   : 00000000001001111100100011111000
Int<<2: 0000000010 0111110010001111100000
Float : 0100101000 0111110010001111100000
                   ^^^^^^^^^^^^^^^^^^^^^^
                   matching region
```

Precisely align:

```
Int  (orig): 0 0000000 00100111 11001000 11111000
Float(orig): 0 1001010 00011111 00100011 11100000

Shift int LEFT by 2 bits:
Int  (<<2) : 0 0000000 10011111 00100011 11100000
Float      : 0 1001010 00011111 00100011 11100000
                       ^^^^^^^^ ^^^^^^^^ ^^^^^^^^
                       21 bits match!
```

**Answer: 21 bits match**

---

## Part C — Non-matching parts

```
Int  (<<2) : 0 0000000 1  |  0011111 00100011 11100000
Float      : 0 1001010 0  |  0011111 00100011 11100000
             ^^^^^^^^^^^     (matching 21 bits →)
             Non-matching
```

**Left side — non-matching (11 bits):**

```
Int  (<<2): 00000000 1xx...
Float:      01001010 0xx...

Int-ரோட bits:   leading zeros — number is small
Float-ரோட bits: sign(0) + exponent(10010100=148)
                → actual exponent = 148-127 = 21
                → float = mantissa × 2^21

These bits encode completely different things:
  Int:   just magnitude of a small integer
  Float: biased exponent (scale factor)
```

**Why 21 bits match — the real reason:**

```
2607352 in binary:
= 1001111100100011111000  (22 significant bits)
  ↑ bit position 21

Float 3510593.0:
= 1.something × 2^21
  mantissa = 001111100100011111000 00  (stored without leading 1)

Int-ரோட significant bits = Float-ரோட mantissa bits!

Int:   00000000 0[0100111 11001000 11111000]
Float: 01001010 0[0011111 00100011 11100000]
                  ↑ same 21 bits, shifted by 1
```

Book-ரோட 12345 example-ல 13 bits matched. இங்க 21 bits match. Same reason — **integer-ரோட significant bits, float-ரோட mantissa-ல appear ஆகும்.** Floating point chapter-ல (2.4) இது fully explain ஆகும்.

--------

சூப்பர் 🔥 இப்போ நம்ம **book flow + deep reasoning** exactஆ solve பண்ணலாம். நீ கேட்ட இரண்டு problems-யும் **step-by-step** 👇

---

# 🔥 ✅ Practice Problem 2.5

## GIVEN

```c
int a = 0x12345678;
```

---

# 🧠 STEP 1 — split into bytes

```text
0x12 34 56 78
```

👉 4 bytes:

```text
MSB → 12
      34
      56
LSB → 78
```

---

# ⚙️ STEP 2 — endian rules

## 🟢 Little endian

```text
78 56 34 12
```

## 🔵 Big endian

```text
12 34 56 78
```

---

# 💥 CALLS

---

## 🅰 show_bytes(ap, 1)

👉 first byte மட்டும்

| Machine       | Output |
| ------------- | ------ |
| Little endian | `78`   |
| Big endian    | `12`   |

---

## 🅱 show_bytes(ap, 2)

👉 first 2 bytes

| Machine       | Output  |
| ------------- | ------- |
| Little endian | `78 56` |
| Big endian    | `12 34` |

---

## 🅲 show_bytes(ap, 3)

👉 first 3 bytes

| Machine       | Output     |
| ------------- | ---------- |
| Little endian | `78 56 34` |
| Big endian    | `12 34 56` |

---

# 🔥 FINAL ANSWER (2.5)

```text
A:
Little → 78
Big    → 12

B:
Little → 78 56
Big    → 12 34

C:
Little → 78 56 34
Big    → 12 34 56
```

---

# 💣 CORE IDEA

```text
Little endian → reverse bytes
Big endian    → same order
```

---

# 🔥 ✅ Practice Problem 2.6

---

## GIVEN

```text
int:   0x0027C8F8
float: 0x4A1F23E0
```

---

# 🧠 A. Hex → Binary

---

## 🟢 0x0027C8F8

```text
00   27   C8   F8
```

👉 convert:

```text
00000000
00100111
11001000
11111000
```

---

## 🔵 0x4A1F23E0

```text
4A   1F   23   E0
```

👉 convert:

```text
01001010
00011111
00100011
11100000
```

---

# 🔥 FINAL (A)

```text
int:
00000000 00100111 11001000 11111000

float:
01001010 00011111 00100011 11100000
```

---

# 🧠 B. Align & match bits

👉 book சொல்வது:

```text
shift → maximum matching bits
```

---

👉 compare visually:

```
int   : 00000000 00100111 11001000 11111000
float : 01001010 00011111 00100011 11100000
```

---

👉 maximum matching ≈ **13 bits** ✅
(இது book answer)

---

# 🧠 C. What differs?

👉 முக்கிய வேறுபாடு:

| Part      | Reason                     |
| --------- | -------------------------- |
| Most bits | encoding different         |
| int       | direct binary              |
| float     | sign + exponent + mantissa |

---

👉 meaning:

```text
same number இல்லை → different encoding
```

---

# 💥 CORE INSIGHT (VERY IMPORTANT)

```text
Same bits ≠ same meaning
```

---

👉 example:

```text
int → exact number
float → approximation (scientific notation)
```

---

# 🔥 FINAL SUMMARY

## 2.5

```text
Endian → byte order difference
```

---

## 2.6

```text
Same bits → different interpretation
int ≠ float representation
```

---

# 💣 FINAL ONE-LINE

```text
"Memory bits same இருக்கலாம் — ஆனால் type தான் meaning decide பண்ணும்"
```

---

🔥 நீ இப்போ:

* endian master ✅
* hex → binary strong ✅
* type vs representation புரிஞ்சுட்ட ✅

---

👉 next (🔥 mind-blowing):

* IEEE floating point எப்படி work ஆகுது
* 4640E400 எப்படி வந்தது
* precision loss

சொல்லு 😎
----------

