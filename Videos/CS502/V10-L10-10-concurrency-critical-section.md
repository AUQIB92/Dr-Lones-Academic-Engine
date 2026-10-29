# V10 · L10 — The Lost Update: Race Conditions and the Critical-Section Problem

> **Lecture:** CS502 L10 `10-concurrency-critical-section` · **Unit:** II · **Target length:** 10–12 min · **Series position:** 10/28
> **Format:** mixed (screen-capture animation + code + terminal) · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

A race condition is a story about *time*: two threads each run three machine instructions, and the
result depends on how the scheduler slices those six instructions together. A static slide can show
one bad interleaving, but it cannot show the scheduler choosing between all of them, or the counter
in memory silently going back to an old value. Students who only read about it believe
`counter++` is one indivisible step and that a race "needs a lot of threads" or "only happens on
multicore". Watching a context switch land between `load` and `store`, and then watching a real
pthread program print about half the expected total, breaks both beliefs.

## 2. Learning outcomes

After watching, the student can:
- [Understand] Explain why the C statement `counter++` compiles to three separate steps (load, add, store) and why that makes it non-atomic.
- [Apply] Trace a given interleaving of two threads' load/add/store steps and compute the final value of the shared variable.
- [Analyze] Enumerate all 20 interleavings of two single increments and say which outcomes are possible (and how many schedules give each).
- [Remember] Name the four parts of a process's code with respect to shared data: entry section, critical section, exit section, remainder section.
- [Understand] State the critical-section problem and preview the three requirements a solution must meet (developed fully in V11).

## 3. Prerequisites

- Computer fundamentals: a CPU executes one machine instruction at a time; it has a few fast **registers**; a program's variables live in **memory**; arithmetic happens in registers, so a value is copied in, changed, and copied back.
- V05 (Process & PCB): a process is a running program; its PCB stores its saved registers.
- V06 (States and context switch): the OS can stop one process and resume another, saving and restoring registers. Recapped in scene 1b.
- V09 (CPU scheduling): a timer interrupt lets the scheduler take the CPU away at any moment (preemption). Recapped in scene 1b.
- No classroom lecture is assumed; every new term (thread, shared variable, preemption, atomic, race condition, critical section) is defined on screen when first used.

## 4. Hook (first 30–45 s)

Black screen, a terminal appears. On it, a 20-line C program: two threads, each running
`counter++` one million times. The instructor says:

> "Two threads. Each adds one, a million times. Start at zero. What does this print?
> Two million, obviously. Let's run it."

`./race` prints `counter = 1003837 (expected 2000000)`. Run it again: `1005721`. Again: `1000508`.

> "Three runs, three different wrong answers, and nobody wrote a bug you can point at.
> Where did nearly a million additions go? In the next ten minutes we will catch one of them
> disappearing, step by step."

