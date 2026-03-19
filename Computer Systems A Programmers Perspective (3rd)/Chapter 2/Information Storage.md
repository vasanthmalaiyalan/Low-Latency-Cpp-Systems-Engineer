# 2.1 Information Storage

## Smallest addressable unit = Byte

Individual bits access பண்றதில்லை. **8 bits = 1 byte** = smallest addressable unit.

```
1 bit:   0 or 1
1 byte:  8 bits → 00000000 to 11111111 (0 to 255)
```

---

## Virtual memory — machine-level program-ரோட view

```
Program-க்கு memory தெரியுற விதம்:

Address:  0x00  0x01  0x02  0x03  ...  0xFF...FF
          [byte][byte][byte][byte] ... [byte]

→ Very large array of bytes
→ Every byte has unique address
→ Set of all addresses = Virtual Address Space
```

**"Virtual"** ஏன்னா — இது just a conceptual image. Actual implementation = DRAM + flash + disk + hardware + OS software combine பண்ணி இந்த illusion கொடுக்குது. (Chapter 9-ல deep explanation.)

---

## Pointer in C — virtual address

```c
int x = 42;
int *ptr = &x;

// ptr-ரோட value = virtual address of first byte of x
// e.g., ptr = 0x7fff1000

// Compiler: ptr is int* → access 4 bytes from that address
// Machine level: just bytes — no type info!
```

Book-ரோட exact point:

- C compiler → type information maintain பண்றது (`int*`, `float*`)
- Machine-level program → **no type information** — everything = block of bytes
- Program itself = sequence of bytes

---

## C versions — gcc flags

```bash
gcc prog.c              # GNU89 (default) + some C99/C11 features
gcc -ansi prog.c        # ANSI / ISO C90
gcc -std=c99 prog.c     # ISO C99
gcc -std=c11 prog.c     # ISO C11
gcc -std=gnu11 prog.c   # ISO C11 + GNU extensions
```

---

## Pointer — two aspects (book சொல்றது)

```
Pointer has:
  Value → location of some object (virtual address)
  Type  → what kind of object stored there (int, float...)
```

Pointers fully understand பண்ண → machine-level representation பாக்கணும். Chapter 3, Section 3.10.1-ல in-depth வரும்.

---

அடுத்து 2.1.1 (Hexadecimal Notation) போகலாமா?