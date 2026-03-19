சரி 🔥 — இப்ப நீ கேட்டது **full deep execution (Step 1)**
👉 *keyboard press இருந்து → file save → RAM inode update வரை*
👉 **exact, clean, no confusion, full pipeline** 👇

---

# 🚀 STEP 1 — FULL EXECUTION (END-TO-END)

---

# 🔹 PART 1 — Keyboard → Kernel (hardware → kernel space)

```text
Keyboard press
↓
Keyboard controller (hardware)
↓
Scan code generate
   (e.g., Shift + 3 → make/break codes)
↓
CPU interrupt (IRQ)
↓
Kernel interrupt handler
↓
Keyboard driver (kernel space)
↓
Input subsystem
↓
Keycode உருவாகும் (e.g., KEY_3, KEY_LEFTSHIFT)
↓
Keyboard layout mapping
↓
Character உருவாகும் → '#'
↓
Encoding → UTF-8 → 0x23
```

👉 இங்கே வரை:

```text
Kernel Space
----------------
TTY buffer:
0xffff2000 : 0x23
```

---

# 🔹 PART 2 — Kernel → User (read path)

Editor already running (vim / vscode)

```c
read(0, buf, 1);
```

Flow:

```text
User Space (editor)
↓
read() syscall
↓
Kernel (tty_read)
↓
TTY buffer check
↓
data available (0x23)
↓
copy_to_user()
↓
User buffer update
```

👉 Result:

```text
User Space
-----------
buf[0] = 0x23   ('#')
```

---

# 🔹 PART 3 — Editor buffer + Screen

```text
editor internal buffer update
↓
screen draw request
↓
write() syscall (stdout / terminal)
↓
kernel tty driver
↓
terminal emulator
↓
GPU
↓
Screen shows: #
```

👉 முக்கியம்:

```text
👉 read() → data பெற
👉 write() → screen காட்ட
```

---

# 🔹 PART 4 — Save file (write path)

User action:

```text
:w   (vim save)
```

Editor call:

```c
write(fd, buffer, size);
```

Example:

```c
write(3, 0x7ffe9100, 1);
```

---

# 🔹 PART 5 — User → Kernel (write syscall)

```text
User Space
-----------
buf: '#'

↓ write()

Kernel Space
------------
sys_write()
↓
copy_from_user()
↓
kernel buffer = 0x23   ✅
```

👉 முக்கியம்:

```text
copy ஆகுவது → data மட்டும் (#)
copy ஆகாதது → fd, address, size
```

---

# 🔹 PART 6 — VFS layer

```text
vfs_write()
↓
fd → struct file
↓
file → inode
↓
filesystem type கண்டுபிடி (ext4)
```

👉 already known:

```text
inode exists
block mapping exists / இல்லா இருக்கலாம்
```

---

# 🔹 PART 7 — ext4_file_write_iter() (🔥 core)

```text
ext4_file_write_iter()

↓ offset = file->f_pos (e.g., 0)
↓ page கண்டுபிடி (page cache)
↓ inode → block mapping check
↓ (if needed) block allocate
↓ data copy → page cache
↓ mark page dirty
↓ inode (RAM) update
```

---

# 🔹 PART 8 — RAM state

```text
Page Cache (RAM)
----------------
[0] = 0x23   (#)

inode (RAM)
------------
size = 1
mtime updated
```

👉 இங்கே:

```text
dirty = 1   🔥
```

---

# ❗ முக்கிய நிலை

```text
❌ disk-ல் இன்னும் இல்லை
✅ RAM-ல் மட்டும் இருக்கிறது
```

---

# 🔹 PART 9 — Return to user

```c
write() → return 1
```

👉 meaning:

```text
"RAM-ல் எழுதிவிட்டேன்"
```

---

# 🧠 FULL PIPELINE (ONE VIEW)

```text
Keyboard
↓
Kernel (driver + input)
↓
TTY buffer (0x23)
↓
read()
↓
copy_to_user()
↓
Editor buffer
↓
write() (screen)
↓
Display '#'

-------------------------

Save:

write(fd, buf, 1)
↓
sys_write()
↓
copy_from_user()
↓
VFS
↓
ext4_file_write_iter()
↓
page cache (RAM)
↓
inode update (RAM)
↓
dirty = 1
```

