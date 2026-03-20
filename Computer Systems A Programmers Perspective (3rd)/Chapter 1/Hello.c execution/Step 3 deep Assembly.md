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

--------

Stack frame concept-ஐ முதலில் புரிஞ்சுக்கோம், அப்புறம் full flow பார்க்கலாம்!

---

## முதலில் — Stack என்றால் என்ன?

```
RAM-ல் ஒரு பகுதி — Stack:

High address  ┌─────────────┐
              │  OS / Kernel │
              ├─────────────┤
              │    Stack     │ ← கீழே வளரும் (↓)
              │      ↓       │
              │    (empty)   │
              │      ↑       │
              │    Heap      │ ← மேலே வளரும்
              ├─────────────┤
              │ Code (.text) │
Low address   └─────────────┘
```

**Plate stack மாதிரி** — மேலே போடுவோம், மேலிருந்து எடுப்போம்.

---

## Frame என்றால் என்ன?

**ஒவ்வொரு function-க்கும் stack-ல் தனி பகுதி = Frame**

```
OS → main() call பண்ணும்
main() → puts() call பண்ணும்

Stack-ல்:
┌─────────────────┐  ← rsp (top)
│   puts() frame  │  ← புதியது மேலே
├─────────────────┤
│   main() frame  │  ← இது "பழைய frame"
├─────────────────┤
│    OS frame     │  ← main-ஐ call பண்ணியது
└─────────────────┘
```

---

## இப்போது Full Flow — Deep explanation:

---

### `endbr64` → Security Check

```
Intel CET (Control-flow Enforcement Technology)

Hacker attack type:
  program-ஓட random இடத்தில் jump பண்ணி
  தன்னுடைய malicious code run பண்ணுவான்

endbr64 என்பது:
  "இந்த இடம் valid function entry point"
  என்று CPU-க்கு mark பண்றது

Valid jump:   endbr64 உள்ள இடம் → ✅ allowed
Invalid jump: endbr64 இல்லாத இடம் → ❌ CPU block!
```

---

### `pushq %rbp` → பழைய Frame Save

**பழைய frame என்றால் OS-ஓட frame!**

```
main() call ஆவதற்கு முன்:

Stack:                    Registers:
┌─────────────┐           rbp → OS frame-ஓட base
│  OS frame   │           rsp → stack top (இங்கே point)
│  ...        │
│  rbp value  │ ← rbp இப்போது இதை point செய்கிறது
└─────────────┘

pushq %rbp என்பது:
  rsp ஒரு step கீழே போகும் (8 bytes)
  அந்த இடத்தில் rbp-ஓட value copy ஆகும்

After pushq:
┌─────────────┐  ← rsp (new top)
│ [old rbp]   │  ← OS frame base address save ஆச்சு!
├─────────────┤
│  OS frame   │
└─────────────┘

ஏன் save பண்றோம்?
main() முடிந்தவுடன் OS-க்கு திரும்ப போக வேண்டும்
OS-ஓட frame address தெரியணும்
அதனால save பண்றோம்!
```

---

### `movq %rsp, %rbp` → புதிய Frame உருவாக்கு

**புதிய frame என்றால் main()-ஓட frame!**

```
Before:
  rsp → stack top (old rbp save ஆன இடம்)
  rbp → இன்னும் OS frame-ஐ point செய்கிறது

movq %rsp, %rbp:
  rsp-ஓட value → rbp-க்கு copy

After:
  rsp → stack top
  rbp → same இடம்! (rsp == rbp இப்போது)

┌─────────────┐  ← rsp AND rbp இரண்டும் இங்கே!
│ [old rbp]   │  ← main() frame ஆரம்பம்
├─────────────┤
│  OS frame   │
└─────────────┘

இதுதான் main()-ஓட புதிய frame base!
இனிமேல் main()-ஓட variables இங்கிருந்து கீழே போகும்
```

---

### `leaq .LC0(%rip), %rax` → String Address Load

```
Memory layout:
┌──────────────────────────────┐
│ .rodata section              │
│ address 0x2004: H e l l o ,  │
│          W o r l d ! \0      │
│          ↑                   │
│         .LC0 label இங்கே     │
└──────────────────────────────┘

rip = இப்போது CPU இந்த instruction-ஐ execute பண்றது
      அந்த instruction-ஓட address

leaq .LC0(%rip):
  rip + offset = .LC0-ஓட exact address கணக்கிடு
  அந்த address → rax-ல் வை

After:
  rax = 0x2004 (string இருக்கும் address)

ஏன் value இல்ல, address?
  puts() function-க்கு string-ஐ கொடுக்க வேண்டும்
  string முழுக்க copy பண்றது slow
  address மட்டும் கொடுத்தால் puts() நேரடியா படிக்கும்!
```

---

### `movq %rax, %rdi` → Argument Ready

```
Linux function call rule (System V AMD64 ABI):
  1st argument → rdi
  2nd argument → rsi
  3rd argument → rdx
  ... and so on

puts("Hello, World!") call பண்ண:
  "Hello, World!"-ஓட address → rdi-ல் இருக்கணும்

rax-ல் address இருக்கு → rdi-க்கு copy!

After:
  rdi = 0x2004 (string address)
  puts() call ஆனதும் rdi-ல் பார்க்கும் → string கிடைக்கும்!
```

---

### `call puts@PLT` → puts() Call!

