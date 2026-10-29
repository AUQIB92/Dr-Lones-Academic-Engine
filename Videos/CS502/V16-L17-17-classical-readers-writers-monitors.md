# V16 · L17 — Forks, Readers and Monitors: The Classical Problems, Part II

> **Lecture:** CS502 L17 `17-classical-readers-writers-monitors` · **Unit:** II · **Target length:** 14–16 min · **Series position:** 16/28
> **Format:** screen-capture animation (round table + reader/writer room) + short code · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

Both problems in this video fail through *timing*, not logic. Dining philosophers deadlocks only
when all five pick up their left fork in the same short window, and a whiteboard drawing of that
moment does not show how easily it happens. Readers–writers starves the writer only when readers
keep overlapping, and that is a pattern over time: the writer waits while the reader count rises
and falls but never reaches zero. Animation shows the fork-grab ripple around the table and a
`read_count` gauge that never empties, which makes the fixes (asymmetric order, at most four at the
table) and the starvation argument concrete. Monitors close the video as the language-level answer
to the semaphore-ordering errors seen in V15.

## 2. Learning outcomes

After watching, the student can:
- [Analyze] Produce the schedule that deadlocks the naive five-philosopher solution and name the circular wait.
- [Analyze] Explain why two fixes prevent deadlock: asymmetric fork order, and a semaphore limiting the table to four philosophers; and why a limit of five does not.
- [Apply] Trace the first readers–writers solution (`rw_mutex`, `mutex`, `read_count`) and give the values after each step.
- [Analyze] Construct an overlapping-reader schedule in which a writer waits indefinitely (writer starvation).
- [Understand] Describe a monitor: shared data + procedures with automatic mutual exclusion, and condition variables with `x.wait()`/`x.signal()`.

## 3. Prerequisites

- Computer fundamentals: memory, a program, a file that several programs may read.
- V06 (States and context switch): blocked processes. Recapped in scene 1b.
- V10 (critical section, mutual exclusion) and V13 (semaphores, `wait`/`signal`, value + queue). Recapped in scene 1b.
- V15 (bounded buffer): the "holding a lock while waiting" deadlock. Recapped in scene 1b.
- V09 (scheduling): the word **starvation** first appeared there (a process that never gets the CPU); redefined on screen here for synchronization.
- No classroom lecture is assumed.

## 4. Hook (first 30–45 s)

Five philosophers, a round table, five forks, one between each pair, and a bowl of noodles.

