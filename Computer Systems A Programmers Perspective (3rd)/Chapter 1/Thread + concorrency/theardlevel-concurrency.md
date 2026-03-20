உன் laptop — Lenovo IdeaPad Gaming 3, 8GB — இந்த exact concepts apply ஆகுது. அதோட specs-ஓட connect பண்ணி explain பண்றேன்.

---

# 1.9.2 Thread-Level Concurrency

## Concurrency — simulated vs real

```
Old (uniprocessor):
  Single CPU, rapidly switches between processes
  "Juggler keeping multiple balls in air"
  Concurrency = illusion (simulated)

Modern (multiprocessor):
  Multiple CPUs actually running simultaneously
  Concurrency = real (parallel execution)
```

---

## Processor taxonomy — Figure 1.16---

## Multi-core — Figure 1.17 (உன் laptop-ல apply)

உன் IdeaPad Gaming 3-ல AMD Ryzen 5 5600H (or Intel i5) இருக்கும். Book-ரோட Figure 1.17 exact-ஆ உன் CPU-க்கு apply ஆகுது:

```
உன் CPU chip (one package):
┌─────────────────────────────────────────────┐
│  Core 0    Core 1    Core 2    Core 3   ...  │
│  ┌──────┐  ┌──────┐  ┌──────┐  ┌──────┐    │
│  │Regs  │  │Regs  │  │Regs  │  │Regs  │    │
│  │L1 i$ │  │L1 i$ │  │L1 i$ │  │L1 i$ │    │
│  │L1 d$ │  │L1 d$ │  │L1 d$ │  │L1 d$ │    │
│  │L2    │  │L2    │  │L2    │  │L2    │    │
│  └──────┘  └──────┘  └──────┘  └──────┘    │
│                                              │
│         L3 cache (shared by all cores)       │
│                                              │
└─────────────────────────────────────────────┘
              ↓
         Main memory (8GB DRAM)
```

Book சொல்றது exact-ஆ: L1 cache = two parts — **i-cache** (instructions) + **d-cache** (data). L2 = unified. L3 = all cores share.

---

## Hyperthreading — உன் laptop-ல exactly நடக்குது

```
Conventional processor:
  Thread switch = ~20,000 clock cycles overhead
  (context save, restore — expensive)

Hyperthreaded processor (உன் CPU):
  Multiple copies of: PC, Register file
  Single copy of:     FPU (floating point unit), ALU

  Thread switch = cycle-by-cycle decision
  No overhead!

உன் Ryzen 5 5600H:
  6 cores × 2 threads = 12 threads simultaneously
  
  Task Manager-ல 12 logical processors தெரியும்
  — அது இதனால!
```

**Real example உன் laptop-ல:**

```
Core 0, Thread 0: Chrome tab rendering
Core 0, Thread 1: Chrome network I/O waiting
                  (Thread 0 cache miss-ல wait → 
                   Thread 1 immediately runs — no waste!)

Core 1, Thread 0: VS Code syntax highlighting
Core 1, Thread 1: VS Code file watcher
...
```

---

## Multiprocessing-ரோட 2 benefits (book சொல்றது)

**1. Simulated concurrency தேவை குறையும்**

உன் laptop-ல Chrome + VS Code + Spotify + Terminal — எல்லாம் simultaneously actually run ஆகும். Single core-ல simulate பண்ண வேண்டியதில்லை.

**2. Single application faster run ஆகும்**

ஆனா condition: program **multiple threads** use பண்ணணும்.

```
Single-threaded program:
  6 cores இருந்தாலும் → 1 core மட்டும் use
  மீதி 5 cores idle!

Multi-threaded program (e.g. video encoding):
  6 cores × 2 threads = 12 threads parallel
  → ~6x faster (Amdahl's law limit-வரை)
```

---

## Node.js connection — உன் stack-க்கு relevant

```javascript
// Node.js — single-threaded (main thread)
// But libuv uses thread pool internally!

const os = require('os');
console.log(os.cpus().length); // உன் laptop: 12

// Worker threads — multi-core use பண்ண
const { Worker } = require('worker_threads');

// CPU-intensive task — 12 workers = 12 cores use
for (let i = 0; i < 12; i++) {
    new Worker('./heavy-computation.js');
}
// உன் laptop-ரோட 12 logical processors எல்லாம் busy!
```

Node.js default-ஆ single thread — I/O-bound tasks-க்கு போதும் (event loop handle பண்றது). CPU-bound tasks-க்கு Worker Threads வேணும் — அப்போதான் உன் 6 cores fully utilize ஆகும்.

---

Chapter 12-ல concurrency deep-ஆ வரும். அடுத்து 1.9.3 (Instruction-Level Parallelism) போகலாமா?