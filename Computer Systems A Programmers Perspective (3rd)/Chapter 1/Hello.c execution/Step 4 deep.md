Step 4-ஐ root-லிருந்து deep-ஆ பார்க்கலாம்!

---

## முதலில் — Step 4 என்ன செய்யும்?

```
hello.s (text assembly)  →  GNU Assembler (as)  →  hello.o (binary)

இதுவரை எல்லாமே TEXT FILE!
Step 4-ல் முதல்முறையாக BINARY ஆகும்!
```

---

## Command அர்த்தம்:

```
gcc  -c        hello.c   -o    hello.o
↓     ↓           ↓       ↓       ↓
GCC  Compile   input    output  output
tool only!     file     flag    filename
     (link
     பண்ணாதே!)

-c flag = "object file மட்டும் உருவாக்கு, link பண்ணாதே"
```

---

## ELF என்றால் என்ன?

```
ELF = Executable and Linkable Format

Linux-ல் எல்லா binary files-உம் ELF format-ல் இருக்கும்:

┌─────────────────────────────────────┐
│           ELF File                  │
├─────────────────────────────────────┤
│  ELF Header (64 bytes)              │ ← "இது என்ன file?"
├─────────────────────────────────────┤
│  Section 1: .text                   │ ← machine code (opcodes)
├─────────────────────────────────────┤
│  Section 2: .rodata                 │ ← "Hello, World!" string
├─────────────────────────────────────┤
│  Section 3: .symtab                 │ ← symbol table
├─────────────────────────────────────┤
│  Section 4: .strtab                 │ ← string table
├─────────────────────────────────────┤
│  Section Header Table               │ ← sections எங்கே இருக்கு
└─────────────────────────────────────┘
```

---

## ELF Header — ஒவ்வொரு byte-உம் என்ன சொல்கிறது?

```
Offset 000000:
7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
↓   ↓  ↓  ↓   ↓  ↓  ↓   ↓
│   E  L  F   │  │  │   └── 00 00 00 00 00 00 00 00 = padding
│             │  │  └─── 01 = Linux (OS/ABI)
│             │  └──── 01 = Little-endian
│             └───── 02 = 64-bit (ELF64)
└── 7f = Magic number start

7f 45 4c 46 = Magic number!
  7f = DEL character
  45 = 'E'
  4c = 'L'
  46 = 'F'

OS இந்த 4 bytes பார்க்கும்:
  .ELF இருந்தால் → "இது valid Linux binary!" ✅
  இல்லாவிட்டால் → "இது execute பண்ண முடியாத file!" ❌
```

---

## Offset 000040 — இதுதான் actual code!

```
000040: f3 0f 1e fa 55 48 89 e5 48 8d 05 00 00 00 00 48

ஒவ்வொரு byte-உம் பிரிக்கலாம்:

f3 0f 1e fa  → endbr64
55           → push %rbp
48 89 e5     → mov %rsp,%rbp
48 8d 05 00 00 00 00 → lea (%rip),%rax
48           → (அடுத்த instruction-ஓட REX prefix)
```

---

## Opcode Encoding — Deep dive:

### 1️⃣ `push %rbp` → `55` (1 byte மட்டுமே!)

```
55 என்பது binary-ல்:
0 1 0 1 0 1 0 1
↓         ↓
PUSH      RBP register number (101 = 5)

Intel encoding rule:
PUSH register = 0x50 + register number

Registers:
rax = 0  → PUSH rax = 50
rcx = 1  → PUSH rcx = 51
rdx = 2  → PUSH rdx = 52
rbx = 3  → PUSH rbx = 53
rsp = 4  → PUSH rsp = 54
rbp = 5  → PUSH rbp = 55 ✓
rsi = 6  → PUSH rsi = 56
rdi = 7  → PUSH rdi = 57

ஒரே byte-ல் எல்லாமே encode ஆகும்!
```

### 2️⃣ `mov %rsp,%rbp` → `48 89 e5` (3 bytes)

