# V05 · L05 — From Program to Process

> **Lecture:** CS502 L05 `05-process-concept-pcb` · **Unit:** I · **Target length:** 11–13 min · **Series position:** 05/28
> **Format:** screen-capture animation (address space filling as code runs) + code + terminal · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

"A process is a program in execution" is one of the most memorised and least
understood sentences in the course. The difference between program and process
is a difference *over time*. The file on disk never changes. The process has a
program counter that moves, a stack that grows and shrinks with each call, and a
heap that grows on `malloc`. A video can run `./counter` and *watch* its four
memory regions come alive, then launch it a second time and show two live
images built from one file. The deck's **Inside the PCB** box is a static list.
Animating an interrupt that forces the kernel to copy `P1`'s PC and registers
into its PCB is what makes students see *why* each field exists.

## 2. Learning outcomes

After watching, the student can:
- [Understand] Distinguish a program (a passive file) from a process (an active execution), and explain why one program can be several processes.
- [Apply] Given a short C program, state which memory region (text, data, heap, stack) holds each variable and each piece of code.
- [Remember] List the seven categories of PCB information (state, PC, CPU registers, scheduling info, memory-management info, accounting info, I/O status).
- [Analyze] Justify each PCB field by naming what would break if the kernel did not record it.
- [Understand] Describe a thread as a second program counter inside the same process, sharing the address space.

## 3. Prerequisites

- Fundamentals: CPU (registers, program counter), memory addresses, disk, program, file; reading a few lines of C.
- **V01**: the process badge (create/schedule/end programs); "a program in execution" as a one-liner.
- **V02**: interrupts save and restore the PC; the timer takes the CPU away; the mode bit.
- **V03**: system calls (`read`) trap into the kernel.
- **V04**: the kernel vs system programs; the shell runs commands as programs.

## 4. Hook (first 30–45 s)

> *On screen: two terminals side by side. In each, the instructor types*
> `./counter` *. Both start printing 1, 2, 3 … at different speeds. One is
> paused waiting for a keypress; the other keeps counting. Then `ls -l counter`
> shows a single file on disk.*
>
> "One file. Two things counting, at different numbers, one paused and one
> running. If I asked 'what is the operating system running right now?', the
> answer cannot be 'the file `counter`' — there's only one of those. So what
> *are* the two things? Today we name them — **processes** — open one up to
> see its four regions of memory, and read the kernel's secret file on each of
> them, the **Process Control Block**."

