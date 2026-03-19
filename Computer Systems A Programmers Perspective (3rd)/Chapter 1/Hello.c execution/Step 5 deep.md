Step 5 — Linker deep-ஆ பார்க்கலாம்!

---

## முதலில் — Linker ஏன் தேவை?

```
hello.o-ல் இருக்கும் problem:

call puts → e8 00 00 00 00
               ↑
         இது எங்கே இருக்கு? தெரியாது!

puts() code நம்ம hello.o-ல் இல்லை!
அது /lib/x86_64-linux-gnu/libc.so.6-ல் இருக்கு!

Linker-ஓட வேலை:
"puts எங்கே இருக்கு கண்டுபிடி, address fill பண்ணு!"
```

---

## Command:

```
gcc    hello.c    -o      hello
↓         ↓        ↓        ↓
GCC    input     output   output
tool   file      flag     filename

(GCC internally linker-ஐ call பண்ணும்)
```

---

## Linker என்ன செய்யும் — Step by step:

---

### 1️⃣ எல்லா object files-உம் collect பண்ணும்

```
நம்ம program small — ஆனால் real world-ல்:

main.o  +  utils.o  +  math.o  +  libc.so
  ↓            ↓           ↓          ↓
எல்லாவற்றையும் ஒரே executable-ஆக combine!

நம்ம case:
hello.o  +  libc.so  →  hello
```

---

### 2️⃣ Symbols கண்டுபிடிக்கும்

```
hello.o-ல் உள்ள symbol table:

┌─────────────────────────────────────┐
│ Symbol    │ Status    │ Location    │
├─────────────────────────────────────┤
│ main      │ DEFINED   │ hello.o     │
│ puts      │ UNDEFINED │ ???         │
└─────────────────────────────────────┘

puts = UNDEFINED!
Linker: "puts எங்கே இருக்கு தேடு..."
libc.so-ல் பார்க்கும்:
┌─────────────────────────────────────┐
│ Symbol    │ Status    │ Location    │
├─────────────────────────────────────┤
│ puts      │ DEFINED   │ libc.so     │ ✅ கண்டுபிடித்தது!
│ printf    │ DEFINED   │ libc.so     │
│ scanf     │ DEFINED   │ libc.so     │
└─────────────────────────────────────┘
```

---

### 3️⃣ Addresses resolve பண்ணும்

```
Before linking:
┌─────────────────────────────────────┐
│ hello.o                             │
│                                     │
│ 0x00: endbr64                       │
│ 0x04: push %rbp                     │
│ 0x05: mov %rsp,%rbp                 │
│ 0x08: lea (%rip),%rax               │
│ 0x0f: mov %rax,%rdi                 │
│ 0x12: call e8 00 00 00 00 ← EMPTY! │
│ 0x17: mov $0,%eax                   │
│ 0x1c: pop %rbp                      │
│ 0x1d: ret                           │
└─────────────────────────────────────┘

After linking:
┌─────────────────────────────────────┐
│ hello (executable)                  │
│                                     │
│ 0x1050: endbr64                     │ ← real address!
│ 0x1054: push %rbp                   │
│ 0x1055: mov %rsp,%rbp               │
│ 0x1058: lea (%rip),%rax             │
│ 0x105f: mov %rax,%rdi               │
│ 0x1062: call e8 d7 fe ff ff ← FILLED!│
│ 0x1067: mov $0,%eax                 │
│ 0x106c: pop %rbp                    │
│ 0x106d: ret                         │
└─────────────────────────────────────┘

e8 00 00 00 00  →  e8 d7 fe ff ff
   ↑                    ↑
placeholder          puts@PLT actual address!
```

---

### 4️⃣ PLT / GOT உருவாக்கும்