---

# 🔑 FINAL ONE-LINE

```text
keyboard → kernel → read → user buffer → write → kernel →
filesystem → RAM page cache → inode update (dirty)
```

---

🔥 இப்ப நீ **complete Step 1 (end-to-end execution)** master பண்ணிட்ட

👉 next stage:

```text
dirty → disk actual write (writeback, block layer, IO scheduler)
```

😈 இது தான் real storage deep dive

---

சொல்லு — next stage போவோமா?


-----

நல்லது — ஒவ்வொரு part-உம் 100% accurate-ஆ explain பண்றேன்.

---

# PART 1 — Keyboard → Kernel

## Key Press → Scan Code

Keyboard-ல் ஒரு microcontroller இருக்கு (Intel 8042 அல்லது modern USB HID controller). Key matrix continuously scan பண்ணும்.

```
நீ '#' press பண்ற = Shift + 3

Keyboard controller detect பண்ணும்:
  Shift key down  → make code
  3 key down      → make code
  3 key up        → break code
  Shift key up    → break code
```

**Scan Code Sets — 3 types இருக்கு:**

```
Set 1 (XT) — legacy
Set 2 (AT) — keyboard hardware default ← இதுதான் keyboard அனுப்பும்
Set 3 — rare

Keyboard → Set 2 codes அனுப்பும்
8042 controller → Set 2 → Set 1-ஆ translate பண்ணும்
Kernel → Set 1 receive பண்ணும்
```

**Correct scan codes (Set 1):**
```
Left Shift press : 0x2A
3 key press      : 0x04  ← Set 2
                   0x02  ← Set 1 (kernel receive பண்றது இது)
Left Shift release: 0xAA (0x2A | 0x80)
3 key release    : 0x82  (0x02 | 0x80)
```

---

## IRQ → CPU Interrupt

```
Keyboard controller → IRQ1 signal → PIC/APIC
↓
CPU current instruction finish பண்ணும்
↓
CPU EFLAGS register-ல் interrupt flag check
↓
IF=1 (interrupts enabled) → accept
↓
Current register state → kernel stack-ல் push:
  RIP (instruction pointer)
  CS (code segment)
  RFLAGS
  RSP (stack pointer)
  SS (stack segment)
↓
IDT[1] lookup → keyboard handler address
↓
handler-க்கு jump
```

**IDT (Interrupt Descriptor Table):**
```
256 entries — ஒவ்வொரு entry-உம் ஒரு handler address
IRQ1 → vector 33 (0x21) → atkbd_interrupt() address
```

---

## Kernel Driver → Input Subsystem

```c
// drivers/input/keyboard/atkbd.c
atkbd_interrupt(struct serio *serio, unsigned char data, unsigned int flags)
{
    // data = scan code (0x02 for '3')
    // Shift state track பண்ணும்
    // input_event() call பண்ணும்
}
```

```
atkbd_interrupt()
↓
input_event(dev, EV_MSC, MSC_SCAN, scancode)  // raw scan code log
input_event(dev, EV_KEY, KEY_LEFTSHIFT, 1)     // shift pressed
input_event(dev, EV_KEY, KEY_3, 1)             // 3 pressed
input_sync(dev)                                 // event complete
```

---

## Keycode → Character

```
Input subsystem → evdev layer → TTY layer
↓
TTY line discipline (n_tty)
↓
Keyboard layout map (keymaps)
KEY_3 + SHIFT → '#'
↓
UTF-8 encode → 0x23
↓
n_tty buffer-ல் store
```

**TTY buffer:**
```
n_tty internal ring buffer
address: kernel virtual address (KASLR randomized — fixed address கொடுக்க முடியாது)
content: [0x23]
```

---

# PART 2 — read() Syscall — Exact Path

## Modern x86-64 Syscall Mechanism

