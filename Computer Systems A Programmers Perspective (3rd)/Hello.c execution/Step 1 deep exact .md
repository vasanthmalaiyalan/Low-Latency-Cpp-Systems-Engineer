சரி 👍
நாம் இதுவரை பேசின **அனைத்து concepts** (keyboard → kernel → user space → editor → filesystem → disk) ஒன்றாக சேர்த்து **exact execution pipeline** காட்டுகிறேன்.
ஒவ்வொரு இடத்திலும் **Hardware / Kernel Space / User Space** என்று தெளிவாக பிரிக்கிறேன்.

---

# 0️⃣ ஆரம்ப நிலை (Editor already running)

**User Space**

உதாரணம் editor:

* Vim
* Visual Studio Code

Editor terminal-ல் ஓடிக் கொண்டிருக்கிறது.

Editor code input காத்திருக்கிறது:

```c
char buf[1];
read(0, buf, 1);
```

Meaning:

| parameter | meaning     |
| --------- | ----------- |
| `0`       | stdin       |
| `buf`     | user memory |
| `1`       | 1 byte      |

User memory:

```
User Space
----------------------
0x7ffe9100 : ??
```

---

# 1️⃣ Keyboard press

**Hardware**

நீ press செய்கிறாய்:

```
Shift + 3
```

PS/2 keyboard controller detect செய்கிறது.

Scan codes generate:

```
Shift press   → 0x12
3 press       → 0x26
3 release     → F0 26
Shift release → F0 12
```

இந்த bytes computer-க்கு அனுப்பப்படும்.

---

# 2️⃣ CPU interrupt

Keyboard hardware interrupt trigger செய்கிறது:

```
IRQ1
```

CPU:

```
User Mode → Kernel Mode
```

Kernel interrupt handler run ஆகும்.

---

# 3️⃣ Keyboard driver

**Kernel Space**

Linux keyboard driver scan code read செய்கிறது.

Example:

```
0x26
```

Driver convert செய்கிறது:

```
scan code → keycode
```

Example Linux keycodes:

```
KEY_3 = 4
KEY_LEFTSHIFT = 42
```

Event generate:

```
EV_KEY KEY_LEFTSHIFT 1
EV_KEY KEY_3        1
EV_KEY KEY_3        0
EV_KEY KEY_LEFTSHIFT 0
```

இதுவரை **character உருவாகவில்லை**.

---

# 4️⃣ Keyboard layout mapping

**Kernel Space**

Kernel terminal subsystem check செய்கிறது:

```
modifier = SHIFT
key = KEY_3
```

Layout table:

```
KEY_3       → '3'
SHIFT+KEY_3 → '#'
```

இப்போது character உருவாகிறது:

```
#
```

Encoding:

```
ASCII = 35
hex = 0x23
binary = 00100011
```

---

# 5️⃣ Kernel TTY buffer

Character store செய்யப்படும்:

```
TTY input buffer
```

Example memory:

```
Kernel Space
----------------------
0xffff2000 : 0x23
```

---

# 6️⃣ read() system call wake up

Editor program:

```
read(0, buf, 1)
```

Kernel இந்த call satisfy செய்யும்.

Kernel copy செய்கிறது:

```
copy_to_user(buf, tty_buffer, 1)
```

---

# 7️⃣ Kernel → User memory copy

Memory change:

```
Kernel Space
----------------------
0xffff2000 : 0x23

copy →

User Space
----------------------
0x7ffe9100 : 0x23
```

Meaning:

```
buf[0] = '#'
```

---

# 8️⃣ Editor internal buffer update

**User Space**

Editor text buffer:

```
#
```

Editor screen redraw செய்கிறது.

Terminal output:

```
#
```

நீ இப்போது screen-ல் **# பார்க்கிறாய்**.

---

# 9️⃣ File save

நீ save செய்கிறாய்:

```
:w
```

Editor system call:

```
write(fd, buffer, size)
```

Example:

```
write(3, 0x7ffe9100, 1)
```

Meaning:

| parameter    | meaning                   |
| ------------ | ------------------------- |
| `3`          | file descriptor (hello.c) |
| `0x7ffe9100` | user memory address       |
| `1`          | bytes                     |

---

# 🔟 Kernel write

**Kernel Space**

System call:

```
sys_write()
```

Kernel copy செய்கிறது:

```
copy_from_user(kernel_buffer, 0x7ffe9100, 1)
```

Memory:

```
Kernel Space
----------------------
kernel_buffer : 0x23
```

---

# 1️⃣1️⃣ VFS layer

Kernel check செய்கிறது:

```
fd = 3
```

Process file descriptor table:

```
0 → stdin
1 → stdout
2 → stderr
3 → hello.c
```

Kernel call chain:

```
sys_write()
↓
vfs_write()
↓
filesystem driver
```

---

# 1️⃣2️⃣ Filesystem driver

Example filesystem:

* ext4