(The deck's **Welcome: One Program, Two Terminals**; running thread
`./counter` launched twice as `P1` and `P2`.)

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:45 | Hook | Two-terminal demo as above, then `ls -l counter` showing one file. Caption: "1 file, 2 ___?" | One program, two running things. Name them today. |
| 2 | 0:45–1:45 | Passive vs active | Deck **Passive vs. Active** table builds row by row. On the left a disk icon with `counter` (grey, still). On the right two glowing boxes, `P1` and `P2`, each with a moving PC arrow. The file is copied ("loaded") into memory twice. | **Program**: a passive file of instructions. **Process** (defined): a program in execution, with a PC and live resources. A program becomes a process when it is *loaded into memory* (recall V01's double-click story). Batch systems called it a *job*. |
| 3 | 1:45–2:30 | What exactly is a process? | Deck **What Exactly Is a Process?**. Definition block, then three tags snap onto `P1`: "code", "current activity: PC + registers", "memory". Stallings' equivalent phrasings fade in under it. | Formal definition. The current activity is what makes two runs different. |
| 4 | 2:30–3:15 | The `./counter` trace | Deck frame **`./counter`** trace: the code panel (from the deck's worked example) with `count`, `step`, `log`, and the loop. A PC arrow steps through the lines once. | Introduce the running code that the next scenes dissect. |
| 5 | 3:15–5:15 | The four regions, animated (centrepiece) | Deck TikZ **The Four Regions of an Address Space** as a tall bar from address 0 (bottom) to max (top). Live animation while the code runs: (1) *text* fills at the bottom with the code; (2) *data* gets `count = 0`; (3) entering `main` pushes a **stack frame** at the top holding `step = 1` and the pointer `log`; (4) `malloc(64)` grows the *heap* upward by a 64-byte block (shown as 0x40 bytes), and an arrow from `log` points into it; (5) the loop calls `print`, so a second frame pushes onto the stack and then pops; (6) `count` ticks 1, 2, 3 in data. The gap between heap and stack is shaded as "free". | Define **text**, **data**, **heap** (memory allocated while the program runs; grows upward) and **stack** (function parameters, return addresses, locals; grows downward). Define **address space**: the range of addresses a process can use. The pointer and the thing it points to live in different regions. Pause-and-predict 1 (§7) before step 4. |
| 6 | 5:15–6:00 | Worked example: where does each variable live? | Deck **Worked Example: Where Does Each Variable Live?**: labels (a)–(d) fly onto the bar. (a) `count` → data. (b) `step` → stack. (c) the 64 bytes → heap, but `log` → stack. (d) the loop instructions → text. Then two bars, P1 and P2, side by side: the text regions are linked by a "shared" bracket, and the data/stack/heap regions are separate. | Each process has its own data, heap and stack. The read-only text of the same program can be shared. That is why the two counters never interfere. |
| 7 | 6:00–6:40 | Process image (Stallings' view) | Deck **Process Image (Stallings' View)**: the bar gains a fourth attachment off to the side, in gold: "process control block". Caption: "memory regions = what it is doing; PCB = what the kernel knows about it". | Bridge from the process's view to the kernel's view. |
| 8 | 6:40–7:30 | States, a glimpse only | Five small state labels (new, ready, running, waiting, terminated) light under P1 and P2 (P1 = waiting for the keyboard, P2 = running). There are no arrows yet; a "V06" tag sits beside them. | Define **process state** in one line. Only one process per CPU can be *running*. The full diagram and its transitions are V06's subject. Keep this to ~45 s. |
| 9 | 7:30–8:30 | What exactly is a PCB? | Deck **What Exactly Is a PCB?** then **Inside the PCB** TikZ: a card titled "PCB of P1" with fields stacking (state, PID, program counter, registers, memory limits, open files, …). | Define **Process Control Block (PCB)**: the kernel's record of one process, kept in kernel memory. Define **PID** (process identifier). One PCB per process, even for the same program. |
| 10 | 8:30–9:45 | Seven categories: why each exists | Deck **The Seven Categories of PCB Information**. Each row appears with a "what breaks without it?" counterfactual animation. **State**: the scheduler would pick a waiting process. **PC**: a resumed process restarts from the top. **Registers**: a resumed process sees another's values. **Scheduling info**: no priorities. **Memory info** (base/limit, page tables): no protection (V02, V19). **Accounting**: no CPU-time limits. **I/O status**: open files forgotten. | Each field earns its place by preventing a specific failure. Pause-and-predict 2 (§7) on the PC row. |
| 11 | 9:45–10:40 | Why save the PC and registers (animated) | Deck **Why Save the PC and Registers?**. The CPU register panel shows P1's values (PC → inside the loop, R1 = 812). A timer interrupt fires (the V02 bolt), and the kernel (gold) copies the PC and registers into P1's PCB. P2's saved values load from P2's PCB; the register panel now shows P2's values. Later the reverse happens, and P1 continues counting from 812 to 813. | The PCB is what makes interruption safe. Saving one process and loading another is a **context switch**: named here, fully covered in V06. |
| 12 | 10:40–11:20 | Worked example: two PCBs, one program | Deck **Worked Example: Two PCBs, One Program** table: P1 waiting (keyboard), PID 4120, PC inside the `read` wrapper, count 812, terminal A. P2 running, PID 4127, PC inside the loop, count 305, terminal B. Text: shared. The rows that differ glow. Then a 10 s terminal clip of `ps -o pid,stat,comm` showing two `counter` lines. | Every glowing row is per-process information, and that is exactly the PCB's job. The PIDs are illustrative; the OS picks them. Linux calls the PCB `task_struct` (deck **A Real PCB: Linux `task_struct`**, one line). |
| 13 | 11:20–12:10 | Threads: a preview | Deck **Threads: A Preview**. One process box contains two PC arrows that move independently over the shared text and heap. Each arrow has its own small stack and register set. Example: a word processor with a "typing" thread and a "spell-check" thread. | Define **thread**: an independent stream of execution inside a process, with its own PC, registers and stack, sharing the code, data and heap. The PCB gains per-thread entries. Sharing memory is powerful and dangerous, which sets up V10. |
| 14 | 12:10–12:40 | Misconceptions blitz | Deck **Common Misconceptions**: "a process is the program file", "two runs share one PCB", "many processes run at once on one CPU". Each flips to the correct statement. | Quick consolidation. |
| 15 | 12:40–13:10 | Recap + bridge | Recap card: "process = program + activity · 4 regions · PCB = kernel's record". Bridge: P1's state label flickers waiting → ready → running, and arrows start to appear between the five state labels. | "P1 was *waiting*, then it was *running* again. Who moved it, when, and what exactly does the kernel do during that switch? **Next, V06: Process States and the Context Switch.**" |

## 6. Worked example (verified)

**Running example (from the deck):**

```c
int count = 0;          /* (a) data  */
int main(void) {
  int step = 1;         /* (b) stack */
  int *log = malloc(64);/* (c) block -> heap; pointer log -> stack */
  while (1) {
    count += step;      /* (d) instructions -> text */
    print(count);
  }
}
```

- Region assignment (a)–(d) is the deck's own answer
  (**Worked Example: Where Does Each Variable Live?**).
- Heap block size: 64 bytes = 0x40 bytes (the hex label used in Scene 5).
- With `step = 1`, after *N* loop iterations `count = N`. The deck's PCB table
  values (P1 count 812, P2 count 305) therefore correspond to 812 and 305
  completed iterations respectively. That is consistent with the two copies
  running "at their own pace".
- PIDs 4120 and 4127 are the deck's illustrative values, not computed.

**Verification:** `count = N × step` for N = 812 and 305, and 64 → 0x40 were
computed with python3 in
`/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/verify.py`.
The output is in `.../videos/verify_output.txt`. Region assignments were
checked against the deck frame and against Silberschatz §3.1 (text/data/heap/
stack). Before recording, compile the real `counter.c` and confirm the hook
demo behaves as shown (both copies count independently; `ps` lists two PIDs).

## 7. Pause-and-predict checkpoints

1. **Scene 5, before `malloc`.** "`log = malloc(64)` is about to run. Which
   region grows, and where does the variable `log` itself live?"
   *Answer:* The heap grows by 64 bytes; `log` (the pointer) is on the
   stack, in `main`'s frame.
2. **Scene 10.** "Suppose the kernel forgot to save P1's program counter
   when it switched to P2. What happens when P1 runs again?"
   *Answer:* It cannot continue where it stopped. It would resume at some
   wrong address (whatever the PC holds), so it misbehaves or crashes.
3. **Scene 6.** "P1 and P2 run the same program. Which of text / data / heap /
   stack can they safely share?" *Answer:* Only text, because it is read-only
   code.

## 8. Misconceptions to address on screen

- *"A process is the program file."* → Scene 2: one grey file, two glowing
  processes.
- *"Two runs of one program share one PCB."* → Scene 12: two PCB cards with
  different PIDs and counts.
- *"Local variables live in the data section."* → Scene 5: `step` appears
  inside the stack frame and vanishes when the frame pops.
- *"A pointer and what it points to are in the same place."* → Scene 5: the
  arrow from `log` (stack) into the heap block.
- *"Many processes can be running on one CPU."* → Scene 8: exactly one
  "running" label is lit at any time.
- *"A thread is a small process with its own memory."* → Scene 13: two PC
  arrows inside *one* address-space bar.

## 9. Visual assets

- **Reuse (from `Slides/CS502/05-process-concept-pcb.tex`):**
  - **Welcome: One Program, Two Terminals**
  - **Roadmap: Four Questions About a Running Program**
  - **Passive vs. Active**
  - **What Exactly Is a Process?**
  - the **`./counter`** trace frame
  - **Socratic Check 1**
  - **The Four Regions of an Address Space** (TikZ bar)
  - **Worked Example: Where Does Each Variable Live?**
  - **Process Image (Stallings' View)**
  - **The Five States** (glimpse only)
  - **What Exactly Is a PCB?**
  - **Inside the PCB** (TikZ card)
  - **The Seven Categories of PCB Information**
  - **Why Save the PC and Registers?**
  - **Worked Example: Two PCBs, One Program**
  - **Stallings' Three-Part View of the PCB** (optional)
  - **A Real PCB: Linux `task_struct`**
  - **Socratic Check 3**
  - **Threads: A Preview**
  - **Common Misconceptions**
  - **Takeaways**
  - **Next: Process States, Schedulers, and Context Switch**
  - The deck's state-diagram frames (**The Process-State Diagram**, **Reading the Diagram**) are held for V06. Use only the five labels here.
- **New:**
  - Live address-space bar that grows and shrinks in sync with a stepping PC (Manim; drive it from a small event list so the frames match the code).
  - PCB save/restore animation with the register panel.
  - Two-PC thread animation.
- **Live demo:**
  - `cc counter.c -o counter`, then run `./counter` in two terminals.
  - `ls -l counter`.
  - `ps -o pid,stat,comm | grep counter` (Linux/macOS).
  - Optional Linux: `cat /proc/<pid>/maps | head` to show the real text/heap/stack ranges. Mention this only; the address values are machine-specific and must not be quoted as facts.

## 10. On-screen text & notation

Define at first use:
- program;
- process;
- job;
- address space;
- text;
- data;
- heap;
- stack;
- stack frame;
- process image;
- process state;
- PCB;
- PID;
- program counter (PC);
- CPU registers;
- context switch (named only);
- thread;
- `task_struct`.

Notation (per `.claude/rules/knowledge-base-CS502.md`):
- processes are `P1`, `P2` (never lowercase);
- states are lowercase `new · ready · running · waiting · terminated`;
- "PCB" is spelled out at first use;
- code is in monospace;
- the stack is drawn at the top and the heap at the bottom, with address 0 at the bottom (matching the deck TikZ);
- kernel data (the PCB) is drawn in gold.

## 11. End-of-video quiz

1. **(MCQ)** In `void f(void){ static int hits = 0; int tmp = 5; }`, where do
   `hits` and `tmp` live? (a) both stack (b) hits data, tmp stack
   (c) hits heap, tmp stack (d) both data. *Answer: (b).* A `static` local
   is stored like a global.
2. **(MCQ)** Which PCB category holds the base and limit registers?
   (a) accounting (b) CPU-scheduling (c) memory-management (d) I/O status.
   *Answer: (c).*
3. **(Numerical)** Three terminals each run the same program `./sum`. How many
   PCBs and how many copies of the program file on disk exist? *Answer:*
   3 PCBs; 1 file.
4. **(Short)** Why must the kernel save the CPU registers, and not just the PC,
   on a switch? *Answer:* The next process overwrites the registers. Without
   the saved values, the resumed process would compute with another process's
   data (for example, a partially updated `count`).
5. **(MCQ)** Two threads of one process share all of the following *except*:
   (a) code (b) global data (c) heap (d) stack. *Answer: (d).*

## 12. References

- Silberschatz, Galvin & Gagne, *Operating System Concepts*, 8e, Ch. 3:
  - §3.1 the process, p. 101 (memory layout pp. 101–102);
  - process state, p. 103;
  - process control block, pp. 103–104;
  - threads, pp. 104–105;
  - Ch. 4 thread concept, p. 153.
- Stallings, *Operating Systems: Internals and Design Principles*, 6e, Ch. 3:
  - §3.1 What is a process, p. 108;
  - §3.2 Process states, p. 111;
  - §3.3 Process description / control block, p. 126.
- Bovet & Cesati, *Understanding the Linux Kernel*, 3e: Ch. 3 Processes (process descriptor, `task_struct`), pp. 79–130.

(Pages from `master_supporting_docs/CS502/supporting_books/*/index.md` and the
deck's **Further Reading (Page-Cited)** frame.)

## 13. Production notes

- **Pacing.** Scene 5 is the centrepiece. Step the PC slowly, one line per
  beat, and let the regions settle before narrating. Scene 11 reuses V02's
  timer bolt, so the callback should feel familiar.
- **Scope guard.** States and transitions belong to V06. Keep Scene 8 under
  45 s, with no arrows.
- **If over length**, cut in this order:
  - Scene 7 (merge it into Scene 9);
  - the `task_struct` line;
  - the misconceptions blitz (its points already appear as overlays).
  - Never cut Scenes 5, 10 or 11.
- **Demo caution.** The hook needs `counter.c` to wait on a keypress in one
  copy. Prepare a version that reads stdin every N counts, so one terminal
  visibly blocks.
- **Accessibility.** The four regions use colour *and* a texture (text =
  solid, data = dots, heap = diagonal, stack = horizontal lines). Captions
  are on, and every region label is spoken.
