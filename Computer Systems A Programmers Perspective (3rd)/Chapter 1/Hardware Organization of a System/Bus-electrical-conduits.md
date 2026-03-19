இந்த section-ல buses பத்தி deeper-ஆ போகிறோம். முன்னாடி high-level-ல பாத்தோம் — இப்போ "word size" concept-ஓட real implications deep-ஆ புரிஞ்சுக்கலாம்.

---

# Buses — Electrical conduits

"Electrical conduits" னு book சொல்றது — literally copper wires/traces on motherboard. உன் motherboard-ல பாத்தா thin parallel lines தெரியும் — அவைதான் buses.

```
Motherboard cross-section (simplified):
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
[CPU]────────────────────────[RAM]
  │ ← 64 parallel copper traces → │
  │   (64-bit bus = 64 wires)      │
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

64-bit bus = 64 wires parallel-ஆ run ஆகுது. ஒரே clock cycle-ல 64 bits (8 bytes) simultaneously travel பண்றது — serial-ஆ அல்ல, **parallel**.

---

# Word Size — இதுதான் key concept

**Word** = CPU-ரோட natural unit of data. ஒரு operation-ல CPU எவ்வளவு bits process பண்ணும் னு define பண்றது.

## 32-bit vs 64-bit — real difference என்ன?

```
32-bit system (old):
  Word size = 4 bytes = 32 bits
  Max RAM addressable = 2^32 = 4,294,967,296 bytes = 4 GB
  Register size = 32 bits
  Pointer size = 4 bytes

64-bit system (modern):
  Word size = 8 bytes = 64 bits
  Max RAM addressable = 2^64 = 16 Exabytes (practically ~256 TB)
  Register size = 64 bits
  Pointer size = 8 bytes
```

இதனால 32-bit OS-ல 4GB-க்கு மேல RAM போட்டாலும் use ஆகாது — address space தீர்ந்துவிடும். 64-bit OS-ல இந்த problem இல்ல.

## Pointer size — C-ல practical impact

```c
#include <stdio.h>

int main() {
    int x = 42;
    int *ptr = &x;

    printf("Size of int:     %zu bytes\n", sizeof(int));
    printf("Size of pointer: %zu bytes\n", sizeof(ptr));
    printf("Size of long:    %zu bytes\n", sizeof(long));

    return 0;
}
```

64-bit Linux-ல output:

```
Size of int:     4 bytes
Size of pointer: 8 bytes   ← word size!
Size of long:    8 bytes   ← word size!
```

32-bit system-ல:

```
Size of int:     4 bytes
Size of pointer: 4 bytes   ← word size!
Size of long:    4 bytes
```

Pointer = memory address. Address-ஐ store பண்ண word size தேவைப்படுது. 64-bit system-ல every pointer = 8 bytes.

---

# Word size — Bus transfer-ல என்ன ஆகுது

```
CPU wants: RAM[0x7fff1000] → register

64-bit bus ஒரு trip-ல transfer பண்றது:
┌────────────────────────────────────────┐
│ 0x7fff1000                             │
│ [B0][B1][B2][B3][B4][B5][B6][B7]      │
│  ←────────── 8 bytes ────────────→     │
│  ←──── one word, one bus cycle ─────→  │
└────────────────────────────────────────┘

char c = RAM[0x7fff1000] போட்டாலும்:
  Bus still 8 bytes transfer பண்றது
  CPU 1 byte மட்டும் use பண்றது
  7 bytes discard — bus always word-aligned transfer
```

இதனால **data alignment** matter ஆகுது:

```c
// Well-aligned: int at address divisible by 4
int x;  // address = 0x1000 (divisible by 4) ✓
        // One bus read-ல போதும்

