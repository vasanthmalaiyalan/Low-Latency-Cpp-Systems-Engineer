```STEP 2 — Preprocessing (hello.i)

Command: gcc -E hello.c -o hello.i

Preprocessor என்ன செய்யும்:

    #include <stdio.h> → stdio.h file-ஓட முழு contents-ஐ copy paste செய்யும்
    #define macros → actual values-ஆல் replace செய்யும்
    Comments remove செய்யும்
    Conditional #ifdef blocks resolve செய்யும்

Result: hello.i என்பது ~800 lines! printf-ஓட declaration எல்லாம் அதில் இருக்கும்.

# 1 "/usr/include/stdio.h" 1 3 4
extern int printf(const char *__restrict __fmt, ...);
# 6 "hello.c" 2
int main() {
    printf("Hello, World!\n");
    return 0;
}
```
-----

ஒவ்வொன்றாக deep-ஆக பார்க்கலாம்!

---

## முதலில் — Command அர்த்தம்

```
gcc  -E        hello.c    -o      hello.i
↓     ↓          ↓         ↓        ↓
GCC  Preprocess  input     output   output
tool  மட்டும்    file      flag     filename
      செய்       
```

**`-E`** = "Preprocessing மட்டும் செய், compile பண்ணாதே, stop here"
**`-o`** = "output file இந்த name-ல் save பண்ணு"

---

## ஏன் `.i` extension?

| Extension | என்ன | Stage |
|---|---|---|
| `.c` | C source | Original |
| `.i` | **i**ntermediate / preprocessed | After preprocessing |
| `.s` | **s**assembly | After compilation |
| `.o` | **o**bject | After assembling |

`.i` = **intermediate** file — fully processed, compiler-க்கு ready ஆன pure C code.

---

## இப்போது Deep dive — ஒவ்வொரு step-உம்:

---

### 1️⃣ `#include <stdio.h>` என்றால் என்ன?

```c
#include <stdio.h>   // இது ஒரு instruction to preprocessor
```

`#` என்பது **preprocessor directive** — compiler-க்கு இல்ல, preprocessor-க்கு மட்டும்.

`stdio.h` disk-ல் இங்கே இருக்கும்:
```
/usr/include/stdio.h
```

Preprocessor என்ன செய்யும்:
```
உன் hello.c:              hello.i result:
                           
#include <stdio.h>    →    [stdio.h-ஓட 800 lines எல்லாம் இங்கே paste]
                           extern int printf(...);
                           extern int scanf(...);
                           typedef struct _IO_FILE FILE;
                           ... (800+ lines)
int main() {               int main() {
    printf("Hi\n");            printf("Hi\n");
}                          }
```

**Literally copy-paste!** Compiler-க்கு `printf` என்றால் என்ன என்று தெரியாது — stdio.h-ல்தான் அதன் declaration இருக்கும்.

---

### 2️⃣ `#define` macros என்றால் என்ன?

```c
#define PI 3.14159
#define MAX 100

int main() {
    float area = PI * r * r;   // preprocessor பார்க்கும்
    int arr[MAX];               // preprocessor பார்க்கும்
}
```

Preprocessor result:
```c
int main() {
    float area = 3.14159 * r * r;  // PI → 3.14159 replace!
    int arr[100];                   // MAX → 100 replace!
}
```

**Find & Replace மட்டுமே!** Preprocessor-க்கு meaning தெரியாது — blindly replace செய்யும்.

---

### 3️⃣ Comments remove:

```c
// இது comment         →   (deleted)
/* இதுவும் comment */  →   (deleted)

int main() {           →   int main() {
```

Compiler-க்கு comments வேண்டாம் — waste of processing.

---

### 4️⃣ `#ifdef` conditional blocks:

```c
#ifdef WINDOWS
    printf("Windows code");   // Windows-ல் மட்டும்
#else
    printf("Linux code");     // Linux-ல் மட்டும்
#endif
```

Linux-ல் compile பண்ணினால் result:
```c
    printf("Linux code");     // Windows block deleted!
```

**Same source code → different OS-ல் different output!** இதுதான் cross-platform possible ஆவதற்கு காரணம்.

---

## hello.i-ல் இருக்கும் இந்த lines என்ன?

```c
# 1 "/usr/include/stdio.h" 1 3 4
```

இது **line marker** — compiler-க்கு சொல்வது:
```
# [line number] "[file name]" [flags]

1   = line 1 from this file
3   = system header file
4   = extern "C" context
```

Error வந்தால் compiler சரியான file+line number காட்ட இதை பயன்படுத்தும்.

---

## `extern int printf(...)` என்றால் என்ன?

```c
extern int printf(const char *__restrict __fmt, ...);
```

| Part | அர்த்தம் |
|---|---|
| `extern` | "இந்த function வேற எங்கோ defined — linker கண்டுபிடிக்கும்" |
| `int` | return type — printf எத்தனை characters print பண்ணியது |
| `printf` | function name |
| `const char *` | string pointer — "Hello World" |
| `...` | variadic — எத்தனை arguments வேணும்னாலும் |

**Declaration மட்டுமே** — actual printf code இங்கே இல்ல! அது libc.so-ல் இருக்கும், linker step-ல் connect ஆகும்.

---

## Overall Summary

```
hello.c (6 lines)
    ↓
Preprocessor:
    → #include expand    → 800 lines paste
    → #define replace    → values substitute  
    → comments delete    → clean code
    → #ifdef resolve     → correct platform code
    ↓
hello.i (~800 lines)
    → Pure C code, no directives
    → Compiler-க்கு ready
    → இன்னும் text file (binary இல்ல)
    → disk-ல் save (RAM இல்ல!)
```

**Key point:** hello.i இன்னும் human-readable text — binary ஆகவில்லை. அது Step 3, 4-ல் நடக்கும்! 🎯