இந்த section-ல இரண்டு parts இருக்கு — Shell என்ன பண்றது, அதற்கு முன்னாடி hardware எப்படி organize ஆகி இருக்கு. Hardware பாத்தா மட்டுமே `./hello` run ஆகும்போது என்ன நடக்குது புரியும். இரண்டையும் deep-ஆ போகிறேன்.

---

# Part 1 — Hardware Organization

## Bus — எல்லாத்தையும் connect பண்ற highwayஒவ்வொரு component-யும் என்ன பண்றது deep-ஆ பாக்கலாம்.

---

## Buses — Data highway

**Bus** = shared electrical wires, multiple components-ஐ connect பண்றது. Data ஒரு fixed chunk size-ல travel பண்றது — இதை **word** னு சொல்வாங்க. Modern 64-bit systems-ல word size = 8 bytes.

மூன்று different buses இருக்கு:

**System bus** — CPU ↔ I/O Bridge. CPU-ரோட speed-க்கு match ஆகுற fast bus.

**Memory bus** — I/O Bridge ↔ Main Memory. Memory access-க்கு dedicated.

**I/O bus** — I/O Bridge ↔ எல்லா devices (disk, USB, graphics). Slower, ஆனா universal — எல்லா devices-உம் ஒரே bus share பண்றது.

I/O Bridge நடுவுல இருந்து இந்த மூன்று buses-ஐயும் translate பண்றது — different speeds-ஐ bridge பண்றது அதோட job.

---

## CPU internals — PC, Registers, ALU

**Program Counter (PC)** — ஒரே ஒரு register, ஆனா மிக முக்கியமான ஒண்ணு. அதுல next execute ஆகவேண்டிய instruction-ரோட **memory address** இருக்கும். CPU ஒரு instruction execute பண்ணும்போது PC automatically next instruction address-க்கு update ஆகும். `jmp` / `call` instructions PC-ஐ manually change பண்றது — அதுதான் branching, function calls எல்லாம்.

**Register file** — CPU chip-ல உள்ளே இருக்கற மிகவும் fast, மிகவும் small storage. x86-64-ல 16 general-purpose registers: `%rax, %rbx, %rcx, %rdx, %rsi, %rdi, %rsp, %rbp, %r8..%r15`. ஒவ்வொண்ணும் 8 bytes. Total = 128 bytes மட்டும்! ஆனா RAM-ஐ விட ~100x fast.

**ALU (Arithmetic Logic Unit)** — actual computation நடக்குற இடம். Addition, subtraction, AND, OR, XOR, comparison — எல்லாம் இங்கே. ALU registers-லிருந்து input எடுக்கும், result registers-ல போடும். RAM-ஐ directly தொடாது — register வழியாதான் எல்லாம்.

---

## Main Memory — DRAM

**DRAM (Dynamic RAM)** — உன் laptop-ல 8GB/16GB RAM. "Dynamic" ஏன்னா ஒவ்வொரு bit-உம் ஒரு capacitor-ல store ஆகுது, அது slowly discharge ஆகும். அதனால milliseconds-ல ஒரு முறை **refresh** பண்ணணும் — அதுவே DRAM-ஐ SRAM-ஐ விட slow பண்றது.

CPU directly RAM address பண்ண முடியும் — `mov (%rdi), %rax` = `rdi` register-ல இருக்கற address-ல memory-லிருந்து 8 bytes எடுத்து `rax`-ல போடு.

---

## I/O Devices

**Disk controller** — HDD/SSD-ஓட intermediary. CPU directly disk-ஐ access பண்ணாது — controller-க்கு command அனுப்பும், controller disk-ஐ handle பண்ணும். DMA (நேத்து பாத்தோம்) இந்த controller use பண்றது.

**Graphics adapter** — `"hello, world\n"` string bytes-ஐ screen-ல pixels-ஆ convert பண்றது. CPU character codes அனுப்பும், adapter அதை render பண்ணும்.

**USB controller** — keyboard, mouse போன்ற devices. Keyboard-ல key press ஆகும்போது USB controller CPU-க்கு **interrupt** அனுப்பும் — CPU என்ன பண்ணிட்டு இருந்தாலும் நிறுத்தி keyboard input process பண்ணும்.

---

# Part 2 — Shell என்னது, என்ன பண்றது?

**Shell** = ஒரு normal user-space program. OS-ரோட part இல்ல. Terminal-ல type பண்றதை படிச்சு, interpret பண்ணி, execute பண்றது மட்டும்தான் வேலை.

```
linux> ./hello
```

இந்த ஒரு line-க்கு shell என்ன பண்றது:

**Step 1 — Prompt print பண்றது**, `linux> ` — just `printf` call.

**Step 2 — Input wait** — `read()` system call. Keyboard character வரும் வரை block ஆகி இருக்கும்.

**Step 3 — Enter press ஆகும்போது** — `./hello` string ready. Shell first word பாக்கும்: `./hello` built-in command-ஆ? (`cd`, `exit`, `echo` மாதிரி built-ins?) இல்லன்னா — executable file னு assume பண்ணும்.

**Step 4 — `fork()` system call** — shell ஒரு child process create பண்றது. Child = shell-ரோட exact copy, same memory, same everything. இப்போ இரண்டு processes இருக்கு: parent shell, child shell-copy.

**Step 5 — child-ல `execve()` system call** — child process-ரோட memory-ஐ `hello` program-ஓட code+data-ஓட replace பண்றது. இனி அந்த process = `hello` program. இங்கேதான் disk-லிருந்து `hello` executable load ஆகுது (DMA வழியா RAM-க்கு).

**Step 6 — parent shell `wait()` call** — child (`hello`) finish ஆகும் வரை parent sleep.

**Step 7 — `hello` program runs** — `main()` execute ஆகும், `printf("hello, world\n")` call ஆகும், string display-ல தெரியும், `return 0` ஆகும்.

**Step 8 — child terminates** — OS child process cleanup பண்றது, parent-க்கு signal அனுப்பும்.

**Step 9 — parent shell `wait()` return** — child முடிஞ்சது தெரியும். மறுபடியும் prompt print பண்ணி next input wait பண்றது.

```
linux>          ← Step 1: prompt
./hello         ← Step 2-3: input read
hello, world    ← Step 7: hello program output
linux>          ← Step 9: back to waiting
```

---

## Fork + Exec — ஏன் இப்படி design பண்ணாங்க?

`fork()` மட்டும் use பண்ணன்னா child parent-ரோட same code run பண்ணும் — useless. `exec()` மட்டும் use பண்ணன்னா current process replace ஆகும் — shell itself gone!

`fork()` + `exec()` combination-ல: shell alive-ஆ இருக்கும் (parent), புது program run ஆகும் (child). இது Unix-ரோட elegant design — ஒவ்வொரு program-உம் isolated process-ல run ஆகுது, shell affect ஆகாது.

இந்த `fork-exec` pattern Node.js-ல `child_process.spawn()` use பண்ணும்போதும் நடக்குது — internally same Linux system calls!

அடுத்து 1.4.2 (Running the hello program — hardware-level detail) போகலாம், அதுல DMA + CPU execution cycle deep-ஆ வருது.