# V03 · L03 — The Journey of One System Call: `write()` From Library to Kernel and Back

> **Lecture:** CS502 L03 `03-system-calls` · **Unit:** I · **Target length:** 9–11 min · **Series position:** 03/28
> **Format:** mixed (screen-capture animation of the trap path + code + terminal) · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

A system call is a *journey in time*: a value is loaded into a register, one
instruction flips the mode bit, the CPU jumps to an entry point the user never
named, a table lookup picks a handler, the kernel checks pointers it does not
trust, and a return value travels back to become `-1` and `errno`. The deck's
**The Trap Path** frame shows the five boxes, but not the *order* or *which
privilege level owns each step*. Students who only read it say "a system call
is a function call into the kernel" and "`printf` is a system call" — both
misconceptions that the deck's **Common Misconceptions** frame lists. An
animation that moves one request through the boxes, with the mode bit
flipping colour, fixes both.

## 2. Learning outcomes

After watching, the student can:
- [Remember] List, in order, the six stations of a system call: user code → library wrapper → trap → system-call table → kernel handler → return path.
- [Understand] Explain why the trap carries only a *number* and why the kernel needs a system-call table to turn that number into a handler.
- [Understand] Distinguish a library call that stays in user space (`strlen`, a buffered `printf`) from one that crosses the boundary (`write`).
- [Apply] Predict the return value and `errno` of `write()` for a valid call, a bad file descriptor, and an invalid user pointer.
- [Analyze] Compare the boundary-crossing cost of copying the same bytes in 1-byte versus large chunks.

## 3. Prerequisites

- Computer fundamentals: a CPU runs instructions held in memory; a register is a small storage slot inside the CPU; a program is a file of instructions; a file lives on a disk.
- V01 *What is an OS*: the OS and its core, the kernel.
- V02 *The machine underneath (interrupts, dual mode)*: the mode bit, user vs. kernel mode, privileged instructions.
- Helpful but not required: reading a few lines of C (a function call, a return value). Every C detail used is explained on screen.

## 4. Hook (first 30–45 s)

> *On screen: a terminal. The instructor types* `cp notes.txt backup.txt` *and presses Enter. The prompt returns instantly.*
>
> "That took a few milliseconds. In that time, `cp` — an ordinary program
> that is *not allowed* to touch the disk — opened a file, created another,
> copied every byte, and closed both. It cannot do any of that itself. Every
> time it needed the disk it had to knock on the kernel's door. Today we
> follow **one knock** — a single `write()` — all the way in and all the way
> back out. By the end you'll know exactly what happens in the instant
> between calling `write` and getting a number back, and why calling it a
> million times is a terrible idea."