```
❌ int 0x80 — 32-bit legacy ONLY, modern 64-bit use பண்றதில்லை
✅ syscall instruction — 64-bit standard
```

```
Editor process:
read(0, buf, 1)
↓
glibc:
  mov rax, 0      ← syscall number for read
  mov rdi, 0      ← fd (stdin)
  mov rsi, buf    ← user buffer address
  mov rdx, 1      ← count
  syscall         ← kernel mode switch
↓
CPU:
  CS, SS → kernel segments
  RSP → kernel stack (per-CPU)
  RIP → entry_SYSCALL_64() [arch/x86/entry/entry_64.S]
↓
entry_SYSCALL_64()
↓
sys_call_table[0] → __x64_sys_read()
↓
ksys_read()
↓
vfs_read()
↓
tty_read()
↓
n_tty_read()
↓
data available check:
  n_tty buffer-ல் 0x23 இருக்கு
↓
copy_to_user(buf, &tty_buf[0], 1)
```

**copy_to_user() — exact work:**
```
1. buf address valid-ஆ இருக்கா? (access_ok() check)
2. Page fault handler setup
3. CPU: kernel page tables → user page accessible பண்ணும்
4. memcpy equivalent
5. Return: bytes copied
```

```
↓
return to entry_SYSCALL_64()
↓
sysret instruction
↓
CPU: kernel mode → user mode
↓
editor: buf[0] = 0x23 = '#'
```

---

# PART 3 — Screen Display — Full Path

```
editor: buf[0] = '#' received
↓
editor internal buffer update
(vim: gap buffer / vscode: piece tree)
↓
cursor position update
↓
write(STDOUT_FILENO, "#", 1)
↓
sys_write() → tty_write()
↓
terminal emulator receive (e.g., GNOME Terminal / Windows Terminal)
↓
VTE / ConPTY parse escape sequences
↓
Pango / Cairo — font selection
↓
FreeType — glyph rasterization
  '#' → font file lookup → bezier curves → pixel bitmap
↓
OpenGL / Vulkan / Cairo draw call
↓
GPU driver (Mesa / proprietary)
↓
GPU: pixels render → framebuffer
↓
Display controller → monitor
↓
Screen: '#' visible
```

---

# PART 4, 5 — write() Syscall — Save Path

```c
write(fd, buffer, size);
// fd     = 3
// buffer = user space virtual address (e.g., 0x7ffe9100)
// size   = 1
```

**fd → kernel object chain:**
```
process → struct task_struct
        → struct files_struct
        → fd_array[3]
        → struct file *
        → struct file {
            f_pos,        // current offset
            f_inode,      // inode pointer
            f_op,         // file operations (ext4_file_operations)
          }
```

**Syscall:**
```
write(3, 0x7ffe9100, 1)
↓
syscall instruction
↓
__x64_sys_write()
↓
ksys_write()
↓
vfs_write()
↓
copy_from_user(kernel_buf, 0x7ffe9100, 1)
  → address valid check
  → page accessible check
  → kernel_buf = 0x23 ✅
```

---

# PART 6 — VFS Layer — Exact

```
vfs_write()
↓
file_operations check:
  ext4_file_operations.write_iter = ext4_file_write_iter
↓
__vfs_write()
↓
ext4_file_write_iter()
```

**VFS abstraction:**
```
同じ vfs_write() call →
  ext4  → ext4_file_write_iter()
  xfs   → xfs_file_write_iter()
  tmpfs → shmem_write_iter()
  btrfs → btrfs_file_write_iter()
```

---

# PART 7 — ext4_file_write_iter() — 100% Accurate

## New file vs Existing file — important distinction:

```
Case 1: Brand new file (first write ever)
→ page cache-ல் page இல்லை
→ disk-ல் data இல்லை
→ new page allocate பண்ணும் (zero-filled)
→ disk read தேவையில்லை ✅

Case 2: Existing file, partial write
→ page cache miss → disk-ல் இருந்து page read
→ modify
→ write back
→ இதுதான் read-modify-write
```