```
call என்பது 2 காரியம் செய்யும்:

1. Return address save:
   ┌─────────────────┐  ← rsp
   │ return address  │  ← "puts முடிந்தவுடன் இங்கே வா"
   ├─────────────────┤
   │ [old rbp]       │
   ├─────────────────┤
   │  OS frame       │
   └─────────────────┘

2. puts() code-க்கு jump!

@PLT என்றால்:
  puts() code நம்ம program-ல் இல்ல
  libc.so என்ற external library-ல் இருக்கு

PLT (Procedure Linkage Table):
  program → PLT → "puts எங்கே இருக்கு?" → libc.so → puts() run!

puts() உள்ளே:
  rdi-ல் உள்ள address படிக்கும்
  "Hello, World!" bytes ஒவ்வொன்னா எடுக்கும்
  Linux write() syscall பண்ணும்
  Kernel terminal-ல் print பண்ணும்
  Terminal-ல் தெரியும்: Hello, World!
```

---

### `movl $0, %eax` → Return Value

```
return 0; → இதுதான்!

Linux rule:
  program successfully முடிந்தால் → 0 return
  error ஆனால்              → non-zero return

eax = 0 → OS-க்கு "சரியாக முடிந்தது" சொல்வது

Terminal-ல் check பண்ணலாம்:
  ./hello
  echo $?    → 0 வரும்! (success)
```

---

### `popq %rbp` → பழைய Frame Restore

```
pushq-ஓட exact opposite!

Before pop:
┌─────────────────┐  ← rsp
│ [old rbp value] │  ← save பண்ணியது
├─────────────────┤
│  OS frame       │
└─────────────────┘

popq %rbp:
  stack top-ல் உள்ள value எடு → rbp-க்கு போடு
  rsp ஒரு step மேலே போகும்

After pop:
┌─────────────────┐  ← rsp
│  OS frame       │
└─────────────────┘
  rbp → OS frame-ஐ மீண்டும் point செய்கிறது!

main()-ஓட frame clean ஆச்சு!
```

---

### `ret` → OS-க்கு திரும்பு

```
ret என்பது:
  stack top-ல் உள்ள return address எடு
  அந்த address-க்கு jump!

Stack-ல் இருப்பது:
┌─────────────────┐  ← rsp
│ return address  │  ← OS-ஓட "main முடிந்தவுடன் இங்கே வா"
└─────────────────┘

ret → அந்த address-க்கு jump → OS-க்கு திரும்பு
OS → eax-ல் 0 பார்க்கும் → "program success!" → process kill
```

---

## Complete Stack Visual — Full Flow

```
Program start:
════════════════════════════════
OS frame இருக்கு, main() call பண்றது

After pushq %rbp:
┌──────────────┐ ← rsp
│ OS rbp value │ saved!
├──────────────┤
│   OS frame   │
└──────────────┘

After movq %rsp,%rbp: (main frame ready)
┌──────────────┐ ← rsp = rbp (main frame base)
│ OS rbp value │
├──────────────┤
│   OS frame   │
└──────────────┘

After call puts: (puts frame உருவாகும்)
┌──────────────┐ ← rsp
│  puts frame  │ puts() running...
│return address│ "Hello, World!" print ஆகும்!
├──────────────┤ ← rbp
│ OS rbp value │
├──────────────┤
│   OS frame   │
└──────────────┘

After puts returns:
┌──────────────┐ ← rsp = rbp
│ OS rbp value │ puts frame gone!
├──────────────┤
│   OS frame   │
└──────────────┘

After popq %rbp + ret:
┌──────────────┐
│   OS frame   │ ← main frame gone! OS-க்கு திரும்பியது
└──────────────┘
════════════════════════════════
Program exit! eax=0 → success!
```

இப்போது frame என்றால் என்ன, பழையது புதியது என்றால் என்ன — clear-ஆ புரிஞ்சதா? 🎯

----

ஒவ்வொன்னா deep-ஆ பார்க்கலாம்!

---

## 1️⃣ Stack vs Heap — என்ன வித்தியாசம்?

```
RAM Layout:
┌─────────────────────────┐ High address
│      OS / Kernel        │
├─────────────────────────┤
│                         │
│   Stack  ↓ கீழே வளரும் │ ← automatic, fast
│   ┌─────────────────┐   │
│   │ function frames │   │
│   │ local variables │   │
│   │ return address  │   │
│   └─────────────────┘   │
│         (empty)         │
│   ┌─────────────────┐   │
│   │ malloc() memory │   │
│   │ dynamic data    │   │
│   └─────────────────┘   │
│   Heap   ↑ மேலே வளரும் │ ← manual, flexible
├─────────────────────────┤
│   .rodata               │ ← "Hello, World!" இங்கே
│   .data                 │ ← global variables
│   .text (code)          │ ← assembly instructions
└─────────────────────────┘ Low address
```

---

## Stack என்றால்:

```c
void main() {
    int x = 10;        // Stack-ல் போகும்
    char name[20];     // Stack-ல் போகும்
}
// function முடிந்தால் → automatic-ஆ free!
```

**யார் manage பண்றது?** CPU தானே — push/pop automatically.

**எது போகும்:**
- Local variables
- Function arguments
- Return address
- Saved registers (rbp போன்றவை)

---

## Heap என்றால்:

```c
void main() {
    int *p = malloc(100);  // Heap-ல் போகும்
    // நீயே free பண்ணணும்!
    free(p);
}
```

