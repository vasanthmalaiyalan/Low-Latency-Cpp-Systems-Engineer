இரண்டு நல்ல கேள்விகள்! ஒவ்வொன்னா பார்க்கலாம்.

---

## கேள்வி 1: ஏன் `.c` பயன்படுத்துகிறோம், `.i` பயன்படுத்தலாமே?

**நீங்கள் சொன்னது 100% சரி!** `.i` பயன்படுத்தலாம்:

```bash
gcc -S hello.c -o hello.s    # இதுவும் work ஆகும்
gcc -S hello.i -o hello.s    # இதுவும் same result!
```

**GCC internally என்ன செய்யும்:**
```
gcc -S hello.c கொடுத்தால்:
    hello.c → [preprocessing] → hello.i → [compilation] → hello.s
                ↑
            Automatically இதை செய்துவிடும்!

gcc -S hello.i கொடுத்தால்:
    hello.i → [compilation] → hello.s
              ↑
          Preprocessing skip! Direct போகும்
```

நாம் தனியா `-E` run பண்ணியது **படிக்கவும் புரியவும்** — real world-ல் நேரடியாக `gcc hello.c` என்று ஒரே command போதும்!

---

## கேள்வி 2: நம்ம `.c` code exact-ஆ இந்த assembly-ஆக மாறுமா?**ஆமா — exact-ஆ இதுதான்!** இப்போதுதான் run பண்ணினேன். இரண்டையும் compare பண்ணலாம்:

---

## நம்ம hello.c → Assembly mapping:

```c
// hello.c                    // hello.s
#include <stdio.h>        →   .section .rodata
                              .LC0: .string "Hello, World!"
                              ↑ string எங்கே store ஆகும்

int main() {              →   main:
                              endbr64        ; function start

    printf("Hello!\n");   →   leaq .LC0(%rip), %rax  ; string address load
                              movq %rax, %rdi          ; argument ready
                              call puts@PLT            ; printf → puts call

    return 0;             →   movl $0, %eax           ; return value = 0
}                         →   popq %rbp
                              ret                      ; function end
```

---

## Extra lines என்ன? — நீங்கள் கவலைப்படலாம்

Document-ல் காட்டியதை விட extra lines இருக்கு:

```asm
.cfi_startproc
.cfi_def_cfa_offset 16
.cfi_offset 6, -16
.LFB0:
.LFE0:
```

இவை எல்லாம் **debug information** — crash ஆனால் எங்கே ஆனது என்று கண்டுபிடிக்க. CPU execute செய்வதில்லை — ignore செய்யலாம்.

---

## ஒரு முக்கியமான விஷயம் — `printf` → `puts` ஆனது ஏன்?

```c
// நாம் எழுதியது:
printf("Hello, World!\n");

// Assembly-ல் வந்தது:
call puts@PLT              // printf இல்ல, puts!
```

**GCC smart-ஆக optimize பண்ணியது!**

```
printf("Hello!\n")  →  format specifier இல்ல (%d, %s இல்ல)
                    →  simple string மட்டுமே
                    →  puts() போதும்! (faster, simpler)
                    →  GCC automatically மாற்றியது
```

நாம் `printf` எழுதினோம், GCC `puts`-ஆக மாற்றியது — **compiler நம்மை விட smart!**

---

## Summary

```
.c → .i → .s என்பது:

hello.c (human language)
    "printf Hello World, return 0"
         ↓ GCC cc1 compiler
hello.s (CPU near language)  
    "string இங்கே வை, address load பண்ணு,
     puts call பண்ணு, 0 return பண்ணு"
         ↓
இன்னும் text — ஆனால் CPU instructions ready!
Binary இல்லை — அது step 4-ல்!
```

Step 3 clear-ஆ புரிஞ்சதா? 🎯


--------

ஒவ்வொரு word-உம் deep-ஆ பார்க்கலாம்!

---

## முதலில் — Big Picture

Assembly என்பது **CPU-ஓட native language** — English மாதிரி CPU-க்கு. ஒவ்வொரு line-உம் CPU-க்கு ஒரு instruction.

---

## 1️⃣ `.section .rodata`

```asm
.section .rodata
```

| Word | அர்த்தம் |
|---|---|
| `.section` | "இனிமேல் வரும் data இந்த பகுதியில் வை" |
| `.rodata` | **R**ead **O**nly **Data** — மாற்ற முடியாத data |