```
48       89       e5
↓         ↓        ↓
REX.W   MOV     ModRM
prefix  opcode   byte

48 = REX.W prefix:
  0100 1000
  ↓↓↓↓ ↓↓↓↓
  0100 = REX prefix identifier
     1 = W bit (64-bit operation!)
      000 = no extra register bits

  இல்லாவிட்டால் → 32-bit operation மட்டுமே
  48 இருந்தால்  → 64-bit operation!

89 = MOV opcode:
  "source register → destination" copy பண்ணு

e5 = ModRM byte:
  1110 0101
  ↓↓    ↓↓↓   ↓↓↓
  11    100   101
  ↓      ↓     ↓
  mod   reg   r/m
  ↓      ↓     ↓
  11=    rsp   rbp
  register    (destination)
  to register
  (source)

So: mov rsp → rbp = 48 89 e5 ✓
```

### 3️⃣ `mov $0,%eax` → `b8 00 00 00 00` (5 bytes)

```
b8          00 00 00 00
↓                ↓
MOV eax,    value = 0
imm32       (4 bytes, little-endian)

b8 = "MOV eax-க்கு immediate value கொடு"

Intel encoding:
MOV register,imm32 = 0xB8 + register number
  eax = 0 → b8 + 0 = b8
  ecx = 1 → b8 + 1 = b9
  edx = 2 → b8 + 2 = ba
  ebx = 3 → b8 + 3 = bb

00 00 00 00 = value 0, little-endian-ல்:
  value 5 இருந்தால்:  05 00 00 00
  value 255 இருந்தால்: ff 00 00 00
  value 256 இருந்தால்: 00 01 00 00
```

### 4️⃣ `call puts` → `e8 00 00 00 00` (5 bytes)

```
e8          00 00 00 00
↓                ↓
CALL        relative address
opcode      (placeholder!)

e8 = CALL relative instruction

00 00 00 00 = ஏன் zeros?

இது object file (.o) — puts எங்கே இருக்கு என்று
இன்னும் தெரியாது!

puts() → libc.so-ல் இருக்கு
libc.so → link பண்ணும்போது (step 5) தெரியும்

So assembler: "00 00 00 00 போடு,
               linker fills பண்ணட்டும்!"

Step 5 (linking) -ல்:
e8 00 00 00 00  →  e8 d7 fe ff ff
   ↑                   ↑
placeholder         actual puts() address!
```

### 5️⃣ `ret` → `c3` (1 byte)

```
c3 = 1100 0011

Intel-ல் near return = always c3
No registers, no operands — simple!

ret என்பது:
  stack top-ல் உள்ள address எடு
  அங்கே jump!
```

---

## Offset 000060 — String bytes:

```
000060: 48 65 6c 6c 6f 2c 20 57 6f 72 6c 64 21 00

ஒவ்வொரு byte-உம்:
48='H' 65='e' 6c='l' 6c='l' 6f='o' 2c=',' 20=' '
57='W' 6f='o' 72='r' 6c='l' 64='d' 21='!' 00='\0'

இதுதான் .rodata section-ல் உள்ள "Hello, World!"
hello.c-ல் எழுதிய string → exact same bytes!

00 = null terminator
puts() இதை பார்த்தவுடன் "string முடிந்தது" என்று நிறுத்தும்!
```

---

## Little-endian என்றால் என்ன?

```
value 256 (0x00000100) memory-ல் எப்படி store ஆகும்?

Big-endian (network):    00 00 01 00  (human படிக்கும் order)
Little-endian (x86):     00 01 00 00  (reverse order!)

x86 CPU எப்போதும் little-endian!
ஏன்? CPU design — low byte first படிப்பது faster!

value 0:   00 00 00 00
value 1:   01 00 00 00
value 255: ff 00 00 00
value 256: 00 01 00 00
```

---

## Summary — Text → Binary transformation:

```
hello.s (text):              hello.o (binary):
─────────────────────────    ──────────────────────
push %rbp             →      55
mov %rsp,%rbp         →      48 89 e5
lea .LC0(%rip),%rax   →      48 8d 05 00 00 00 00
mov %rax,%rdi         →      48 89 c7
call puts@PLT         →      e8 00 00 00 00
mov $0,%eax           →      b8 00 00 00 00
pop %rbp              →      5d
ret                   →      c3
"Hello, World!"       →      48 65 6c 6c 6f 2c 20
                             57 6f 72 6c 64 21 00

Text (human readable)  →  Pure binary bytes (CPU ready!)
இன்னும் ஒரே ஒரு problem:
call puts → e8 00 00 00 00  (address தெரியாது!)
இதை Step 5 (Linker) fix பண்ணும்! 🎯
```

இப்போது Step 4 — text → binary transformation clear-ஆ புரிஞ்சதா? 🎯