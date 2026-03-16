சரி 👍
நாம் **STEP-1**-ல் பார்த்த மாதிரி (hardware → kernel → user → filesystem) இப்போது **STEP-2 Preprocessing**-ஐயும் **exact execution + Linux internals** உடன் பார்க்கலாம்.
இங்கு முக்கியம்: **compiler mostly user space-ல் தான் வேலை செய்கிறது**. Kernel வேலை mainly **file I/O + process management**.

நாம் பார்க்கும் pipeline:

```
terminal command
↓
shell
↓
kernel (process create)
↓
gcc process (user space)
↓
cpp preprocessor
↓
read header files
↓
text transformation
↓
write hello.i
```

---

# STEP 2 — Preprocessing (hello.i)

Command:

```bash
gcc -E hello.c -o hello.i
```

Meaning:

| option       | meaning                    |
| ------------ | -------------------------- |
| `gcc`        | compiler driver            |
| `-E`         | preprocessing மட்டும் செய் |
| `hello.c`    | input                      |
| `-o hello.i` | output file                |

---

# 1️⃣ Terminal command

**User Space**

நீ terminal-ல் type செய்கிறாய்:

```bash
gcc -E hello.c -o hello.i
```

Terminal program (shell) example:

* Bash

Shell command parse செய்கிறது.

Example parsed arguments:

```
argv[0] = "gcc"
argv[1] = "-E"
argv[2] = "hello.c"
argv[3] = "-o"
argv[4] = "hello.i"
```

---

# 2️⃣ Process creation

Shell kernel-க்கு request செய்கிறது.

System call:

```c
fork()
execve("/usr/bin/gcc", argv, env)
```

**Kernel Space**

Kernel செய்யும் வேலை:

```
create new process
load gcc executable
setup memory
```

Process memory layout:

```
User Space
-----------------
.text
.data
.heap
.stack
```

gcc program start ஆகும்.

---

# 3️⃣ gcc driver

**User Space**

gcc driver program decide செய்கிறது:

```
pipeline
```

Normal compilation:

```
cpp → cc1 → as → ld
```

ஆனால் `-E` flag இருந்ததால்:

```
cpp மட்டும்
```

Preprocessor program:

```
cpp
```

---

# 4️⃣ hello.c open

gcc file open செய்கிறது.

User space call:

```c
open("hello.c", O_RDONLY)
```

**Kernel Space**

Kernel VFS layer:

```
sys_open()
↓
vfs_open()
↓
ext4 filesystem
```

File locate:

```
hello.c → inode → disk blocks
```

Kernel return செய்கிறது:

```
fd = 3
```

User space-க்கு.

---

# 5️⃣ File read

gcc source file read செய்கிறது.

User space call:

```c
read(3, buffer, size)
```

Kernel:

```
sys_read()
↓
vfs_read()
↓
page cache
↓
disk (if needed)
```

Kernel copy:

```
kernel buffer → user buffer
```

User memory example:

```
0x7fff5000
------------------
#include <stdio.h>
int main() {
    printf("Hello\n");
}
```

---

# 6️⃣ Preprocessor start

**User Space**

Program:

```
cpp
```

Input:

```
source text
```

Preprocessor analyse செய்யும் directives:

```
#include
#define
#ifdef
comments
```

---

# 7️⃣ #include expansion

Code:

```c
#include <stdio.h>
```

Preprocessor search path:

```
/usr/include
```

User call:

```c
open("/usr/include/stdio.h")
```

Kernel:

```
sys_open()
↓
VFS
↓
filesystem
↓
inode
```

File read:

```c
read(fd, buffer, size)
```

Header content load செய்யப்படும்.

Example snippet:

```c
extern int printf(const char *, ...);
```

---

# 8️⃣ Copy paste include

Preprocessor **text replace** செய்கிறது.

Before:

```c
#include <stdio.h>

int main() {
```

After:

```c
extern int printf(const char *, ...);

int main() {
```

Actually:

```
stdio.h + other headers
≈ hundreds lines
```

---

# 9️⃣ Macro expansion

Example:

```c
#define MAX 100
int arr[MAX];
```

Preprocessor replace செய்கிறது:

```
int arr[100];
```

Process:

```
token search
↓
text replacement
```

---

# 🔟 Comment removal

Input:

```c
// comment
/* comment */
```

Output:

```
(deleted)
```

Reason:

```
compiler-க்கு தேவையில்லை
```

---

# 1️⃣1️⃣ Conditional compilation

Example:

```c
#ifdef DEBUG
printf("debug\n");
#endif
```

Preprocessor check செய்கிறது:

```
macro defined?
```

Result:

```
block keep / remove
```

---

# 1️⃣2️⃣ Output buffer

Final preprocessed text memory-ல் இருக்கும்.

Example:

```
User Space
-----------------------
0x7fff8000

extern int printf(const char *, ...);

int main() {
    printf("Hello\n");
    return 0;
}
```

---

# 1️⃣3️⃣ Write hello.i

gcc call செய்கிறது:

```c
open("hello.i", O_WRONLY)
write(fd, buffer, size)
```

Kernel:

```
sys_write()
↓
VFS
↓
ext4
↓
page cache
↓
disk
```

Disk block example:

```
inode → blocks
```

File created:

```
hello.i
```

---

# STEP-2 Complete pipeline

```
User Space
-----------
terminal command
↓
shell
↓
gcc process
↓
cpp preprocessor
↓
text transformation

Kernel Space
-------------
open()
read()
write()
filesystem
disk I/O
```

---

# Result

Output file:

```
hello.i
```

Example content:

```
extern int printf(const char *, ...);

int main() {
    printf("Hello, World!\n");
    return 0;
}
```

Real output:

```
~800 lines
```

---

# Important CSAPP concept

இந்த stage-ல் நடப்பது:

```
source code transformation
```

Binary இன்னும் இல்லை.

Pipeline:

```
hello.c
↓
preprocessor
↓
hello.i
```

---

✅ இப்போது **STEP-2 exact execution (user space + kernel space + filesystem)** முழுவதும் பார்த்தோம்.

---

நீ விரும்பினால் அடுத்ததாக நான் **STEP-2-இன் உள்ளே cpp internal algorithm (include search, macro table, token buffer)** எப்படி வேலை செய்கிறது என்பதை **compiler source code level-ல்** காட்ட முடியும்.
அது systems programming-ல் மிகவும் deep concept.