**ஏன் read only?**
```
"Hello, World!" → நாம் மாற்றவே மாட்டோம்
                → CPU தவறாக மாற்றிவிடக் கூடாது
                → So read only section-ல் வை!
```

---

## 2️⃣ `.LC0: .string "Hello, World!"`

```asm
.LC0:   .string   "Hello, World!"
 ↓         ↓            ↓
Label    directive     actual string
```

| Word | அர்த்தம் |
|---|---|
| `.LC0` | **L**ocal **C**onstant **0** — இந்த string-ஓட address-க்கு ஒரு name/label |
| `.string` | "இந்த text-ஐ bytes-ஆக memory-ல் வை" |
| `"Hello, World!"` | actual content |

**Label என்றால்:**
```
Memory address 0x560 → "Hello, World!" bytes இருக்கும்
.LC0              → அந்த 0x560 address-க்கு நாம் வைத்த nickname!

எங்கும் .LC0 பயன்படுத்தினால் → 0x560 என்று அர்த்தம்
```

---

## 3️⃣ `main:`

```asm
main:
```

**உன் `int main()` இதுவே!**

```
C:         int main() {
Assembly:  main:          ← function start-ஓட label
```

OS program run பண்ணும்போது `main:` label-ஐ தேடும் — இங்கிருந்து execute ஆரம்பிக்கும்.

---

## 4️⃣ `endbr64`

```asm
endbr64
```

| Word | அர்த்தம் |
|---|---|
| `end` | **End**point |
| `br` | **Br**anch |
| `64` | 64-bit mode |

**Security feature** — hacker தவறான இடத்தில் jump பண்ண முயன்றால் CPU block செய்யும். நம்ம code-க்கு directly தேவையில்லை — GCC automatically போடும்.

---

## 5️⃣ Registers — `%rbp`, `%rsp`, `%rax`, `%rdi`, `%rip`

Register என்பது **CPU-ஓட உள்ளே இருக்கும் tiny memory boxes** — RAM இல்ல, CPU chip-லேயே இருக்கும்:

```
CPU chip உள்ளே:
┌─────┬─────┬─────┬─────┬─────┐
│ rax │ rbp │ rsp │ rdi │ rip │  ← registers (64-bit boxes)
└─────┴─────┴─────┴─────┴─────┘
  8B    8B    8B    8B    8B
```

| Register | Full name | Role | நம்ம code-ல் |
|---|---|---|---|
| `%rbp` | Base Pointer | Stack frame-ஓட bottom | Function start/end-ல் save/restore |
| `%rsp` | Stack Pointer | Stack-ஓட current top | Frame உருவாக்க |
| `%rax` | Accumulator | Calculation + return value | String address வைக்க, 0 return |
| `%rdi` | Destination Index | Function-ஓட 1st argument | puts()-க்கு string pass பண்ண |
| `%rip` | Instruction Pointer | இப்போது execute ஆகும் instruction address | String-ஓட relative address கண்டுபிடிக்க |

**Stack என்றால்:**
```
RAM-ல் ஒரு பகுதி — function variables, return address வைக்கும் இடம்
Plate stack மாதிரி — மேலே போடுவோம், மேலிருந்து எடுப்போம்

rsp → எப்போதும் stack-ஓட top-ஐ point செய்யும்
rbp → இந்த function-ஓட bottom-ஐ point செய்யும்
```

---

## 6️⃣ `pushq %rbp`

```asm
pushq %rbp
```

| Word | அர்த்தம் |
|---|---|
| `push` | Stack-ல் மேலே வை |
| `q` | **Q**uad word = 8 bytes (64-bit) |
| `%rbp` | Base pointer-ஓட current value |

**என்ன நடக்கும்:**
```
main() call ஆகுமுன்:  stack-ல் caller function-ஓட rbp இருக்கும்
main() ஆரம்பத்தில்:   அந்த பழைய rbp-ஐ save பண்ணணும்
                       → pushq %rbp → stack-ல் வை!

ஏன்? main() முடிந்தவுடன் caller-க்கு திரும்ப போக!
```

---

## 7️⃣ `movq %rsp, %rbp`

```asm
movq %rsp, %rbp
```

