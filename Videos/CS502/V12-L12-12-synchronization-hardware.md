# V12 · L12 — Synchronization Hardware: Locks the CPU Can Guarantee

> **Lecture:** CS502 L12 `12-synchronization-hardware` · **Unit:** II · **Target length:** 13–15 min · **Series position:** 12/28
> **Format:** mixed (screen-capture animation of CPUs + memory, code + terminal demo) · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

V10 showed that `counter++` breaks when two processes interleave, and V11 fixed it in software
with Peterson's algorithm. This video shows why the real fix needs the hardware's help: the
instruction that *reads* a lock and the instruction that *sets* it must be one indivisible step,
and only the CPU and memory system can promise that. The key moment is one that a slide cannot
show: two CPUs reading a lock that says "free" at the same nanosecond. Students who only read the
textbook code for `test_and_set` often think it is an ordinary C function that anyone could write,
and they miss why the bounded-waiting version needs a `waiting[]` array. Animation fixes both.
A live run of an atomic counter against a plain counter makes the difference concrete.

## 2. Learning outcomes

After watching, the student can:
- [Understand] Explain why disabling interrupts gives mutual exclusion on one CPU but not on a multiprocessor.
- [Remember] State what `test_and_set` and `compare_and_swap` do, including what each returns, and that each executes atomically.
- [Apply] Build a spinlock from `test_and_set` and from `compare_and_swap`, and trace two CPUs competing for it.
- [Analyze] Trace the bounded-waiting `test_and_set` algorithm and give the order in which waiting processes enter the critical section.
- [Understand] Compare busy waiting (spinning) with blocking, and say when each is the better choice.

## 3. Prerequisites

- Basic computer fundamentals: CPU, registers, main memory, what one machine instruction is, what a multicore CPU is.
- V02: interrupts, the timer, user vs kernel mode (dual mode).
- V05–V06: process, PCB, process states (`ready`, `running`, `waiting`), context switch.
- V10: race condition, critical section, the entry/exit section structure.
- V11: the three requirements (mutual exclusion, progress, bounded waiting) and Peterson's solution.

## 4. Hook (first 30–45 s)

Split screen. Left: a C loop `counter++` executed by two threads, one million times each.
Right: a terminal.

