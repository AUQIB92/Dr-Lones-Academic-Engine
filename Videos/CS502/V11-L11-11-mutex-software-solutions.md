# V11 · L11 — Three Rules, Three Failures, One Fix: Peterson's Solution

> **Lecture:** CS502 L11 `11-mutex-software-solutions` · **Unit:** II · **Target length:** 12–14 min · **Series position:** 11/28
> **Format:** screen-capture animation (two-lane state machine) + whiteboard summary · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

Every broken mutual-exclusion attempt looks correct on paper; it only fails under one specific
interleaving that the student never thinks to try. A video can *play* that interleaving, freeze it
at the moment both processes are stuck (or both are inside), and then show Peterson's solution
surviving the very same schedule. Students who read the code statically tend to (a) not see why
strict alternation fails at all, since it never lets two processes in, and (b) memorise Peterson's
code without being able to say what `turn` is doing. The animated shared-variable panel
(`flag[0]`, `flag[1]`, `turn`) makes each line's effect visible.

## 2. Learning outcomes

After watching, the student can:
- [Remember] State the three requirements of a critical-section solution: mutual exclusion, progress, bounded waiting.
- [Analyze] For a proposed two-process entry/exit protocol, produce an interleaving that violates a named requirement.
- [Understand] Explain why strict alternation (a `turn` variable alone) satisfies mutual exclusion but violates progress.
- [Analyze] Explain why `flag[]` alone either deadlocks (set-then-test) or violates mutual exclusion (test-then-set).
- [Apply] Trace Peterson's solution through a given schedule, giving `flag[0]`, `flag[1]`, `turn` after every step, and say which process enters first.

## 3. Prerequisites

- Computer fundamentals: a program is a sequence of instructions; variables live in shared memory; a CPU runs one instruction at a time.
- V06 (States and context switch) and V09 (scheduling): the OS can switch between processes after any instruction (preemption). Recapped in scene 1b.
- V10 (race condition and critical section): `counter++` is not atomic; the entry/critical/exit/remainder template. Recapped in scene 1b.
- Comfort reading a C-style `while (cond) ;` loop (an empty loop that repeats until the condition becomes false); defined on screen in scene 4.
- No classroom lecture is assumed.

## 4. Hook (first 30–45 s)

Two students, one lab printer, and a whiteboard with a single word written on it: the name of
whose turn it is.