**யார் manage பண்றது?** Programmer — manually `malloc` / `free`.

**எது போகும்:**
- Runtime-ல் size தெரியும் data
- Big data
- Long-lived data (function முடிந்தாலும் இருக்கணும்)

---

## Stack vs Heap — Quick table:

| | Stack | Heap |
|---|---|---|
| Size | Fixed, small (~8MB) | Large, flexible |
| Speed | Very fast | Slower |
| Manage | CPU automatic | Programmer manual |
| Life | Function உள்ளே மட்டும் | நீ free பண்ணும் வரை |
| Error | Stack overflow | Memory leak |

---

## 2️⃣ `#include` binary/hex → Assembly-ல் என்ன ஆனது?

**நம்ம hello.c-ல் இருந்தது:**

```
Disk-ல் bytes:
# i  n  c  l  u  d  e     < s  t  d  i  o  .  h  >
23 69 6e 63 6c 75 64 65 20 3c 73 74 64 69 6f 2e 68 3e
```

**Assembly-ல் வந்ததா?** ❌ வரவே இல்லை!

```
#include <stdio.h>  →  Preprocessor-ல் உபயோகப்பட்டது
                    →  stdio.h contents copy ஆச்சு
                    →  hello.i-ல் declarations வந்தது
                    →  Compiler அந்த declarations படித்தது
                    →  Assembly-ல் trace இல்லை — gone!
```

---

## 3️⃣ hello.c → Assembly — எல்லாம் என்ன ஆனது?

```
┌─────────────────────────────────────────────────────────────────┐
│                    hello.c (disk-ல் bytes)                      │
├──────────────────┬──────────────────────────────────────────────┤
│ Code             │ Disk bytes (hex)                             │
├──────────────────┼──────────────────────────────────────────────┤
│ #include<stdio.h>│ 23 69 6e 63 6c 75 64 65 ...                 │
│ int main() {     │ 69 6e 74 20 6d 61 69 6e ...                 │
│ printf(          │ 70 72 69 6e 74 66 28 ...                    │
│ "Hello, World!"  │ 48 65 6c 6c 6f 2c 20 57 6f 72 6c 64 21 ... │
│ return 0;        │ 72 65 74 75 72 6e 20 30 ...                 │
│ }                │ 7d                                           │
└──────────────────┴──────────────────────────────────────────────┘
         ↓ Preprocessing
┌─────────────────────────────────────────────────────────────────┐
│                         hello.i                                 │
├──────────────────┬──────────────────────────────────────────────┤
│ #include gone!   │ stdio.h-ஓட 800 lines paste ஆச்சு           │
│ extern printf(..)│ ← இது வந்தது (declaration)                  │
│ int main()...    │ ← உன் code same                             │
└──────────────────┴──────────────────────────────────────────────┘
         ↓ Compilation (cc1)
┌─────────────────────────────────────────────────────────────────┐
│                         hello.s                                 │
├──────────────────┬──────────────────────────────────────────────┤
│ C code           │ Assembly-ல் என்ன ஆனது                       │
├──────────────────┼──────────────────────────────────────────────┤
│ #include         │ ❌ GONE — no trace                           │
│ "Hello, World!"  │ ✅ .rodata → .string "Hello, World!"        │
│ int main()       │ ✅ main: label ஆனது                         │
│ printf(...)      │ ✅ call puts@PLT ஆனது                       │
│ return 0         │ ✅ movl $0, %eax ஆனது                       │
│ { }              │ ✅ pushq/popq/ret ஆனது                      │
└──────────────────┴──────────────────────────────────────────────┘
```

---

## 4️⃣ "Hello, World!" string — எங்கே போனது?

**இது most important!** String-ஓட journey:

```
hello.c disk-ல்:
"Hello, World!\n"
48 65 6c 6c 6f 2c 20 57 6f 72 6c 64 21 0a 00
H  e  l  l  o  ,     W  o  r  l  d  !  \n \0

         ↓ Compiler பார்க்கும்

hello.s-ல்:
.section .rodata        ← read-only data section-ல் வை
.LC0:
    .string "Hello, World!"  ← same string, label கொடு

         ↓ Assembler (step 4)

hello.o binary-ல்:
.rodata section-ல் exact bytes:
48 65 6c 6c 6f 2c 20 57 6f 72 6c 64 21 0a 00
↑ இதுவே disk-ல் இருந்த same bytes — RAM-ல் load ஆகும்!
```

---

## 5️⃣ Complete Journey — ஒரே diagram:

