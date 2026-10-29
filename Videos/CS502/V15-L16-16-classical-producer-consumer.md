# V15 · L16 — The Bounded Buffer: Three Semaphores and One Fatal Swap

> **Lecture:** CS502 L16 `16-classical-producer-consumer` · **Unit:** II · **Target length:** 12–14 min · **Series position:** 15/28
> **Format:** screen-capture animation (ring buffer + three semaphore gauges) + code · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

The bounded-buffer solution is six lines per process, and almost every student can reproduce it
from memory. Few can explain why the order of the two `wait` calls matters, because the deadlock
needs a particular state (a full buffer) and a particular sequence (the producer grabs the lock
first) to appear. Animation lets three semaphore gauges (`mutex`, `empty`, `full`) and the buffer
slots move together, so the viewer sees that `empty + full` tracks the slots and that `mutex`
guards only the slot update. Then the swapped version can be driven into the stuck state and
frozen there, with both processes visibly asleep in different queues.

## 2. Learning outcomes

After watching, the student can:
- [Understand] Explain the role of each semaphore in the bounded-buffer solution: `mutex` (mutual exclusion on the buffer), `empty` (free slots), `full` (filled slots), and their initial values 1, N, 0.
- [Apply] Trace a producer/consumer schedule and give `mutex`, `empty`, `full` and the item count after each operation.
- [Analyze] Show, with a concrete schedule, that swapping `wait(empty)` and `wait(mutex)` in the producer can deadlock, and identify the state in which it happens.
- [Analyze] Decide whether a given reordering of the `wait` or `signal` calls is safe.
- [Remember] State the invariant `empty + full ≤ N` (equal to N when no process is between its two semaphore operations).

## 3. Prerequisites

- Computer fundamentals: memory, an array, a program running in a loop.
- V05 (Process & PCB), V06 (States and context switch): what a *blocked* process is. Recapped in scene 1b.
- V10 (race condition, critical section) and V13 (semaphores: `wait`/`signal`, counting vs. binary, value + waiting queue). Recapped in scene 1b.
- V14 (Semaphore patterns and IPC): processes communicating through a shared buffer; the "signal an event with a semaphore initialised to 0" pattern. Recapped in one line.
- No classroom lecture is assumed.

## 4. Hook (first 30–45 s)

A canteen counter with exactly three tray slots. A cook puts trays on, a student takes them off.

