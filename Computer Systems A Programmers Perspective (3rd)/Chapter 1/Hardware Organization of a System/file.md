# 1.7.4 Files

## Core idea — Book exact definition

> "A file is a sequence of bytes, nothing more and nothing less."

## Everything is a file

Every I/O device — file-ஆ model ஆகுது:

```
Disk        → file
Keyboard    → file
Display     → file
Network     → file
```

எல்லா input/output-உம் **reading and writing files** மூலம் நடக்குது — small set of system calls known as **Unix I/O**.

---

## ஏன் இது powerful

Book சொல்ற reason: applications-க்கு எல்லா varied I/O devices-உம் **uniform view** கிடைக்குது.

**Example (book சொல்றது):** Disk file manipulate பண்ற programmer-க்கு specific disk technology தெரியவே தெரியாது. Same program different disk technologies use பண்ற different systems-ல run ஆகும்.

---

## Unix I/O — Same calls, every device

```c
// Disk file
int fd = open("hello.txt", O_RDONLY);
read(fd, buf, 100);

// Keyboard (stdin = fd 0)
read(0, buf, 100);

// Display (stdout = fd 1)
write(1, "hello\n", 6);

// Network socket
int sock = socket(...);
read(sock, buf, 100);   // same read()!
write(sock, buf, 100);  // same write()!
```

`open`, `read`, `write`, `close` — இந்த same system calls எல்லா devices-க்கும் work ஆகும். Chapter 10-ல Unix I/O deep-ஆ வரும்.

---

அடுத்து 1.8 (Networks) போகலாமா?