```
┌────────────────────────────────────────────────────────┐
│  hello.c (Text file — disk)                            │
│  '#','i','n','c'... = ASCII bytes                      │
│  'H','e','l','l','o'... = ASCII bytes                  │
└───────────────┬────────────────────────────────────────┘
                │ gcc -E (Preprocessor)
                ▼
┌────────────────────────────────────────────────────────┐
│  hello.i (Text file — disk)                            │
│  #include → GONE                                       │
│  stdio.h contents → PASTED (800 lines)                 │
│  printf declaration → இருக்கு                          │
│  உன் code → same                                       │
└───────────────┬────────────────────────────────────────┘
                │ gcc -S (Compiler cc1)
                ▼
┌────────────────────────────────────────────────────────┐
│  hello.s (Text file — disk)                            │
│  C syntax → GONE                                       │
│  "Hello,World!" → .rodata section-ல்                  │
│  main() → main: label                                  │
│  printf() → call puts@PLT                              │
│  return 0 → movl $0,%eax                               │
│  இன்னும் TEXT — binary இல்லை!                         │
└───────────────┬────────────────────────────────────────┘
                │ as (Assembler) — Step 4
                ▼
┌────────────────────────────────────────────────────────┐
│  hello.o (Binary file — disk)                          │
│  .text:   f3 0f 1e fa 55 48 89 e5 ...  ← opcodes      │
│  .rodata: 48 65 6c 6c 6f 2c 20 57 ...  ← string bytes │
│  இப்போது BINARY — human படிக்க முடியாது!              │
└───────────────┬────────────────────────────────────────┘
                │ ld (Linker) — Step 5
                ▼
┌────────────────────────────────────────────────────────┐
│  hello (Executable — disk)                             │
│  puts@PLT → libc.so connected                         │
│  addresses → resolved                                  │
└───────────────┬────────────────────────────────────────┘
                │ ./hello (Run)
                ▼
┌────────────────────────────────────────────────────────┐
│  RAM (Memory)                                          │
│  Stack:  main() frame, return address                  │
│  Heap:   (நம்ம program use பண்றதில்லை)                │
│  .text:  f3 0f 1e fa ... ← CPU execute பண்றது         │
│  .rodata: "Hello, World!" bytes ← CPU படிக்கும்        │
└───────────────┬────────────────────────────────────────┘
                │
                ▼
        Terminal: Hello, World! ✅
```

இப்போது `#include` முதல் terminal வரை எல்லாம் clear-ஆ புரிஞ்சதா? 🎯

---

இதோ — ஒரே diagram-ல் full flow!

```
═══════════════════════════════════════════════════════════════
              RAM - Full Picture (Program run ஆகும்போது)
═══════════════════════════════════════════════════════════════

High Address
┌─────────────────────────────────────────────────────────┐
│                    OS / Kernel                          │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  STACK (↓ கீழே வளரும்)                                 │
│  ┌───────────────────────────────────────────────┐      │
│  │          [1] Program START                    │      │
│  │  ┌─────────────────────────────┐              │      │
│  │  │       OS frame              │              │      │
│  │  │  rbp ──────────────────→ [OS frame base]  │      │
│  │  │  return address (to OS)     │              │      │
│  │  └─────────────────────────────┘              │      │
│  │                ↓ main() call                  │      │
│  │                                               │      │
│  │          [2] pushq %rbp                       │      │
│  │  ┌─────────────────────────────┐              │      │
│  │  │  [old rbp - OS frame addr] │ ← rsp        │      │
│  │  ├─────────────────────────────┤              │      │
│  │  │       OS frame              │              │      │
│  │  └─────────────────────────────┘              │      │
│  │       OS-ஓட rbp value save ஆச்சு!             │      │
│  │                                               │      │
│  │          [3] movq %rsp,%rbp                   │      │
│  │  ┌─────────────────────────────┐              │      │
│  │  │  [old rbp]      ← rsp,rbp  │ main() frame │      │
│  │  ├─────────────────────────────┤  ready!      │      │
│  │  │       OS frame              │              │      │
│  │  └─────────────────────────────┘              │      │
│  │                                               │      │
│  │          [4] leaq + movq (registers)          │      │
│  │                                               │      │
│  │  CPU Registers:                               │      │
│  │  ┌──────┬──────────────────────────────────┐  │      │
│  │  │ rax  │ = 0x2004 ("Hello,World!" address)│  │      │
│  │  │ rdi  │ = 0x2004 (puts()-க்கு argument)  │  │      │
│  │  │ rbp  │ = main() frame base              │  │      │
│  │  │ rsp  │ = stack current top              │  │      │
│  │  │ rip  │ = இப்போது execute ஆகும் address  │  │      │
│  │  └──────┴──────────────────────────────────┘  │      │
│  │                                               │      │
│  │          [5] call puts@PLT                    │      │
│  │  ┌─────────────────────────────┐              │      │
│  │  │     puts() frame            │ ← rsp        │      │
│  │  │  (internal puts variables)  │              │      │
│  │  ├─────────────────────────────┤              │      │
│  │  │  return address             │              │      │
│  │  │  (puts முடிந்தவுடன் இங்கே) │              │      │
│  │  ├─────────────────────────────┤              │      │
│  │  │  [old rbp]       ← rbp     │ main() frame │      │
│  │  ├─────────────────────────────┤              │      │
│  │  │       OS frame              │              │      │
│  │  └─────────────────────────────┘              │      │
│  │     puts() running...                         │      │
│  │     rdi → 0x2004 → "Hello,World!" படிக்கும்  │      │
│  │     write() syscall → kernel → terminal print │      │
│  │                                               │      │
│  │          [6] puts() returns                   │      │
│  │  ┌─────────────────────────────┐              │      │
│  │  │  [old rbp]       ← rsp,rbp │ ← rsp        │      │
│  │  ├─────────────────────────────┤              │      │
│  │  │       OS frame              │              │      │
│  │  └─────────────────────────────┘              │      │
│  │     puts() frame GONE!                        │      │
│  │                                               │      │
│  │          [7] movl $0,%eax                     │      │
│  │  ┌──────┬──────────────────────────────────┐  │      │
│  │  │ eax  │ = 0  (return 0 ready)            │  │      │
│  │  └──────┴──────────────────────────────────┘  │      │
│  │                                               │      │
│  │          [8] popq %rbp                        │      │
│  │  ┌─────────────────────────────┐              │      │
│  │  │       OS frame   ← rsp     │              │      │
│  │  │  rbp → OS frame base again │              │      │
│  │  └─────────────────────────────┘              │      │
│  │     main() frame GONE!                        │      │
│  │                                               │      │
│  │          [9] ret                              │      │
│  │     return address எடு → OS-க்கு jump        │      │
│  │     OS → eax=0 பார்க்கும் → SUCCESS!         │      │
│  │     Process terminate!                        │      │
│  └───────────────────────────────────────────────┘      │
│                                                         │
│  HEAP (↑ மேலே வளரும்)                                  │
│  ┌───────────────────────────────────────────────┐      │
│  │  (நம்ம program malloc பண்றதில்லை — EMPTY)    │      │
│  └───────────────────────────────────────────────┘      │
│                                                         │
├─────────────────────────────────────────────────────────┤
│  .rodata  (Read Only)                                   │
│  ┌───────────────────────────────────────────────┐      │
│  │  0x2004: 48 65 6c 6c 6f 2c 20 57 6f 72 6c    │      │
│  │          H  e  l  l  o  ,     W  o  r  l     │      │
│  │          64 21 0a 00                          │      │
│  │          d  !  \n \0                          │      │
│  │          ↑                                    │      │
│  │         .LC0 label → "Hello, World!"          │      │
│  └───────────────────────────────────────────────┘      │
│                                                         │
├─────────────────────────────────────────────────────────┤
│  .text  (Code — Execute ஆகும் instructions)             │
│  ┌───────────────────────────────────────────────┐      │
│  │  f3 0f 1e fa → endbr64                        │      │
│  │  55          → pushq %rbp                     │      │
│  │  48 89 e5    → movq %rsp,%rbp                 │      │
│  │  48 8d 05 .. → leaq .LC0(%rip),%rax           │      │
│  │  48 89 c7    → movq %rax,%rdi                 │      │
│  │  e8 .. .. .. → call puts@PLT                  │      │
│  │  b8 00 00 00 → movl $0,%eax                   │      │
│  │  5d          → popq %rbp                      │      │
│  │  c3          → ret                            │      │
│  └───────────────────────────────────────────────┘      │
│                                                         │
Low Address                                               │
└─────────────────────────────────────────────────────────┘

Terminal Output:
┌─────────────────┐
│ Hello, World!   │  ✅
└─────────────────┘
═══════════════════════════════════════════════════════════════
```

