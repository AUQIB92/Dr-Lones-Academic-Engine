# V06 · L06 — One CPU, Three Programs: Process States, Queues and the Context Switch

> **Lecture:** CS502 L06 `06-process-states-schedulers` · **Unit:** I · **Target length:** 10–12 min · **Series position:** 06/28
> **Format:** screen-capture animation (state diagram + queue lanes + timeline), short whiteboard recap · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

On a one-core machine a process's *state* changes dozens of times per second,
and every change is a PCB moving from one queue to another. The deck's
**The State Diagram** and **The Queueing Diagram** frames are static maps;
students memorize the six arrows but cannot say *which event* fires each one,
*who* fires it, or *where the PCB is* at an instant. They also confuse a
mode switch with a context switch. A video can move PCB cards between queues
in real time while a state diagram lights up the matching arrow, and put the
context switch on a stopwatch — which is exactly what a slide cannot do.

## 2. Learning outcomes

After watching, the student can:
- [Remember] Name the five states (`new · ready · running · waiting · terminated`) and the six legal transitions, with the event that triggers each.
- [Understand] Explain why `waiting → running` and `ready → waiting` never happen.
- [Apply] Given a sequence of events (time-slice expiry, I/O request, I/O completion, admission), state for each instant which process is running and the contents of the ready queue and device queue.
- [Understand] Distinguish the long-, short- and medium-term schedulers by what they move and how often.
- [Apply] Compute the context-switch overhead fraction `s/(q+s)` and decide whether an event causes a mode switch only or a full context switch.

## 3. Prerequisites

- Computer fundamentals: CPU, memory, registers, a program as a file of instructions.
- V02 *The machine underneath (interrupts, dual mode)*: interrupts, the timer, user vs. kernel mode.
- V03 *The journey of one system call*: a system call enters the kernel through a trap.
- V05 *The process and its PCB*: process vs. program; the PCB.
- Each of these is recapped on screen in 10–20 s (scene 2).

## 4. Hook (first 30–45 s)

> *On screen: a lab desktop with three windows — `gcc` scrolling build output,
> a text editor with a blinking cursor, and a "backup scheduled for 02:00"
> notification. A single-core CPU icon sits in the corner.*
>
> "This machine has **one** CPU core. `gcc` is compiling, the editor is
> echoing my keystrokes, a backup is queued for tonight. All three feel like
> they're running. Here's the question: at this exact instant, how many of
> them are *actually* running? … Exactly one. So where are the other two,
> what is the OS doing with them, and how does it hand the CPU from one to
> the next — thousands of times a second — without any of them noticing?"

