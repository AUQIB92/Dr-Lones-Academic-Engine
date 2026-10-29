# V13 · L13 — Semaphores: A Counter With a Waiting Room

> **Lecture:** CS502 L13 `13-semaphores` · **Unit:** II · **Target length:** 11–13 min · **Series position:** 13/28
> **Format:** screen-capture animation (semaphore box + queue) + short code segment · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

A semaphore is two things moving at once: an integer that goes up and down, and a queue of
blocked processes that fills and drains. On a slide students see `wait` and `signal` as two short
code blocks and lose the link between them: that a negative value counts the waiting processes,
that `signal` wakes exactly one of them, and that the woken process does *not* decrement again. An
animation that shows the value and the queue changing together on every call makes that
connection, and it separates the busy-waiting definition from the blocking implementation, which
textbooks present one after the other.

## 2. Learning outcomes

After watching, the student can:
- [Remember] State the definitions of `wait(S)` and `signal(S)` and that each must execute atomically.
- [Understand] Distinguish a counting semaphore (value range unrestricted) from a binary semaphore (0 or 1), and give one use of each.
- [Apply] Trace a sequence of `wait`/`signal` calls on a blocking semaphore, giving the value and the waiting-queue contents after each call.
- [Analyze] Interpret a negative semaphore value as the number of blocked processes, and compute the final value after n waits and m signals.
- [Apply] Use a semaphore initialised to 0 to force statement S1 in `P1` to run before S2 in `P2`.

## 3. Prerequisites