| Word | அர்த்தம் |
|---|---|
| `mov` | Move — copy பண்ணு |
| `q` | 64-bit (8 bytes) |
| `%rsp` | Source — stack pointer |
| `%rbp` | Destination — base pointer |

```
rsp-ஓட value → rbp-க்கு copy

இப்போது rbp = rsp = stack top
→ இதுதான் main()-ஓட stack frame base!
```

---

## 8️⃣ `leaq .LC0(%rip), %rax`

```asm
leaq .LC0(%rip), %rax
```

| Word | அர்த்தம் |
|---|---|
| `lea` | **L**oad **E**ffective **A**ddress — address-ஐ load பண்ணு |
| `q` | 64-bit |
| `.LC0(%rip)` | rip register-ஐ base-ஆக வைத்து .LC0-ஓட address கணக்கிடு |
| `%rax` | அந்த address-ஐ rax-ல் வை |

```
rip = இப்போது CPU எந்த instruction execute பண்றது
.LC0 = "Hello, World!" இருக்கும் இடம்

leaq = "Hello, World!" string-ஓட address → rax-ல் வை
```

**mov vs lea வித்தியாசம்:**
```
mov → அந்த address-ல் உள்ள VALUE எடு
lea → அந்த ADDRESS-ஐயே எடு (value வேண்டாம்)

நமக்கு string-ஓட address வேணும் → lea!
```

---

## 9️⃣ `movq %rax, %rdi`

```asm
movq %rax, %rdi
```

```
rax-ல் இருக்கும் string address → rdi-க்கு copy

ஏன்? Linux rule:
  puts("Hello") call பண்ணும்போது
  1st argument எப்போதும் rdi-ல் இருக்கணும்!

rdi = puts()-க்கு "இந்த string print பண்ணு" என்று சொல்வது
```

---

## 🔟 `call puts@PLT`

```asm
call puts@PLT
```

| Word | அர்த்தம் |
|---|---|
| `call` | Function-ஐ call பண்ணு, முடிந்தவுடன் திரும்பு |
| `puts` | Print string function (printf-ஐ GCC optimize பண்ணியது) |
| `@PLT` | **P**rocedure **L**inkage **T**able — puts எங்கே இருக்கு என்று linker கண்டுபிடிக்கும் |

```
PLT என்றால்:
puts() code நம்ம program-ல் இல்ல → libc.so-ல் இருக்கு
PLT = "linker, puts-ஐ கண்டுபிடி, connect பண்ணு"
```

---

## 1️⃣1️⃣ `movl $0, %eax`

```asm
movl $0, %eax
```

| Word | அர்த்தம் |
|---|---|
| `mov` | Copy |
| `l` | **L**ong = 4 bytes (32-bit) |
| `$0` | `$` = literal number, `0` = value zero |
| `%eax` | rax-ஓட lower 32 bits |

```
return 0; → இதுதான் assembly-ல்!
eax = 0 → "program successfully முடிந்தது" என்று OS-க்கு சொல்வது
```

---

## 1️⃣2️⃣ `popq %rbp`

```asm
popq %rbp
```

```
push-ஓட opposite!
Stack-ல் save பண்ணிய பழைய rbp-ஐ திரும்ப எடு
→ Caller function-ஓட stack frame restore!
```

---

## 1️⃣3️⃣ `ret`

```asm
ret
```

**Return** — function முடிந்தது:
```
Stack-ல் இருக்கும் return address எடு
→ அந்த address-க்கு jump
→ main()-ஐ call பண்ணிய OS-க்கு திரும்பு
→ Program exit!
```

---

## Full Flow — ஒரே பார்வையில்

```
main: ஆரம்பம்
  endbr64          → Security check
  pushq %rbp       → பழைய frame save
  movq %rsp,%rbp   → புதிய frame உருவாக்கு
  
  leaq .LC0,%rax   → "Hello,World!" address → rax
  movq %rax,%rdi   → address → rdi (argument ready)
  call puts@PLT    → puts("Hello,World!") call!
                      → Terminal-ல் print ஆகும்!
  
  movl $0,%eax     → return value = 0
  popq %rbp        → பழைய frame restore
  ret              → OS-க்கு திரும்பு
```

இப்போது clear-ஆ புரிஞ்சதா? 🎯