```
PLT = Procedure Linkage Table
GOT = Global Offset Table

ஏன் இரண்டு tables தேவை?

libc.so = shared library
  ஒரே libc.so → 100 programs share பண்ணும்!
  ஒவ்வொரு program-க்கும் copy வேண்டாம்!

Problem:
  libc.so எந்த address-ல் load ஆகும் என்று
  compile time-ல் தெரியாது!
  Runtime-ல் OS decide பண்ணும்!

Solution: PLT + GOT

Program → PLT[puts] → GOT[puts] → libc.so puts()
              ↑              ↑
         fixed address   runtime-ல் filled!

┌─────────────────────────────────────────┐
│ PLT (code section)                      │
│ puts@PLT:                               │
│   jmp  GOT[puts]    ← GOT-ல் பார்       │
│   push index                            │
│   jmp  resolver     ← first time மட்டும்│
└─────────────────────────────────────────┘
              ↓
┌─────────────────────────────────────────┐
│ GOT (data section)                      │
│ GOT[puts] = 0x7f8b4c2a1230  ← libc puts│
└─────────────────────────────────────────┘
```

---

### 5️⃣ ELF Sections combine பண்ணும்

```
hello.o sections:        hello executable sections:
┌──────────────┐         ┌──────────────────────────┐
│ .text        │    →    │ .text (all code combined) │
├──────────────┤         ├──────────────────────────┤
│ .rodata      │    →    │ .rodata (all strings)     │
├──────────────┤         ├──────────────────────────┤
│ .symtab      │    →    │ .symtab (resolved!)       │
└──────────────┘         ├──────────────────────────┤
                         │ .plt (new! PLT table)     │
libc.so:                 ├──────────────────────────┤
┌──────────────┐         │ .got (new! GOT table)     │
│ puts code    │    →    ├──────────────────────────┤
│ printf code  │         │ .dynamic (shared lib info)│
└──────────────┘         └──────────────────────────┘
```

---

## Final Executable — hello:

```
./hello run பண்ணும்போது:

┌─────────────────────────────────────────┐
│          OS (kernel)                    │
│                                         │
│  1. hello ELF file திறக்கும்            │
│  2. Magic number check: 7f 45 4c 46 ✅  │
│  3. Sections RAM-ல் load பண்ணும்:       │
│                                         │
│  RAM:                                   │
│  ┌─────────────────────────────────┐    │
│  │ .text   → code load            │    │
│  │ .rodata → "Hello,World!" load  │    │
│  │ .plt    → PLT load             │    │
│  │ .got    → GOT load             │    │
│  └─────────────────────────────────┘    │
│                                         │
│  4. libc.so RAM-ல் load பண்ணும்         │
│  5. GOT-ல் puts() address fill பண்ணும் │
│  6. main() address-க்கு jump!           │
└─────────────────────────────────────────┘
         ↓
   CPU execute ஆரம்பிக்கும்!
         ↓
   Terminal: Hello, World! ✅
```

---

## Complete Journey — Final Summary:

```
hello.c                     (ASCII text — disk)
    ↓ Preprocessor
hello.i                     (expanded C — disk)
    ↓ Compiler (cc1)
hello.s                     (assembly text — disk)
    ↓ Assembler (as)
hello.o                     (ELF binary, incomplete)
    │   puts = 00 00 00 00  (placeholder)
    ↓ Linker (ld)
hello                       (ELF binary, complete!)
    │   puts = d7 fe ff ff  (real address!)
    │   PLT/GOT added
    ↓ OS loads
RAM                         (sections loaded)
    │   .text → CPU executes
    │   .rodata → "Hello,World!" ready
    │   libc.so → puts() connected
    ↓ CPU
Terminal: Hello, World! ✅

Total transformation:
  6 lines C code
  → 800 lines preprocessed
  → 15 lines assembly
  → 1504 bytes object file
  → 16KB executable
  → RAM-ல் load
  → CPU opcodes execute
  → 1 line output! 🎯
```

இப்போது Linker + Executable — எல்லாமே clear-ஆ புரிஞ்சதா? 🎯