```
ext4_file_write_iter()
↓
inode lock (inode_lock)
↓
file offset: f_pos = 0
↓
generic_perform_write()
↓
page cache lookup:
  find_or_create_page(inode->i_mapping, page_index)
↓
new file → zero page allocate
↓
iov_iter_copy_from_user_atomic()
  → data copy: page[0] = 0x23
↓
set_page_dirty(page)
  → page->flags |= PG_dirty
  → inode → dirty inode list-ல் add
↓
inode update (RAM only):
  i_size = 1
  i_mtime = current_time()
  i_ctime = current_time()
↓
inode_unlock()
```

---

# PART 8 — RAM State — Exact

```
Page Cache:
  struct page {
    flags: PG_dirty = 1, PG_uptodate = 1
    mapping: → inode->i_mapping
    index: 0 (first page)
    data: [0x23, 0x00, 0x00, ...]  // 4KB page, rest zeros
  }

inode (RAM — struct inode):
  i_size  = 1
  i_mtime = <current timestamp>
  i_ctime = <current timestamp>
  i_state = I_DIRTY_SYNC | I_DIRTY_DATASYNC

Dirty tracking:
  Page: PG_dirty flag set
  Inode: dirty list-ல் இருக்கு
  Superblock: dirty inodes list
```

---

# PART 9 — Return Value — Exact

```c
write() → return 1
```

**Meaning:**
```
"1 byte page cache-ல் எழுதப்பட்டது"
Disk guarantee: இல்லை
```

**Disk guarantee வேணும்னா:**
```c
// Option 1:
fsync(fd);
// page cache → disk flush, inode metadata flush
// return: disk write confirm ஆனா மட்டும்

// Option 2:
fdatasync(fd);
// data flush மட்டும், metadata (mtime) flush இல்லை
// fsync-ஐ விட faster

// Option 3: open பண்ணும்போதே
open("file", O_WRONLY | O_SYNC);
// every write() → automatic disk sync
// performance cost அதிகம்

// Real usage:
// PostgreSQL, SQLite → fsync() mandatory
// அதனால்தான் crash-safe
```

---

# FULL 100% ACCURATE PIPELINE

```
⌨️  Shift+3 press
    ↓
🔌  Keyboard controller → Set 2 scan codes
    8042 → Set 1 translate
    IRQ1 → APIC → CPU
    ↓
💾  CPU: registers save → kernel stack
    IDT[33] → atkbd_interrupt()
    ↓
🖥️  atkbd_interrupt()
    → input_event(EV_KEY, KEY_3, 1)
    ↓
📥  Input subsystem → evdev → TTY
    n_tty line discipline
    KEY_3 + SHIFT → '#' → UTF-8 0x23
    n_tty ring buffer: [0x23]
    ↓
📖  editor: read(0, buf, 1)
    syscall instruction (NOT int 0x80)
    entry_SYSCALL_64()
    sys_read() → tty_read() → n_tty_read()
    copy_to_user() → buf[0] = 0x23
    sysret → user mode
    ↓
✏️  editor buffer: '#' added
    ↓
🖥️  write(1, "#", 1)
    tty_write → terminal emulator
    Pango/FreeType → glyph rasterize
    OpenGL/Vulkan → GPU
    framebuffer → display
    Screen: '#'

──────── SAVE (:w) ────────

💾  write(3, buf, 1)
    syscall → __x64_sys_write()
    vfs_write()
    copy_from_user() → kernel_buf = 0x23
    ↓
🗂️  fd[3] → struct file → f_op → ext4
    ext4_file_write_iter()
    ↓
📄  page cache:
    new file → zero page allocate (no disk read)
    page[0] = 0x23
    PG_dirty = 1
    ↓
📊  inode (RAM):
    i_size=1, i_mtime=now
    I_DIRTY_SYNC flag set
    ↓
✅  return 1

Status:
  RAM: ✅ data இருக்கு
  Disk: ❌ இன்னும் போகல
  Power cut: ⚠️ data lost

fsync(fd) → disk guarantee
```

---

# Next: Dirty → Disk (Writeback Path)