> "Three slots. The cook must stop when all three are full; the student must stop when all three
> are empty; and they must never grab the same slot at the same moment. Here is a correct
> solution in six lines each. Now I make one tiny edit: I swap two lines in the cook's code.
> It still compiles. It still runs. It still passes every quick test. And then one day, with three
> trays on the counter, the canteen freezes forever: cook and student both standing still. Let's
> find out which two lines, and why."

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:45 | Hook | Canteen counter with 3 slots; cook and student icons; an "edit" cursor swaps two highlighted lines; counter freezes with 3 trays, both figures greyed out with "zzz" | One swap, permanent freeze; which lines and why |
| 1b | 0:45–1:10 | Recap | 6 s cards: **semaphore** (V13: an integer + a queue; `wait` decrements and blocks if the result is negative, `signal` increments and wakes one waiter); **blocked** (V06: parked by the OS, uses no CPU, runs again only when woken); **critical section** (V10); **binary vs. counting semaphore** (V13) | Everything today is built from these four ideas |
| 2 | 1:10–2:10 | The problem | Ring buffer of N = 3 slots with `in` and `out` arrows; producer and consumer lanes; define **producer** (creates items), **consumer** (uses them), **bounded buffer** (a fixed-size shared array) | Three things must be enforced: producer waits when full, consumer waits when empty, buffer updates never interleave |
| 3 | 2:10–3:20 | Three semaphores | Three gauges appear: `mutex = 1` (lock icon), `empty = 3` (three empty-slot icons), `full = 0`. Code for both processes builds line by line (§6) | `empty` counts free slots, `full` counts filled slots, `mutex` protects `in`/`out` and the slot write. Counting semaphores for counting, binary for exclusion |
| 4 | 3:20–5:20 | Correct trace | The scripted trace (§6) plays: consumer arrives first and blocks on `full`; producer adds three items; on its fourth try blocks on `empty`; consumer removes one; producer completes. Gauges and slots change every step; trace table builds on the right | Narrate the gauges. Point out: blocking happens only at `empty`/`full`, never while holding `mutex` |
| 5 | 5:20–5:50 | Pause & predict 1 | Freeze: count = 3, empty = 0, full = 3, mutex = 1, producer about to call `wait(empty)` | Pause. (Answer: producer blocks; empty would go below 0.) |
| 6 | 5:50–6:40 | The invariant | Overlay `empty + full` as a running sum next to the gauges; it reads 3 except during the short gap between a process's `wait` on one and `signal` on the other | A check viewers can apply to their own traces: the model checker confirms `empty + full ≤ 3` in all 52 reachable states, and item count stays in 0..3 |
| 7 | 6:40–7:10 | The swap | Producer code: lines `wait(empty)` and `wait(mutex)` swap places with a slide animation; the new lines turn amber | "Grab the lock first, then check for space." Seems harmless; it is not |
| 8 | 7:10–9:10 | Driving into deadlock | Swapped producer runs three full iterations (buffer fills: count 3, empty 0, full 3). Fourth iteration: `wait(mutex)` succeeds (mutex 0, padlock closes, producer holds key), `wait(empty)` blocks: producer token moves into `empty`'s queue **still holding the key**. Consumer: `wait(full)` → full 2; `wait(mutex)` → blocks, token into `mutex`'s queue. Red ring connects the two queues. "Deadlock" stamp | Narrate: producer sleeps holding the lock; the only process that could create an empty slot needs that lock. Each waits for the other forever. Model checker: exactly 1 deadlock state, reached after 17 steps at the earliest |
| 9 | 9:10–9:40 | Pause & predict 2 | Card: "What if instead the *consumer* swaps its two waits, `wait(mutex)` then `wait(full)`? When can it deadlock?" | Pause. (Answer: on an *empty* buffer, immediately: consumer takes mutex, blocks on full; producer passes `wait(empty)`, blocks on mutex. Two steps.) |
| 10 | 9:40–10:30 | Consumer swap shown | Fast 2-step animation from the initial state; same red ring | The mirror image: lock-then-wait-for-items with no items |
| 11 | 10:30–11:20 | Swapping the signals is harmless | Producer's `signal(mutex)` and `signal(full)` swapped; gauge animation runs; "0 deadlocks" badge | `signal` never blocks, so order of signals only affects who wakes slightly sooner. Rule: **wait for the resource-counting semaphore first, lock last; never block while holding the lock** |
| 12 | 11:20–12:10 | Many producers, many consumers | Two producer and two consumer lanes; one sweep animation; badge "324 states, 0 deadlocks, never two in the buffer update" | Same code works for multiple producers and consumers; `mutex` is what keeps two producers from writing the same slot |
| 13 | 12:10–13:10 | Recap + bridge | Summary card: initial values 1 / N / 0; ordering rule; invariant. End card for **V16** | Bridge to **V16**: "One buffer, two kinds of process. Next: what if many processes only *read* and a few *write*, and what if five philosophers share five forks? Plus the construct that makes semaphore mistakes like today's harder to write: the monitor." |

## 6. Worked example (verified)

**Code (N = 3).** Shared: `buffer[3]`, `in = 0`, `out = 0`; semaphores `mutex = 1`, `empty = 3`, `full = 0`.

