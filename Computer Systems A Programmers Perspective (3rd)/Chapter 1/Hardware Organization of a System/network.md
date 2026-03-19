# 1.8 Systems Communicate with Other Systems Using Networks

## Core idea

இதுவரை system-ஐ isolated-ஆ treat பண்ணோம். Real world-ல systems networks மூலம் linked.

**Network = just another I/O device** (Figure 1.14)

```
Main memory → Network adapter → another machine
                 (instead of local disk)

Another machine → Network adapter → Main memory
```

Same pattern — sequence of bytes copy பண்றது. Destination மட்டும் different.

---

## Telnet example — Figure 1.15, 5 steps

![alt text](<Screenshot 2026-03-18 222519.png>)

## 5 Steps — book exact flow

**Step 1** — User types "hello" at keyboard → local telnet client receives it.

**Step 2** — Client sends "hello" string across network to telnet server.

**Step 3** — Telnet server passes string to remote shell → shell runs hello program → output goes back to telnet server.

**Step 4** — Telnet server forwards `"hello, world\n"` string across network to client.

**Step 5** — Client prints `"hello, world\n"` on local display.

---

## Book-ரோட key point

> "This type of exchange between clients and servers is typical of all network applications."

Email, instant messaging, WWW, FTP, telnet — எல்லாமே இந்த same pattern: **copy information from one machine to another**.

Chapter 11-ல network applications build பண்றது + simple Web server build பண்றது learn ஆகும்.

---

அடுத்து 1.9 (Important Themes) போகலாமா?