```
pdflush / wb_workfn thread
↓
dirty ratio threshold check
(default: 20% RAM dirty ஆனா flush)
↓
writeback_single_inode()
↓
block layer
↓
bio (Block I/O) structure
↓
IO scheduler (mq-deadline / none)
↓
NVMe / SATA driver
↓
disk controller
↓
actual write
↓
completion interrupt
↓
PG_dirty clear
↓
I_DIRTY clear
```

போவோமா?

----

# Scan Code Sets — Deep Dive

---

## முதல்ல Background: ஏன் multiple sets இருக்கு?

Keyboard protocol 1981-ல் IBM PC-ல் start ஆச்சு. அப்புறம் hardware evolve ஆனது, ஆனால் **backward compatibility** வேணும் என்பதால் பழைய format-உம் support பண்ணாங்க. அதனால் 3 generations of scan code sets உருவானது.

---

## Set 1 — XT (1981)

IBM PC XT-ல் introduce ஆனது. First generation.

```
Hardware: Intel 8048 keyboard controller
PC side:  Intel 8255 chip

'A' key press   → 0x1E  (make code)
'A' key release → 0x9E  (break code = make | 0x80)
```

**Pattern simple:**
```
break code = make code | 0x80

make:  0x1E = 0001 1110
break: 0x9E = 1001 1110
            = 0x1E | 0x80
```

**Problem:** Only 83 keys support. New keys (F11, F12, arrow keys) → escape sequence வேணும்.

```
Extended keys → 0xE0 prefix
Right Ctrl press: E0 1D
Right Ctrl release: E0 9D
```

---

## Set 2 — AT (1984)

IBM PC AT-ல் introduce ஆனது. Modern keyboards default இது.

```
Hardware: Intel 8042 controller (keyboard side)

'A' key press   → 0x1C        (make code)
'A' key release → 0xF0 0x1C   (break code = F0 prefix + make)
```

**Set 1 vs Set 2 comparison:**

```
Key        Set 1 Make   Set 2 Make
─────────────────────────────────
A          0x1E         0x1C
S          0x1F         0x1B
3          0x04         0x02  ← உன் question இது!
Space      0x39         0x29
Enter      0x1C         0x5A
Left Shift 0x2A         0x12
```

**Break code difference:**
```
Set 1: make | 0x80
  A release = 0x1E | 0x80 = 0x9E

Set 2: F0 prefix + make code
  A release = F0 1C
```

**Extended keys:**
```
Set 2-ல் extended keys → E0 prefix
Right Ctrl press:   E0 14
Right Ctrl release: E0 F0 14
```

---

## Set 3 — PS/2 Extended (1984)

IBM PS/2 keyboards-ல் introduce ஆனது. Rare — almost use ஆவதில்லை.

```
'A' key press   → 0x1C  (make)
'A' key release → 0xF0 0x1C  (break — Set 2 போல)
```

**Key difference:**
```
Set 2: Extended keys → E0 prefix needed
Set 3: Every key → unique code, E0 prefix தேவையில்லை
       Simple, clean, ஆனால் adoption இல்லை
```

---

## 8042 Controller — The Translator

இதுதான் central piece. Keyboard-ல் ஒரு 8042, PC motherboard-ல் ஒரு 8042.

```
Physical path:

[Keyboard]                    [Motherboard]
  8048/8051          PS/2 cable        8042
  microcontroller  ──────────────►  controller
  
  Set 2 codes                    Set 1-ஆ translate
  அனுப்பும்                      பண்ணி CPU-க்கு அனுப்பும்
```

**8042 என்ன பண்றது exactly:**

```
Step 1: Keyboard → Set 2 code அனுப்பும்
        'A' press → 0x1C

Step 2: 8042 receive பண்ணும்

Step 3: 8042 internal translation table:
        Set 2: 0x1C → Set 1: 0x1E

Step 4: 8042 → IRQ1 trigger

Step 5: CPU → I/O port 0x60 read பண்ணும்
        Set 1 code: 0x1E
```

**Translation table example (8042 internal):**