// Misaligned: int spanning two words
// address = 0x1003 (not divisible by 4) ✗
// Two bus reads தேவைப்படும் — slower!
```

Modern compilers automatically align data. `struct`-ல padding போடுவது இதனாலதான்:

```c
struct Example {
    char a;    // 1 byte  → address 0
    // 3 bytes padding! ← compiler add பண்றது
    int  b;    // 4 bytes → address 4 (aligned)
    char c;    // 1 byte  → address 8
    // 7 bytes padding!
    long d;    // 8 bytes → address 16 (aligned)
};
// Total: 24 bytes (not 14!)
```

`sizeof(struct Example) = 24` — 10 bytes of actual data, 10 bytes of padding!

---

# "We do not assume any fixed definition of word size" — Book ஏன் இப்படி சொல்றது?

CS:APP-ல examples 32-bit-உம் 64-bit-உம் mix-ஆ இருக்கும். ஒரே concept-ஐ both architectures-ல explain பண்றதால் "word" fixed-ஆ define பண்ணாம, context பொறுத்து சொல்வோம் னு சொல்றாங்க.

Real-world-ல இது matter ஆகுற ஒரு example:

```c
// 32-bit system-ல safe:
int count = 2000000000; // 2 billion — fits in 32-bit int

// 64-bit system-ல long use பண்ணணும் for large numbers:
long big = 9000000000L; // 9 billion — needs 64 bits

// Portable code எழுத:
#include <stdint.h>
int32_t  always_32bit;  // exactly 32 bits, any platform
int64_t  always_64bit;  // exactly 64 bits, any platform
size_t   pointer_sized; // platform word size (32 or 64)
```

Node.js-ல இந்த concept:

```javascript
// JavaScript numbers = 64-bit floating point always
// But for binary data:
const buf = Buffer.alloc(8);
buf.writeInt32LE(42, 0);  // 32-bit write
buf.writeBigInt64LE(9000000000n, 0); // 64-bit write

// Node.js native addons (.node files) C-ல எழுதும்போது
// word size matter ஆகுது
```

---

# Bus speed vs CPU speed — The real bottleneck

```
Modern CPU clock: 3-5 GHz = 3-5 billion cycles/second
Memory bus clock: 3200-6400 MHz (DDR4/DDR5)

ஆனா effective bandwidth:
CPU-க-Cache (L1): ~1 TB/s
Cache-to-RAM:      ~50 GB/s
RAM-to-Disk:       ~500 MB/s (SATA SSD)
                   ~3-7 GB/s (NVMe SSD)

Gap ratio:
L1 cache vs RAM:  20x faster
RAM vs NVMe:      10-100x faster
RAM vs HDD:       100,000x faster
```

Bus bandwidth = word size × bus frequency:

```
DDR4-3200: 64-bit bus × 3200 MHz × 2 (double data rate)
         = 8 bytes × 3,200,000,000 × 2
         = ~51 GB/s theoretical peak

Real-world: ~40 GB/s (overhead காரணமா)
```

இதனால cache இல்லன்னா modern 5GHz CPU-வே memory wait-ல **70-80% time waste** பண்ணும். CPU fast, bus slow — cache இந்த gap-ஐ hide பண்றது.

---

# Book-ரோட "don't worry about complexity" — ஏன் சொல்றாங்க?

Figure 1.4-ல பாத்தா: CPU, ALU, PC, Register file, Bus interface, System bus, Memory bus, I/O bridge, I/O bus, USB controller, Graphics adapter, Disk controller — எல்லாம் ஒரே diagram-ல இருக்கு.

Book இதை one shot-ல explain பண்ண try பண்றதில்லை. Chapter-by-chapter:

```
Chapter 1  → Big picture (இப்போ இங்கே இருக்கோம்)
Chapter 3  → CPU internals, registers, ALU deep
Chapter 5  → Memory performance, optimization
Chapter 6  → Cache hierarchy, bus bandwidth
Chapter 9  → Virtual memory, address translation
Chapter 10 → I/O, system calls
```

ஒவ்வொரு chapter-உம் Figure 1.4-ரோட ஒரு part-ஐ zoom-in பண்ணும். End-ல whole picture crystal clear ஆகும்.

அடுத்து 1.4.2 — `./hello` run பண்ணும்போது இந்த hardware-ல step-by-step என்ன நடக்குது னு போகலாம். DMA + fetch-decode-execute cycle அங்க வருது.