> "Two threads. Each adds one to a shared counter a million times. The answer should be two
> million. Let's run it."
> (Terminal prints `plain 1546283`.)
> "Half a million increments vanished. Run it again." (`1495646`.) "Different wrong answer.
> Now watch the same program, same machine, but with one change: we use an instruction the
> CPU promises cannot be interrupted halfway."
> (Terminal prints `atomic 2000000`.)
> "What did the hardware do that our C code could not? That is this video."

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:45 | Hook | Terminal run of `atomic_counter` (§6, real output); `plain` line in red, `atomic` and `spinlock` lines in green | Same loop, two answers. Name the question: what does "atomic" buy us? |
| 2 | 0:45–1:15 | Recap V10–V11 | Card stack slides in: (a) `counter++` = load / add / store, two lanes interleaving; (b) entry–CS–exit–remainder box; (c) Peterson code greyed with the three requirement badges | **Recap (15–20 s).** A race condition is when the result depends on interleaving. The critical section is the code that touches shared data. Peterson works in theory, with two processes, if memory behaves simply |
| 3 | 1:15–2:15 | Why Peterson is not enough today | Two CPU cores with private caches; a store from core 0 sits in a "store buffer" box before reaching memory; core 1 reads the old value. Caption: "modern CPUs may reorder loads and stores" | Peterson relies on stores reaching memory in program order. Modern processors and compilers may reorder them, so Peterson is not guaranteed to work. We need help from the hardware. Define **atomic**: an operation that happens entirely or not at all, with no other CPU seeing a half-done state |
| 4 | 2:15–3:30 | Idea 1: disable interrupts | Single CPU: the timer interrupt arrow (from V02) is blocked by a "cli" shield while the process is in its CS. Then two CPUs: CPU 0 disables *its* interrupts; CPU 1 runs unaffected into the same CS. Both tokens enter; red flash | On one CPU, a process loses the CPU only through an interrupt (recall V02's timer). Turn interrupts off and nobody else can run. But on a multiprocessor each CPU has its own interrupts: switching off yours does not stop the other CPU. Also: telling every CPU wastes time, delays the clock, and must be a privileged (kernel-mode) instruction, so user programs cannot use it |
| 5 | 3:30–4:45 | `test_and_set` defined | Code card: `boolean test_and_set(boolean *target) { boolean rv = *target; *target = true; return rv; }` with a thick bracket labelled "ONE hardware instruction". Memory cell `lock` shown; arrow reads old value out, writes `true` in, in one flash | Define **test-and-set**: returns the old value and sets the word to true, atomically. It looks like C but it is a single instruction. If two CPUs execute it at the same time, the hardware runs them one after the other, in some order |
| 6 | 4:45–6:00 | Spinlock from TAS | Code: `while (test_and_set(&lock)) ; /* CS */ lock = false;`. Two CPUs race: both issue TAS in the same cycle; hardware serialises: CPU 0 gets `false` (enters), CPU 1 gets `true` (spins, loop arrow spinning). CPU 0 clears; CPU 1's next TAS returns `false`, enters | Define **spinlock**: a lock where the waiter loops ("spins") testing it. Walk through who gets which return value. Ask: mutual exclusion? Yes. Progress? Yes. Bounded waiting? Hold on |
| 7 | 6:00–7:00 | `compare_and_swap` defined | Code card: `int compare_and_swap(int *value, int expected, int new_value) { int temp = *value; if (*value == expected) *value = new_value; return temp; }`. Cell `value` = 0; CAS(0→1) returns 0, cell becomes 1; second CAS(0→1) returns 1, cell stays 1 (§6) | Define **compare-and-swap (CAS)**: "change it only if it still holds what I expect". Always returns the old value. On x86 this is the `cmpxchg` instruction |
| 8 | 7:00–7:45 | Spinlock from CAS | Code: `while (compare_and_swap(&lock, 0, 1) != 0) ; /* CS */ lock = 0;`. Same two-CPU race animation with CAS | Same pattern, different instruction. Success means "the old value was 0 and I put 1 there" |
| 9 | 7:45–8:15 | Pause & predict 1 | Freeze: lock = 1 (held by P0), P1 and P2 spinning. "P0 releases. Who gets in next?" | Pause. Answer: whoever's TAS/CAS the hardware happens to run first. No order is promised, so P2 could lose every time: that breaks bounded waiting |
| 10 | 8:15–10:15 | Bounded waiting with TAS | Code on left (§6). Right: four process circles P0–P3 around a ring; `waiting[i]` lamps under each; `lock` cell. Trace: P2 in CS, P0, P1, P3 lamps lit. P2 exits: a pointer scans j = 3 → finds P3; P3's lamp turns off, P3 enters *without* touching `lock`. P3 exits: scan j = 0 → P0. P0 exits: scan j = 1 → P1. P1 exits: scan 2, 3, 0 all dark, back to 1 → `lock = false` | The exiting process does not just drop the lock; it looks around the ring in order (i+1, i+2, …) and hands the CS directly to the next waiter. So any waiter gets in within n − 1 turns. Order on screen: P2, P3, P0, P1 |
| 11 | 10:15–10:45 | Pause & predict 2 | Ring of six: P4 in CS, lamps lit at P1, P3, P5. "P4 exits. Who enters?" | Pause. Answer: P5 (first lit lamp scanning 5, 0, 1, …) |
| 12 | 10:45–11:45 | Atomic variables | Code: `atomic_long atom; atomic_fetch_add(&atom, 1);` vs `long plain; plain++;`. Back to the terminal output: three runs (§6), `plain` different every time, `atomic` always 2000000 | Define **atomic variable**: a variable whose updates (increment, add, swap) are done with atomic instructions such as CAS, provided by the language library. Good for one counter or flag; they do not protect a multi-step critical section with several variables |
| 13 | 11:45–13:00 | Busy waiting vs blocking | Two lanes. Lane A, "spin": waiting process stays `running`, CPU meter at 100%. Lane B, "block": waiting process goes to `waiting` state (V06 diagram), CPU given to someone else; context-switch cost shown as a small toll booth | Define **busy waiting**: looping on a condition while holding the CPU. Define **blocking**: the process sleeps in a queue and the OS wakes it later. Spin when the wait is shorter than two context switches and you have more than one CPU; block when the wait may be long. On a single CPU, spinning is pointless: the lock holder cannot run while you spin |
| 14 | 13:00–14:00 | Recap + bridge | Summary cards: interrupts-off (single CPU only) · TAS · CAS · bounded-waiting ring · atomic variable · spin vs block. Final frame: an integer box with a queue under it, labelled "?" | Hardware gives us one atomic step; everything else is built on it. These locks spin. Next video (V13): a counter with a waiting room, the semaphore, which lets waiters block instead of spin |

## 6. Worked example (verified)

**A. Live demo program** (`atomic_counter.c`, C11 atomics + POSIX threads):

```c
#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#define N 1000000
long plain = 0;                         /* ordinary shared counter */
atomic_long atom = 0;                   /* atomic variable */
atomic_flag lock = ATOMIC_FLAG_INIT;    /* TAS spinlock */
long locked = 0;
void *work(void *arg) {
    for (int i = 0; i < N; i++) {
        plain++;                                   /* racy */
        atomic_fetch_add(&atom, 1);                /* atomic read-modify-write */
        while (atomic_flag_test_and_set(&lock)) ;  /* acquire: spin */
        locked++;                                  /* critical section */
        atomic_flag_clear(&lock);                  /* release */
    }
    return NULL;
}
int main(void) {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, work, NULL);
    pthread_create(&t2, NULL, work, NULL);
    pthread_join(t1, NULL); pthread_join(t2, NULL);
    printf("expected %d\nplain    %ld\natomic   %ld\nspinlock %ld\n",
           2*N, plain, (long)atom, locked);
    return 0;
}
```

Commands and three real runs (Apple clang 21, macOS arm64, `-O0`):

```
$ gcc -O0 -pthread atomic_counter.c -o atomic_counter
$ ./atomic_counter          (run 1)     (run 2)     (run 3)
expected 2000000
plain                        1546283     1495646     1413129
atomic                       2000000     2000000     2000000
spinlock                     2000000     2000000     2000000
```

The `plain` value changes on every run; the narration must say "a wrong number, different every
time", not quote a fixed value. `atomic` and `spinlock` were exactly 2000000 on all three runs.

**B. CAS trace.** `value = 0`.
`compare_and_swap(&value, 0, 1)` returns **0**, `value` becomes **1** (lock acquired).
A second `compare_and_swap(&value, 0, 1)` returns **1**, `value` stays **1** (caller must spin).

**C. Bounded-waiting TAS** (Silberschatz form), n = 4:

```
do {                                     // process Pi
    waiting[i] = true;
    key = true;
    while (waiting[i] && key)
        key = test_and_set(&lock);
    waiting[i] = false;
        /* critical section */
    j = (i + 1) % n;
    while ((j != i) && !waiting[j])
        j = (j + 1) % n;
    if (j == i) lock = false;            // nobody waiting
    else        waiting[j] = false;      // hand CS directly to Pj
        /* remainder section */
} while (true);
```

Start: P2 is in the CS; `waiting[0] = waiting[1] = waiting[3] = true`.

| Step | Exiting | Scan | Next in CS | Still waiting |
|---|---|---|---|---|
| 1 | P2 | j = 3 → waiting | P3 | P0, P1 |
| 2 | P3 | j = 0 → waiting | P0 | P1 |
| 3 | P0 | j = 1 → waiting | P1 | — |
| 4 | P1 | j = 2, 3, 0 → none, back to 1 | — (`lock = false`) | — |

CS order: **P2, P3, P0, P1**. Worst-case wait for any process: **n − 1 = 3** turns.
Checkpoint 2 (n = 6, P4 in CS, waiting P1, P3, P5): next is **P5**.

**How it was verified.** Part A was compiled and run three times with the commands shown; the
numbers are the program's actual output. Parts B and C (and the checkpoint) are the output of
`v12_bw.py`, which implements CAS and the exit-section scan literally. Both are in
`/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/`
(`v12/atomic_counter.c`, `v12_bw.py`).

## 7. Pause-and-predict checkpoints

1. **(Scene 9, ~7:45)** P0 releases a plain TAS spinlock; P1 and P2 are spinning. Who enters next?
   → **Either; the hardware gives no order, so one process can lose repeatedly (no bounded waiting).**
2. **(Scene 11, ~10:15)** Six processes; P4 exits; P1, P3, P5 waiting. Who enters? → **P5.**
3. **(Scene 4, quick)** On a 4-core machine, P0 on core 0 disables interrupts. Can P1 on core 2 enter the same CS? → **Yes; core 2's interrupts and execution are unaffected.**

## 8. Misconceptions to address on screen

- "`test_and_set` is a C function, so I can write my own." → Scene 5: bracket "ONE hardware instruction"; then show a hand-written C version being interleaved between two CPUs and failing.
- "Disabling interrupts is a general locking method." → Scene 4: the two-CPU animation where both enter.
- "A spinlock is always wasteful." → Scene 13: on a multiprocessor with a short CS, spinning avoids two context switches.
- "A TAS spinlock is fair." → Scene 9 and 10: plain TAS has no order; only the `waiting[]` hand-off gives bounded waiting.
- "Atomic variables solve every race." → Scene 12: an atomic counter does not make "check balance then withdraw" atomic.
- "The waiter in the bounded-waiting algorithm grabs the lock itself." → Scene 10: P3 enters because `waiting[3]` was cleared, while `lock` stays `true`; highlight that the lock cell does not change.

## 9. Visual assets

- **Reuse:** no `Slides/CS502/12-synchronization-hardware.tex` exists yet; follow the visual grammar of `Slides/CS502/08-cpu-scheduling-basics.tex` (`block` for definitions, `alertblock` for the running thread / warnings, `exampleblock` for traces). Reuse the V10 load/add/store interleaving lanes and the V06 state diagram.
- **New:**
  - Two-core diagram with private caches and a store buffer (scene 3).
  - Interrupt shield animation, single vs dual core (scene 4).
  - `lock` memory cell with atomic read+write flash; two-CPU arbitration (scenes 5–8).
  - Process ring with `waiting[]` lamps and a scanning pointer (scenes 10–11), Manim or PowerPoint morph, one keyframe per hand-off.
  - Spin vs block lanes with CPU meter (scene 13).
- **Live demo:**
  ```
  gcc -O0 -pthread atomic_counter.c -o atomic_counter
  ./atomic_counter
  ./atomic_counter
  ```
  Run on a multicore machine. Use `-O0`: at higher optimisation the compiler may collapse the plain loop and hide the race.

**Keyframe list for the animator** (one line = one click; numbers from §6 only):

- Scene 3 — K1 two cores, each with a cache; K2 core 0 stores `flag[0] = true` into its store buffer;
  K3 core 1 reads `flag[0]` from memory and still sees `false`; K4 caption "reordered".
- Scene 4 — K1 one CPU, timer arrow hits the shield and bounces; K2 second CPU appears;
  K3 CPU 0 shield up, CPU 1 has none; K4 both tokens inside the CS box, red outline.
- Scene 6 — K1 `lock = false`; K2 both CPUs issue TAS (same tick); K3 arbiter lets CPU 0 first:
  returns `false`, cell turns `true`; K4 CPU 1's TAS returns `true`, loop icon spins;
  K5 CPU 0 writes `lock = false`; K6 CPU 1's TAS returns `false`, enters.
- Scene 7 — K1 `value = 0`; K2 CAS(0→1) returns 0, cell 1; K3 second CAS(0→1) returns 1, cell stays 1.
- Scene 10 — K1 ring P0–P3, P2 in CS, lamps on at P0, P1, P3; K2 P2 exits, pointer at j = 3;
  K3 lamp 3 off, P3 into CS (lock cell unchanged, still `true`); K4 P3 exits, pointer j = 0;
  K5 P0 enters; K6 P0 exits, pointer j = 1, P1 enters; K7 P1 exits, pointer visits 2, 3, 0,
  returns to 1; K8 `lock = false`.
- Scene 12 — K1 the two declarations side by side; K2 three terminal runs stacked, the
  `plain` column highlighted as different each time.

## 10. On-screen text & notation

- **atomic** — done entirely or not at all; no other CPU sees a half-done state (define in scene 3).
- **test-and-set (TAS)** — `test_and_set(&lock)`: returns old value, sets `true` (scene 5).
- **compare-and-swap (CAS)** — `compare_and_swap(&value, expected, new_value)`: returns old value; writes only if old = expected (scene 7).
- **spinlock** — lock whose waiter loops testing it (scene 6).
- **atomic variable** — variable with atomic update operations, e.g. `atomic_fetch_add` (scene 12).
- **busy waiting** vs **blocking** (scene 13).
- **multiprocessor** — a machine with more than one CPU core sharing memory (scene 4).
- Section names lowercase: *entry section · critical section · exit section · remainder section*.
- Processes `P0 … Pn−1`, index `i` for "me" and `j` for "the one I scan".
- Use `true`/`false` for TAS (`boolean`), `0`/`1` for CAS (`int`), as in Silberschatz.

## 11. End-of-video quiz

1. **(MCQ)** Why is disabling interrupts unsuitable for mutual exclusion on a multiprocessor?
   (a) it is too slow on one CPU (b) other CPUs keep running and can enter the CS (c) it breaks the timer permanently (d) it causes deadlock
   **Answer: (b).**
2. **(Numerical)** `x = 5`. A CPU executes `compare_and_swap(&x, 5, 9)` and then `compare_and_swap(&x, 5, 2)`. What do the two calls return, and what is `x` at the end?
   **Answer: returns 5 then 9; x = 9.**
3. **(Numerical)** In the bounded-waiting TAS algorithm with n = 5, P1 is in the CS and P0 and P3 are waiting. In what order do the next processes enter? **Answer: P3 then P0.** (Scan 2, 3 → P3; P3 exits, scan 4, 0 → P0.)
4. **(MCQ)** A plain TAS spinlock `while (test_and_set(&lock));` satisfies which requirements?
   (a) mutual exclusion only (b) mutual exclusion and progress, not bounded waiting (c) all three (d) none
   **Answer: (b).**
5. **(Short answer)** On a single-CPU system, a thread spins on a lock held by a thread that is not running. Why is this a waste? **Answer:** the holder cannot run (and release the lock) until the spinner's time slice ends, so the whole slice is wasted; blocking would hand the CPU to the holder.

## 12. References

- Silberschatz, Galvin & Gagne, *Operating System Concepts*, 8th ed., Ch. 6 — synchronization hardware: test-and-set, compare-and-swap (p.231); critical-section requirements (p.227); Peterson's solution (p.229).
- Stallings, *Operating Systems: Internals and Design Principles*, 6th ed., Ch. 5 §5.2 — mutual exclusion: hardware support (p.216).
- Bovet & Cesati, *Understanding the Linux Kernel*, 3rd ed., Ch. 5 (pp.189–226) — atomic operations and spin locks in the Linux kernel (optional depth).

## 13. Production notes

- **Pacing:** scenes 6 and 10 are the core. In scene 10, pause on every scan step and let the pointer move one position per beat.
- Always put the return value on screen next to each TAS/CAS call; students confuse "returns old" with "returns new".
- The hook relies on the live race. Record several runs and keep a fallback clip with the §6 outputs; never claim the `plain` value is fixed.
- **Over length?** Shorten scene 3 to one caption ("CPUs may reorder memory operations") and drop the CAS spinlock animation (scene 8), keeping its code card.
- **Under length?** Add a short scene on the x86 `lock` prefix or the `xchg` instruction as another atomic swap.
- **Accessibility:** spinning vs entered tokens differ by shape (loop arrow vs check mark), not only colour; Okabe–Ito palette; captions on; terminal font at least 24 pt.
- **Bridge line (final 10 s):** "Spinning works when waits are short. When they are long, we want the waiter to go to sleep. That is the semaphore: V13."

**Time budget** (computed from the storyboard times; see the over-length notes above for what to cut):

| Scene | Title | Seconds | Running total |
|---|---|---|---|
| 1 | Hook | 45 | 0:45 |
| 2 | Recap V10–V11 | 30 | 1:15 |
| 3 | Why Peterson is not enough today | 60 | 2:15 |
| 4 | Idea 1: disable interrupts | 75 | 3:30 |
| 5 | `test_and_set` defined | 75 | 4:45 |
| 6 | Spinlock from TAS | 75 | 6:00 |
| 7 | `compare_and_swap` defined | 60 | 7:00 |
| 8 | Spinlock from CAS | 45 | 7:45 |
| 9 | Pause & predict 1 | 30 | 8:15 |
| 10 | Bounded waiting with TAS | 120 | 10:15 |
| 11 | Pause & predict 2 | 30 | 10:45 |
| 12 | Atomic variables | 60 | 11:45 |
| 13 | Busy waiting vs blocking | 75 | 13:00 |
| 14 | Recap + bridge | 60 | 14:00 |

**Viewer questions to expect in comments** (answers for a pinned reply):

- Is `atomic_flag` the same as `test_and_set`? Yes in behaviour: `atomic_flag_test_and_set` sets the flag and returns its old value atomically.
- Why does the plain counter lose updates even on a fast machine? `plain++` is three steps (load, add, store), exactly the V10 interleaving.
- Does the bounded-waiting version still spin? Yes; it bounds *how many turns* a process waits, not *how* it waits.
