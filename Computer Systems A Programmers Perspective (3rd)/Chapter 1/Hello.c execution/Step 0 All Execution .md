# Hello World: Source to Execution
C program — Source file → Opcode → CPU Execution வரை deep dive

## GCC Compilation Pipeline
C source code ஒரு simple text file. ஆனால் CPU-க்கு text தெரியாது — binary bytes மட்டுமே தெரியும். GCC இந்த transformation-ஐ 4 stages-ல் செய்கிறது:

| Stage       | Tool | Input   | Output              | File    |
|-------------|------|---------|---------------------|---------|
| 1. Preprocessing | cpp  | hello.c | Macros expand       | hello.i |
| 2. Compilation   | cc1  | hello.i | Assembly text       | hello.s |
| 3. Assembly      | as   | hello.s | Machine code        | hello.o |
| 4. Linking       | ld   | hello.o | Executable          | hello   |

### STEP 1 — Source Code (hello.c)
உங்களுக்கு தெரிந்த C code — pure text (ASCII bytes):
```c
#include <stdio.h>
int main() {
    printf("Hello, World!\n");
    return 0;
}
```
இந்த file disk-ல் எப்படி இருக்கும்? '#' என்பது 0x23, 'i' என்பது 0x69 — எல்லாமே ASCII bytes. CPU இதை நேரடியாக execute செய்ய முடியாது.

### STEP 2 — Preprocessing (hello.i)
Command: `gcc -E hello.c -o hello.i`

Preprocessor என்ன செய்யும்:
- `#include <stdio.h>` → stdio.h file-ஓட முழு contents-ஐ copy paste செய்யும்
- `#define` macros → actual values-ஆல் replace செய்யும்
- Comments remove செய்யும்
- Conditional `#ifdef` blocks resolve செய்யும்

Result: hello.i என்பது ~800 lines! printf-ஓட declaration எல்லாம் அதில் இருக்கும்.
```
# 1 "/usr/include/stdio.h" 1 3 4
extern int printf(const char *__restrict __fmt, ...);
# 6 "hello.c" 2
int main() {
    printf("Hello, World!\n");
    return 0;
}
```

### STEP 3 — Compilation to Assembly (hello.s)
Command: `gcc -S hello.c -o hello.s`

GCC-ஓட cc1 compiler இந்த C code-ஐ x86-64 assembly-ஆக மாற்றும்:
```
.section  .rodata
.LC0:    .string  "Hello, World!"     ; string constant

.text
.globl  main
main:
    endbr64                 ; security: branch protection
    pushq   %rbp            ; save base pointer
    movq    %rsp, %rbp      ; setup stack frame
    leaq    .LC0(%rip), %rax ; load string address
    movq    %rax, %rdi      ; 1st argument = string
    call    puts@PLT        ; call puts() function
    movl    $0, %eax        ; return value = 0
    popq    %rbp            ; restore base pointer
    ret                     ; return
```
Assembly இன்னும் text — human readable. ஆனால் இப்போது CPU instructions-ஆக மாறிவிட்டது.

#### Assembly Registers புரிந்துகொள்ள:
| Register | Purpose                          | இந்த code-ல்              |
|----------|----------------------------------|---------------------------|
| %rbp     | Base Pointer — stack frame base  | Stack frame setup         |
| %rsp     | Stack Pointer — current stack top| Frame create              |
| %rax     | Accumulator — return value       | String address, return 0  |
| %rdi     | 1st Function Argument            | printf/puts-க்கு string pass |
| %rip     | Instruction Pointer              | String-ஓட relative address |

### STEP 4 — Assembling to Object File (hello.o)
Command: `gcc -c hello.c -o hello.o`

GNU Assembler (as) இந்த text assembly-ஐ binary machine code-ஆக மாற்றும். Result: ELF (Executable and Linkable Format) object file.

ELF File Header — actual hex dump:
| Offset   | Hex bytes                                  | ASCII              |
|----------|--------------------------------------------|--------------------|
| 000000   | 7f 45 4c 46 02 01 01 00  00 00 00 00 00 00 00 00 | .ELF............  |
| 000040   | f3 0f 1e fa 55 48 89 e5  48 8d 05 00 00 00 00 48 | ....UH..H......H  |
| 000050   | 89 c7 e8 00 00 00 00 b8  00 00 00 00 5d c3        | ............].    |
| 000060   | 48 65 6c 6c 6f 2c 20 57  6f 72 6c 64 21 00       | Hello, World!.    |

முதல் 4 bytes: 7f 45 4c 46 = .ELF — இது ELF magic number, Linux executable-ஓட signature.

Actual Opcodes — Disassembly:
| Address | Opcode bytes      | Assembly instruction                  |
|---------|-------------------|---------------------------------------|
| 0x00:   | f3 0f 1e fa       | endbr64 ; CET security                |
| 0x04:   | 55                | push %rbp ; 1 byte!                   |
| 0x05:   | 48 89 e5          | mov %rsp,%rbp ; REX + opcode + ModRM  |
| 0x08:   | 48 8d 05 00 00 00 00 | lea (%rip),%rax ; string address     |
| 0x0f:   | 48 89 c7          | mov %rax,%rdi ; arg1 = string         |
| 0x12:   | e8 00 00 00 00    | call puts ; CALL instruction          |
| 0x17:   | b8 00 00 00 00    | mov $0,%eax ; return 0                |
| 0x1c:   | 5d                | pop %rbp ; 1 byte!                    |
| 0x1d:   | c3                | ret ; 1 byte!                         |