**Key Takeaway:**

```
Stack  → functions, frames, registers — temporary, auto cleanup
Heap   → malloc data — manual, நம்ம program use பண்றதில்லை
.rodata → "Hello,World!" string — permanent, read only
.text  → opcodes — CPU இதை மட்டுமே execute பண்ணும்!
```

இப்போது ஒரே பார்வையில் எல்லாமே புரிஞ்சதா? 🎯


----

மிக நல்ல கேள்வி! இதை ஒவ்வொன்னா root-லிருந்து பார்க்கலாம்!

---

## முதலில் — Program run ஆவதற்கு முன்பே என்ன நடக்கும்?

```
நீங்கள் terminal-ல் ./hello type பண்றீர்கள்

Terminal (bash) என்பது ஒரு program — அதுவே running இருக்கு
bash-க்கு stack-ல் frame இருக்கு

bash → OS-கிட்ட கேட்கும் → "hello program run பண்ணு"
OS (kernel) → hello-ஓட .text, .rodata RAM-ல் load பண்ணும்
OS → main()-ஐ call பண்ணும்

அப்போது stack இப்படி இருக்கும்:

┌─────────────────────────┐
│     bash frame          │ ← bash-ஓட variables
│     bash variables      │
│     bash return address │
├─────────────────────────┤
│   OS/libc startup frame │ ← __libc_start_main()
│   இதுதான் main()-ஐ     │    என்ற function
│   call பண்றது!         │    இதுதான் "OS frame"
│                         │
│   rbp → இந்த frame base │
│   return address →      │
│   "main முடிந்தவுடன்    │
│    இங்கே திரும்பு"      │
└─────────────────────────┘

இதுதான் [1] Program START-ல் உள்ள OS frame!
```

---

## [1] → [2] : `pushq %rbp` — ஏன்? எதனால்? எதற்கு?

```
இப்போது நிலை:
  rbp = OS frame-ஓட base address (example: 0x7fff1000)
  rsp = stack top

கேள்வி: main() ஏன் rbp-ஐ save பண்றது?

காரணம்:
  rbp என்பது ஒரே ஒரு register — CPU-ல் ஒன்றே ஒன்று!
  main()-க்கு தன்னுடைய frame-க்கு rbp வேணும்
  ஆனால் அதில் OS frame address இருக்கு

  மாற்றினால்? → OS frame address போய்விடும்!
  main() முடிந்தவுடன் OS-க்கு திரும்ப முடியாது!

Solution:
  மாற்றுவதற்கு முன் → stack-ல் save பண்ணு!

Before pushq:
  rbp = 0x7fff1000  (OS frame address)
  rsp = 0x7fff0ff8

  Stack:
  ┌─────────────────────┐
  │   OS frame          │ 0x7fff1000 ← rbp இங்கே
  └─────────────────────┘

pushq %rbp என்பது:
  Step 1: rsp = rsp - 8  →  0x7fff0ff0
  Step 2: [rsp] = rbp    →  0x7fff0ff0 இடத்தில் 0x7fff1000 எழுது

After pushq:
  rsp = 0x7fff0ff0
  Stack:
  ┌─────────────────────┐ 0x7fff0ff0 ← rsp (new top)
  │ 0x7fff1000          │ ← old rbp saved! (OS frame address)
  ├─────────────────────┤
  │   OS frame          │ 0x7fff1000
  └─────────────────────┘

இப்போது rbp-ஐ மாற்ளலாம் — OS address safe-ஆ stack-ல் இருக்கு!
```

