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