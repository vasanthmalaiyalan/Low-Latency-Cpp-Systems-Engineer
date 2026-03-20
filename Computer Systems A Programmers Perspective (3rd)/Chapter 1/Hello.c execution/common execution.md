இப்போது **Section 1.2 – “Programs Are Translated by Other Programs into Different Forms”** முழுவதையும் **ஒரு கருத்தும் விடாமல் எளிய தமிழில்** explain செய்கிறேன். இது CS:APP-ல் மிகவும் முக்கியமான concept.

---

# 1️⃣ Program ஆரம்பத்தில் மனிதர்களுக்கான வடிவில் இருக்கும்

`hello.c` program:

```c
#include <stdio.h>

int main()
{
    printf("hello, world\n");
    return 0;
}
```

இந்த code:

* மனிதர்கள் படிக்க முடியும்
* high-level language

அதாவது:

```text
C language
```

---

# 2️⃣ Computer நேரடியாக C code run செய்ய முடியாது

CPU புரிவது:

```text
machine instructions
```

Example:

```
10110000 01100001
```

அதனால்:

```text
C program
↓
machine language
```

ஆக மாற்ற வேண்டும்.

இந்த மாற்றம் செய்யும் programs:

```text
compilation system
```

---

# 3️⃣ Compilation system என்ன

Compilation system = programs collection.

இதில் 4 phases:

```
Preprocessor
Compiler
Assembler
Linker
```

இவை சேர்ந்து source code-ஐ executable ஆக மாற்றும்.

---

# 4️⃣ Unix system-ல் compile command

Example command:

```bash
gcc -o hello hello.c
```

இங்கு:

* `gcc` → compiler driver
* `hello.c` → source program
* `hello` → executable file

`gcc` driver பின்னால் பல tools run செய்கிறது.

---

# 5️⃣ Compilation stages overview

```
hello.c
   ↓
Preprocessor
   ↓
hello.i
   ↓
Compiler
   ↓
hello.s
   ↓
Assembler
   ↓
hello.o
   ↓
Linker
   ↓
hello (executable)
```

---

# 6️⃣ Phase 1 — Preprocessing

Program:

```
Preprocessor
```

இதன் பெயர்:

```
cpp
```

வேலை:

```
# directives process
```

Example:

```c
#include <stdio.h>
```

இதன் அர்த்தம்:

```
stdio.h file contents add
```

அதாவது:

source file modify செய்யப்படும்.

Output:

```
hello.i
```

இது இன்னும்:

```
C code (text file)
```

---

# 7️⃣ Phase 2 — Compilation

Program:

```
compiler (cc1)
```

வேலை:

```
C → Assembly language
```

Output file:

```
hello.s
```

Example assembly code:

```asm
main:
subq $8, %rsp
movl $.LC0, %edi
call puts
movl $0, %eax
addq $8, %rsp
ret
```

---

# 8️⃣ Assembly language என்ன

Assembly language:

* CPU instructions textual form
* low-level programming

Example:

```
movl $0, %eax
```

இதன் meaning:

```
register eax = 0
```

---

# 9️⃣ Assembly ஏன் useful

Different languages:

```
C
Fortran
```

இவை compile ஆன பிறகு:

```
same assembly language
```

ஆக மாறும்.

அதனால் assembly ஒரு common layer.

---

# 🔟 Phase 3 — Assembly

Program:

```
assembler
```

Tool name:

```
as
```

வேலை:

```
assembly → machine code
```

Output:

```
hello.o
```

இதற்கு பெயர்:

```
relocatable object file
```

---

# 1️⃣1️⃣ Object file என்ன

Object file:

```
binary file
```

இதில் இருக்கும்:

```
machine instructions
data
metadata
```

Example:

```
hello.o
```

இந்த file text editor-ல் open செய்தால்:

```
gibberish
```

போல தெரியும்.

ஏனெனில் அது binary.

---

# 1️⃣2️⃣ Example

`main` function instructions:

```
17 bytes
```

machine code-ஆ encode செய்யப்படும்.

---

# 1️⃣3️⃣ Phase 4 — Linking

Problem:

`hello.c` program:

```c
printf("hello")
```

`printf` function:

```
hello.c file-ல் இல்லை
```

அது standard C library-ல் இருக்கும்.

---

# 1️⃣4️⃣ Standard library object file

Example:

```
printf.o
```

இது precompiled object file.

---

# 1️⃣5️⃣ Linker வேலை

Program:

```
ld
```

வேலை:

```
hello.o + printf.o → merge
```

அதாவது:

external functions connect.

---

# 1️⃣6️⃣ Final output

Linker produce செய்கிறது:

```
hello
```

இது:

```
executable object file
```

இதற்கு short:

```
executable
```

---

# 1️⃣7️⃣ Executable file என்ன

Executable file:

```
binary program
```

OS இதை:

```
memory-ல் load
```

செய்து CPU execute செய்யும்.

---

# 1️⃣8️⃣ Compilation pipeline diagram

```
hello.c
   │
   ▼
Preprocessor (cpp)
   │
   ▼
hello.i
   │
   ▼
Compiler (cc1)
   │
   ▼
hello.s
   │
   ▼
Assembler (as)
   │
   ▼
hello.o
   │
   ▼
Linker (ld)
   │
   ▼
hello executable
```

---

# 1️⃣9️⃣ GNU project (aside)

`gcc` compiler உருவான project:

👉 GNU Project

Founder:

👉 Richard Stallman

Started:

```
1984
```

Goal:

```
free Unix-like system
```

---

# 2️⃣0️⃣ GNU tools

GNU project develop செய்த tools:

| Tool      | Purpose                 |
| --------- | ----------------------- |
| emacs     | editor                  |
| gcc       | compiler                |
| gdb       | debugger                |
| assembler | machine code generation |
| linker    | object merging          |

---

# 2️⃣1️⃣ Linux connection

Modern Linux system:

```
Linux kernel
+
GNU tools
```

இதுதான் complete environment.

Linux popularity-க்கு GNU tools முக்கிய காரணம்.

---

# ⭐ Section summary

இந்த section சொல்லும் main ideas:

1️⃣ High-level programs directly run ஆகாது
2️⃣ Compilation system programs translate செய்கின்றன
3️⃣ Four phases:

```
Preprocessor
Compiler
Assembler
Linker
```

4️⃣ Output:

```
executable binary
```

---

# 🔑 Most important CS:APP concept

```
Programs are translated by other programs
```

அதாவது:

```
C program
↓
compiler programs
↓
machine instructions
↓
CPU execution
```

---

💡 அடுத்த section (1.3)-ல் மிகவும் powerful concept வரும்:

```
Hardware organization of a system
```

அதாவது:

```
CPU
RAM
Disk
I/O devices
Bus
```

இவை எப்படி connect ஆகி program run செய்கிறது என்பதை explain செய்வார்கள்.

நீ விரும்பினால் அதை **ஒரு single diagram-ல் system முழுவதையும் explain** செய்து தரலாம் — அது CS:APP chapter-1-ன் heart.
