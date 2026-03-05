
ஆம் — **Computer இறுதியாக எல்லாவற்றையும் `0` மற்றும் `1` ஆகத்தான் store செய்கிறது.**  
ஆனால் நாம் சொல்வது `# = 35`, `i = 105` போன்றவை **decimal representation** மட்டும். உண்மையில் அது **binary bits** ஆக memory-ல் இருக்கும்.

அதை step-by-step பார்ப்போம்.

----------

# 1️⃣ Computer உண்மையில் எப்படி store செய்கிறது

Computer memory-ல் information:

0 1 0 0 0 1 1 0 ...

இப்படி **bits** ஆக தான் இருக்கும்.

bit =

0  
1

8 bits சேர்ந்து:

1 byte

----------

# 2️⃣ Character → ASCII number

ஒரு character-க்கு ASCII standard ஒரு number assign செய்கிறது.

Example:


| Character | ASCII number |
| --------- | ------------ |
| `#`       | 35           |
| `i`       | 105          |
| `n`       | 110          |
| `c`       | 99           |


இது **decimal representation**.

----------

# 3️⃣ அந்த number binary ஆக மாறும்

Computer decimal store செய்யாது.  
அதை **binary (0,1)** ஆக convert செய்து store செய்யும்.

Example:

### `#`

ASCII decimal:

35

Binary:

00100011

----------

### `i`

ASCII decimal:

105

Binary:

01101001

----------

### `n`

ASCII decimal:

110

Binary:

01101110

----------

### `c`

ASCII decimal:

99

Binary:

01100011

----------

# 4️⃣ Memory-ல் எப்படி இருக்கும்

Example:

#include

Memory-ல் roughly இப்படித்தான் இருக்கும்:

#   i   n   c   l   u   d   e  
00100011  
01101001  
01101110  
01100011  
01101100  
01110101  
01100100  
01100101

அதாவது:

👉 **ஒவ்வொரு character = 1 byte = 8 bits**

----------

# 5️⃣ ஏன் புத்தகத்தில் 35,105 போன்ற numbers எழுதுகிறார்கள்?

Binary படிக்க கடினம்.

01101001

இதைக் காட்டிலும்

105

எழுதுவது எளிது.

அதனால்:

-   Humans → decimal
    
-   Computer → binary
    

----------

# 6️⃣ முக்கிய concept

இதுதான் புத்தகத்தில் சொல்வது:

### **Information = Bits + Context**

Memory-ல்:

01101001

Context இல்லாமல் meaning தெரியாது.

Context இருந்தால்:

-   ASCII → `i`
    
-   integer → `105`
    
-   instruction → machine code
    

----------

✅ **Short answer**

நீ சொன்னது சரி:

Computer **0,1 bits ஆகத்தான் store செய்கிறது.**

`# = 35` என்பது:

decimal representation

ஆனால் memory-ல்:

35 → 00100011

----------

💡 உனக்கு இன்னும் interesting fact சொல்கிறேன்:

**Hello world program முழுவதும் memory-ல் எப்படி binary ஆக இருக்கும் என்று பார்த்தால் தான் CS:APP chapter 1 முழுவதும் புரியும்.**

நீ விரும்பினால்:

👉 `hello.c` file memory-ல் எப்படி **binary bytes ஆக இருக்கும்** என்று diagram காட்டி explain செய்கிறேன்.
---
# 💾 hello.c — ASCII to Binary & Compilation Pipeline

---

## 1️⃣ Programmer எழுதும் Source Code

```c
#include <stdio.h>

int main()
{
    printf("hello, world\n");
    return 0;
}
```

**File பெயர்:** `hello.c`  
✔ இது ஒரு text file

---

## 2️⃣ Source File Memory-ல் எப்படி Store ஆகும்

```
Computer text-ஐ characters ஆக store செய்யாது
அது ASCII numbers ஆக store செய்கிறது
```

**Example — `#include` characters → ASCII numbers:**

| Character | ASCII |
|-----------|-------|
| # | 35 |
| i | 105 |
| n | 110 |
| c | 99 |
| l | 108 |
| u | 117 |
| d | 100 |
| e | 101 |

---

## 3️⃣ ASCII → Binary

```
Computer decimal store செய்யாது
அது binary bits ஆக store செய்கிறது
```

