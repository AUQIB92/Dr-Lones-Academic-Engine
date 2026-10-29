# V02 · L02 — The Machine Underneath

> **Lecture:** CS502 L02 `02-os-operations-protection` · **Unit:** I · **Target length:** 12–14 min · **Series position:** 02/28
> **Format:** screen-capture animation (hardware diagram, interrupt flow, mode-bit indicator) + one terminal demo · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

V01 kept saying "the kernel takes the CPU back" and "the kernel notices the
click". This video shows *how*, and every part of the answer is motion in time:

- a signal travels up a wire;
- the CPU abandons its current instruction stream and jumps through a table;
- a handler runs, and the interrupted program resumes as if nothing happened;
- a single bit flips from 1 to 0 and back.

The deck's **The Mode Transition Graph** and **A Quantum on the Timeline**
frames are static snapshots. Students who see only snapshots believe that "the
OS runs in the background", or that "the mode bit is a variable the kernel
sets". Animating one runaway `while(1);` loop being stopped by the timer
dissolves both.

## 2. Learning outcomes

After watching, the student can:
- [Remember] Name the parts of a computer system the OS relies on: CPU, main memory, device controllers with local buffers, device drivers, and the shared bus.
- [Understand] Trace the interrupt cycle in order: device signals → CPU finishes the current instruction → saves state → looks up the interrupt vector → runs the ISR → restores state → resumes.
- [Understand] Order the storage hierarchy (registers → cache → main memory → SSD/disk → tape/optical). Explain the speed/size/cost/volatility trade-off.
- [Apply] Compute the timer-interrupt rate and the switching overhead for a given quantum and context-switch cost.
- [Analyze] Explain why each of the mode bit, the privileged instructions and the timer is necessary, by showing what a buggy program could do without it.

## 3. Prerequisites

- Fundamentals: CPU, memory, disk, program, file; binary numbers; the CPU's fetch–execute of one instruction after another.
- **V01**:
  - what an OS and the kernel are;
  - the one-line definitions of user/kernel mode and system call;
  - time-sharing needs a way to "take the CPU back".

## 4. Hook (first 30–45 s)

> *On screen: a shared lab machine. A student types a one-line C program,*
> `int main(void){ while(1); }`*, compiles it and runs it. A CPU meter jumps
> to 100 % on one core. The cursor still moves. In a second terminal the
> instructor types* `kill` *with the process number, and the meter drops.*
>
> "That program never asks for anything. It never reads a file, never prints,
> never ends. It just spins. And the machine stays alive: the mouse moves, and
> another person can kill the program. How? The operating system is *also*
> just a program, and it needs the same CPU that this loop is hogging. Today we
> open the hood and find three pieces of hardware the OS can't live without:
> **interrupts**, **a mode bit**, and **a timer**."