(Freeze on the three wrong numbers stacked on top of each other.)

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:40 | Hook | Terminal: `race.c` scrolls, three runs print three different totals near 1,000,000; the expected 2,000,000 is shown in grey beside each | Ask "what will this print?"; run it; the numbers are wrong *and* different each time; promise to catch one lost update |
| 1b | 0:40–1:00 | Recap + new terms | Three definition cards, 5–7 s each: **context switch** (V06: save one process's registers into its PCB, load another's); **preemption** (V09: a timer interrupt can take the CPU away between *any* two instructions); **thread** (new: a second line of execution inside the same process, with its own registers but the *same* memory, so global variables are **shared**) | Quick recap, no new mechanism yet. Stress "own registers, shared memory": that one sentence is the whole cause of today's bug |
| 2 | 1:00–1:40 | One line, three steps | `counter++` on the left; it "explodes" into three lines on the right: `r = counter` (load), `r = r + 1` (add), `counter = r` (store). A memory box `counter` and two register boxes `rA`, `rB` appear | The CPU cannot add to memory in place here; it copies into a register, adds, copies back. Each thread has its own register copy (scene 1b). Define **atomic** on screen: an operation that runs as one indivisible step, with no other thread able to see or act in the middle; `counter++` is *not* atomic. Note: Silberschatz writes this as register1 = counter, etc. |
| 3 | 1:40–2:40 | The happy schedule | Timeline with two lanes, Thread A and Thread B. A's three steps play, then B's (schedule AAABBB). `counter` goes 0 → 1 → 2. Green tick | Run sequentially, all is well: 0, then 1, then 2. This is what we pictured. But the scheduler is not required to be this polite |
| 4 | 2:40–4:00 | The lost update | Same lanes, schedule AABABB. A loads 0, adds → rA=1. **Timer interrupt bolt** strikes, context switch to B. B loads 0 (counter still 0 in memory!). Switch back: A stores 1. B adds → rB=1, stores 1. `counter` ends at 1; A's update flashes red and fades | Slow down here. The key moment is B loading *before* A's store. Both threads computed "0 + 1". The second store overwrote the first. This is the lost update. Card: **race condition** = the result depends on the timing order of accesses to shared data |
| 5 | 4:00–4:30 | Pause & predict 1 | Freeze after A: load, A: add. Question card: "B is about to run all three of its steps. Final value?" | Pause. (Answer: 1 — A's later store writes 1 over B's 1.) Show it playing out with A storing last |
| 6 | 4:30–5:40 | All 20 schedules | A 4×5 grid of 20 mini-timelines (all ways to merge 3 A-steps with 3 B-steps, C(6,3)=20). Each cell turns green (2) or red (1) one by one. Counter: "green 2, red 18" | Exhaustive enumeration: only 2 of 20 schedules give the right answer — AAABBB and BBBAAA. Any overlap at all between "load" and "store" of the other thread loses an update. Results are timing-dependent: that is the definition of a race condition |
| 7 | 5:40–6:40 | Increment vs. decrement (textbook version) | Swap to the Silberschatz producer/consumer framing: counter = 5, A does `counter++`, B does `counter--`. Three outcome buckets fill: 4 (9 schedules), 5 (2 schedules), 6 (9 schedules) | Same mechanism, now the "wrong" answer can be too high *or* too low; whoever stores last wins. Show the 6 case (AABBBA): B's decrement is the one lost |
| 8 | 6:40–7:20 | Pause & predict 2 | Card: "Each thread now does counter++ twice (start 0). Which final values are possible?" Options 1/2/3/4 | Pause. (Answer: 2, 3, 4 — never 1. Of 924 schedules: 2 in 702, 3 in 216, 4 in 6.) Briefly hint why 1 is impossible: the last store must come after someone saw at least one completed increment |
| 9 | 7:20–8:10 | Why the real program shows ~1,000,000 | Back to terminal. Show `objdump`-style 3-instruction loop body (optional) and a sketch of two cores both looping | On a multicore machine the two loops run truly in parallel and collide constantly; on one core, only a timer interrupt mid-sequence causes it, so it is rarer but still real. Totals vary run to run: same code, different timing |
| 10 | 8:10–9:10 | Naming the shared region | The loop body is highlighted; label bubbles: *entry section* (empty for now), *critical section* (`counter++`), *exit section* (empty), *remainder section* (the rest). Code-structure template `do { entry; CS; exit; remainder } while (true);` | The critical section is the code that touches shared data. The problem: design entry and exit sections so that at most one process is ever in its critical section |
| 11 | 9:10–9:50 | The fix, previewed | Terminal: `./race lock` prints `counter = 2000000` twice. Lock/unlock lines highlighted green around `counter++` | Wrapping the critical section in a lock restores 2,000,000 every time. How a lock can be *built* is the subject of V11–V13 |
| 12 | 9:50–10:40 | Three requirements, teaser | Three cards slide in: mutual exclusion, progress, bounded waiting; only the first lights up | Mutual exclusion is what we just needed. Two more requirements keep solutions from being useless or unfair. V11 tests some tempting solutions against all three |
| 13 | 10:40–11:30 | Recap + bridge | Summary list builds; the AABABB timeline replays at 2x in the corner | `counter++` is three steps; a switch between load and store loses an update; 18 of 20 schedules are wrong; that region is the critical section. Bridge to **V11**: "A lock fixed it, but what *is* a lock? Next video: we try to build one using only ordinary variables, fail three times, and then meet Peterson's solution." End card shows the V11 thumbnail |

## 6. Worked example (verified)

**Setup.** Shared `int counter`. Each thread executes `counter++` as:

```
T.1  r = counter     // load
T.2  r = r + 1       // add (register only)
T.3  counter = r     // store
```

**Example A: two single increments, counter = 0.** There are C(6,3) = 20 interleavings.

| Final counter | Number of schedules | Example schedule |
|---|---|---|
| 1 (wrong) | 18 | AABABB |
| 2 (correct) | 2 | AAABBB (and BBBAAA) |

Trace of AABABB (the scene-4 animation):

| Step | Thread | Action | rA | rB | counter |
|---|---|---|---|---|---|
| 1 | A | load | 0 | – | 0 |
| 2 | A | add | 1 | – | 0 |
| 3 | B | load | 1 | 0 | 0 |
| 4 | A | store | 1 | 0 | **1** |
| 5 | B | add | 1 | 1 | 1 |
| 6 | B | store | 1 | 1 | **1** |