| Character | ASCII (decimal) | Binary (8 bits) |
|-----------|----------------|-----------------|
| # | 35 | 00100011 |
| i | 105 | 01101001 |
| n | 110 | 01101110 |
| c | 99 | 01100011 |

---

## 4️⃣ hello.c Memory Diagram

**Example — `#include` memory-ல் bytes:**

```
Address      Byte (binary)
--------------------------------
0x1000       00100011   (#)
0x1001       01101001   (i)
0x1002       01101110   (n)
0x1003       01100011   (c)
0x1004       01101100   (l)
0x1005       01110101   (u)
0x1006       01100100   (d)
0x1007       01100101   (e)
```

✔ ஒவ்வொரு character = 1 byte = 8 bits

---

## 5️⃣ முழு hello.c File எப்படி இருக்கும்

**Simplified representation:**

```
Address   Character   ASCII   Binary
-----------------------------------------
1000      #           35      00100011
1001      i           105     01101001
1002      n           110     01101110
1003      c           99      01100011
...
1010      p           112     01110000
1011      r           114     01110010
1012      i           105     01101001
1013      n           110     01101110
1014      t           116     01110100
1015      f           102     01100110
```

✔ இதுதான் source file representation

---

## 6️⃣ Program Compile Stages

```
hello.c
   │
   ▼
Preprocessor
   │
   ▼
Compiler
   │
   ▼
Assembler
   │
   ▼
Linker
   │
   ▼
Executable (hello)
```

---

## 7️⃣ Stage 1 — Preprocessing

**Command:**
```
gcc hello.c
```

**இதில்:**
```
#include
#define
macros
→ expand செய்யப்படும்
```

**Example:**
```
#include <stdio.h>
↓
stdio library declarations add ஆகும்
```

**Output:** `hello.i`  
⚠️ இன்னும் இது ASCII text தான்

---

## 8️⃣ Stage 2 — Compilation

**Compiler C code-ஐ convert செய்கிறது:**
```
C → Assembly
```

**Output:** `hello.s`

**Assembly example:**
```asm
movl $0, %eax
call printf
```

⚠️ இது கூட இன்னும் ASCII text — Assembly file text தான்

---

## 9️⃣ Stage 3 — Assembler

**Assembler:**
```
Assembly → Machine code
```

✔ இதில் தான் → ASCII instructions → binary instructions

**Example:**

Assembly:
```asm
mov eax, 1
```

Machine code:
```
10111000 00000001
```

**Output:** `hello.o`  
✔ இது binary object file

---

## 🔟 Stage 4 — Linking

**Linker:**
```
hello.o + libraries → executable
```

**Example:**
```
printf() → library-ல் இருக்கும்
Linker அதை சேர்க்கும்
```

**Output:** `hello`  
✔ இது final executable binary

---

## 1️⃣1️⃣ Final Executable Memory Layout

**Executable run ஆகும்போது OS load செய்யும்:**

```
+----------------------+
| Machine Instructions |
+----------------------+
| Global Variables     |
+----------------------+
| Heap                 |
+----------------------+
| Stack                |
+----------------------+
```

✔ CPU machine instructions execute செய்யும்

---

## 1️⃣2️⃣ முழு Pipeline (Very Important)

```
Programmer writes code
        │
        ▼
hello.c (ASCII text)
        │
        ▼
Preprocessor
        │
        ▼
hello.i (ASCII text)
        │
        ▼
Compiler
        │
        ▼
hello.s (ASCII assembly text)
        │
        ▼
Assembler
        │
        ▼
hello.o (binary machine code)
        │
        ▼
Linker
        │
        ▼
hello executable (binary)
```

---

## ⭐ முக்கிய Takeaway

| Stage | Representation |
|-------|---------------|
| Source code | ASCII text |
| Preprocessed code | ASCII text |
| Assembly | ASCII text |
| Object file | Binary |
| Executable | Binary |

---

## ✅ Answer to Main Doubt

```
Source code  → ASCII characters
Compile      → Assembly text
Assembler    → Binary machine code

👉 Binary instructions உருவாகும் stage = Assembler
```

---

## 💡 Next Level Concept
*(CS:APP Chapter 1-ல் வரும்)*

```
hello run ஆகும்போது:
  OS-ல் process
  virtual memory
  cache
  CPU pipeline
  
எல்லாம் எப்படி involve ஆகிறது
```