```
producer:                          consumer:
while (true) {                     while (true) {
  /* produce next_item */            wait(full);        // C1
  wait(empty);      // P1            wait(mutex);       // C2
  wait(mutex);      // P2            item = buffer[out]; out = (out+1) % 3;  // C3
  buffer[in] = next_item;            signal(mutex);     // C4
  in = (in+1) % 3;  // P3            signal(empty);     // C5
  signal(mutex);    // P4            /* consume item */
  signal(full);     // P5          }
}
```

**Correct trace** (C = consumer, P = producer, count = items in buffer):

| # | Step | mutex | empty | full | count | Note |
|---|---|---|---|---|---|---|
| 0 | start | 1 | 3 | 0 | 0 | |
| 1 | C: wait(full) | 1 | 3 | 0 | 0 | **C blocks** (no items) |
| 2–6 | P: P1…P5 (item 1) | 1 | 2 | 1 | 1 | after P1: empty 2; P2: mutex 0; P3: count 1; P4: mutex 1; P5: full 1 (C could now wake; in this script C is resumed later) |
| 7–11 | P: P1…P5 (item 2) | 1 | 1 | 2 | 2 | |
| 12–16 | P: P1…P5 (item 3) | 1 | 0 | 3 | 3 | buffer full |
| 17 | P: wait(empty) | 1 | 0 | 3 | 3 | **P blocks** |
| 18 | C: wait(full) | 1 | 0 | 2 | 3 | |
| 19 | C: wait(mutex) | 0 | 0 | 2 | 3 | |
| 20 | C: remove | 0 | 0 | 2 | 2 | |
| 21 | C: signal(mutex) | 1 | 0 | 2 | 2 | |
| 22 | C: signal(empty) | 1 | 1 | 2 | 2 | P can proceed |
| 23 | P: wait(empty) | 1 | 0 | 2 | 2 | |
| 24 | P: wait(mutex) | 0 | 0 | 2 | 2 | |
| 25 | P: add | 0 | 0 | 2 | 3 | |
| 26 | P: signal(mutex) | 1 | 0 | 2 | 3 | |
| 27 | P: signal(full) | 1 | 0 | 3 | 3 | full again |

*Simplification for the animation:* the simulator models a blocked `wait` as "cannot proceed yet"
and retries it later (rows 1 → 18, 17 → 23), which produces the same gauge values as the
queue-based semaphore of V13 at every row shown. On screen, show the consumer token in `full`'s
queue from row 1 to row 5, then moving to ready; just keep the gauge values above.

**Deadlock with the producer's waits swapped** (`wait(mutex)` before `wait(empty)`): shortest path
found = 17 steps. The producer completes three iterations (state: count 3, empty 0, full 3,
mutex 1). Step 16: producer `wait(mutex)` → mutex 0. Producer `wait(empty)` → blocks (empty 0).
Step 17: consumer `wait(full)` → full 2. Consumer `wait(mutex)` → blocks (mutex 0).
**Stuck state:** count 3, empty 0, full 2, mutex 0; producer blocked on `empty` holding `mutex`,
consumer blocked on `mutex`.

Full path for the scene 8 animation (P = swapped producer; values after each step):

| Steps | Who | Operations | mutex | empty | full | count |
|---|---|---|---|---|---|---|
| 1–5 | P | wait(mutex), wait(empty), add, signal(mutex), signal(full) | 1 | 2 | 1 | 1 |
| 6–10 | P | same, item 2 | 1 | 1 | 2 | 2 |
| 11–15 | P | same, item 3 | 1 | 0 | 3 | 3 |
| 16 | P | wait(mutex) succeeds | 0 | 0 | 3 | 3 |
| — | P | wait(empty) **blocks** (holding mutex) | 0 | 0 | 3 | 3 |
| 17 | C | wait(full) | 0 | 0 | 2 | 3 |
| — | C | wait(mutex) **blocks** | 0 | 0 | 2 | 3 |

**Deadlock with the consumer's waits swapped:** shortest path = 2 steps from the start: producer
`wait(empty)` → empty 2; consumer `wait(mutex)` → mutex 0; then consumer blocks on `full` (0) and
producer blocks on `mutex` (0).