```
Set 2 → Set 1
──────────────
0x1C  → 0x1E  (A)
0x1B  → 0x1F  (S)
0x02  → 0x04  (3) ← உன் example
0x12  → 0x2A  (Left Shift)
0x5A  → 0x1C  (Enter)
0x29  → 0x39  (Space)
```

**8042 translation disable பண்ணலாம்:**
```
OS → 8042-க்கு command அனுப்பலாம்:
"translation off — raw Set 2 codes கொடு"

Modern Linux → இப்படி பண்ணி
              raw Set 2 codes பெறுவதும் உண்டு
              atkbd driver handle பண்ணும்
```

---

## Kernel Side — atkbd driver

```c
// drivers/input/keyboard/atkbd.c

atkbd_interrupt(struct serio *serio,
                unsigned char data,   // ← Set 1 code (from 8042)
                unsigned int flags)
{
    // data = 0x04 (Set 1 code for '3')

    // Extended key check
    if (data == 0xe0) {
        atkbd->emul = 1;  // next byte is extended
        return;
    }

    // Break code check (bit 7)
    if (data & 0x80) {
        // Key release
        keycode = data & 0x7f;  // remove break bit
        value = 0;              // key up
    } else {
        // Key press
        keycode = data;
        value = 1;              // key down
    }

    // Set 1 code → Linux keycode
    // atkbd->keycode[] lookup table
    linux_keycode = atkbd->keycode[keycode];

    input_event(dev, EV_KEY, linux_keycode, value);
}
```

**Linux keycode layer:**
```
Set 1 scan code → Linux keycode (different numbering)
0x02 → KEY_3  (value: 4 in linux/input-event-codes.h)
0x2A → KEY_LEFTSHIFT (value: 42)
```

---

## '#' உருவாகும் full path — exact

```
Physical:
  Shift + 3 press
  ↓
Keyboard (8048):
  Set 2 make codes generate:
  Left Shift → 0x12
  3          → 0x02
  ↓
PS/2 wire:
  0x12 அனுப்பும்
  0x02 அனுப்பும்
  ↓
8042 (motherboard):
  0x12 → translate → 0x2A  (Set 1 Left Shift make)
  0x02 → translate → 0x04  (Set 1 '3' make)

  ⚠️ Note: Set 1 '3' = 0x04, Set 2 '3' = 0x02
  இதுதான் earlier confusion — kernel Set 1 receive பண்றது

  IRQ1 trigger (twice — one per key)
  ↓
CPU (I/O port 0x60):
  read 0x2A → Left Shift make
  read 0x04 → 3 make
  ↓
atkbd_interrupt():
  0x2A → KEY_LEFTSHIFT → input_event pressed
  0x04 → KEY_3        → input_event pressed
  ↓
Input subsystem:
  EV_KEY, KEY_LEFTSHIFT, 1
  EV_KEY, KEY_3, 1
  ↓
TTY line discipline (n_tty):
  KEY_3 + SHIFT state = 1
  keymap lookup → '#'
  UTF-8 encode  → 0x23
  n_tty buffer  → [0x23]
```

---

## Summary Table

```
Set   Year  Default     Make      Break         Used By
────────────────────────────────────────────────────────
1     1981  XT legacy   0x04(3)   0x84(3 up)    Kernel receives
2     1984  Keyboard    0x02(3)   F0 02(3 up)   Keyboard sends
3     1984  Rare        0x02(3)   F0 02(3 up)   Almost unused

Flow:
Keyboard → Set 2 → [8042 translate] → Set 1 → Kernel
```

---

## USB Keyboards — Modern Reality

```
PS/2: 8042 controller path (above)

USB:
  Keyboard → USB HID protocol
  → USB HID scan codes (different from PS/2!)
  → usbhid kernel driver
  → input subsystem
  → atkbd bypass! (direct to input layer)
  → same Linux keycodes result

USB HID '3' → Usage ID 0x20
PS/2 Set 2  '3' → 0x02
PS/2 Set 1  '3' → 0x04

எல்லாமே finally → KEY_3 (Linux keycode 4)
```

இதுவே 100% complete scan code explanation. Next — dirty writeback → disk போவோமா?