---

## [2] → [3] : `movq %rsp, %rbp` — ஏன்? எதனால்? எதற்கு?

```
இப்போது நிலை:
  rbp = 0x7fff1000 (இன்னும் OS frame-ஐ point செய்கிறது)
  rsp = 0x7fff0ff0 (stack top)

கேள்வி: ஏன் rsp-ஐ rbp-க்கு copy பண்றோம்?

காரணம்:
  main()-க்கு தன்னுடைய frame வேணும்
  frame base = இப்போது stack எங்கே இருக்கோ அங்கே!
  rsp இப்போது stack top = main() frame ஆரம்பிக்கும் இடம்
  அதை rbp-ல் வை → இதுதான் main()-ஓட frame base!

movq %rsp, %rbp:
  rbp = rsp = 0x7fff0ff0

After:
  ┌─────────────────────┐ 0x7fff0ff0 ← rsp = rbp இரண்டும் இங்கே!
  │ 0x7fff1000(old rbp) │            ← main() frame இங்கிருந்து
  ├─────────────────────┤
  │   OS frame          │ 0x7fff1000
  └─────────────────────┘

ஏன் இரண்டும் same இடம் point செய்கிறது?
  main()-ல் local variables இல்லை!
  variables இருந்தால் rsp மேலும் கீழே போயிருக்கும்:

  example: int x = 5; இருந்தால்:
  ┌─────────────────────┐ ← rsp (x-க்கு இடம்)
  │ x = 5               │
  ├─────────────────────┤ ← rbp (frame base)
  │ 0x7fff1000(old rbp) │
  ├─────────────────────┤
  │   OS frame          │
  └─────────────────────┘
```

---

## [3] → [4] : `leaq + movq` — ஏன்? எதனால்? எதற்கு?

```
இப்போது:
  "Hello, World!" string .rodata-ல் இருக்கு (address: 0x2004)
  puts()-க்கு அந்த address கொடுக்கணும்

leaq .LC0(%rip), %rax:
  ┌──────────────────────────────────────┐
  │ .rodata section                      │
  │ 0x2004: H e l l o , W o r l d ! \0  │
  │         ↑                            │
  │        .LC0 label                    │
  └──────────────────────────────────────┘

  rip = இப்போது execute ஆகும் instruction address
  .LC0 = rip இருந்து எத்தனை bytes தூரத்தில் string இருக்கு
  leaq = அந்த exact address கணக்கிட்டு rax-ல் வை

  After: rax = 0x2004

movq %rax, %rdi:
  Linux rule — puts("string") call பண்ண
  string address rdi-ல் இருக்கணும் (1st argument rule)

  rax = 0x2004 → rdi = 0x2004

  Registers now:
  ┌──────┬────────────────────────────┐
  │ rax  │ 0x2004 (string address)   │
  │ rdi  │ 0x2004 (puts argument)    │
  │ rbp  │ 0x7fff0ff0 (frame base)   │
  │ rsp  │ 0x7fff0ff0 (stack top)    │
  └──────┴────────────────────────────┘
```

---

## [4] → [5] : `call puts@PLT` — ஏன்? எதனால்? எதற்கு?

```
call என்பது 2 steps:

Step 1 — Return address push:
  puts() முடிந்தவுடன் main()-ல் எங்கே திரும்பணும்?
  call-க்கு அடுத்த instruction address = return address
  அதை stack-ல் push பண்ணும்:

  rsp = rsp - 8
  [rsp] = next instruction address (0x0017 example)

  Stack:
  ┌─────────────────────┐ ← rsp
  │ return addr (0x0017)│ ← "puts முடிந்தவுடன் இங்கே வா"
  ├─────────────────────┤ ← rbp
  │ 0x7fff1000(old rbp) │
  ├─────────────────────┤
  │   OS frame          │
  └─────────────────────┘

Step 2 — puts()-க்கு jump:

@PLT என்றால்:
  puts() code நம்ம program-ல் இல்ல
  /lib/libc.so.6 என்ற file-ல் இருக்கு

  PLT table:
  program → PLT[puts] → GOT → libc.so → actual puts()

puts() உள்ளே என்ன நடக்கும்:

  puts()-க்கும் தன்னுடைய frame வேணும்!
  Stack:
  ┌─────────────────────┐ ← rsp (puts frame)
  │   puts() frame      │
  │   internal vars     │
  ├─────────────────────┤
  │ return addr (0x0017)│
  ├─────────────────────┤ ← rbp (main frame)
  │ 0x7fff1000(old rbp) │
  ├─────────────────────┤
  │   OS frame          │
  └─────────────────────┘

  puts() என்ன செய்யும்:
  rdi = 0x2004 → அந்த address போகும்
  0x2004-ல் உள்ள bytes ஒவ்வொன்னா படிக்கும்:
  H → e → l → l → o → , → (space) → W → o → r → l → d → ! → \n → \0(\0 = stop!)
  write(1, "Hello,World!\n", 13) syscall பண்ணும்
  Kernel → terminal-ல் print பண்ணும்!
  Terminal: Hello, World! ✅
```