**Exhaustive results** (each line one atomic step, processes loop forever):

| Variant | Reachable states | Deadlock states | Other checks |
|---|---|---|---|
| Correct, 1 producer + 1 consumer | 52 | 0 | count ∈ {0,1,2,3}; empty + full ≤ 3 everywhere |
| Correct, 2 producers + 2 consumers | 324 | 0 | never two processes in the buffer update; count ∈ {0..3} |
| Producer waits swapped | 51 | 1 | as traced above |
| Consumer waits swapped | — | 1 | 2-step path |
| Producer signals swapped | — | 0 | count ∈ {0..3} |

**Quiz arithmetic.** N = 5, producer runs alone first: it inserts 5 items and blocks on the 6th
(empty 0, full 5). After the consumer removes 2: empty 2, full 3.

**How it was verified.** All numbers come from `mc.py` (function `v09`), an explicit-state model
checker that explores every interleaving of the producer and consumer programs above and reports
deadlocks (states where no process can move), mutual-exclusion violations, the count range and the
shortest path to each deadlock; the trace table is its scripted run. The N = 5 quiz arithmetic was
run in `quiz.py`. Files: `/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/`.

## 7. Pause-and-predict checkpoints

1. **(Scene 5, ~5:20)** count 3, empty 0, full 3, mutex 1; producer calls `wait(empty)`. → **It blocks.**
2. **(Scene 9, ~9:10)** Consumer swaps its waits. When can it deadlock? → **On an empty buffer, right at the start: consumer holds mutex waiting on full; producer waits on mutex.**
3. **(Scene 11, quick)** Producer swaps its two *signal* calls. Deadlock possible? → **No.** `signal` never blocks.

## 8. Misconceptions to address on screen

- "`mutex` alone is enough; `empty`/`full` are optimisations." → Scene 4: without `empty`, the producer would overwrite an unread slot when the buffer is full; show a slot being overwritten in a 3 s side clip.
- "Any order of the `wait` calls works because each one is correct on its own." → Scenes 8 and 10, the frozen red ring.
- "Deadlock means a bug that shows up quickly in testing." → The producer swap needs a full buffer *and* a specific order; the shortest path is 17 steps.
- "`empty + full` always equals N." → Scene 6: it dips below N between a process's `wait` and its matching `signal`; the invariant is ≤ N.
- "With two producers we need another semaphore." → Scene 12: no; `mutex` already serialises the slot update.
- "Signals must also be in a particular order." → Scene 11 badge: 0 deadlocks.

## 9. Visual assets

- **Reuse:** `Slides/CS502/16-classical-producer-consumer.tex` is not built; when it is, reuse its code frame and keep the same line labels (P1…P5, C1…C5). Reuse the semaphore box (value + queue) designed for V13 so the series is visually continuous; colour roles as in `Slides/CS502/08-cpu-scheduling-basics.tex` (`\good` / `\bad`).
- **New:**
  - Ring buffer with 3 slots, `in`/`out` arrows; items as tray icons.
  - Three gauges (`mutex`, `empty`, `full`), each a V13-style semaphore box with its own queue.
  - "Holding the key" badge on the producer token while it sleeps (scene 8): the visual that explains the deadlock.
  - Red wait-for ring between the two queues (reused in V16 and V17).
  - Model-checker badges (52 / 324 / 1 states) as small lower-third captions.
- **Live demo (optional, 60 s):** a pthread version with `sem_t` on Linux; the swapped variant hangs with a full buffer. Not pre-run for this brief; if used, run on the recording machine first and add a `printf` before each `sem_wait` so the hang point is visible.

## 10. On-screen text & notation

