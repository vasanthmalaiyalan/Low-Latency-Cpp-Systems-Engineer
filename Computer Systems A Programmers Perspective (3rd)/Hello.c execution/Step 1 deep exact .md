சரி 👍
இப்போது **STEP-1 முழு flow-ஐ Linux kernel source level-க்கு அருகில் இருக்கும் real path-ஆக** பார்க்கலாம்.
(Keyboard → Kernel driver → TTY → User program → Save → Filesystem → Disk)

நாம் இரண்டு பகுதி பார்ப்போம்:

1️⃣ **Typing (# screen-ல் தோன்றுவது)**
2️⃣ **Save செய்தபின் disk-க்கு எழுதுவது**

---

# PART 1 — Keyboard → Screen

## 1️⃣ Keyboard hardware

நீ press செய்கிறாய்:

```
Shift + 3
```

Keyboard controller detect செய்கிறது:

```
scan code
```

Example (PS/2 set-2):

```
Shift press   → 0x12
3 press       → 0x26
3 release     → F0 26
Shift release → F0 12
```

இந்த data bus வழியாக computer-க்கு வரும்.

---

# 2️⃣ CPU interrupt

Hardware interrupt:

```
IRQ1
```

CPU interrupt handler run செய்யும்.

Kernel interrupt entry:

```
arch/x86/kernel/irq.c
```

Call path:

```
do_IRQ()
↓
handle_irq()
↓
keyboard interrupt handler
```

---

# 3️⃣ Linux keyboard driver

Driver file:

```
drivers/input/keyboard/atkbd.c
```

Driver scan code read செய்கிறது.

Example:

```
0x26
```

Driver convert செய்கிறது:

```
scan code → keycode
```

Example:

```
KEY_3
KEY_LEFTSHIFT
```

---

# 4️⃣ Linux input subsystem

Kernel subsystem:

```
drivers/input/input.c
```

Event create செய்யும்:

```
EV_KEY KEY_LEFTSHIFT 1
EV_KEY KEY_3         1
EV_KEY KEY_3         0
EV_KEY KEY_LEFTSHIFT 0
```

Meaning:

| field  | meaning        |
| ------ | -------------- |
| EV_KEY | keyboard event |
| KEY_3  | key identifier |
| 1      | press          |
| 0      | release        |

---

# 5️⃣ Keyboard layout mapping

Kernel TTY layer.

Source:

```
drivers/tty/vt/keyboard.c
```

Mapping table:

```
KEY_3 → '3'
SHIFT + KEY_3 → '#'
```

Character create ஆகிறது:

```
'#'
```

Encoding:

```
ASCII / UTF-8
```

Value:

```
0x23
```

---

# 6️⃣ Kernel TTY buffer

Kernel store செய்யும்:

```
tty input buffer
```

Memory example:

```
Kernel Space
-------------------
0xffff2000 : 0x23
```

இதுவரை **User space ஒன்றும் பார்க்கவில்லை**.

---

# 7️⃣ User program waiting

Editor program (example):

* Vim
* Visual Studio Code

Editor call:

```c
read(0, buf, 1);
```

Meaning:

| parameter | meaning     |
| --------- | ----------- |
| fd        | stdin       |
| buf       | user buffer |
| count     | bytes       |

---

# 8️⃣ Kernel → User copy

Kernel function:

```
tty_read()
↓
copy_to_user()
```

Example memory:

```
Kernel Space
0xffff2000 : 0x23
```

Copy →

```
User Space
0x7ffe91a0 : 0x23
```

---

# 9️⃣ Editor buffer update

Editor store செய்யும்:

```
editor text buffer
```

Example:

```
User Space
------------------
0x7ffe9100 : 0x23
```

Character:

```
#
```

---

# 🔟 Screen draw

Editor screen update:

```
editor
↓
write()
↓
kernel tty driver
↓
terminal
↓
GPU
↓
screen
```

Screen-ல் தெரியும்:

```
#
```

இதுதான் **நீ key press செய்த உடனே screen-ல் தெரியும் reason**.

---

# PART 2 — Save file

இப்போது நீ save செய்கிறாய்:

```
:w
```

---

# 1️⃣ User space write

Editor call:

```c
write(fd, buffer, size);
```

Example:

```c
write(3, 0x7ffe9100, 1);
```

Meaning:

| parameter  | meaning         |
| ---------- | --------------- |
| 3          | file descriptor |
| 0x7ffe9100 | user memory     |
| 1          | bytes           |

---

# 2️⃣ System call entry

CPU switch செய்யும்:

```
Ring 3 → Ring 0
```

Kernel entry:

```
sys_write()
```

Source:

```
fs/read_write.c
```

---

# 3️⃣ Kernel copy

Kernel function:

```
copy_from_user()
```

Example:

```
copy_from_user(kernel_buffer, 0x7ffe9100, 1)
```

Kernel memory:

```
Kernel Space
------------------
kernel_buffer : 0x23
```

---

# 4️⃣ VFS layer

Kernel Virtual File System:

```
vfs_write()
```

Source:

```
fs/vfs.c
```

Purpose:

```
filesystem independent interface
```

---

# 5️⃣ Filesystem driver

Example filesystem:

* ext4

Functions:

```
ext4_file_write_iter()
↓
ext4_map_blocks()
↓
ext4_mb_new_blocks()
```

Free block allocation:

```
bitmap check
↓
free block find
```

Example:

```
block 8421
```

---

# 6️⃣ Inode update

Example inode:

```
inode 14523
```

Metadata update:

```
size
timestamps
block pointers
```

Example:

```
inode 14523
size : 1 byte
block: 8421
```

---

# 7️⃣ Disk write

Data write:

```
block 8421
```

Disk:

```
0x23
```

Binary stored.

---

# FINAL FULL FLOW

```
keyboard press
↓
scan code
↓
kernel keyboard driver
↓
input subsystem
↓
keycode
↓
layout mapping
↓
character '#'
↓
kernel tty buffer
↓
read()
↓
user buffer
↓
editor buffer
↓
screen draw
↓
save command
↓
write()
↓
sys_write
↓
copy_from_user
↓
VFS
↓
filesystem
↓
inode update
↓
disk block write
```

---

✅ இப்போது **STEP-1 முழுவதும் hardware → kernel → user → filesystem → disk** exact execution பார்த்தோம்.

---

நீ விரும்பினால் அடுத்ததாக நான் ஒரு **மிக powerful diagram** காட்டலாம்:

```
CPU registers
Kernel memory
User memory
TTY buffer
Editor buffer
Disk block
```

இதில் **actual addresses உடன் (# எப்படி RAM-ல் move ஆகிறது)** என்பதை காட்ட முடியும்.
அது systems understanding-க்கு மிகவும் powerful.