---

## [5] → [6] : puts() returns

```
puts() முடிந்தது → ret பண்ணும்
stack-ல் உள்ள return address (0x0017) எடுக்கும்
main()-ல் அடுத்த instruction-க்கு jump!

puts() frame GONE — stack clean:
  ┌─────────────────────┐ ← rsp = rbp
  │ 0x7fff1000(old rbp) │
  ├─────────────────────┤
  │   OS frame          │
  └─────────────────────┘
```

---

## [6] → [7] : `movl $0, %eax`

```
return 0; → CPU-க்கு என்ன சொல்கிறோம்?

Linux exit rule:
  program exit code → eax register-ல் வை
  0 = success
  1,2.. = error codes

movl $0, %eax:
  eax = 0

Terminal-ல் verify பண்ணலாம்:
  ./hello
  echo $?   → 0 வரும்!

  ./hello_with_error
  echo $?   → 1 வரும்!
```

---

## [7] → [8] : `popq %rbp` — Cleanup

```
pushq-ஓட exact reverse!

Before pop:
  ┌─────────────────────┐ ← rsp
  │ 0x7fff1000(old rbp) │ ← save பண்ணியது step [2]-ல்
  ├─────────────────────┤
  │   OS frame          │
  └─────────────────────┘

popq %rbp:
  Step 1: rbp = [rsp]  → rbp = 0x7fff1000 (OS frame address திரும்பி வந்தது!)
  Step 2: rsp = rsp + 8

After pop:
  ┌─────────────────────┐ ← rsp
  │   OS frame          │ ← rbp இப்போது இதை point செய்கிறது
  └─────────────────────┘

main()-ஓட frame முற்றிலும் GONE!
rbp மீண்டும் OS frame-ஐ point செய்கிறது — exactly step [1] மாதிரி!
```

---

## [8] → [9] : `ret` — Complete!

```
ret என்பது:
  stack top-ல் உள்ள address எடு → அங்கே jump!

Stack-ல் இப்போது:
  ┌─────────────────────┐ ← rsp
  │   OS return address │ ← "__libc_start_main திரும்பு"
  └─────────────────────┘

ret → OS-க்கு jump!
OS → eax பார்க்கும் → 0 இருக்கு → "Success!"
OS → process terminate பண்ணும்
RAM-ல் இருந்த எல்லாமே free!
```

---

## ஒரே பார்வையில் — WHY chain:

```
pushq %rbp    → ஏன்? OS frame address காப்பாற்ற
                எதனால்? rbp ஒரே register, மாற்றணும்
                எதற்கு? main() முடிந்தவுடன் OS-க்கு திரும்ப

movq rsp,rbp  → ஏன்? main()-ஓட frame base set பண்ண
                எதனால்? rsp = இப்போது stack top = frame start
                எதற்கு? main()-ஓட variables இங்கிருந்து போகும்

leaq+movq     → ஏன்? string address rdi-ல் வைக்க
                எதனால்? Linux rule — arg1 = rdi
                எதற்கு? puts()-க்கு string கொடுக்க

call puts     → ஏன்? actual print பண்ண
                எதனால்? printf/puts libc-ல் இருக்கு
                எதற்கு? Terminal-ல் Hello,World! காட்ட

movl $0,eax   → ஏன்? exit code set பண்ண
                எதனால்? Linux rule — exit code = eax
                எதற்கு? OS-க்கு "success" சொல்ல

popq %rbp     → ஏன்? OS frame address restore பண்ண
                எதனால்? pushq-ல் save பண்ணியது
                எதற்கு? ret-ல் OS-க்கு சரியா திரும்ப

ret           → ஏன்? main() முடிந்தது
                எதனால்? return address stack-ல் இருக்கு
                எதற்கு? OS process terminate பண்ண
```

இப்போது ஒவ்வொன்றுக்கும் ஏன், எதனால், எதற்கு — clear-ஆ புரிஞ்சதா? 🎯

------

இல்லை! `rsp` OS இல்லை!

---

## rsp என்றால் என்ன?

```
rsp = Register Stack Pointer

CPU-ல் உள்ள ஒரு register மட்டுமே!
"Stack-ல் இப்போது top எங்கே இருக்கு" 
என்று point செய்யும் arrow மாதிரி!
```

---

## Simple visual:

```
Stack (RAM-ல்):              rsp (CPU register):

┌─────────────────┐
│   item 3        │ ← rsp ───────────────→ இங்கே top இருக்கு
├─────────────────┤
│   item 2        │
├─────────────────┤
│   item 1        │
└─────────────────┘

push பண்ணினால்:
┌─────────────────┐
│   item 4        │ ← rsp ───────────────→ top கீழே நகர்ந்தது
├─────────────────┤
│   item 3        │
├─────────────────┤
│   item 2        │
├─────────────────┤
│   item 1        │
└─────────────────┘
```

---

## OS frame என்றால் என்ன தனியாக?