(This is the deck's **Welcome: The Program That Never Yields**. Its running
thread is "follow one runaway process to the timer interrupt that takes the
CPU away".)

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:45 | Hook | Terminal demo above; CPU meter pegged; the `kill` command; the meter drops. Freeze, then a caption: "Who took the CPU back?" | One spinning program, a live machine. The OS needs the same CPU, so something must hand it back. |
| 2 | 0:45–2:00 | Computer-system organization | Build-up diagram: CPU and memory on a horizontal **bus** (defined: the shared wires that carry addresses, data and control signals). Controllers hang off the bus: disk controller, USB/keyboard controller, graphics controller. Each controller grows a small **local buffer** box. Beside each controller, inside a gold "kernel" band, a **device driver** box. | Define **device controller**: the small piece of hardware in charge of one device type, with its own buffer and registers. Define **device driver**: the kernel software that knows how to talk to that controller. The CPU and the controllers run *at the same time*, so the CPU needs a way to hear when a controller is done. |
| 3 | 2:00–2:50 | Polling vs. interrupts (motivation) | Split screen. Left: the CPU repeatedly asks "done yet?" (a loop counter climbing, with the busy CPU coloured grey = wasted). Right: the CPU runs a program; the controller "taps its shoulder" when done. | **Polling**: the CPU keeps checking a status register, wasting cycles. **Interrupt** (defined): a hardware signal from a device that makes the CPU stop and run kernel code. Interrupts let the CPU do useful work while devices are busy. |
| 4 | 2:50–4:30 | The interrupt cycle, animated (centrepiece) | One keystroke. (1) The key is pressed; the keyboard controller puts the code in its buffer and raises a line on the bus (a pulse travels). (2) The CPU *finishes the current instruction* (instruction box completes). (3) The hardware saves the PC and flags into a "saved state" box. (4) The interrupt number indexes the **interrupt vector** (an animated array of handler addresses); a row lights up. (5) The **ISR** (defined: interrupt service routine) runs in gold and copies the character into kernel memory. (6) The saved state is restored; the user program's arrow continues from the exact next instruction. | Define **interrupt vector** in one line: a table of handler addresses indexed by interrupt number, so there is no searching. Stress "resume" twice: the interrupted program does not know it was interrupted. Pause-and-predict 1 (§7) before step 6. Name **trap/exception** as the software-caused cousin (divide by zero, a system call). |
| 5 | 4:30–5:30 | Storage hierarchy | A pyramid builds bottom-up: tape/optical → HDD → SSD → main memory (DRAM) → cache → registers. Three arrows along the side: "faster ↑", "bigger ↓", "cheaper per byte ↓". A dashed line marks **volatile** (above main memory, inclusive) vs **non-volatile**. A data block animates upward as it is used (disk → RAM → cache → register). | The CPU can only work on what is in registers and memory, and the disk is far away. **Caching** (defined): keep a copy of recently used data in a faster level. The OS manages the RAM↔disk boundary (V19–V25 and V27); the hardware manages cache. No specific latencies on screen; only the ordering. |
| 6 | 5:30–6:30 | What goes wrong without a boundary | Deck frame **What Goes Wrong Without a Boundary**, as four short "disaster cards" that each explode a model machine. A bad pointer overwrites the kernel. A program disables interrupts. A program reprograms the timer to never fire. A program sends raw disk commands that bypass permissions. | One buggy or malicious program must not be able to take the machine down. Each card is mapped to the mechanism that will stop it: mode bit plus privileged instructions, memory protection, and the timer. |
| 7 | 6:30–7:40 | Dual mode and the mode bit | Deck frame **One Bit, Two Worlds**. A big hardware toggle labelled **mode bit**: **1 = user** (blue), **0 = kernel** (gold). The toggle is drawn *inside the CPU*, not in RAM. A user program tries `store 0 → mode bit`; a red "trap" bolt fires (deck **The Mode Bit Is Hardware, Not a Variable**). | Define **dual-mode operation**. The bit is CPU state that ordinary stores cannot reach. Only events flip it toward the kernel: interrupts, exceptions and traps. Returning to the user is a special privileged instruction. |
| 8 | 7:40–8:40 | Privileged instructions | Deck frame **Who May Execute What?** as two columns that sort themselves. Instruction chips fly in and drop into "privileged" (set the mode bit, disable interrupts, load the timer, raw I/O, change the page-table base, halt) or "unprivileged" (add, load/store own memory, branch, read the clock, trap). Then a privileged chip is executed in user mode and the screen flashes "trap → kernel → process terminated". | Define **privileged instruction**. Breaking the rule does not silently fail; it traps to the kernel, which normally kills the process. Pause-and-predict 2 (§7). |
| 9 | 8:40–9:30 | The mode transition graph | Deck TikZ **The Mode Transition Graph**: two nodes, user (1) and kernel (0). Arrows animate in turn: *trap/system call* (user→kernel), *interrupt/exception* (user→kernel), *return-from-trap* (kernel→user). A short replay of a `write` call (deck **Worked Trace: One `write` Crosses the Boundary**) moves a token around the loop. | Entry into the kernel is always a controlled hardware event. The kernel *returns* to user code; it never "calls" it. The `write` path is V03's subject, so only preview it here. |
| 10 | 9:30–10:40 | The timer | Deck frames **The Loop That Never Leaves User Mode** and **The Timer Is the External Event**. A countdown register loaded with **10000** ticks at 1 MHz (= 10 ms) counts down while `while(1);` spins. At 0 the timer raises an interrupt and the mode bit flips to 0. Then **A Quantum on the Timeline**: a 0–50 ms axis with 10 ms slices alternating between P1 (the runaway) and P2. | Define **timer**: a hardware countdown that interrupts the CPU when it reaches zero. Define **quantum** (time slice). Loading the timer is privileged, so the runaway cannot stop it. Deck **Timer + Scheduler = Preemption**. Define **preemption**: taking the CPU away from a running program. Over one second with two programs, the runaway gets 50 quanta, not the whole second (§6). |
| 11 | 10:40–11:40 | Worked example: what does a quantum cost? | Deck table **Worked Example: What Does a Quantum Cost?** with switch cost 12 µs. The rows animate in for Q = 100 µs, 1 ms and 10 ms: interrupts/s 10 000, 1 000 and 100; overhead 12 %, 1.2 % and 0.12 %. A slider then drags Q down to 1 µs and the overhead bar shoots to 1200 % (deck **Socratic Check 2**). | The overhead is about switch cost ÷ Q. A smaller Q means snappier response but more waste. This is the first appearance of the scheduling trade-off (V08–V09). Footnote the exact share, switch ÷ (Q + switch), which gives 0.12 % at 10 ms (§6). |
| 12 | 11:40–12:30 | Three things the kernel protects | Deck frames **Three Things the Kernel Protects**, **Memory Protection in One Slide** and **I/O Protection**. CPU (timer), memory (base/limit fence around a process: an access past the fence fires a trap), I/O (all I/O instructions privileged, so a process must ask). Then **Protection vs. Security** as a one-line card. | **Protection** (defined): controlling which programs can touch which resources. **Security**: defending against outside attacks (passwords, malware). Base/limit is detailed in V19. |
| 13 | 12:30–13:20 | The full control loop (hook resolved) | Deck **Worked Trace: Stopping the Runaway Process** replayed as a single continuous animation. P1 spins (mode 1) → the timer hits 0 → interrupt → mode 0 → the kernel saves P1's state → the scheduler picks the shell → mode 1. The instructor's `kill` becomes a system call → mode 0 → P1 is removed. | Every piece of the hook, now explained: the interrupt, the mode bit, the timer and the system call. This is the deck's **The Full Control Loop**. |
| 14 | 13:20–13:50 | Recap + bridge | Recap card: "hardware gives the OS three powers: interrupts (hear devices), the mode bit (enforce rules), the timer (take the CPU back)". Bridge: the `kill` command's arrow freezes at the trap instruction; zoom on the word **system call**. | "The `kill` command didn't *force* its way in. It *asked*, through one special doorway. **Next, V03: System Calls.** We follow one request from your code, through the trap, to the kernel and back." |

## 6. Worked example (verified)

**Running example:** a runaway `while(1);` (P1) shares one CPU with the
shell (P2) under a timer quantum.

**A. Timer arithmetic (Scene 10).** The timer counts at 1 MHz (one tick per
µs). Loading it with **10000** gives an interrupt after 10 ms. One quantum is
therefore Q = 10 ms, which means 100 timer interrupts per second. With two
ready processes alternating, the runaway P1 receives **50** quanta in one
second, not the whole second.

**B. Quantum cost (Scene 11; from the deck's worked-example frame).**
Context-switch cost s = 12 µs.

| Quantum Q | Timer interrupts/s | Overhead s/Q (deck formula) | Exact share s/(Q+s) |
|---|---|---|---|
| 100 µs | 10 000 | 12 % | 10.71 % |
| 1 ms | 1 000 | 1.2 % | 1.19 % |
| 10 ms | 100 | 0.12 % | 0.12 % |
| 1 µs | 1 000 000 | 1200 % | (switching dominates) |

The deck's formula s/Q compares switch time to useful time, so it can
exceed 100 %. The exact column gives the fraction of all CPU time that is
spent switching. Narrate the deck's version and footnote the other one.

**Verification:** all numbers (10000 ticks, 100/s, 50 quanta, every table
cell) were computed with python3 in
`/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/verify.py`.
The output is saved in `.../videos/verify_output.txt`. The deck rows
(12 %, 1.2 %, 0.12 %, 1200 %) match the script exactly.

## 7. Pause-and-predict checkpoints

1. **Scene 4, before resume.** "The ISR has stored the character. Where does
   the CPU go next: back to the start of the user program, to the instruction
   that was interrupted, or to the one after it?" *Answer:* To the
   next instruction the program had not yet run (the saved PC). The
   instruction in progress had already completed.
2. **Scene 8.** "Which of these may a user program execute: (a) read the
   clock, (b) set the timer, (c) disable interrupts, (d) trap into the kernel?"
   *Answer:* (a) and (d). (b) and (c) are privileged.
3. **Scene 11.** "Switch cost 12 µs. Roughly what overhead does a 2 ms quantum
   give, by the deck formula?" *Answer:* 12/2000 = 0.6 %.
   [Recompute in the §6 script before recording.]

## 8. Misconceptions to address on screen

- *"The OS runs in the background all the time."* → Scene 13: the CPU's
  instruction arrow is in P1 *or* in the kernel. The kernel runs only after an
  event (deck **The OS Is a Program Too**).
- *"The mode bit is a variable in memory."* → Scene 7: the user store to it
  traps, because the bit lives inside the CPU (deck **Common Misconceptions**).
- *"An interrupt restarts the program."* → Scene 4: the saved PC lets it
  resume at the exact spot.
- *"Executing a privileged instruction in user mode is just ignored."* →
  Scene 8 flash: it traps, and the process is usually terminated.
- *"Faster storage is always better, so use only RAM."* → Scene 5 pyramid:
  RAM is volatile and costly per byte, so the hierarchy exists because no
  single level wins on every axis.
- *"A smaller quantum is always better."* → Scene 11 slider to 1 µs.

## 9. Visual assets

- **Reuse (from `Slides/CS502/02-os-operations-protection.tex`):**
  - **Welcome: The Program That Never Yields**
  - **Roadmap: Four Questions About Control**
  - **The OS Is a Program Too**
  - **What Goes Wrong Without a Boundary**
  - **One Bit, Two Worlds**
  - **Who May Execute What?**
  - **The Mode Transition Graph** (TikZ)
  - **The Mode Bit Is Hardware, Not a Variable**
  - **Worked Trace: One `write` Crosses the Boundary**
  - **The Loop That Never Leaves User Mode**
  - **The Timer Is the External Event**
  - **A Quantum on the Timeline** (TikZ, 10 ms slices)
  - **Timer + Scheduler = Preemption**
  - **Worked Example: What Does a Quantum Cost?**
  - **Socratic Check 2**
  - **Three Things the Kernel Protects**
  - **Memory Protection in One Slide**
  - **I/O Protection**
  - **Protection vs. Security**
  - **The Full Control Loop**
  - **Worked Trace: Stopping the Runaway Process**
  - **Common Misconceptions**
  - **Next: System Calls**
  - Also: the V01 deck's **Where the OS Sits: The User/Kernel Boundary** for context.
- **New (not in the L02 deck; build fresh):**
  - Bus + controllers + buffers + drivers diagram (Motion Canvas).
  - Interrupt-cycle animation with the vector-table lookup (Manim).
  - Storage-hierarchy pyramid with the volatile line.
  - Polling-vs-interrupt split screen.
  - Countdown-timer register.
- **Live demo:**
  - `cat > spin.c <<'E'` / `int main(void){ while(1); }` / `E`, then `cc spin.c -o spin && ./spin &`.
  - Run `top` (or Activity Monitor) to see 100 % on one core, then `kill %1`.
  - Optional Linux extra: `cat /proc/interrupts` for a few seconds, to show interrupt counts climbing.

## 10. On-screen text & notation

Define each term at first use (lower-third):
- bus;
- device controller;
- local buffer;
- device driver;
- polling;
- interrupt;
- trap / exception;
- interrupt vector;
- ISR;
- storage hierarchy;
- volatile;
- caching;
- dual-mode operation;
- mode bit;
- privileged instruction;
- timer;
- quantum;
- preemption;
- protection;
- security.

Notation (per `.claude/rules/knowledge-base-CS502.md`):
- mode bit **user = 1, kernel = 0**;
- processes are `P1`, `P2`;
- units are µs/ms, written with a space (`10 ms`);
- user = blue, kernel = gold, matching V01.

## 11. End-of-video quiz

1. **(MCQ)** Put in order: (i) ISR runs, (ii) state saved, (iii) device raises
   an interrupt, (iv) the interrupted program resumes, (v) the current
   instruction completes. *Answer:* iii → v → ii → i → iv.
2. **(Numerical)** The timer runs at 1 MHz and the quantum must be 4 ms. What
   value is loaded into the counter, and how many timer interrupts occur per
   second? *Answer:* 4000; 250 per second.
   [Arithmetic: 4e-3 × 1e6 = 4000; 1/4e-3 = 250. Check with the §6 script.]
3. **(MCQ)** Which of these must be privileged?
   (a) adding two registers (b) loading the timer (c) reading the time of day
   (d) calling a library function. *Answer: (b).*
4. **(MCQ)** Without a timer, which failure becomes possible?
   (a) a program reads another's memory (b) a program keeps the CPU forever
   (c) a program writes the disk directly (d) a cache miss.
   *Answer: (b).*
5. **(Short)** Why must the instruction that sets the mode bit itself be
   privileged? *Answer:* Otherwise any user program could set the bit to 0 and
   become the kernel, and the whole protection scheme would collapse.

## 12. References

- Silberschatz, Galvin & Gagne, *Operating System Concepts*, 8e:
  - Ch. 1 (computer-system organization, interrupts, storage structure);
  - §1.5 OS operations, dual-mode operation / mode bit and timer, pp. 20–23 (also pp. 21–22 in the index);
  - DMA p. 44;
  - Ch. 13 I/O hardware: polling and interrupts p. 556.
- Stallings, *Operating Systems: Internals and Design Principles*, 6e:
  - Ch. 1 §1.4 Interrupts p. 15;
  - §1.5 The memory hierarchy p. 26;
  - §1.6 Cache memory p. 29;
  - §1.7 I/O communication techniques p. 33;
  - Ch. 3 §3.5 Execution of the OS p. 140;
  - §3.6 Security issues p. 143.
- Bovet & Cesati, *Understanding the Linux Kernel*, 3e: Ch. 4 Interrupts and exceptions, pp. 131–188.

(Pages from `master_supporting_docs/CS502/supporting_books/*/index.md` and the
deck's **Further Reading (Page-Cited)** frame.)

## 13. Production notes

- **Pacing.** Scene 4 (the interrupt cycle) is the heart of the video. Run it
  once at normal speed, then once in slow motion with labels. Do not cut the
  slow pass.
- **If over length**, cut in this order:
  - the polling split screen (Scene 3) down to 20 s;
  - the Protection-vs-Security card;
  - the exact-share column (keep it in the description only).
- **Demo safety.** On a multi-core machine `while(1);` pegs only one core, so
  say so explicitly. The point is that the *kernel* still runs, not that the
  machine has one core.
- **Consistency.** Keep the gold "kernel glow" from V01. The mode-bit widget
  introduced here (top-right corner) reappears in V03, V06 and V07.
- **Accessibility.** Captions on; mode is shown by colour *and* by the
  digit (1/0) *and* by the label (USER/KERNEL), so it never depends on colour
  alone. Avoid fast flashing in the trap bolts (keep under 3 flashes/s).