- Computer fundamentals: CPU, memory, a program as a sequence of instructions.
- V06 (States and context switch): process states `ready`, `running`, `waiting` (blocked) and the queues the OS keeps. Recapped in scene 1b.
- V10 (race condition, critical section) and V11 (busy waiting, Peterson's solution). Recapped in scene 1b.
- V12 (Hardware sync: test-and-set / compare-and-swap): used to explain how `wait`/`signal` are themselves made atomic (scene 12). Recapped in one line there.
- No classroom lecture is assumed.

## 4. Hook (first 30–45 s)

A university computer lab with exactly two free PCs and a door attendant holding a clicker counter.

> "The lab has two free PCs. The attendant starts the counter at 2. Each student who walks in
> clicks it down. Asha: 1. Bilal: 0. Chen arrives: the counter would go to minus one, so Chen
> sits on the bench outside. Divya arrives: minus two, second on the bench.
> Question: the counter says minus two. What does that number *tell* the attendant?
> Hold that thought. That attendant is a semaphore, and by the end of this video you will be able
> to run one in your head."

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:45 | Hook | Lab door, attendant with counter "2"; four student avatars arrive; counter ticks 1, 0, −1, −2; two sit on a bench | Two PCs, four students; the counter goes negative; what does −2 mean? |
| 1b | 0:45–1:05 | Recap | Cards, ~6 s each: **critical section** and **mutual exclusion** (V10); **busy waiting** (V11: a loop that burns CPU while waiting); **blocked / waiting state** (V06: a process that cannot run is parked by the OS and uses no CPU until woken); **atomic** (one indivisible step, V10) | Today we replace spinning with blocking |
| 2 | 1:05–1:45 | From spin locks to semaphores | Peterson's `while` loop from V11 with a "CPU burning" flame; then a box labelled `S` with an integer inside | Peterson (V11) and test-and-set (V12) work but spin. Dijkstra's semaphore: an integer accessed only by two atomic operations. Mention `P`/`V` once as the historical names; use `wait`/`signal` from here on |
| 3 | 1:45–3:00 | Definition 1: busy-waiting form | Code: `wait(S) { while (S <= 0) ; S--; }`  `signal(S) { S++; }`. Box `S` animates: value 1 → 0 on wait; a second wait spins (flame icon) | The textbook's first definition. The test and the decrement must be indivisible, otherwise we are back to V10. Value never goes below 0 in this form |
| 4 | 3:00–4:30 | Definition 2: blocking form | Box `S` now has two compartments: `value` and a `list` (queue). Code: `wait: value--; if (value < 0) { add me to list; block(); }`  `signal: value++; if (value <= 0) { remove P from list; wakeup(P); }` | Instead of spinning, the process puts itself to sleep in the semaphore's own queue (state → waiting). `signal` moves one sleeper back to ready. Now the value *can* be negative |
| 5 | 4:30–6:30 | The animated trace (worked example) | Box `S` value = 2, empty queue; four process tokens P1–P4. Each call animates: value changes, token moves into CS area or into queue, or out of queue into CS. Trace table on the right fills row by row (§6) | Narrate all eight calls. Key moments: P3's wait takes value to −1 and P3 joins the queue; P1's signal takes value to −1, *not* 0, and P3 leaves the queue and goes straight into the CS without decrementing again |
| 6 | 6:30–7:00 | Pause & predict 1 | Freeze after row 5 (value −1, queue [P4]). "P2 now calls signal. Value? Queue? Who runs?" | Pause. (Answer: value 0, queue empty, P4 is woken and enters.) |
| 7 | 7:00–7:40 | Reading the value | Number line: positive = free units; 0 = none free, nobody waiting; negative = −(number waiting) | Back to the hook: −2 meant two students on the bench. This reading holds for the blocking form only |
| 8 | 7:40–8:50 | Binary semaphore as a lock | Code: `mutex = 1; do { wait(mutex); CS; signal(mutex); remainder } while (true);`. Two processes; second one goes into the queue; on signal, it moves to the CS | A semaphore initialised to 1 is a mutual-exclusion lock. Contrast counting semaphore (initialised to N: N identical resources, e.g. two lab PCs) with binary (0 or 1). Silberschatz: binary semaphores are also called mutex locks on some systems |
| 9 | 8:50–9:50 | Semaphore for ordering | Two lanes: P1 runs `S1; signal(synch);` and P2 runs `wait(synch); S2;` with `synch = 0`. Variant A: P2 arrives first → value −1, P2 blocks; P1 runs S1, signals → value 0, P2 wakes and runs S2. Variant B: P1 first → value 1; P2's wait → 0, no block | Initialising to 0 turns a semaphore into a "wait for event" signal. Either arrival order gives S1 before S2 |
| 10 | 9:50–10:20 | Pause & predict 2 | Card: "S = 7. Processes perform 20 wait and 15 signal operations (any order, all complete). Final value?" | Pause. (Answer: 7 − 20 + 15 = 2.) Each wait is −1 and each signal +1 regardless of blocking |
| 11 | 10:20–11:20 | What can go wrong | Three quick vignettes: (a) a process forgets `signal(mutex)` → queue grows forever; (b) `signal` then `wait` swapped → two in CS; (c) two semaphores S, Q, P0 waits S then Q, P1 waits Q then S → both in queues of each other (deadlock picture) | Semaphores are powerful but unchecked; mistakes are easy and silent. (c) previews **deadlock** (define on screen: each process waits for something only another waiting process can release; treated fully in V17) and the classical problems (V15, V16) |
| 12 | 11:20–12:00 | Atomicity of wait/signal itself | 10 s recap card of V12: **test-and-set** reads a lock variable and sets it to true in one atomic instruction. Then box `S` with a small lock icon; caption "single CPU: disable interrupts; multiprocessor: spin lock / test-and-set around wait and signal" | The busy-waiting did not disappear completely; it moved to a few instructions inside wait/signal, so it is short |
| 13 | 12:00–12:40 | Recap + bridge | Summary cards; end card for **V14** (Semaphore patterns and inter-process communication) | Value + queue; negative means waiters; 1 = lock, 0 = ordering, N = pool. Bridge to **V14**: "Semaphores coordinate processes that share memory. What if they share nothing? Next video: semaphore patterns, then processes talking by passing messages." Mention that V15 will combine three semaphores in the bounded buffer |

## 6. Worked example (verified)

**Blocking semaphore, Silberschatz form.**

```
wait(S):   S.value--;  if (S.value < 0) { add this process to S.list; block(); }
signal(S): S.value++;  if (S.value <= 0) { remove a process P from S.list; wakeup(P); }
```

Queue is FIFO. Initial `S.value = 2` (two lab PCs), empty queue. Calls in this order:

| # | Call | value after | queue after | Effect |
|---|---|---|---|---|
| 0 | (start) | 2 | [ ] | |
| 1 | P1 wait | 1 | [ ] | P1 enters |
| 2 | P2 wait | 0 | [ ] | P2 enters |
| 3 | P3 wait | −1 | [P3] | P3 blocks |
| 4 | P4 wait | −2 | [P3, P4] | P4 blocks |
| 5 | P1 signal | −1 | [P4] | P1 leaves; **P3 woken**, enters |
| 6 | P2 signal | 0 | [ ] | P2 leaves; **P4 woken**, enters |
| 7 | P3 signal | 1 | [ ] | P3 leaves, nobody to wake |
| 8 | P4 signal | 2 | [ ] | P4 leaves; back to start |

At every row, the number of processes in the "CS area" is at most 2, and whenever value < 0 the
queue length equals −value (rows 3, 4, 5).

**Ordering example.** `synch = 0`. If P2 calls `wait(synch)` first: value −1, P2 blocked; P1 runs
S1 and calls `signal(synch)`: value 0, P2 woken, runs S2. If P1 signals first: value 1; P2's wait
takes it to 0 without blocking.

**Binary semaphore as a lock (scene 8), FIFO queue, `mutex = 1`:**

| # | Call | value after | queue after | Who is in the CS |
|---|---|---|---|---|
| 0 | start | 1 | [ ] | nobody |
| 1 | P1 wait | 0 | [ ] | P1 |
| 2 | P2 wait | −1 | [P2] | P1 |
| 3 | P1 signal | 0 | [ ] | P2 (woken) |
| 4 | P2 signal | 1 | [ ] | nobody |

(Same rules as the main trace with initial value 1; each row follows from the two definitions above.)

**Ordering pattern (scene 9), both arrival orders:**

| Order | Step | synch after | P2 state |
|---|---|---|---|
| P2 first | P2: wait(synch) | −1 | blocked |
| | P1: S1; signal(synch) | 0 | woken, runs S2 |
| P1 first | P1: S1; signal(synch) | 1 | not yet arrived |
| | P2: wait(synch) | 0 | passes, runs S2 |

**Checkpoint arithmetic.** S = 7, 20 waits, 15 signals: 7 − 20 + 15 = **2** (no process left
blocked). S = 3, 5 waits then 2 signals: after the waits value −2 with 2 blocked; after the
signals value **0** with both woken and an empty queue.

**How it was verified.** The trace table is the output of a scripted semaphore simulator
(`mc.py`, function `v08`) that implements the two operations above with a FIFO queue and records
value, queue and process states after every call. The checkpoint and quiz arithmetic was run in
`quiz.py`, which counts blocked processes as it goes. Both are in
`/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/`.

## 7. Pause-and-predict checkpoints

1. **(Scene 6, ~6:30)** value −1, queue [P4]. P2 signals. → **value 0, queue empty, P4 woken and enters.**
2. **(Scene 10, ~9:50)** S = 7; 20 waits and 15 signals complete. Final value? → **2.**
3. **(Scene 9, quick)** In the ordering pattern, why must `synch` start at 0 and not 1? → **With 1, P2's wait would pass immediately and S2 could run before S1.**

## 8. Misconceptions to address on screen

- "A woken process decrements the semaphore again." → Scene 5 row 5: value goes −2 → −1 on P1's signal and P3 enters with no second decrement; caption "already counted".
- "A semaphore value can never be negative." → Scene 3 vs scene 4: true for the busy-waiting definition, false for the blocking implementation; label which definition is on screen at all times.
- "`signal` with nobody waiting is lost." → Row 7: the value rises to 1, so a later `wait` passes. (Contrast with monitor condition variables in V16, where a signal with no waiter *is* lost.)
- "Binary semaphore and counting semaphore are different mechanisms." → Same operations; only the initial value and intended range differ.
- "Semaphores remove busy waiting entirely." → Scene 12: short busy-waiting remains inside `wait`/`signal` on multiprocessors.
- "Wait means wait for a fixed time." → Name: wait for the value to be positive; the process blocks, not sleeps on a timer.

## 9. Visual assets

- **Reuse:** `Slides/CS502/13-semaphores.tex` is not built; when it is, reuse its wait/signal code frame and the trace table. Visual grammar (block/exampleblock colour roles) from `Slides/CS502/08-cpu-scheduling-basics.tex`.
- **New:**
  - The semaphore box: value compartment + FIFO queue lane; tokens P1–P4 that slide between "outside", "CS area", "queue" (Manim or PowerPoint morph; one keyframe per call).
  - Hook: lab door, attendant clicker, bench.
  - Number line for value interpretation (scene 7).
  - Deadlock vignette with S and Q (scene 11), reusable later in V16 and V17.
- **Live demo (optional, 45 s):** POSIX semaphore with two "PC slots":
  ```
  # compile a 4-thread program using sem_init(&s, 0, 2), sem_wait, sleep(1), sem_post
  gcc -pthread lab.c -o lab && ./lab
  ```
  Expected behaviour: two threads print "entered" immediately, the other two about one second later. Note that on macOS unnamed `sem_init` is not supported; record on Linux or use named `sem_open`. (Not pre-run for this brief; run and check on the recording machine before using it.)

## 10. On-screen text & notation

- **`wait(S)` / `signal(S)`** throughout; mention **`P`/`V`** once in scene 2 only (course notation rule: never mix them on the same screen).
- Semaphore fields: `S.value`, `S.list`; operations `block()` and `wakeup(P)`.
- Process names `P1`, `P2`, `P3`, `P4` (capital P, no spaces).
- **counting semaphore** (unrestricted range); **binary semaphore** (0/1); **mutex** as the conventional name of a binary semaphore used for mutual exclusion.
- States: `ready`, `running`, `waiting` (blocked) as in V05–V06.
- Always label which definition is on screen: "busy-waiting definition" vs. "blocking implementation".

## 11. End-of-video quiz

1. **(Numerical)** A counting semaphore starts at 7. Processes complete 20 `wait` and 15 `signal` operations. What is its final value? **Answer: 2.**
2. **(Numerical)** A blocking semaphore S starts at 3. Five processes call `wait(S)`, none signals. How many processes are blocked and what is `S.value`? **Answer: 2 blocked; S.value = −2.**
3. **(MCQ)** With the blocking implementation, `S.value = −1` and the queue holds `P5`. A process calls `signal(S)`. Which is correct afterwards?
   (a) value 0, P5 still blocked (b) value 0, P5 ready (c) value −1, P5 ready (d) value 1, P5 ready
   **Answer: (b).**
4. **(MCQ)** To ensure statement `X` in `P1` executes before statement `Y` in `P2`, using semaphore `T`:
   (a) T = 1; P1: `X; signal(T)`; P2: `wait(T); Y` (b) T = 0; P1: `X; signal(T)`; P2: `wait(T); Y` (c) T = 0; P1: `wait(T); X`; P2: `Y; signal(T)` (d) T = 1; P1: `wait(T); X`; P2: `signal(T); Y`
   **Answer: (b).** (c) forces the opposite order; (a) and (d) allow Y first.
5. **(Short answer)** Why must `wait` and `signal` be atomic? **Answer:** they read-modify-write the shared value; interleaving them would produce the same lost update as `counter++` in V10.

## 12. References

- Silberschatz, Galvin & Gagne, *Operating System Concepts*, 8th ed., Ch. 6 — semaphores: usage, implementation, counting vs. binary (p.234); synchronization hardware used to make them atomic (p.231).
- Stallings, *Operating Systems: Internals and Design Principles*, 6th ed., Ch. 5 §5.3 — semaphores (p.219).
- Bovet & Cesati, *Understanding the Linux Kernel*, 3rd ed., Ch. 5 (pp.189–226) — Linux kernel semaphores and spin locks, optional depth.

## 13. Production notes

- **Pacing:** scene 5 is the core; one call per click, pause on rows 3, 5 and 6. Read the value aloud before animating the queue so viewers predict.
- Keep the box and queue in the same place for scenes 4–9.
- Remove the busy-waiting definition from the screen once scene 4 starts, or keep it greyed and labelled, so viewers do not mix the two.
- **Over length?** Cut scene 11's vignette (b) and scene 12 to one caption. Keep scene 9 (ordering); it is reused in V15.
- **Under length?** Add a three-PC variant (initial value 3) as a viewer exercise.
- **Accessibility:** tokens carry text labels (P1…P4), not only colours; queue order shown left→right with arrows; Okabe–Ito palette; captions on.
- **Do not** show the optional live demo unless it was run on the recording machine; the brief's numbers come from the simulator, not from a pthread run.

### Scene-by-scene recording checklist

| Scene | Must be visible before narration starts | Common recording slip |
|---|---|---|
| 1b | Four recap cards | Saying "P and V" here (save it for scene 2) |
| 3 | Label "busy-waiting definition" | Showing a negative value in this form |
| 4 | Label "blocking implementation"; value and list compartments | Forgetting `<= 0` (not `< 0`) in `signal` |
| 5 | All four tokens outside, value 2 | Decrementing again when P3 wakes |
| 8 | Two tokens, `mutex = 1` | Calling it a "mutex" before defining the word |
| 9 | Both lanes and `synch = 0` | Showing only one arrival order |
| 12 | V12 recap card | Implying semaphores remove all spinning |
| 13 | V14 end card | Missing the bridge sentence |