```
┌─────────────────┐
│  OS frame       │ → RAM-ல் உள்ள memory பகுதி
│  (libc startup) │   __libc_start_main() function
│                 │   இது main()-ஐ call பண்றது
└─────────────────┘
        ↑
        │
rsp இதை point செய்யலாம்     ← rsp = pointer மட்டுமே!
rbp இதை point செய்யலாம்     ← rbp = pointer மட்டுமே!

OS frame = memory location (RAM)
rsp/rbp  = அந்த location-ஐ point செய்யும் CPU registers
```

---

## Summary:

| | என்ன | எங்கே |
|---|---|---|
| `OS frame` | Memory பகுதி — libc startup data | RAM-ல் |
| `rsp` | "Stack top எங்கே" என்று சொல்லும் pointer | CPU chip-ல் |
| `rbp` | "Frame base எங்கே" என்று சொல்லும் pointer | CPU chip-ல் |

**rsp = GPS arrow மாதிரி — இடம் இல்லை, direction மட்டுமே!** 🎯

---------

Great question! இதை simple-ஆ பார்க்கலாம்!

---

## Frame Base என்றால் என்ன?

```
ஒவ்வொரு function-க்கும் stack-ல் தனி பகுதி = Frame

main() frame:
┌─────────────────┐ ← rsp (top — மாறிக்கொண்டே இருக்கும்)
│  local var x    │
│  local var y    │
│  saved registers│
├─────────────────┤ ← rbp (bottom — இது மாறாது!)
│  old rbp value  │   இதுதான் "Frame Base"
└─────────────────┘
```

**Frame Base = அந்த function-ஓட frame ஆரம்பிக்கும் இடம்!**

---

## ஏன் frame base தேவை?

```c
int main() {
    int x = 10;   // rbp - 4  இடத்தில்
    int y = 20;   // rbp - 8  இடத்தில்
    int z = 30;   // rbp - 12 இடத்தில்
}
```

```
Stack:
┌─────────────────┐ ← rsp (push/pop-ல் மாறும்)
│  z = 30         │ rbp - 12
│  y = 20         │ rbp - 8
│  x = 10         │ rbp - 4
├─────────────────┤ ← rbp (இது மாறாது — fixed!)
│  old rbp        │
└─────────────────┘

CPU x கண்டுபிடிக்க:  rbp - 4  → direct!
CPU y கண்டுபிடிக்க:  rbp - 8  → direct!
CPU z கண்டுபிடிக்க:  rbp - 12 → direct!
```

---

## rsp vs rbp வித்தியாசம்:

```
rsp = உயரும் தண்ணீர் மட்டம் மாதிரி — மாறிக்கொண்டே இருக்கும்
rbp = கப்பலின் நங்கூரம் மாதிரி  — fixed, அசையாது!

┌─────────────────┐ ← rsp (push பண்ண கீழே போகும்)
│                 │        (pop பண்ண மேலே வரும்)
│  variables      │
│                 │
├─────────────────┤ ← rbp (function முழுக்க same இடம்!)
│  old rbp        │
└─────────────────┘
```

---

## Simple-ஆ சொன்னால்:

```
வீட்டு address மாதிரி:

rbp = வீட்டு main door (fixed!)
rsp = வீட்டுக்குள் நீங்கள் இருக்கும் இடம் (மாறும்!)

CPU variable தேட:
  "rbp இருந்து எத்தனை steps?" → கண்டுபிடிக்கும்!
  rsp மாறினாலும் பரவாயில்லை — rbp fixed!
```

இப்போது frame base புரிஞ்சதா? 🎯

---------

மிக நல்ல observation! நீங்கள் சரியாகவே யோசிக்கிறீர்கள்!

---

## நீங்கள் சொல்வது சரிதானா?

**ஆமா — நம்ம hello.c-க்கு local variables இல்லை!**

```c
int main() {
    printf("Hello, World!\n");  // variable இல்லை!
    return 0;
}
```

So technically:
```
pushq %rbp   → save old frame
movq rsp,rbp → new frame setup

இந்த 2 lines-உம் நம்ம program-க்கு
practically தேவையில்லை!
```

---

## ஆனால் GCC எப்போதும் போடும் — ஏன்?

```
காரணம் 1: GCC-ஓட default behaviour
─────────────────────────────────
GCC எல்லா function-க்கும் automatically
frame setup போடும்

"இந்த function-க்கு variables இல்லை,
 skip பண்ணலாம்" என்று தனியா check பண்றதில்லை
 (default-ஆ)

காரணம் 2: Debugger-க்கு தேவை
─────────────────────────────────
gdb போன்ற debuggers:
  rbp இருந்தால் → stack trace காட்ட முடியும்
  rbp இல்லாவிட்டால் → crash எங்கே என்று தெரியாது!

காரணம் 3: Calling convention
─────────────────────────────────
Linux rule:
  எந்த function-ஐ call பண்ணினாலும்
  rbp save/restore பண்ணணும்
  இது ஒரு standard rule!
```

---

## Optimize பண்ணினால் போய்விடும்!

```bash
# Normal compile:
gcc hello.c -o hello
# → pushq %rbp, movq %rsp,%rbp இருக்கும்

# Optimized compile:
gcc -O2 hello.c -o hello
# → pushq %rbp, movq %rsp,%rbp GONE!
# GCC "தேவையில்லை" என்று remove பண்ணும்!
```

நீங்கள் கேட்டது **compiler optimization-ஓட exact concept!** 🎯