(Deck frame **Welcome: Who Is Using the CPU Right Now?**: `P1 = gcc`,
`P2 = editor`, `P3 = backup`.)

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:40 | Hook | Three windows + one-core icon; then the windows shrink into three PCB cards labelled `P1 gcc`, `P2 editor`, `P3 backup`. | Only one runs; the OS keeps a record (PCB) for each; today we follow these three cards. |
| 2 | 0:40–1:20 | Recap from V02, V03, V05 (≈ 40 s) | A PCB card flips open: state, `PC`, registers, memory limits, open files (deck frame **Recall from Lecture 5**). The *state* field is circled. Side icons: a ticking timer (interrupt), a door (system call). | A **process** = a program in execution. The **PCB** (process control block) = the kernel's record card for one process; `PC` = program counter, the address of the next instruction. An **interrupt** = a hardware signal that makes the CPU jump into the kernel; the **timer** fires one periodically. A **system call** = a program's request to the kernel (V03). Everything today is about one PCB field — the state — and where the card lives. |
| 3 | 1:20–2:40 | Build the state diagram | Deck's **The State Diagram** TikZ is drawn box by box: `new`, `ready`, `running`, `waiting`, `terminated`; each arrow animates in with its label (admitted, scheduler dispatch, interrupt, I/O or event wait, I/O or event completion, exit). A side table fills row by row: *event* and *who acts* (deck **The Six Legal Transitions**). | Six arrows, each with a trigger and an actor. Long-term scheduler admits; short-term scheduler + dispatcher dispatches; timer interrupt preempts; the process itself asks for I/O; an I/O-completion interrupt wakes it; `exit` ends it. |
| 4 | 2:40–3:20 | The two forbidden arrows | Two dashed red arrows `waiting → running` and `ready → waiting` appear, then are struck through. | A woken process must queue for the CPU; only a running process can issue the request it would wait on. |
| 5 | 3:20–4:30 | One keystroke in the editor | `P2`'s card moves: `running` → keyboard device queue (state `waiting`) → keypress flash → ready queue (`ready`) → CPU (`running`) → back to keyboard queue. The diagram arrow lights in sync each time. Counter "transitions: 1, 2, 3". | Deck's **Worked Trace: `P2`, the Editor, for One Keystroke**. Note step 4: the key is pressed, but `P2` does *not* run yet — it joins the ready queue. Three transitions per keystroke; the editor lives in `waiting`. |
| 6 | 4:30–5:40 | Queues as lanes | Three horizontal lanes: *job queue*, *ready queue*, *keyboard queue*, with a CPU box. Linked-list arrows between PCB cards (deck **The Ready Queue and Device Queues as Linked Lists**). Then the event table from **Worked Trace: Queue Membership Over Time** plays row by row, `t0`–`t5`, cards sliding between lanes. | Every row: exactly one card on the CPU; every card in exactly one place. At `t3` the key interrupt moves `P2` to the ready queue but `P1` keeps the CPU until its slice expires at `t4`. At `t5` `P3` is admitted. |
| 7 | 5:40–6:30 | Four ways to leave the CPU | Four exit doors from the CPU box: I/O request, slice expires, create child and wait, interrupt/event wait. `P2` walks out door 1, `P1` door 2, a shell door 3. | Deck **Four Ways to Leave the CPU**. Same four routes reappear in the queueing diagram. |
| 8 | 6:30–7:40 | Three schedulers, three clocks | Three stopwatches on the diagram: long-term over `new → ready` (minutes), short-term over `ready → running` (milliseconds), medium-term over swap out/in (seconds). The seven-state diagram (deck **The Seven-State Diagram**) fades in with `ready/suspend` and `blocked/suspend`. | Long-term controls the degree of multiprogramming and the I/O-bound/CPU-bound mix; short-term runs constantly, so it must be fast; medium-term swaps to relieve memory. |
| 9 | 7:40–8:50 | The context switch in slow motion | Deck's **The Context Switch on a Timeline** figure animated: `P0` lane executes, interrupt arrow, gold box "save PCB₀ / reload PCB₁", `P1` lane executes, system call arrow, gold box, `P0` resumes. A checklist of the six steps from **The Steps of a Context Switch** ticks during each gold box. | Save registers into the PCB, update state, link onto a queue, scheduler picks, switch memory map, restore registers, return to user mode. Gold boxes = pure overhead. Scheduler = policy, dispatcher = mechanism. |
| 10 | 8:50–9:40 | Mode switch vs. context switch | Split screen: left, `getpid` — the token goes into the kernel and comes straight back to the same process (no gold box). Right, `read` on an empty pipe — the process's card drops to a pipe queue and another card takes the CPU (gold box). | Test: does the caller keep the CPU when the kernel returns? Every context switch includes mode switches; most mode switches cause no context switch (deck **Mode Switch Is Not a Context Switch**, **Socratic Check 3**). |
| 11 | 9:40–10:40 | The cost on a stopwatch | `P1`/`P2` trace on a ms timeline (deck **Worked Trace: `P1` and `P2` on One Core**): key at 3 ms (mode switch only), slice ends at 10 ms (switch #1), `P2` blocks at 10.2 ms (switch #2). Then the overhead table animates as `q` shrinks: bars of useful work vs. overhead. | Two switches × 5 µs = 10 µs in 10.2 ms ≈ 0.1 %. But shrink the slice and overhead climbs: 0.05 % at 10 ms, 0.50 % at 1 ms, 4.76 % at 100 µs, 33.3 % at 10 µs. |
| 12 | 10:40–11:30 | `P3`'s life end to end + bridge | `P3`'s card travels the full seven-state path once at speed (deck **`P3`'s Life, End to End**), each scheduler's stopwatch ticking as it acts. Teaser: a terminal where `ls` is typed and a new card appears from nowhere. | States, queues, schedulers, switches are one machine. **Next, V07 — fork, exec, wait:** where do these cards come from? We'll watch a process split into two with `fork`, become a new program with `exec`, and be collected with `wait`. |

## 6. Worked example (verified)

**Cast:** one core; `P1 = gcc` (long CPU bursts), `P2 = editor` (short bursts,
mostly waiting), `P3 = backup` (submitted, not yet admitted).

**(a) Queue membership over time** (deck frame **Worked Trace: Queue
Membership Over Time**):

| Time | Event | Running | Ready queue | Keyboard queue |
|---|---|---|---|---|
| `t0` | start | `P1` | `P2` | — |
| `t1` | `P1`'s slice expires | `P2` | `P1` | — |
| `t2` | `P2` calls `read` | `P1` | — | `P2` |
| `t3` | key pressed (interrupt) | `P1` | `P2` | — |
| `t4` | `P1`'s slice expires | `P2` | `P1` | — |
| `t5` | `P3` admitted | `P2` | `P1`, `P3` | — |

Invariant checked row by row: exactly one running process; each PCB in
exactly one place (`P3` is in the job queue, state `new`, until `t5`).

**(b) Timed trace and overhead** (deck **Worked Trace: `P1` and `P2` on One
Core**), context-switch cost `s = 5 µs`:

| t (ms) | Event | Kind |
|---|---|---|
| 0 | `P1` running, `P2` waiting on keyboard | — |
| 3 | key pressed; `P2 → ready`; `P1` resumes | interrupt, mode switch only |
| 10 | `P1`'s slice expires; `P1 → ready`, `P2 → running` | context switch #1 |
| 10.2 | `P2` echoes key, calls `read`; `P2 → waiting`, `P1 → running` | context switch #2 |

Kernel entries: 3 (interrupt, timer, system call). Switching time:
2 × 5 µs = 10 µs; 10 µs / 10.2 ms = 0.098 % ≈ 0.1 %.

**(c) Overhead fraction `s/(q+s)` with `s = 5 µs`** (deck **Worked Example:
Overhead of Switching**):

| Slice `q` | Switches/s ≈ `1/(q+s)` | Overhead |
|---|---|---|
| 10 ms | ≈ 99.95 | ≈ 0.05 % |
| 1 ms | ≈ 995 | ≈ 0.50 % |
| 100 µs | ≈ 9524 | ≈ 4.76 % |
| 10 µs | ≈ 66 667 | ≈ 33.3 % |

**(d) Snapshot count** (deck **How Many Processes per State?**): 40 processes,
4 cores, 9 ready ⇒ at most 4 running, the remaining 40 − 4 − 9 = 27 waiting
(or new/terminated).

**How the numbers were verified.** Table (c) and the 0.098 % figure in (b)
were recomputed with `python3` in `scratchpad/videos/` (`1/(q+s)` and
`s/(q+s)·100` for `q ∈ {10⁻², 10⁻³, 10⁻⁴, 10⁻⁵}` s, and `10/10200·100`); all
match the deck to the printed precision. Tables (a) and the event list in (b)
were hand-checked against the six legal transitions (each row's change is
exactly one legal arrow). (d) is integer arithmetic.

## 7. Pause-and-predict checkpoints

1. **After scene 5, step 3 (keypress):** "The key has been pressed. Does `P2`
   run now?" — *Answer:* No. The interrupt moves it `waiting → ready`; it
   runs only when the short-term scheduler picks it (and on our trace, `P1`
   keeps the CPU until its slice expires).
2. **Before scene 10:** "`getpid()` and `read()` on an empty pipe both enter
   the kernel. Which one causes a context switch?" — *Answer:* only `read`;
   the caller blocks, so the kernel must run another process. `getpid` is a
   mode switch only.
3. **During scene 11, before the last row:** "Slice 10 µs, switch 5 µs —
   what fraction of the CPU is overhead?" — *Answer:* 5/15 ≈ 33.3 %.

## 8. Misconceptions to address on screen

- **"A process that finishes I/O goes straight back to running."** → Scene 4:
  the struck-through `waiting → running` arrow; scene 5's card stops in the
  ready lane.
- **"`ready` and `waiting` are the same — both are 'not running'."** →
  Scene 6: different lanes; ready needs only the CPU, waiting needs an event.
- **"Every system call or interrupt is a context switch."** → Scene 10
  split screen; scene 11's 3 ms keypress row labelled *mode switch only*.
- **"The scheduler and the dispatcher are the same thing."** → Scene 9
  label: policy vs. mechanism.
- **"A smaller time slice is always better."** → Scene 11's shrinking-`q`
  bar chart reaching 33.3 % overhead.
- **"Many processes can be running on one core if they're quick."** →
  The single CPU box that holds exactly one card in every frame.

## 9. Visual assets

- **Reuse (from `Slides/CS502/06-process-states-schedulers.tex`):**
  - TikZ in frame **The State Diagram** (coordinate map in the source
    comments; reuse as the master diagram — highlight arrows per event).
  - TikZ in frame **The Seven-State Diagram** (scene 8).
  - Frame **The Six Legal Transitions** (table; scene 3 side panel).
  - Frames **The Ready Queue and Device Queues as Linked Lists** and
    **The Queueing Diagram** (scene 6 lanes).
  - TikZ in frame **The Context Switch on a Timeline** (`P0`/OS/`P1` lanes
    with gold save/reload boxes; scene 9).
  - Frames **The Steps of a Context Switch**, **The Dispatcher**,
    **Mode Switch Is Not a Context Switch**, **Worked Example: Overhead of
    Switching**, **Worked Trace: `P1` and `P2` on One Core**,
    **`P3`'s Life, End to End**, **Common Misconceptions**.
  - Prose in `Notes/CS502/06-process-states-schedulers-notes.tex` for the
    instructor's talking points on schedulers and swapping.
- **New:**
  - Motion Canvas (or PowerPoint morph) scene with PCB cards that move
    between lanes while the corresponding diagram arrow pulses — the single
    reusable animation for scenes 5, 6 and 12.
  - Stopwatch overlays for the three schedulers (scene 8).
  - Overhead bar chart for `q` = 10 ms → 10 µs (scene 11).
  - `/render-lecture-video` can render the deck's step-sequence for
    **Worked Trace: Queue Membership Over Time** if a quick version is needed.
- **Live demo (optional, 30 s):** on the recording Mac, `ps -o pid,stat,comm`
  after starting `sleep 30 &` shows an `S` (sleeping = waiting) state, and
  `yes > /dev/null &` shows `R`. Kill both afterwards (`kill %1 %2`). State
  letters are OS-specific — present them as "the OS's own labels for these
  states", not as textbook names.

## 10. On-screen text & notation

- First-use definitions required on screen: process, PCB, state, queue (a waiting line of PCBs), ready/device/job queue, scheduler (long/short/medium-term), dispatcher, swapping, context switch, mode switch, time slice.
- Processes `P1`, `P2`, `P3` (never lowercase); names `gcc`, editor, backup.
- States in monospace lowercase: `new`, `ready`, `running`, `waiting`,
  `terminated`; seven-state additions `ready/suspend`, `blocked/suspend`.
- **PCB**, **ready queue**, **device queue**, **job queue**.
- **long-term / short-term / medium-term scheduler**, **dispatcher**,
  **dispatch latency**, **context switch**, **mode switch**.
- Overhead formula: `overhead fraction = s / (q + s)`; `s` = switch time,
  `q` = slice. Units µs and ms written explicitly.

## 11. End-of-video quiz

1. **(MCQ)** Which transition is triggered by the process itself through a
   system call?
   (a) `ready → running` (b) `running → ready` (c) `running → waiting`
   (d) `waiting → ready`
   — **Answer: (c).**
2. **(MCQ, GATE-style)** On a 2-core machine with 25 processes in memory, 6
   are ready. What is the *minimum* number in the waiting state, assuming no
   process is new or terminated?
   — **Answer:** at most 2 run, so at least 25 − 2 − 6 = **17** wait.
3. **(Numerical)** Context-switch time 2 µs, time slice 18 µs, every slice
   ends in a switch. What percentage of CPU time is overhead?
   — **Answer:** 2/(18 + 2) = **10 %**.
4. **(Short answer)** A process issues a disk read and its pages are later
   swapped out while it waits. Which scheduler acted, and which state is it
   now in (seven-state model)?
   — **Answer:** medium-term scheduler; `blocked/suspend`.
5. **(MCQ)** Which event causes a mode switch but no context switch?
   (a) the running process's slice expires and another process is ready
   (b) a keyboard interrupt wakes a waiting process while the running process
   keeps the CPU (c) the running process calls `read` on an empty pipe
   (d) the running process calls `exit`
   — **Answer: (b).**

## 12. References

- Silberschatz, Galvin & Gagne, *Operating System Concepts*, 8e — Ch. 3
  §3.1 process states, p. 103 (PCB, p. 103); §3.2 process scheduling,
  queues, schedulers, context switch, pp. 105–110; Ch. 5 §5.1 (dispatcher),
  from p. 183.
- Stallings, *Operating Systems: Internals and Design Principles*, 6e —
  Ch. 3 §3.2 process states (two-, five-, seven-state models), p. 111;
  §3.4 process control, mode and process switching, p. 135.
- Bovet & Cesati, *Understanding the Linux Kernel*, 3e — Ch. 3 Processes
  (process switch), pp. 79–130.

## 13. Production notes

- **One master diagram.** Keep the state diagram on screen (shrunk to a
  corner) from scene 3 to scene 12 and pulse the matching arrow every time a
  card moves — this coupling is the whole point of the video.
- **Pacing warning:** scene 6 has six rows; give each row a 5–7 s beat and an
  audible tick. Viewers should be able to pause on any row.
- **If over length:** shorten scene 8 (drop the seven-state diagram, keep the
  three stopwatches) and scene 7 (show two doors, mention the other two).
  Never cut scenes 9–10.
- **If under length:** add the 40-process snapshot (deck **How Many
  Processes per State?**) as a 30 s insert after scene 6.
- **Accessibility:** captions on; states distinguished by label and shape as
  well as colour; gold overhead boxes also carry a hatched pattern; avoid
  red/green as the only cue for "forbidden" arrows (use strike-through).
- Use real time units on the timeline in scene 11 (0, 3, 10, 10.2 ms); do
  not stretch the 0.2 ms segment so far that it looks comparable to 10 ms
  without a "not to scale" label.