Filesystem determine செய்கிறது:

```
inode number
data block
offset
```

Example:

```
hello.c → inode 14523
```

inode structure:

```
inode 14523
-----------------
size: 56
blocks: [8421]
```

---

# 1️⃣3️⃣ Disk block update

Disk block size:

```
4096 bytes
```

Actual data write:

```
block 8421
-------------------
23
-------------------
```

Meaning:

```
#
```

---

# COMPLETE PIPELINE

```
Hardware
---------
keyboard press
↓
scan code

Kernel Space
-------------
interrupt
↓
keyboard driver
↓
scan code → keycode
↓
keyboard layout mapping
↓
character '#'
↓
TTY buffer

User Space
-----------
read()
↓
editor buffer
↓
screen display

User Space
-----------
write()

Kernel Space
-------------
copy_from_user()
↓
VFS
↓
filesystem driver
↓
inode update
↓
disk block write
```

---

# Final concept (CSAPP idea)

CSAPP book சொல்லும் core idea:

```
All information in a computer system is represented as bits.
```

Example:

```
'#'
ASCII 35
hex 0x23
binary 00100011
```

அதே value:

```
keyboard
RAM
kernel
filesystem
disk
```

எங்கும் **bits ஆகவே travel செய்கிறது**.

---

✅ இப்போது நாம் **keyboard → OS → program → filesystem → disk** முழு execution flow பார்த்துவிட்டோம்.

---

நீ விரும்பினால் அடுத்ததாக நான் **`gcc hello.c` compile execution deep pipeline (preprocessor → compiler → assembler → linker → ELF → CPU execution)** இதே மாதிரி deep-ஆ காட்டலாம்.
அது CSAPP Chapter-1ல் வரும் next big concept.


----

# inode clarity 

ஆம் 👍 **மிகவும் சரியான புரிதல்** — ஆனால் ஒரு சிறிய திருத்தம் தேவை.
**inode already இருக்கிறது**, ஆனால் **free block allocation inode-க்கு முன் நடக்கும்** என்று சொல்லுவது முழுமையாக சரி அல்ல.
சரியான வரிசை என்னவென்றால்:

> **filesystem முதலில் inode-ஐ identify செய்கிறது → பிறகு free block allocate செய்கிறது → பிறகு inode-ஐ update செய்கிறது**

அதாவது **inode இல்லாமல் allocator வேலை செய்யாது**, ஏனெனில் allocator-க்கு file context (inode) தேவை.

இதை exact order-ஆ பார்க்கலாம்.

---

# 1️⃣ write() system call

User space

```c
write(fd, buf, 1);
```

Kernel path:

```
sys_write()
↓
vfs_write()
↓
filesystem (ext4)
```

---

# 2️⃣ inode identify

Kernel முதலில் file-ஐ கண்டுபிடிக்கும்.

```
fd → struct file → struct inode
```

Example

```
hello.c → inode 14523
```

inode metadata:

```
inode 14523
----------------
size: 56
block pointers: [8420]
```

---

# 3️⃣ file offset determine

filesystem பார்க்கும்:

```
file size = 56
```

write செய்ய வேண்டிய இடம்:

```
offset = 56
```

---

# 4️⃣ free block allocation

இங்கே தான் allocator வேலை செய்யும்.

ext4 allocator:

```
ext4_mb_new_blocks()
```

filesystem bitmap பார்க்கும்:

```
free block bitmap
--------------------------
8420 used
8421 free
8422 free
```

allocator select செய்கிறது:

```
block = 8421
```

---

# 5️⃣ inode update

இப்போது inode update செய்யப்படும்.

Before:

```
inode 14523
blocks: [8420]
```

After:

```
inode 14523
blocks: [8420, 8421]
```

அதாவது:

```
inode → data block pointer
```

---

# 6️⃣ page cache update

Linux direct disk write செய்யாது.

RAM-ல் page cache update:

```
page cache
------------------
block 8421
data = 0x23
```

page mark செய்யப்படும்:

```
dirty page
```

---

# 7️⃣ writeback

background kernel thread flush செய்யும்:

```
page cache
↓
block layer
↓
device driver
↓
disk
```

---

# 8️⃣ disk block write

disk update:

```
block 8421
----------------
0x23
----------------
```

meaning

```
'#'
```

---

# 🔑 Correct execution order

```
write()
↓
identify inode
↓
calculate file offset
↓
allocate free block
↓
update inode block pointer
↓
update page cache
↓
writeback
↓
disk write
```

---

✔️ அதனால் உன் statement:

> inode-க்கு முன்பே allocator நடக்கிறது

❌ **சரியானது அல்ல**

✔️ **correct statement**

```
inode identify
↓
allocator free block choose
↓
inode update
```

---

💡 ஒரு முக்கிய insight:

Linux filesystem-ல் **inode = file identity**
**block allocator = storage manager**

இரண்டும் சேர்ந்து தான் file write நடக்கும்.

------