(This reuses the deck's opening frame **Welcome: How Many Times Does `cp`
Enter the Kernel?** and its running thread "follow `cp` through its system
calls".)

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:40 | Hook | Terminal: `cp notes.txt backup.txt`; prompt returns. Freeze-frame; zoom on the word `cp`. A "knock" icon appears over a door labelled *kernel*. | `cp` is user code; it can't touch the disk; every privileged action is a request; today: one request, end to end. |
| 1b | 0:40–1:00 | Recap from V01–V02 (20 s) | Two lanes: blue *user mode* ("your programs"), gold *kernel mode* ("the OS core"); a lock on the disk icon; a mode-bit dial reading 1 / 0. | The **kernel** is the part of the OS that always runs with full privilege. The CPU has a **mode bit**: 1 = user mode (restricted), 0 = kernel mode (anything allowed). Touching a device is a **privileged instruction** — illegal in user mode. So a program must *ask*. |
| 2 | 1:00–1:40 | API vs. system call | Two-layer split screen (from deck frame **Two Views of the Same Service**): top layer "API: `write()` in libc", bottom layer "system call #N in the kernel". An arrow animates from one to the other. | The name you call is the *API*; the thing the kernel serves is the *system call*. Programmers see the API; the kernel sees a number. Define on screen: **system call** = a program's request for a kernel service; **API** = the function name a programmer calls (e.g. `write` in the C library, **libc**). Why not link the kernel into every program? Protection — the recap's lock. |
| 3 | 1:30–2:20 | Station 1: user code | Code panel: `n = write(fd, buf, 13);` with `fd = 1`, `buf → "Hello, world\n"`. A glowing token labelled `write(1, buf, 13)` sits in a *user-mode* lane (blue). Mode-bit indicator top-right shows **1 (user)**. | This line looks like any function call. The arguments are ordinary values: a small integer, a pointer, a length. Nothing privileged yet. |
| 4 | 2:20–3:20 | Station 2: the library wrapper | Token slides into a box "libc wrapper (stub)". Three register slots fill one by one: `arg0 ← fd`, `arg1 ← buf`, `arg2 ← n`; then a fourth slot `syscall-number ← write's number`. | The wrapper is still user code. Its job: put the arguments where the kernel expects them (registers — method 1 of the three parameter-passing methods), and put the *number* of the service in a designated register. On Linux x86-64 `write` is number 1 (deck frame **The System-Call Table**); on the Mac we're recording on, the header says `SYS_write` is 4 — numbers are per-OS ABI, not universal. |
| 5 | 3:20–4:20 | Station 3: the trap | The wrapper executes one instruction, `syscall` (legacy: `int 0x80`). Big visual moment: the lane changes colour from blue to gold, mode bit flips **1 → 0**, the saved user `PC` and flags drop into a "saved context" box. | One instruction does three things in hardware: save where the user was, switch to kernel mode, jump to the kernel's fixed trap entry. The user did not choose the destination — that's the point. |
| 6 | 4:20–5:10 | Station 4: the system-call table | Animated array: indices 0, 1, 2, 3, … each holding a function pointer. The number from the register slides in as an index; the matching row lights up and an arrow jumps to "`write` handler". | The trap carried only a number. The table turns the number into the right routine in one indexed step — no searching. Adding a new call = new table entry + new handler, not a new instruction. The number is ABI: it must stay stable across kernel versions. |
| 7 | 5:10–6:10 | Station 5: the handler — trust nothing | Handler box with a checklist ticking: "Is `fd` an open file of *this* process?" ✓ · "Does `buf … buf+n` lie in *user* memory?" ✓ · "copy 13 bytes from user to kernel" · "hand to the device/file layer". Then a red alternative branch shows a bad `fd` and a bad pointer. | The kernel validates before it copies (deck frame **Pointer Validation: Never Trust User Memory**). A user pointer into kernel memory would be an attack. Only after the checks does the actual write happen. |
| 8 | 6:10–7:00 | Station 6: the return | Return value `13` drops into the return register; saved context box empties back; lane colour gold → blue; mode bit **0 → 1**; the token pops out of the wrapper as `n = 13`. Second pass: a kernel error code pops out, the wrapper converts it to `-1` and writes `errno`. | Return path mirrors entry. Success: a count. Failure: the library returns `-1` and sets `errno` (deck frame **Return Values and `errno`**). The kernel cannot print an error for you — the caller must check. |
| 9 | 7:00–8:00 | Live demo 1: three `write`s | Terminal running `./w` (see §9). Output appears line by line; each line is annotated with which branch of scene 7 it took. | Valid call → 3. Bad `fd` 99 → `-1`, `errno = 9` "Bad file descriptor". Pointer `8` (not mapped) → `-1`, `errno = 14` "Bad address". The kernel's checks are visible from user space. |
| 10 | 8:00–8:50 | Live demo 2: the cost of crossing | Terminal running `./batch`. Then a bar animation: 1 048 576 tiny arrows crossing the boundary vs. 1 fat arrow. | Same 1 MB, two ways. 1-byte chunks: 1 048 576 crossings; one big write: 1 crossing. On the recording machine, the 1-byte loop took about 0.8–0.9 s; the single call, about a microsecond. The bytes are the same; the crossings are not (deck frame **The Cost of Crossing the Boundary**). |
| 11 | 8:50–9:30 | `printf` is not a system call | Animation: five `printf` calls fill a user-space buffer box; only when it flushes does a single `write` token go down the trap path. | `printf` formats into a buffer in user space; it calls `write` only when the buffer is flushed. Many library calls never trap at all (`strlen`, `memcpy`). This buffer comes back in V07 when `fork` copies it. |
| 12 | 9:30–10:15 | Recap + bridge to V04 | The full six-station path redrawn as a loop (the deck's **The Trap Path** TikZ figure), each station lighting in sequence once more at speed; then the kernel box zooms open into a blurred interior with a "?" | Six stations; one instruction changes privilege; a number, a table, a suspicious handler, a register on the way out. **Next, V04 — How the OS is organized:** we've knocked on the kernel's door; next we open it and see how the code behind it is structured (monolithic, layered, microkernel, modules), and which programs ship with the OS. |

## 6. Worked example (verified)

**Running example:** `n = write(fd, buf, 13);` with `fd = 1` (standard output)
and `buf` pointing to `"Hello, world\n"` (13 bytes). This is the deck's
**Worked Trace: `write(fd, buf, n)`** frame made concrete.

**Step-by-step trace (matches the deck's seven-step enumerate):**

1. `fd = 1`, `buf`, `n = 13` placed in argument registers.
2. `write`'s system-call number placed in the designated register.
3. Wrapper executes `syscall` (x86-64) or `int 0x80` (legacy x86).
4. Hardware: save user `PC` + flags, mode bit 1 → 0, jump to trap entry.
5. Kernel indexes the system-call table, enters the `write` handler,
   validates `fd` and the user range `[buf, buf+13)`, copies, writes.
6. Return value (here `13`) placed in a register; restore user context,
   mode bit 0 → 1.
7. On error the wrapper returns `-1` and sets `errno`.

**Three outcomes shown in the demo (program `w.c`, §9):**

| Call | Return | `errno` | Message |
|---|---|---|---|
| `write(1, "hi\n", 3)` | `3` | — | (prints `hi`) |
| `write(99, "hi\n", 3)` | `-1` | `9` | Bad file descriptor (`EBADF`) |
| `write(1, (char*)8, 3)` | `-1` | `14` | Bad address (`EFAULT`) |

**System-call numbers shown on screen:**

| OS / ABI | `write` number | Source |
|---|---|---|
| Linux x86-64 | 1 | stated in deck frame **The System-Call Table** |
| macOS (recording machine) | 4 (`SYS_write`) | `<sys/syscall.h>` of the installed SDK; same header gives `SYS_read` 3, `SYS_exit` 1, `SYS_getpid` 20 |

**Batching demo (program `batch.c`, §9):** 1 MB = 1 048 576 bytes written to
`/dev/null`.

| Strategy | System calls | Measured time (two runs, recording Mac) |
|---|---|---|
| 1-byte `write` in a loop | 1 048 576 | 0.939 s and 0.803 s (≈ 770–900 ns per call) |
| one `write` of 1 048 576 bytes | 1 | ≈ 0.000001 s |

**How the numbers were verified.**
- The three `write` outcomes and the errno values (9, 14) are from compiling
  and running `w.c` with `gcc` on the recording machine (macOS), in
  `scratchpad/videos/sc/`.
- `SYS_write = 4` etc. were read with `grep` from the SDK's
  `usr/include/sys/syscall.h`. The Linux value 1 is taken from the deck
  (not re-measured on Linux here); say "on Linux x86-64" when showing it.
- The call count 1 048 576 = 2^20 is exact (the loop counter was printed).
  The timings are *machine-dependent measurements*, not constants: re-run
  `./batch` on the recording machine and read off whatever it prints;
  only quote "roughly a second vs. a microsecond".

## 7. Pause-and-predict checkpoints

1. **After scene 5 (the trap):** "The trap instruction has switched to kernel
   mode. How does the kernel know the program wanted `write` and not `read`?"
   — *Answer:* the wrapper put `write`'s system-call number in a designated
   register before the trap; the kernel uses it to index the system-call
   table.
2. **Before demo 1 (scene 9):** "I call `write(99, "hi\n", 3)` — descriptor
   99 was never opened. What comes back, and does the program crash?" —
   *Answer:* no crash; `write` returns `-1` and `errno` is `9` (`EBADF`). The
   kernel validated `fd` and refused.
3. **Before demo 2 (scene 10):** "Writing 1 MB one byte at a time vs. in one
   call — how many times more kernel entries?" — *Answer:* 1 048 576 vs. 1,
   i.e. 1 048 576 times more crossings for identical data.

## 8. Misconceptions to address on screen

- **"A system call is just a function call."** → Scene 5's colour flip and
  mode-bit flip: an ordinary call never changes privilege or jumps to an
  address the caller didn't name.
- **"`printf` is a system call."** → Scene 11's buffer box: five `printf`s,
  one `write`.
- **"The kernel can trust the user's pointer."** → Scene 7's red branch and
  demo 1's `EFAULT` line.
- **"Every library call traps into the kernel."** → Scene 11: `strlen` drawn
  staying entirely in the blue user lane.
- **"System-call numbers are universal."** → Scene 4's two-row table: Linux
  x86-64 says 1, macOS says 4.

## 9. Visual assets

- **Reuse (from `Slides/CS502/03-system-calls.tex`):**
  - TikZ figure in frame **The Trap Path** (five `dag-node` boxes: User code →
    Library stub → Trap → Syscall table → Kernel handler, with the return
    edge) — the backbone of scenes 3–8 and 12.
  - Frame **Worked Trace: `write(fd, buf, n)`** — the seven-step list, as an
    on-screen checklist that ticks during scenes 3–8.
  - Frames **The System-Call Table**, **Return Values and `errno`**,
    **Pointer Validation: Never Trust User Memory**, **Method 1: Registers**,
    **The Cost of Crossing the Boundary**, **Common Misconceptions**.
  - Frame **Two Views of the Same Service** for scene 2.
  - Notes `Notes/CS502/03-system-calls-notes.tex` for the extended
    explanation the instructor can paraphrase.
- **New:**
  - Manim or Motion Canvas animation of the six-station path with a persistent
    mode-bit indicator (blue = user `1`, gold = kernel `0`) and the four
    register slots filling (scene 4).
  - Animated system-call table (an indexed array with a highlighted row).
  - Buffer-fill animation for `printf` (scene 11).
  - 1-byte vs. 1-MB "crossings" bar animation (scene 10).
- **Live demo (exact commands; sources in `scratchpad/videos/sc/`):**
  ```c
  /* w.c */
  #include <stdio.h>
  #include <unistd.h>
  #include <errno.h>
  #include <string.h>
  int main(void){
    ssize_t r = write(1, "hi\n", 3);
    fprintf(stderr, "write ok -> %zd\n", r);
    r = write(99, "hi\n", 3);
    fprintf(stderr, "write(99) -> %zd errno=%d %s\n", r, errno, strerror(errno));
    r = write(1, (char*)8, 3);
    fprintf(stderr, "write(bad ptr) -> %zd errno=%d %s\n", r, errno, strerror(errno));
    return 0;
  }
  ```
  ```
  gcc -o w w.c && ./w
  gcc -O2 -o batch batch.c && ./batch
  grep -E "SYS_(write|read|exit|getpid)\s" $(xcrun --show-sdk-path)/usr/include/sys/syscall.h
  ```
  `batch.c` opens `/dev/null`, times a loop of 1 048 576 one-byte `write`s,
  then one `write` of the whole buffer, and prints both counts and times.
  - **Optional Linux insert (lab machine):** `strace -c cp notes.txt backup.txt`
    and `strace -e trace=write ./w`. These were *not* run for this brief
    (recording machine is macOS); if used, quote only what the recording shows.

## 10. On-screen text & notation

- **system call**, **API**, **library wrapper (stub)**, **trap**,
  **system-call number**, **system-call table**, **handler**, **`errno`**,
  **ABI**, **mode bit** (user `1` / kernel `0`, recapped from V02).
- First-use definitions required on screen: kernel, mode bit, privileged instruction, system call, API, libc, trap, register, file descriptor (`fd` = a small integer the kernel gives a program for each open file; `1` = the screen/standard output), `errno`, ABI (the fixed numbering contract between programs and the kernel).
- Function names in monospace: `write`, `read`, `printf`, `strlen`.
- Write the call as `write(fd, buf, n)` everywhere (deck form).
- Parameter passing: "registers / block (table) in memory / stack" — the
  deck's three methods, in that order.
- Never say "interrupt" for the `syscall` instruction; say **trap**
  (knowledge base: trap = software-initiated, interrupt = hardware).

## 11. End-of-video quiz

1. **(MCQ)** Which of these *always* enters the kernel when called?
   (a) `strlen` (b) `printf("x")` (c) `getpid` (d) `memcpy`
   — **Answer: (c).** `printf` usually only fills a buffer; `strlen`/`memcpy`
   are pure user-space code.
2. **(Numerical)** A program copies a 4 MB file using `read` and `write` with
   a 4 KB buffer (binary units). Ignoring the final 0-byte `read`, how many
   system calls does the copy loop make in total?
   — **Answer:** 4 MB / 4 KB = 1024 reads + 1024 writes = **2048**.
3. **(MCQ)** After the trap instruction executes, which of the following is
   *not* done by hardware?
   (a) save the user `PC` (b) switch to kernel mode (c) look up the handler in
   the system-call table (d) jump to the kernel's trap entry point
   — **Answer: (c)** — the table lookup is done by kernel software.
4. **(Short answer)** `write(fd, buf, n)` returns `-1` with `errno = EFAULT`.
   Which kernel check failed?
   — **Answer:** the user range `[buf, buf+n)` was not valid user memory
   (pointer validation).
5. **(MCQ, GATE-style)** Why does the trap carry a small integer instead of
   the address of the kernel routine?
   (a) addresses are too large for a register (b) so the user cannot jump to
   an arbitrary kernel address (c) to make the call faster than a function
   call (d) because the kernel has no addresses
   — **Answer: (b).**

## 12. References

- Silberschatz, Galvin & Gagne, *Operating System Concepts*, 8e — Ch. 2
  §2.3 System calls, pp. 55–58; §2.4 Types of system calls, pp. 58–66.
- Bovet & Cesati, *Understanding the Linux Kernel*, 3e — Ch. 10 System
  Calls, pp. 398–419 (entering/exiting a system call, parameter passing,
  the system-call table).
- Stallings, *Operating Systems: Internals and Design Principles*, 6e —
  Ch. 3 §3.4 Process control, p. 135 (mode switching context).

(All pages as listed in `master_supporting_docs/CS502/supporting_books/*/index.md`
and the deck's **Further Reading (Page-Cited)** frame.)

## 13. Production notes

- **Pacing:** scenes 4–8 are the core; give each station a clear "arrival"
  beat (sound tick + highlight) so viewers can count six stations.
- **Mode-bit indicator** must stay visible in the same corner for the whole
  of scenes 3–8; it is the single most important visual cue.
- **Demo safety:** run `./w` and `./batch` once before recording so the
  binaries are warm; the timings will differ slightly from the brief — say
  "about a second" rather than a precise figure.
- **If over length:** cut scene 2 to 20 s (one sentence + the two-layer
  picture) and drop the optional `strace` insert. Do *not* cut scene 7 (the
  validation step).
- **If under length:** add a 30 s insert on the other two parameter-passing
  methods (block in memory, stack) using frames **Method 2** and **Method 3**.
- **Accessibility:** burn-in captions; user/kernel lanes must differ by
  label and pattern (dashed border for kernel), not only by blue/gold;
  monospace text at least 24 pt on 1080p.
- Do not show internal kernel source on screen; the brief stays at the
  textbook level of detail.