- Semaphores: `mutex`, `empty`, `full` (lowercase, monospace), initial values **1, N, 0**.
- Operations: `wait(S)` / `signal(S)` only (no `P`/`V` in this video).
- Process names: `producer`, `consumer` in monospace in code; "P" and "C" in the trace table, with a legend.
- N = 3 on screen throughout the trace; state it at first use.
- Definitions on screen at first use: **producer**, **consumer**, **bounded buffer**, **deadlock** ("a set of processes each waiting for an event that only another process in the set can cause"), **invariant** ("a statement that stays true at every step").
- Line labels P1–P5 and C1–C5 as in §6.

## 11. End-of-video quiz

1. **(MCQ)** In the bounded-buffer solution with N slots, the initial values of `mutex`, `empty`, `full` are:
   (a) 0, N, 0 (b) 1, 0, N (c) 1, N, 0 (d) 1, N, N
   **Answer: (c).**
2. **(Numerical)** N = 5. The producer runs alone from the start until it blocks. How many items has it inserted? Then the consumer removes 2 items. What are `empty` and `full`? **Answer: 5 items; empty = 2, full = 3.**
3. **(MCQ)** Which change can cause deadlock?
   (a) producer: `signal(full)` before `signal(mutex)` (b) producer: `wait(mutex)` before `wait(empty)` (c) consumer: `signal(empty)` before `signal(mutex)` (d) none of these
   **Answer: (b).**
4. **(Short answer)** In the producer-swap deadlock, give the values of `mutex`, `empty`, `full` in the stuck state (N = 3). **Answer: mutex 0, empty 0, full 2** (count 3).
5. **(MCQ)** A student removes `mutex` entirely but keeps `empty` and `full`, with one producer and one consumer that use separate `in` and `out` indices. Which is true? (a) deadlock becomes possible (b) the buffer can overflow (c) with exactly one producer and one consumer the slot accesses do not conflict, but with two producers two items can be written to the same slot (d) nothing changes for any number of processes
   **Answer: (c).** *(Reasoning question; the two-producer case is the scene 12 point. Not model-checked; present as discussion.)*

## 12. References

- Silberschatz, Galvin & Gagne, *Operating System Concepts*, 8th ed., Ch. 6 — classic problems of synchronization, the bounded-buffer problem (p.239); semaphores (p.234) for the recap.
- Stallings, *Operating Systems: Internals and Design Principles*, 6th ed., Ch. 5 §5.3 — semaphores, including the producer/consumer problem (p.219); Appendix A §A.2 — race conditions and semaphores (p.751).

## 13. Production notes

- **Pacing:** scene 4 has 27 rows; group the producer's three full iterations (rows 2–16) into three quick 5-step bursts and slow down only at rows 1, 17 and 22. Scene 8 (deadlock) gets the most time.
- Keep the three gauges in a fixed row; viewers compare by position.
- In scene 8, linger 3 s on the frozen frame with both "zzz" icons and the key badge before narrating.
- **Over length?** Cut scene 12 to a caption and drop quiz 5. Do not cut scene 10 (consumer swap); it is a frequent exam question.
- **Under length?** Show the overwrite failure without `empty` as a full clip.
- **Accessibility:** gauges show numbers, not just fill levels; deadlock ring uses a dashed pattern in addition to red; Okabe–Ito palette; captions on.
- Quiz 5 is a discussion question; label it "think about it" rather than presenting a verified answer as computed.

### Scene-by-scene recording checklist

| Scene | Must be visible before narration starts | Common recording slip |
|---|---|---|
| 1b | Four recap cards | Running over 30 s |
| 3 | All three gauges with initial values 1 / 3 / 0 | Writing `full = N` by habit |
| 4 | Trace table header and the ring buffer | Moving the `in` arrow on `wait(empty)` instead of on the slot write |
| 6 | Running sum `empty + full` | Claiming it is always exactly 3 |
| 8 | Key badge on the producer token | Not showing that the producer still holds `mutex` while asleep |
| 10 | Initial state reset | Forgetting to reset the buffer to empty |
| 13 | V16 end card | Missing the bridge sentence |