#### Opcode Encoding விளக்கம்:
| Instruction     | Bytes            | Explanation                                      |
|-----------------|------------------|--------------------------------------------------|
| push %rbp       | 55               | Single byte — REX.W implicit, register encoded in opcode |
| mov %rsp,%rbp   | 48 89 e5         | 48=REX.W prefix, 89=MOV r/m←r, e5=ModRM(11 100 101) |
| mov $0,%eax     | b8 00 00 00 00   | b8=MOV eax,imm32, 00000000=value 0 little-endian |
| call puts       | e8 00 00 00 00   | e8=CALL rel32, 00000000=placeholder (linker fills) |
| ret             | c3               | Single byte near return                         |

### STEP 5 — ELF Sections
Object file பல sections-ஆக organize ஆகும்:

| Section     | Type          | Contents                                      |
|-------------|---------------|-----------------------------------------------|
| .text       | Code (AX)     | Actual machine instructions — CPU execute செய்வது |
| .rodata     | Read-only data| "Hello, World!" string — 14 bytes             |
| .data       | Read-write data| Global variables (இங்கு empty)               |
| .bss        | Uninitialized | Zero-initialized globals (இங்கு empty)       |
| .symtab     | Symbol table  | main, puts symbols — linker-க்கு தேவை        |
| .rela.text  | Relocations   | Linker fill செய்ய வேண்டிய addresses           |

Symbols: `0000000000000000 T main` = main function defined here. `U puts` = puts function Undefined — linker கண்டுபிடிக்கணும்.

### STEP 6 — Linking (hello executable)
Command: `gcc hello.c -o hello`

Linker (ld) என்ன செய்யும்:
- hello.o + libc.so (C library) combine செய்யும்
- puts@PLT — printf/puts-ஓட actual address fill செய்யும்
- Relocation entries resolve செய்யும் (00 00 00 00 placeholder → real address)
- ELF executable உருவாக்கும் — OS load செய்யும் படி

Before linking: `e8 00 00 00 00` call <????>  
After linking: `e8 d7 fe ff ff` call <puts@plt>  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;↑  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Linker filled this!

### STEP 7 — CPU Execution
`./hello` run பண்ணும் போது:
- OS kernel ELF file-ஐ read செய்யும்
- .text section → RAM-ல் load (executable memory page)
- .rodata section → RAM-ல் load (read-only page)
- CPU-ஓட Instruction Pointer (RIP) → main-ஓட address-க்கு jump
- ஒவ்வொரு opcode byte-ஐயும் fetch → decode → execute

#### CPU Fetch-Decode-Execute Cycle:
- Clock 1: RIP=0x0 Fetch: f3 0f 1e fa → Decode: endbr64 → Execute
- Clock 2: RIP=0x4 Fetch: 55 → Decode: push rbp → Execute: RSP-=8, [RSP]=RBP
- Clock 3: RIP=0x5 Fetch: 48 89 e5 → Decode: mov rbp,rsp → Execute: RBP=RSP
- Clock 4: RIP=0x8 Fetch: 48 8d 05... → Decode: lea rax,[rip+0] → Execute: RAX=string addr
- Clock 5: RIP=0xf Fetch: 48 89 c7 → Decode: mov rdi,rax → Execute: RDI=string addr
- Clock 6: RIP=0x12 Fetch: e8 d7 fe ff → Decode: call puts → Execute: puts() called  
  &nbsp;&nbsp;&nbsp;&nbsp;...puts() internally calls write() syscall...  
  &nbsp;&nbsp;&nbsp;&nbsp;...kernel writes 'Hello, World!' to stdout...
- Clock N: RIP=0x17 Fetch: b8 00 00 00 → Decode: mov eax,0 → Execute: EAX=0
- Clock N+1: RIP=0x1c Fetch: 5d → Decode: pop rbp → Execute
- Clock N+2: RIP=0x1d Fetch: c3 → Decode: ret → Execute: return to OS

## Complete Flow — Summary

```
hello.c  →[gcc -E]→  hello.i   (macros expanded, ~800 lines)
hello.i  →[cc1]→    hello.s   (x86-64 assembly text)
hello.s  →[as]→     hello.o   (ELF binary, opcodes + sections)
hello.o  →[ld]→     hello     (linked, addresses resolved)
hello    →[OS]→     RAM       (.text + .rodata loaded)
RAM      →[CPU]→    Execution (fetch → decode → execute per opcode)
                          ↓
                "Hello, World!" printed!
```

Key insight: "Hello, World!" என்ற 13 characters RAM-ல் .rodata section-ல் bytes-ஆக இருக்கும். CPU நேரடியாக string படிக்காது — puts() function-ஐ call செய்யும், அது Linux syscall write()-ஐ call செய்யும், kernel terminal-ல் print செய்யும்!