> "Asha and Bilal share a printer. Rule: you may print only when the whiteboard shows your name;
> when you finish, you write the other person's name. Two people can never print at once.
> Perfect, right?
> Asha prints, writes 'Bilal'. Bilal prints, writes 'Asha'. Now Bilal needs to print again
> and Asha has gone home for the weekend. The printer is free. Bilal waits until Monday.
> Nothing is wrong with mutual exclusion here. Something else is broken. Today we name it,
> and then we fix it with three lines of code."

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:45 | Hook | Cartoon printer, whiteboard showing "Asha" → "Bilal" → "Asha"; Bilal tapping his foot next to an idle printer; calendar flips to Monday | Turn-taking keeps them apart but wastes the printer; something besides mutual exclusion matters |
| 1b | 0:45–1:05 | Recap from V10 / V06 | Three 6 s cards: **critical section** (code that touches shared data); the four-part template *entry · critical · exit · remainder*; **preemption** (a switch can happen between any two lines, so we must assume the worst schedule). Small replay of the V10 lost-update timeline | Today's job: write the entry and exit sections using only ordinary shared variables |
| 2 | 1:05–2:00 | The three requirements | Three cards: **Mutual exclusion** (two processes never both in CS), **Progress** (if no one is in the CS, the choice of who enters next involves only processes that want to, and cannot be postponed forever), **Bounded waiting** (a limit on how many times others may enter after I ask, before I get in). Each gets a pictogram | Define each carefully; stress that progress is about *processes in their remainder section not blocking others*. Bounded waiting is a fairness bound, not a time bound |
| 3 | 2:00–2:30 | The test rig | Two lanes P0 and P1, each with a program-counter arrow; a shared-variable panel on the right; three status lamps (ME / P / BW) | From now on every attempt is run in this rig. The scheduler (us) picks which lane steps next |
| 4 | 2:30–4:00 | Attempt 1: `turn` alone | Code: `while (turn != i) ;  CS;  turn = j;`. Definition card: **busy waiting** (spinning) = repeatedly testing a condition in an empty loop, using the CPU while waiting. Animation: P0 enters (turn=0), exits writing turn=1; P1 enters, exits writing turn=0; P0 parks in a long remainder section (sleeping icon); P1 arrives at entry again: spins on `turn != 1` forever. ME lamp green, **Progress lamp red** | Exactly the hook. Mutual exclusion holds in all 16 reachable states (define **reachable state** in one line: every combination of program positions and variable values some schedule can produce), but a process in its remainder section (P0) is deciding who may enter. Strict alternation also forces A-B-A-B: a process that wants to run twice in a row cannot |
| 5 | 4:00–4:30 | Pause & predict 1 | Card: "Replace turn with flags. P_i sets flag[i] = true, then waits while flag[j]. What can go wrong?" | Pause. (Answer: both set their flags, then both wait forever.) |
| 6 | 4:30–5:40 | Attempt 2a: flags, set-then-test | Code: `flag[i] = true; while (flag[j]) ; CS; flag[i] = false;`. Animation: P0 sets flag[0]=T; **context switch**; P1 sets flag[1]=T; both lanes hit the `while` and spin; panel shows flag = [T, T] with a padlock icon; Progress lamp red (deadlock) | Two steps, and the system is stuck. Define **deadlock** on screen: every process is waiting for something only another waiting process can provide, so none moves. The model checker finds exactly one stuck state among 21: both at the loop with both flags true. Each is too polite |
| 7 | 5:40–6:40 | Attempt 2b: flags, test-then-set | Code: `while (flag[j]) ; flag[i] = true; CS; flag[i] = false;`. Animation: P0 tests flag[1] (F) → passes; switch; P1 tests flag[0] (F) → passes; P0 sets flag[0]; P1 sets flag[1]; **both arrows inside CS**, red flash; **ME lamp red** | Swapping two lines turns a deadlock into a collision. The test and the set are separated, which is the same flaw as load/store in V10 |
| 8 | 6:40–7:40 | Peterson's idea | Code builds line by line: `flag[i] = true; turn = j; while (flag[j] && turn == j) ; CS; flag[i] = false;`. Side label: "flag = I want in; turn = I let you go first" | Combine both: announce interest (flag), then politely give way (turn = j). Wait only if the other *wants in* and it *is the other's turn*. Whoever writes `turn` last is the one who waits |
| 9 | 7:40–9:30 | Peterson traced (worked example) | Rig replays the near-simultaneous arrival schedule from §6 step by step; panel values update each step; at step 6 P1's `while` glows amber "blocked"; P0 enters, exits; P1's condition turns false and it enters | Narrate each row of the trace table. Point: both arrived together, the tie was broken by `turn`, and the one who wrote `turn` last (P1 wrote turn = 0) yielded |
| 10 | 9:30–10:00 | Pause & predict 2 | Freeze at step 4 (flag=[T,T], turn=0). "Who enters?" | Pause. (Answer: P0, since P1 wrote turn last. P1's condition `flag[0] && turn==0` is true.) |
| 11 | 10:00–11:20 | Why all three lamps stay green | Three mini-arguments with the panel: (ME) both inside would need turn = 0 and turn = 1 at once; (Progress) if P_j is in its remainder, flag[j] = F so P_i walks in; (BW) after P_j exits and comes back, P_j sets turn = i, so P_j cannot pass P_i again: at most 1 entry. A badge: "checked: 42 reachable states, 0 violations" | Give the intuition, then say it was model-checked exhaustively: no state with both in CS, no stuck state, and the other process can enter at most once while you wait |
| 12 | 11:20–12:10 | Two tempting mistakes | Split screen: (a) `turn = j` written *before* `flag[i] = true`; (b) `turn = i` instead of `turn = j`. Both show a 6-step (a) / short (b) path ending with both arrows inside the CS | Order matters and "giving away" matters; both variants break mutual exclusion. Also flag: on modern CPUs that reorder memory writes, even correct Peterson needs memory barriers. That is why V12 moves to hardware instructions |
| 13 | 12:10–13:00 | Recap + bridge | Scorecard table fills: attempt × (ME, Progress, BW) ticks/crosses; next-video card for **V12** ("Hardware synchronization: test-and-set and compare-and-swap") | Requirements, three failures, one fix that only works for two processes and needs careful memory ordering. Bridge to **V12**: "What if the hardware gave us one instruction that tests and sets in a single atomic step? Next video." |

## 6. Worked example (verified)

**Peterson's solution for process `P_i` (j = 1 − i):**

```
L0: flag[i] = true;
L1: turn = j;
L2: while (flag[j] && turn == j) ;   // busy wait
L3: critical section
L4: flag[i] = false;
L5: remainder section
```

**Schedule (near-simultaneous arrival):** P0, P1, P0, P1, P0, P1, P0, P0, P0, P1, P1, P1.
Initial: flag[0] = F, flag[1] = F, turn = 0.

| Step | Who | Line executed | flag[0] | flag[1] | turn | Note |
|---|---|---|---|---|---|---|
| 1 | P0 | L0 | T | F | 0 | P0 wants in |
| 2 | P1 | L0 | T | T | 0 | P1 wants in |
| 3 | P0 | L1 | T | T | 1 | P0 gives way |
| 4 | P1 | L1 | T | T | 0 | P1 gives way (writes last) |
| 5 | P0 | L2 → pass | T | T | 0 | turn ≠ 1, P0 enters |
| 6 | P1 | L2 blocked | T | T | 0 | flag[0] ∧ turn = 0 |
| 7 | P0 | L3 (CS) | T | T | 0 | |
| 8 | P0 | L4 | F | T | 0 | P0 leaves |
| 9 | P0 | L5 | F | T | 0 | remainder |
| 10 | P1 | L2 → pass | F | T | 0 | flag[0] now F |
| 11 | P1 | L3 (CS) | F | T | 0 | |
| 12 | P1 | L4 | F | F | 0 | done |

**The three failed attempts, as shown on screen (process `P_i`, j = 1 − i):**

```
Attempt 1 (turn only)        Attempt 2a (set, then test)    Attempt 2b (test, then set)
shared int turn = 0;         shared bool flag[2] = {F, F};  shared bool flag[2] = {F, F};

while (turn != i) ;          flag[i] = true;                while (flag[j]) ;
  critical section           while (flag[j]) ;              flag[i] = true;
turn = j;                      critical section               critical section
  remainder section          flag[i] = false;               flag[i] = false;
                               remainder section              remainder section
```

**Strict-alternation progress failure, step by step (scene 4):**

| Step | Event | turn | P0 | P1 |
|---|---|---|---|---|
| 1 | P0 passes `while (turn != 0)` | 0 | in CS | remainder |
| 2 | P0 exits: `turn = 1` | 1 | remainder | remainder |
| 3 | P1 passes, runs CS, exits: `turn = 0` | 0 | remainder (long) | remainder |
| 4 | P1 wants in again: `while (turn != 1)` | 0 | remainder (long) | **spins** |

The CS is free, but P1 cannot enter until P0, which is in its remainder section, takes its turn.

**Flags set-then-test deadlock (scene 6):** P0 `flag[0] = true` → flags [T, F]; P1 `flag[1] = true` → [T, T]; both now spin on `while (flag[j])`.

**Flags test-then-set collision (scene 7):** P0 tests flag[1] = F, passes; P1 tests flag[0] = F, passes; P0 sets flag[0] = T; P1 sets flag[1] = T; both are in the CS.

**Failed attempts, with the violating schedule found by search:**

| Attempt | Reachable states | Mutual exclusion | Progress / deadlock | Counter-example |
|---|---|---|---|---|
| `turn` only (strict alternation) | 16 | holds (0 violations) | **violates progress** (P1 waits with turn = 0 while P0 is in its remainder; state is reachable) | P0 CS → turn = 1 → P1 CS → turn = 0 → P0 stays in remainder, P1 spins |
| `flag` set-then-test | 21 | holds | **deadlock** (1 stuck state: flag = [T, T], both at the loop) | P0: flag[0]=T; P1: flag[1]=T; both spin |
| `flag` test-then-set | 25 | **violated** | — | P0 tests (F); P1 tests (F); P0 sets; P1 sets; both in CS |
| Peterson | 42 | holds | holds (no stuck state; a waiting process is never blocked while the other is in its remainder) | — ; bounded waiting = 1 |
| Peterson with `turn = j` before `flag[i] = true` | — | **violated** | — | P0: turn=1; P1: turn=0; P1: flag[1]=T; P1 tests flag[0]=F → enters; P0: flag[0]=T; P0 tests turn==1? no (turn=0) → enters |
| Peterson with `turn = i` | — | **violated** | — | (found by search; show the path only if time permits) |

In the two "both waiting" states of Peterson (both at L2, flags [T, T]), exactly one may proceed:
turn = 0 lets P0 through, turn = 1 lets P1 through.

**How it was verified.** An explicit-state model checker (`mc.py`, function `v07`) explores every
interleaving of the two processes for each protocol (each line is one atomic step, processes loop
forever), and checks: (i) no reachable state has both in the CS; (ii) no reachable state where no
process can move; (iii) progress: no state where a process is stuck at L2 while the other is in
its remainder; (iv) bounded waiting: the maximum number of CS entries by the other process while
one waits at L2 (found = 1 for both processes). The trace table is the program's scripted run.
Script: `/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/mc.py`.

## 7. Pause-and-predict checkpoints

1. **(Scene 5, ~4:00)** Flags only, set then test: what goes wrong? → **Both set their flag, both spin forever (deadlock).**
2. **(Scene 10, ~9:30)** flag = [T, T], turn = 0, both at the `while`. Who enters? → **P0.** P1 wrote turn last, so P1 waits.
3. **(Scene 12, quick)** If each process writes `turn = i` (itself) instead of `turn = j`, which requirement breaks? → **Mutual exclusion.**

## 8. Misconceptions to address on screen

- "If two processes are never in the CS together, the solution is correct." → Scene 4: strict alternation passes mutual exclusion but fails progress; the red Progress lamp.
- "Progress means the program eventually finishes." → Scene 2 definition card; progress is about who participates in choosing the next entrant.
- "Bounded waiting means a time limit." → Scene 11: the bound is a *count* of entries by others (here 1).
- "`turn` in Peterson means whose turn it is to enter." → Label it "who must yield if both want in"; the trace shows P0 entering while turn = 0 because P1 set it.
- "The order of the first two lines doesn't matter." → Scene 12 path with `turn` written first.
- "Peterson works on any modern machine as written." → One sentence + caption in scene 12 about memory reordering; motivates V12.
- "Peterson extends to N processes by just adding flags." → Out of scope; say it is a two-process solution in this form.

## 9. Visual assets

- **Reuse:** `Slides/CS502/11-mutex-software-solutions.tex` is not built yet; when it is, reuse its Peterson code frame and the requirements definitions verbatim. Take the two-lane colours from V10 so the series looks the same; take block/colour semantics (`\good`/`\bad`) from `Slides/CS502/08-cpu-scheduling-basics.tex`.
- **New:**
  - Test rig: two lanes with PC arrows + shared-variable panel + three lamps (Motion Canvas recommended; each step is a keyframe).
  - Hook cartoon: printer, whiteboard name swap, calendar flip.
  - Final scorecard table (attempt × requirement) built cell by cell.
  - Optional: a 42-node state graph flash (render from `mc.py` with Graphviz) for the "checked" badge, shown for 2 seconds only.
- **Live demo (optional, 30 s):** `python3 mc.py` scrolling to the "Peterson: states 42 mutex viol 0 deadlocks 0" line, to show a model checker exists. Skip if over time.

## 10. On-screen text & notation

- Processes `P0`, `P1`; in generic code `P_i` and `j = 1 − i`.
- Shared variables in monospace: `flag[0]`, `flag[1]`, `turn`; values shown as T/F and 0/1.
- Requirements spelled exactly: **mutual exclusion**, **progress**, **bounded waiting**.
- Section names lowercase: entry section, critical section, exit section, remainder section.
- "busy waiting" / "spin" used for the `while (...) ;` loop; do not call it "blocking" (blocking queues arrive in V13).
- Definition cards on first use: **busy waiting**, **reachable state**, **deadlock**, **starvation** (waiting indefinitely while others proceed; used when explaining bounded waiting), **memory reordering** (scene 12, one line).
- Silberschatz names the attempts only in passing; call them Attempt 1 (strict alternation), Attempt 2a, Attempt 2b on screen.

## 11. End-of-video quiz

1. **(MCQ)** Strict alternation with a single shared `turn` variable violates which requirement?
   (a) mutual exclusion (b) progress (c) bounded waiting (d) none of them
   **Answer: (b).**
2. **(Trace, numerical)** Peterson, initial flag = [F, F], turn = 0. Schedule: P1 L0, P1 L1, P0 L0, P0 L1, then both test their `while`. Give `turn` and the process that enters first.
   **Answer:** after P1 L1 turn = 0, after P0 L1 turn = 1; P1 enters first (P0 wrote turn last and waits). *(Same rule as the worked example with roles swapped.)*
3. **(MCQ)** A protocol uses `while (flag[j]) ; flag[i] = true;` as the entry section. Which is possible?
   (a) both processes in the CS together (b) deadlock with both flags true (c) neither (d) both
   **Answer: (a).** The flag is set only after the test; in this variant a deadlock state is not reachable.
4. **(Numerical)** In Peterson's solution, while P0 waits at its `while` loop, at most how many times can P1 enter its critical section before P0 does? **Answer: 1.**
5. **(Short answer)** Why does Peterson's solution need *both* `flag` and `turn`? **Answer:** `flag` alone can deadlock or collide; `turn` alone forces alternation and fails progress; `flag` says who is interested, `turn` breaks the tie.

## 12. References

- Silberschatz, Galvin & Gagne, *Operating System Concepts*, 8th ed., Ch. 6 — the critical-section problem and its three requirements (p.227); Peterson's solution (p.229, Fig 6.2).
- Stallings, *Operating Systems: Internals and Design Principles*, 6th ed., Appendix A §A.1 — mutual exclusion: software approaches, including the successive attempts and Peterson's algorithm (p.745).
- Stallings 6e Ch. 5 §5.1 — principles of concurrency (p.207), for the requirement list in Stallings' wording.

## 13. Production notes

- **Pacing:** scenes 6–7 must feel fast (the failures take 2–4 steps); scene 9 must feel slow (12 steps, one click each). Speed contrast is the point.
- Keep the shared-variable panel in the same screen position across all attempts so viewers compare values by position.
- **Over length?** Drop the variant (b) half of scene 12 and checkpoint 3; keep variant (a), since it is the most common exam trap.
- **Under length?** Add the strict-alternation "A must alternate with B" demonstration: P0 wants in twice in a row and cannot.
- **Accessibility:** lamps use shape *and* colour (tick/cross icons, not only green/red); Okabe–Ito palette; captions on; code at ≥ 20 pt; announce lamp changes verbally.
- **Accuracy caution:** say "model-checked" only about the abstract model (each line atomic, sequentially consistent memory), not about real hardware.

### Scene-by-scene recording checklist

| Scene | Must be visible before narration starts | Common recording slip |
|---|---|---|
| 1b | V10 lost-update replay | Spending more than 20 s on recap |
| 2 | All three requirement cards | Defining progress as "the program finishes" |
| 4 | Lamps ME / P / BW and the `turn` value | Forgetting to show P0 *in its remainder* (sleeping icon) |
| 6–7 | Flag values T/F in the panel | Swapping the two attempts' names |
| 9 | Empty trace table, flags F/F, turn 0 | Updating `turn` and the PC arrow out of sync |
| 11 | "42 states, 0 violations" badge | Claiming it proves correctness on real hardware |
| 13 | V12 end card | Missing the bridge sentence |