> "Each philosopher thinks, gets hungry, picks up the fork on the left, then the fork on the right,
> eats, and puts both down. Every philosopher follows the rules exactly. Watch: all five get hungry
> at the same moment. Left fork... left fork... left fork... left fork... left fork. Now everyone
> reaches right. There is no fork there. Nobody is wrong, nobody gives up, and nobody ever eats again.
> Today: why, two ways to fix it, and a second problem where *one* process waits forever while
> everyone else is perfectly happy."

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:45 | Hook | Top-down round table, P0–P4, forks c0–c4; the left-fork ripple; five hands reaching right; freeze; "∞" | Everyone obeys the rules and the system stops |
| 1b | 0:45–1:15 | Recap | Cards, ~6 s each: **semaphore** and `wait`/`signal` (V13); **binary semaphore as a lock** (V13); **deadlock** (V15: each process waits for something only another waiting process can release, the "holding the key while asleep" picture); **starvation** (new for synchronization: a process waits indefinitely even though the system as a whole keeps making progress) | Two failure modes today: deadlock (nobody moves) and starvation (one never moves) |
| 2 | 1:15–2:15 | The code | Each fork is a binary semaphore initialised to 1. Philosopher i: `wait(chopstick[i]); wait(chopstick[(i+1)%5]); eat; signal(chopstick[i]); signal(chopstick[(i+1)%5]);` Line highlights | Picking up a fork = `wait` on its semaphore. Mutual exclusion on each fork is guaranteed; two neighbours can never hold the same fork |
| 3 | 2:15–3:30 | The deadlock | Replay the hook slowly with semaphore values on each fork: all five `wait(left)` succeed (all forks 0); each `wait(right)` blocks; a dashed circle P0→c1→P1→c2→…→P4→c0→P0 is drawn: **circular wait** | Model checker: in the 392 reachable states there is exactly one deadlock, the state where every philosopher holds the left fork. Each holds one and waits for the next: the cycle is the deadlock |
| 4 | 3:30–4:00 | Pause & predict 1 | Card: "Fix idea: allow at most 4 philosophers at the table at once. Can it still deadlock?" | Pause. (Answer: no. With 4 people and 5 forks, at least one person can get both forks.) |
| 5 | 4:00–5:20 | Fix 1: at most four | A door semaphore `room = 4` at the table entrance; P4 waits outside; four left forks taken; c4 (P3's right fork) is still free, so P3 eats, releases, chain unwinds. Then a contrast clip with `room = 5`: the deadlock is back | Pigeonhole intuition: four people, five forks. Checker: 4,711 states, 0 deadlocks with room = 4; with room = 5 the single deadlock state reappears |
| 6 | 5:20–6:30 | Fix 2: asymmetric order | Even philosophers pick left then right, odd ones right then left; arrows on the table show the pick order; replay simultaneous hunger: two neighbours compete for the *same first* fork, one loses and waits *holding nothing* | Breaking the symmetry breaks the cycle: no circle of hold-and-wait can form. Checker: 393 states, 0 deadlocks. Variant: only P4 reversed also gives 0 deadlocks. Mention the third Silberschatz fix (pick up both forks only if both are free, inside a critical section) in one line; it returns as the monitor solution |
| 7 | 6:30–7:00 | Deadlock-free is not the whole story | Caption + one-liner: "deadlock-free does not mean starvation-free"; at most 2 philosophers eat at once (checker: max 2 simultaneous eaters) | Fairness is a separate property; the next problem shows starvation without any deadlock |
| 8 | 7:00–8:20 | Readers–writers setup | A shared file icon; reader icons (eye) and writer icons (pen). Rules on screen: many readers together OK; a writer needs the file alone. Code of the **first readers–writers** solution: `rw_mutex = 1`, `mutex = 1`, `read_count = 0` (§6) | First reader in locks out writers (`wait(rw_mutex)`); last reader out lets them in (`signal(rw_mutex)`); `mutex` only protects `read_count` |
| 9 | 8:20–10:00 | Starvation trace | The scripted trace (§6): R1 enters (read_count 1, rw_mutex 0); W arrives and blocks; R2 enters (2); R1 leaves (1); R1 re-enters (2); R2 leaves (1); R2 re-enters (2); W still blocked. `read_count` gauge oscillates 1–2, never 0; W's wait timer keeps growing | Readers overlap like a relay team handing over a baton; the count never reaches zero, so `rw_mutex` is never released. W is starved, yet readers make progress, so it is not deadlock. Checker confirms safety: no reader/writer overlap in any state (2R1W: 85 states; 3R2W: 412 states), up to 3 readers at once |
| 10 | 10:00–10:30 | Pause & predict 2 | Freeze in scene 9 with read_count = 1, R2 reading, R1 about to re-enter. "If R1 waits until R2 has left, what happens to W?" | Pause. (Answer: read_count reaches 0, the last reader signals `rw_mutex`, W enters.) |
| 11 | 10:30–11:20 | The other variant | Two-column card: first variant (readers preferred, writers may starve) vs. second variant (writers preferred, readers may starve); Linux kernel has reader–writer locks too | Name only; the code of the second variant is out of scope for this video |
| 12 | 11:20–13:20 | Monitors | V15's swapped-wait bug flashes as motivation. Monitor diagram: a box with shared data, procedures, an entry queue; "only one process active inside at a time" stamp. Condition variable `x` with its own queue; `x.wait()` (suspend, release the monitor) and `x.signal()` (resume exactly one waiter; **if none waits, nothing happens**). Dining-philosophers monitor sketch: `state[i] ∈ {THINKING, HUNGRY, EATING}`, `pickup(i)`, `putdown(i)`, `test(i)` | The compiler inserts the lock, so the programmer cannot forget or misorder it. Contrast with semaphore `signal`, which is remembered in the value. The monitor solution is deadlock-free but can still starve a philosopher |
| 13 | 13:20–14:10 | Pause & predict 3 | Card: "A monitor process calls `x.signal()` and no process is waiting on x. Then a process calls `x.wait()`. Does it block?" | Pause. (Answer: yes, the signal was not stored.) |
| 14 | 14:10–15:10 | Recap + bridge | Three-column card: dining philosophers (deadlock, 2 fixes), readers–writers (starvation), monitors (structure). End card for **V17** | Bridge to **V17**: "Twice today a circle of waiting froze everything. Next video turns that circle into a graph and names the four conditions that every deadlock needs, so we can break one of them on purpose." |

## 6. Worked example (verified)

### A. Dining philosophers (N = 5)

Philosopher `Pi`, forks `c i` (left) and `c (i+1) mod 5` (right), each a binary semaphore = 1.

**Deadlock schedule:** P0 `wait(c0)`, P1 `wait(c1)`, P2 `wait(c2)`, P3 `wait(c3)`, P4 `wait(c4)` →
all fork semaphores 0; now every `wait(right)` blocks. Wait-for cycle:
P0 → c1 (held by P1) → P1 → c2 → P2 → c3 → P3 → c4 → P4 → c0 → P0.

| Variant | Reachable states | Deadlock states |
|---|---|---|
| Naive: all left-then-right | 392 | **1** (all five holding their left fork) |
| Asymmetric: even L-first, odd R-first | 393 | 0 |
| Only P4 right-first | — | 0 |
| Room semaphore = 4 (at most four at the table) | 4,711 | 0 |
| Room semaphore = 5 (no real limit) | — | 1 |
| Maximum philosophers eating at once (asymmetric; also room = 4) | — | 2 |

### B. First readers–writers solution

```
semaphore rw_mutex = 1, mutex = 1;  int read_count = 0;

writer:                       reader:
  wait(rw_mutex);               wait(mutex);
  /* write */                   read_count++;
  signal(rw_mutex);             if (read_count == 1) wait(rw_mutex);
                                signal(mutex);
                                /* read */
                                wait(mutex);
                                read_count--;
                                if (read_count == 0) signal(rw_mutex);
                                signal(mutex);
```

**Starvation trace** (R1, R2 readers; W writer):

| # | Event | rw_mutex | read_count | Readers reading | W |
|---|---|---|---|---|---|
| 0 | start | 1 | 0 | 0 | — |
| 1 | R1 enters (first reader takes rw_mutex) | 0 | 1 | 1 | — |
| 2 | W: wait(rw_mutex) | 0 | 1 | 1 | **blocked** |
| 3 | R2 enters | 0 | 2 | 2 | blocked |
| 4 | R1 leaves | 0 | 1 | 1 | blocked |
| 5 | R1 enters again | 0 | 2 | 2 | blocked |
| 6 | R2 leaves | 0 | 1 | 1 | blocked |
| 7 | R2 enters again | 0 | 2 | 2 | blocked |
| … | the pattern repeats | 0 | 1↔2 | ≥ 1 | blocked forever |

`read_count` never returns to 0, so no reader executes `signal(rw_mutex)`. The writer starves; the
readers keep making progress, so this is not deadlock.

**Safety checks:** with 2 readers + 1 writer (85 states) and 3 readers + 2 writers (412 states):
0 deadlocks, never a writer together with a reader, never two writers; up to 3 readers read at once.

### C. Monitor sketch for scene 12 (conceptual, no numbers)

```
monitor DiningPhilosophers {
  enum { THINKING, HUNGRY, EATING } state[5];
  condition self[5];

  void pickup(int i) {
    state[i] = HUNGRY;
    test(i);
    if (state[i] != EATING) self[i].wait();
  }
  void putdown(int i) {
    state[i] = THINKING;
    test((i + 4) % 5);      // left neighbour
    test((i + 1) % 5);      // right neighbour
  }
  void test(int i) {
    if (state[(i + 4) % 5] != EATING && state[i] == HUNGRY
        && state[(i + 1) % 5] != EATING) {
      state[i] = EATING;
      self[i].signal();
    }
  }
  initialization_code() { for (int i = 0; i < 5; i++) state[i] = THINKING; }
}
```

Narration points for the sketch: a philosopher eats only if neither neighbour is eating, so both
forks are taken together and no hold-and-wait can occur; the monitor's built-in lock means
`pickup` and `putdown` never interleave. This follows the structure of the textbook's monitor
solution; it was not model-checked for this brief, so do not attach numbers to it.

**How it was verified.** `mc.py` (function `v10`) model-checks every interleaving of the
philosopher programs (each `wait`/`signal` one atomic step) and of the readers–writers program
above (each line one atomic step, the `if … wait(rw_mutex)` modelled as one step that blocks when
`rw_mutex` = 0), reporting reachable states, deadlocks and reader/writer overlap. The starvation
table is the program's scripted run, with each "enters"/"leaves" row standing for that reader's
5-line entry or exit code. The monitor material is conceptual (no numbers). Files in
`/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/`.

## 7. Pause-and-predict checkpoints

1. **(Scene 4, ~3:30)** At most four philosophers at the table: can it deadlock? → **No** (4 people, 5 forks; someone gets two).
2. **(Scene 10, ~10:00)** If R1 waits for R2 to leave before re-entering, what happens to W? → **read_count hits 0, rw_mutex is signalled, W writes.**
3. **(Scene 13, ~13:20)** `x.signal()` with no waiter, then `x.wait()`: block? → **Yes.** A monitor signal is lost if no one waits (unlike a semaphore's).

## 8. Misconceptions to address on screen

- "Deadlock means someone broke the rules." → Scene 3: every philosopher follows the code exactly; the circle forms anyway.
- "Deadlock and starvation are the same thing." → Scene 1b definitions + scene 9: readers progress while W starves.
- "Giving each fork its own semaphore makes the solution correct." → It gives mutual exclusion per fork, but not freedom from deadlock (scene 2 vs. scene 3).
- "Limiting the table to five fixes it" or "any limit works". → Scene 5 contrast clip: 5 deadlocks, 4 does not.
- "Readers–writers first variant is unsafe." → It is safe (no overlap in any state); its problem is fairness.
- "`x.signal()` in a monitor works like `signal(S)` on a semaphore." → Scene 13: the condition variable has no memory.
- "A monitor solution cannot starve." → Scene 12 caption: deadlock-free, but a philosopher can still starve.

## 9. Visual assets

- **Reuse:** `Slides/CS502/17-classical-readers-writers-monitors.tex` is not built; when it is, reuse its round-table TikZ (Silberschatz Fig 6.13 style) and the monitor diagram. Semaphore box from V13, wait-for ring from V15, colour roles from `Slides/CS502/08-cpu-scheduling-basics.tex`.
- **New:**
  - Round table with numbered philosophers and forks; each fork shows its semaphore value (1/0).
  - Arrows for pick-up order (asymmetric fix); the room/door semaphore for fix 1.
  - Reader–writer room with eye/pen icons, `read_count` gauge, writer wait timer.
  - Monitor box: entry queue, single-occupancy stamp, condition-variable queues.
- **Live demo:** none; this video is pure animation. (Optional: show the `mc.py` line "naive left-then-right: states 392 deadlocks 1" for 3 s.)

## 10. On-screen text & notation

- Philosophers `P0`–`P4`; forks `c0`–`c4` with `chopstick[i]` in code (Silberschatz's name); say "fork" or "chopstick" consistently, chosen once.
- Semaphores `rw_mutex`, `mutex`; integer `read_count`; `room` for the four-at-the-table fix.
- `wait(S)` / `signal(S)` for semaphores; `x.wait()` / `x.signal()` for monitor condition variables. Say explicitly that these are different operations with the same names.
- Definitions on screen at first use: **deadlock**, **starvation**, **circular wait**, **reader**, **writer**, **monitor**, **condition variable**.
- Monitor states: `THINKING`, `HUNGRY`, `EATING`.

## 11. End-of-video quiz

1. **(MCQ)** Five philosophers, each picking left then right. Which change guarantees no deadlock?
   (a) a semaphore allowing at most 5 at the table (b) philosopher P4 picks right then left, the rest left then right (c) each fork becomes a counting semaphore initialised to 2 (d) philosophers think for random times
   **Answer: (b).** (a) is the same as no limit; (c) breaks mutual exclusion on forks; (d) only makes deadlock less likely.
2. **(Numerical)** With the "at most four at the table" fix and N = 5, what is the maximum number of philosophers who can be *eating* at the same time? **Answer: 2.** (Each eater needs two of the 5 forks, so 3 eaters would need 6. Model-checked: with room = 4, the maximum over all 4,711 reachable states is 2.)
3. **(Trace)** First readers–writers solution, start state. R1 enters, R2 enters, W calls `wait(rw_mutex)`, R1 leaves. Give `read_count`, `rw_mutex`, and W's state. **Answer: read_count 1, rw_mutex 0, W blocked.**
4. **(MCQ)** In the first readers–writers solution, which process can starve? (a) a reader (b) a writer (c) both (d) neither **Answer: (b).**
5. **(MCQ)** In a monitor, a process executes `x.signal()` when no process is suspended on `x`. The effect is: (a) the next `x.wait()` returns immediately (b) the signal is queued (c) nothing (d) deadlock **Answer: (c).**

## 12. References

- Silberschatz, Galvin & Gagne, *Operating System Concepts*, 8th ed., Ch. 6 — classic problems of synchronization: readers–writers and dining philosophers (p.239, Fig 6.13); monitors (p.244).
- Stallings, *Operating Systems: Internals and Design Principles*, 6th ed., Ch. 5 §5.4 — monitors (p.232); §5.6 — readers/writers problem (p.245); Ch. 6 §6.6 — dining philosophers problem (p.282).
- Bovet & Cesati, *Understanding the Linux Kernel*, 3rd ed., Ch. 5 (pp.189–226) — read/write spin locks, seqlocks, RCU, for students who want the real-kernel versions.

## 13. Production notes

- **Length risk:** this is the longest brief. Three topics; keep each in its own act with a section card (Dining philosophers / Readers–writers / Monitors).
- **Over length?** Shorten scene 11 to a single caption and drop pause 3 (keep the misconception line in scene 12). Do not cut scene 9: the starvation animation is the reason for the video.
- **Under length?** Animate the Silberschatz monitor `test(i)` logic for one philosopher.
- Make the fork-grab ripple in scenes 1 and 3 identical so viewers recognise the replay.
- Quiz 2: the "max 2 eating" figure was model-checked for both the room = 4 fix and the asymmetric fix.
- **Accessibility:** philosophers numbered, forks labelled; holding shown by position and an outline, not colour alone; Okabe–Ito palette; captions on.

### Scene-by-scene recording checklist

| Scene | Must be visible before narration starts | Common recording slip |
|---|---|---|
| 1, 3 | All five forks labelled with value 1 | Ripple too fast to count; use 0.6 s per philosopher |
| 1b | Four definition cards | Reading the cards word for word; paraphrase |
| 2 | Fork semaphore values | Calling forks "locks" and "semaphores" interchangeably without saying they are binary semaphores |
| 5 | Door with `room` value | Forgetting to show `room = 5` contrast |
| 6 | Pick-order arrows on every seat | Mislabelling which philosophers are odd/even |
| 8–9 | `rw_mutex`, `mutex`, `read_count` gauges | Letting `read_count` touch 0 in the animation (it must not) |
| 12 | Entry queue and condition queues as separate lanes | Mixing up semaphore `signal` and condition `x.signal()` |
| 14 | V17 end card | Missing the bridge sentence |