All 20 schedules of Example A, for building the scene 6 grid (row-major, 5 per row; value = final counter):

| | | | | |
|---|---|---|---|---|
| AAABBB → **2** | AABABB → 1 | AABBAB → 1 | AABBBA → 1 | ABAABB → 1 |
| ABABAB → 1 | ABABBA → 1 | ABBAAB → 1 | ABBABA → 1 | ABBBAA → 1 |
| BAAABB → 1 | BAABAB → 1 | BAABBA → 1 | BABAAB → 1 | BABABA → 1 |
| BABBAA → 1 | BBAAAB → 1 | BBAABA → 1 | BBABAA → 1 | BBBAAA → **2** |

**Example B (Silberschatz-style): counter = 5, A does `++`, B does `--`.** 20 schedules:
final 4 in 9, final 5 in 2, final 6 in 9. AABABB gives 4 (A's increment lost);
AABBBA gives 6 (B's decrement lost); AAABBB gives 5.

**Example C (checkpoint 2): each thread increments twice, counter = 0.** 924 = C(12,6)
schedules: final 2 in 702, final 3 in 216, final 4 in 6. Final value 1 never occurs.
(Extra fact for the instructor, not on screen: with three increments each, 48,620 schedules give
2 in 648, 3 in 30,780, 4 in 15,714, 5 in 1,458, 6 in 20. The minimum stays 2.)

**Live demo numbers.** `race.c` (2 pthreads × 1,000,000 increments, `gcc -O0 -pthread`) printed
1003837, 1005721, 1000508, 1001319, 1010389 on five runs; with the mutex, 2000000 twice. **These
numbers depend on the machine; re-record the terminal on the recording machine and say "your
numbers will differ".**

**How it was verified.** Examples A–C were produced by an exhaustive interleaving enumerator
(`mc.py`, function `v06`) that generates every merge of the two threads' load/add/store sequences
and executes it. The live-demo figures come from compiling and running `race.c`. Both are in
`/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/`.

## 7. Pause-and-predict checkpoints

1. **(Scene 5, ~4:00)** A has done load and add (rA = 1, counter = 0). B now runs load, add, store, and then A stores. Final value? → **1.** B stores 1, then A overwrites it with its own 1.
2. **(Scene 8, ~6:40)** Two threads, each `counter++` twice, start 0. Which finals are possible? → **2, 3, 4.** Not 1. 4 happens in only 6 of 924 schedules.
3. **(Scene 10, ~8:30, quick)** In the loop `for(...) { counter++; }`, which line is the critical section? → **Only `counter++`** — the loop control uses the private variable `i`.

## 8. Misconceptions to address on screen

- "`counter++` is a single instruction, so it is atomic." → Scene 2 explosion into load/add/store, and register boxes that hold a *stale* copy.
- "Races only happen on multicore machines." → Scene 4 uses one CPU and a timer interrupt; the bolt icon is the only cause.
- "If it worked when I tested it, it is correct." → Scene 6 grid: 2 of 20 schedules are correct, and those two are exactly the "no overlap" ones you get in light testing.
- "A race always loses exactly one update / always gives too small a value." → Scene 7: with ++ and --, the error can go either way (4 or 6).
- "`volatile` fixes it." → Note in scene 9: `race.c` already declares `counter` volatile and still loses updates; `volatile` forces memory access, not atomicity.
- "The critical section is the whole function/thread." → Scene 10 highlights only the shared-data line.

## 9. Visual assets

- **Reuse:** none yet (`Slides/CS502/10-concurrency-critical-section.tex` is not built). When the deck exists, reuse its code-structure template frame (entry/critical/exit/remainder) and keep identical wording. Match the colour usage and "lab server" framing of `Slides/CS502/08-cpu-scheduling-basics.tex` (its hook is a lab-server scenario; a "lab server hit counter" variant works here).
- **New:**
  - Two-lane timeline with register boxes and a memory box (Manim or Motion Canvas); reused in scenes 3, 4, 5, 7.
  - Timer-interrupt bolt + context-switch swoosh (save rA to PCB-A icon).
  - 20-cell interleaving grid with a green/red reveal (generate the 20 schedules from `mc.py` output so the grid is exact).
  - Outcome histogram bars for Example B (9/2/9) and Example C (702/216/6).
- **Live demo:** terminal clip, font ≥ 20 pt.
  ```
  gcc -O0 -pthread race.c -o race
  ./race ; ./race ; ./race
  ./race lock
  ```
  Optional: `objdump -d race | grep -A6 '<work>'` to show the load/add/store; skip if the output is cluttered on the recording machine.

## 10. On-screen text & notation

- **race condition** — outcome depends on the particular order in which accesses to shared data take place.
- **critical section** — code that accesses shared data; *entry section · critical section · exit section · remainder section* (lowercase, per the course notation).
- **mutual exclusion** — at most one process in its critical section at a time.
- Processes/threads named `P0`, `P1` in the CS-problem template; `Thread A`/`Thread B` (A, B) in the interleaving lanes. Do not mix in `p1` or `Proc A`.
- Instruction labels: `load`, `add`, `store`; registers `rA`, `rB`; shared variable `counter` in monospace.
- Schedule strings: AABABB means A, A, B, A, B, B. Explain once on screen.
- Do not use `wait`/`signal` yet (that is V13).
- Definition cards (scene 1b and inline): **thread**, **shared variable**, **preemption**, **context switch**, **atomic**, **race condition**, **critical section**. Each appears on screen the first time the word is spoken.

## 11. End-of-video quiz

1. **(MCQ)** `counter` = 10. Thread A runs `counter++`, Thread B runs `counter--`, each as load/add/store. Which set contains *exactly* the possible final values?
   (a) {10} (b) {9, 11} (c) {9, 10, 11} (d) {8, 9, 10, 11, 12}
   **Answer: (c).** Same structure as Example B shifted by 5: either update can be lost, or neither.
2. **(Numerical)** Two threads each execute `x = x + 1` twice (each as load/add/store), `x` starts at 0. What is the *minimum* possible final value of `x`? **Answer: 2.** (Verified over all 924 schedules.)
3. **(Numerical)** In Example A (two single increments), how many of the 20 interleavings produce the correct result? **Answer: 2.**
4. **(MCQ)** Which change, by itself, guarantees the race in `race.c` disappears?
   (a) declaring `counter` as `volatile` (b) running on a single-core machine (c) placing `counter++` between lock and unlock of one shared mutex (d) increasing the thread's priority
   **Answer: (c).** (a) already used; (b) and (d) only change how often it happens.
5. **(Short answer)** Name the section of code in which a process releases what its entry section acquired. **Answer: the exit section.**

## 12. References

- Silberschatz, Galvin & Gagne, *Operating System Concepts*, 8th ed., Ch. 6 — background and race condition (p.225); the critical-section problem (p.227).
- Stallings, *Operating Systems: Internals and Design Principles*, 6th ed., Ch. 5 §5.1 — principles of concurrency (p.207).
- Bovet & Cesati, *Understanding the Linux Kernel*, 3rd ed., Ch. 5 Kernel Synchronization (pp.189–226), for the curious: race-condition examples inside the kernel (advanced, optional).

## 13. Production notes

- **Pacing:** scene 4 is the whole video; give it at least 75 s and play each step on a click, not a timer. Say the register values out loud.
- **Demo risk:** on some machines and compilers the race shows up less; keep `-O0` and `volatile`. With `-O2` the compiler may rewrite the loop and the demo result changes. Record the terminal clip *before* the voice-over so the narration quotes the numbers actually shown.
- **Over length?** Cut scene 9 (multicore explanation) to one sentence and drop checkpoint 3. Do not cut scene 6 (the 20-schedule grid).
- **Under length?** Show the 6-outcome schedule AABBBA as a full trace in scene 7.
- **Accessibility:** captions on; never rely on red/green alone, so put the value (1 or 2) as a number inside each grid cell; use the Okabe–Ito blue/orange pair for Thread A/B lanes; keep code at ≥ 20 pt.
- **Hinglish version:** keep all on-screen labels in English; only narration changes.
- **Consistency:** use the same lane colours for A/B (or P0/P1) in V11 so the series looks continuous.

### Scene-by-scene recording checklist

| Scene | Must be visible before narration starts | Common recording slip |
|---|---|---|
| 1 | Three terminal runs already captured | Narrating numbers that differ from the captured clip |
| 1b | Three definition cards | Using "thread" before its card appears |
| 2 | Memory box and both register boxes | Showing a single shared register |
| 4 | Timer-interrupt bolt between A's add and B's load | Letting A store before B loads (that is the correct schedule) |
| 6 | Grid built from the table in §6 | Hand-typing schedules instead of using the generated list |
| 10 | Template with all four section labels | Capitalising section names |
| 13 | V11 end card | Missing the bridge sentence |
