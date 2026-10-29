# V18 · L19 — The Banker's Algorithm: Saying "Not Yet" to Stay Safe

> **Lecture:** CS502 L19 `19-deadlock-handling` · **Unit:** II · **Target length:** 13–16 min · **Series position:** 18/28
> **Format:** screen-capture animation (matrix animation) with a short whiteboard segment · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

The Banker's algorithm is a *process*, not a table. On a slide students see four finished matrices with a safe sequence printed underneath. They never see the `Work` vector grow column by column, the scan pointer skip a process and come back to it later, or a request being pretended into the state and then rolled back. Two errors follow from reading only the static version. Students treat "unsafe" as if it meant "deadlocked", and they check a request against `Available` without running the safety test. An animation in which `Work` visibly accumulates and a pretend-allocation visibly rolls back corrects both.

## 2. Learning outcomes

After watching, the student can:
- [Apply] Compute the `Need` matrix and the `Available` vector from `Max`, `Allocation`, and the total resource vector.
- [Apply] Run the safety algorithm step by step on a 5-process, 3-resource-type state and report a safe sequence or declare the state unsafe.
- [Apply] Run the resource-request algorithm (three checks: `Request ≤ Need`, `Request ≤ Available`, pretend-and-test safety) and decide grant / wait.
- [Analyze] Distinguish *unsafe* from *deadlocked*, and explain why avoidance is more conservative than detection.
- [Understand] Place prevention, avoidance, and detection on a "how much do we know in advance / how much do we restrict" spectrum.

## 3. Prerequisites

- **Computer fundamentals:** a program runs on a CPU and uses devices (printers, drives); a running program can be paused and resumed.
- **V05 Process:** a process is a running program; it can be *waiting* (blocked) until something it needs becomes available.
- **V17 Deadlock basics:** resources and instances, request–use–release, the resource-allocation graph, the four necessary conditions. Recapped on screen in scene 2 (20 s).
- No classroom lecture is assumed. Vector comparison (`X ≤ Y` component-wise) is defined on screen in scene 5.

## 4. Hook (first 30–45 s)

> *A bank has 10 printers, 6 scanners, and 8 tape drives, and five long-running jobs have already borrowed some of them. Job P4 asks for 3 printers and 2 scanners, and right now the bank has 3 printers and 3 scanners free, so the request fits. Should the OS say yes?*
>
> *(beat)* "Most people say yes, because the devices are sitting there idle. By the end of this video you'll see that granting it leaves the system with **no** order in which everyone can finish. The OS will refuse a request it could physically satisfy, and you'll be able to prove why it's right to."

On screen: the three resource icons with counts, five process cards, and P4's request bubble flashing. A "Yes / No?" poll stays in the corner until scene 15.

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:40 | Hook | Resource icons A=10 (printers), B=6 (scanners), C=8 (tape drives); five process cards P0–P4; P4's bubble `(3,2,0)` pulses; "Grant?" poll appears | Pose the question; the request fits; plant the doubt; promise a proof |
| 2 | 0:40–1:00 | Recap from V17 (20 s) | Mini RAG: P1 holds R1 and wants R2, P2 holds R2 and wants R1; the cycle glows; the four condition cards flash in turn | Define on screen: *resource type* (a kind of device), *instance* (one unit of it), *deadlock* (each process in a set waits for something held by another in the set). The four conditions, one line each |
| 3 | 1:00–1:40 | Three strategies | Horizontal bar: **Prevention** (left), **Avoidance** (centre), **Detection & recovery** (right); each label slides in with its tag | Prevention: make one condition impossible. Avoidance: know each process's maximum claim in advance and check every request. Detection: let deadlock happen, then find it and clean up. Today is the centre |
| 4 | 1:40–2:20 | Vectors and matrices | One row of three numbers labelled "vector (A, B, C)"; five such rows stack into a grid labelled "matrix: one row per process" | Define *vector* and *matrix*. Colour-code columns A, B, C consistently |
| 5 | 2:20–2:50 | Comparing vectors | `(1,2,2) ≤ (3,3,1)?` Each component pair is compared: ✓ ✓ ✗ → overall ✗ | Define `X ≤ Y`: true only if *every* component satisfies it. A single failure means the whole comparison fails |
| 6 | 2:50–3:40 | Max and Allocation | `Max` grid fades in (each process's declared ceiling), then `Allocation` (what each process holds now) | Define both. `Max` is declared when the process starts, which is the "advance knowledge" avoidance needs |
| 7 | 3:40–4:20 | Available | Column sums of `Allocation` animate downward: 7, 3, 7; subtracted from Total (10, 6, 8) → `Available = (3,3,1)` | Define `Available` = free instances right now |
| 8 | 4:20–5:10 | Need = Max − Allocation | `Max` slides over `Allocation`; cells subtract one row at a time into a new `Need` grid; row P2 highlighted: `8−3, 1−0, 3−2 = 5,1,1` | Define `Need`: the most a process may still ask for. It is a ceiling, not a current request |
| 9 | 5:10–5:50 | Safe state, defined | Whiteboard: "safe ⇔ some order lets every process get its remaining Need, finish, and release". Venn diagram: all states ⊃ unsafe ⊃ deadlocked; safe outside unsafe | Define *safe state*, *safe sequence*, *unsafe state*. Unsafe does not mean deadlocked: it *may* lead there. The OS stays in the safe region |
| 10 | 5:50–6:30 | Safety algorithm setup | `Work` box (= Available, 3,3,1) pinned top-right; `Finish[]` row of five empty boxes; scan pointer at P0 | Define `Work` (resources free in the imagined run) and `Finish` (has this process been shown able to finish?) |
| 11 | 6:30–7:30 | Safety pass 1 | Pointer visits P0 ✗(A), P1 ✗(C), P2 ✗(A), **P3 ✓** → P3's *Allocation* row (1,1,1) flies into Work → (4,4,2); Finish[3] ticks; P4 ✗(A) | Each check is shown component by component. When a process passes, it finishes and *returns what it holds* |
| 12 | 7:30–8:00 | **Pause-and-predict 1** | Freeze at the start of pass 2, Work = (4,4,2) | See §7 |
| 13 | 8:00–9:00 | Safety pass 2 | P0 ✗(A), **P1 ✓** → (6,4,2); **P2 ✓** → (9,4,4); **P4 ✓** → (9,5,6); pass 3: **P0 ✓** → (10,6,8); all Finish ticked; sequence ⟨P3, P1, P2, P4, P0⟩ printed | Work ends equal to Total, a built-in sanity check. Other safe orders may exist; finding one is enough |
| 14 | 9:00–10:10 | Request granted: P0 asks (0,2,0) | Three gate icons: ① `≤ Need` ✓ ② `≤ Available` ✓ ③ pretend: Available ghosts to (3,1,1), Allocation₀ (1,3,2), Need₀ (5,0,2); mini safety run at ×2 → safe → stamp **GRANTED** | Define *request* and *pretend-allocation*. Checks ① and ② are cheap; ③ is the real test |
| 15 | 10:10–11:20 | Request denied: P4 asks (3,2,0) (hook payoff) | ① ✓ ② ✓ ③ pretend: Available → (0,1,1), Need₄ → (2,0,2); pointer sweeps all five rows, every row red; stamp **UNSAFE → P4 WAITS**; the pretend values animate *back* (rollback) | Define *rollback*. Resolve the hook: nothing is deadlocked yet, but the OS declines the risk. The poll in the corner flips to "No" |
| 16 | 11:20–11:50 | **Pause-and-predict 2** | Request P2 (0,1,1): gates ① ② green; freeze before ③ | See §7 |
| 17 | 11:50–12:50 | Prevention in 60 s | Four condition cards flip to their prevention tactic: spool the device / request everything at once / allow preemption / number resources and request in order; a cost tag drops under each | Prevention restricts how processes may request; it is cheap per request but wastes utilisation. Resource ordering is the tactic used in practice (lock hierarchies) |
| 18 | 12:50–13:50 | Detection and recovery in 60 s | Same table layout, `Need` column relabelled `Request`; caption "same loop, current requests instead of maximum claims"; any unfinished process at the end is deadlocked; recovery icons: terminate a process / preempt and roll it back | Detection needs no advance `Max` and runs periodically. Define *recovery* |
| 19 | 13:50–14:40 | Recap + bridge to V19 | Three-column card (needs / when it runs / cost); Banker's cost O(m·n²) per request; end card "Next: V19 Memory basics" | Recap the three request checks; real kernels rarely run Banker's because `Max` is rarely known. Bridge: the most contested resource is memory. Where do a process's bytes live (binding, the MMU, fragmentation)? |

## 6. Worked example (verified)

**Resource types:** A (printers), B (scanners), C (tape drives). **Total** = (10, 6, 8).

### 6.1 Input state

| Process | Allocation (A B C) | Max (A B C) |
|---|---|---|
| P0 | 1 1 2 | 6 3 4 |
| P1 | 2 0 0 | 3 2 2 |
| P2 | 3 0 2 | 8 1 3 |
| P3 | 1 1 1 | 2 2 2 |
| P4 | 0 1 2 | 5 3 4 |

### 6.2 Available

| | A | B | C |
|---|---|---|---|
| Total | 10 | 6 | 8 |
| Σ Allocation | 7 | 3 | 7 |
| **Available** | **3** | **3** | **1** |

### 6.3 Need = Max − Allocation

| Process | Max − Allocation | Need |
|---|---|---|
| P0 | (6−1, 3−1, 4−2) | 5 2 2 |
| P1 | (3−2, 2−0, 2−0) | 1 2 2 |
| P2 | (8−3, 1−0, 3−2) | 5 1 1 |
| P3 | (2−1, 2−1, 2−1) | 1 1 1 |
| P4 | (5−0, 3−1, 4−2) | 5 2 2 |

### 6.4 Safety algorithm on the initial state (in-order repeated scan)

| Step | Pass | Check | Need | Work before | Need ≤ Work? | Work after | Finish |
|---|---|---|---|---|---|---|---|
| 1 | 1 | P0 | 5 2 2 | 3 3 1 | no (A: 5 > 3) | 3 3 1 | – |
| 2 | 1 | P1 | 1 2 2 | 3 3 1 | no (C: 2 > 1) | 3 3 1 | – |
| 3 | 1 | P2 | 5 1 1 | 3 3 1 | no (A) | 3 3 1 | – |
| 4 | 1 | P3 | 1 1 1 | 3 3 1 | **yes** | 4 4 2 | P3 |
| 5 | 1 | P4 | 5 2 2 | 4 4 2 | no (A) | 4 4 2 | – |
| 6 | 2 | P0 | 5 2 2 | 4 4 2 | no (A) | 4 4 2 | – |
| 7 | 2 | P1 | 1 2 2 | 4 4 2 | **yes** | 6 4 2 | P1 |
| 8 | 2 | P2 | 5 1 1 | 6 4 2 | **yes** | 9 4 4 | P2 |
| 9 | 2 | P4 | 5 2 2 | 9 4 4 | **yes** | 9 5 6 | P4 |
| 10 | 3 | P0 | 5 2 2 | 9 5 6 | **yes** | 10 6 8 | P0 |

**Safe sequence:** ⟨P3, P1, P2, P4, P0⟩. Final Work = Total (10,6,8) ✓.

### 6.5 Request 1, granted: P0 requests (0,2,0)

| Check | Comparison | Result |
|---|---|---|
| ① Request ≤ Need₀ | (0,2,0) ≤ (5,2,2) | ✓ |
| ② Request ≤ Available | (0,2,0) ≤ (3,3,1) | ✓ |
| ③ Pretend | Available (3,1,1); Allocation₀ (1,3,2); Need₀ (5,0,2) | run safety |

| Step | Finishes | Work after |
|---|---|---|
| start | – | 3 1 1 |
| 1 | P3 (Need 1,1,1) | 4 2 2 |
| 2 | P1 (Need 1,2,2) | 6 2 2 |
| 3 | P2 (Need 5,1,1) | 9 2 4 |
| 4 | P4 (Need 5,2,2) | 9 3 6 |
| 5 | P0 (Need 5,0,2) | 10 6 8 |

Safe → **grant**.

### 6.6 Request 2, denied: P4 requests (3,2,0)

| Check | Comparison | Result |
|---|---|---|
| ① Request ≤ Need₄ | (3,2,0) ≤ (5,2,2) | ✓ |
| ② Request ≤ Available | (3,2,0) ≤ (3,3,1) | ✓ |
| ③ Pretend | Available (0,1,1); Need₄ (2,0,2) | run safety |

| Process | Need | ≤ Work (0,1,1)? |
|---|---|---|
| P0 | 5 2 2 | no |
| P1 | 1 2 2 | no |
| P2 | 5 1 1 | no |
| P3 | 1 1 1 | no (A) |
| P4 | 2 0 2 | no |

No process can proceed → **unsafe → P4 waits**; state rolled back.

### 6.7 Further request outcomes from the initial state (used in §7 and §11)

| Request | ① ≤ Need | ② ≤ Available | Pretend Available | ③ Safe? | Outcome |
|---|---|---|---|---|---|
| P2 (0,1,1) | ✓ | ✓ | 3 2 0 | no process passes | wait (unsafe) |
| P1 (1,0,2) | ✓ | ✗ (C: 2 > 1) | – | not run | wait |
| P3 (1,1,1) | ✓ | ✓ | 2 2 0 | ⟨P3,P1,P2,P4,P0⟩ | grant |
| P1 (1,1,0) | ✓ | ✓ | 2 2 1 | ⟨P3,P1,P2,P4,P0⟩ | grant |

**Verification.** Every number above was produced by `bankers.py` in `/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/` (python3). The script computes Available and Need, runs the safety algorithm with an in-order repeated scan and prints every check, and runs the three-check request algorithm for every request listed. Final Work = Total was confirmed as a consistency check. The instance is original (not the Silberschatz pp.298–300 example).

## 7. Pause-and-predict checkpoints

1. **Scene 12 (Work = (4,4,2), pointer back at P0).** "Who is the next process to pass?"
   *Answer:* P0 fails on A (5 > 4). **P1** (1,2,2) passes, and Work becomes (6,4,2).
2. **Scene 16 (P2 requests (0,1,1)).** "It is within Need and within Available. Grant or wait?"
   *Answer:* **Wait.** The pretend Available is (3,2,0). Every process still needs at least one C, and C is 0, so the state is unsafe.
3. *(Optional, scene 19)* "If we *had* granted P4's request, would the system be deadlocked right now?"
   *Answer:* No. Nobody is blocked yet. Unsafe only means the OS can no longer *guarantee* completion if every process asks for its full Max.

## 8. Misconceptions to address on screen

- **"If the request fits in Available, grant it."** → Scene 15: the fit check goes green, the safety check goes red, and the rollback is animated.
- **"Unsafe = deadlocked."** → The Venn diagram in scene 9, repeated as a callout in scene 15.
- **"The safe sequence is unique."** → Lower-third in scene 13: other orders may be safe as well.
- **"Need is what the process is requesting now."** → Scene 8 caption: Need is the remaining ceiling. A Request is a separate vector, and it must be ≤ Need.
- **"When a process passes, add its Need to Work."** → Scene 11: the *Allocation* row flies into Work, not the Need row.
- **"Detection uses Max."** → Scene 18 relabels the column as `Request`.

## 9. Visual assets

- **Reuse:** `Slides/CS502/19-deadlock-handling.tex` is not built yet. Once it is, reuse its matrix tables. Reuse V17's mini-RAG and four-condition cards. Match the `booktabs` table style of the worked-example frames in `Slides/CS502/08-cpu-scheduling-basics.tex`.
- **New:**
  - Matrix animation (Manim `MathTable`, or Motion Canvas): column sums, cell-wise subtraction, scan pointer, an accumulating Work box, Finish ticks.
  - A three-gate "request checker" widget, reused in scenes 14–16.
  - Strategy bar and Venn diagram (PowerPoint morph is enough).
- **Live demo (optional, 30 s):** `python3 bankers.py`, scrolled to the `Request P4 [3, 2, 0]` block. Present it as "a checker you can use on the assignment".

### Instructor whiteboard version

For recording without animation software (one camera on a whiteboard, or a tablet with a pen):
1. Pre-draw three blank 5×3 grids labelled Max, Allocation, Need, plus boxes for Total, Available, and Work. Fill in Max and Allocation before you press record.
2. Write the column sums under Allocation in a second colour, then write Available. Fill Need one row at a time, saying each subtraction aloud.
3. For the safety run, draw a five-box Finish row. For each check, point at the Need row and the Work box. Write ✓ or ✗ *per component* beneath the row.
4. On a pass, physically trace an arrow from that process's Allocation row to Work, cross out the old Work, and write the new one beside it. Keep the crossed-out values visible, so the history of Work stays on the board.
5. For each request, write the three checks as a numbered list. Do the pretend values in pencil or a dashed colour and erase them on rollback, so the erasure itself is the rollback.
6. Use a separate small board, or a sticky note, for the prevention and detection summaries.

## 10. On-screen text & notation

- **Defined on screen at first use:** resource type, instance, deadlock (2); prevention, avoidance, detection (3); vector, matrix (4); `X ≤ Y` (5); Max, Allocation (6); Available (7); Need (8); safe / unsafe state, safe sequence (9); Work, Finish (10); request, pretend-allocation (14); rollback (15); recovery (18).
- Named objects are capitalised: **`Available`, `Max`, `Allocation`, `Need`**, plus `Work`, `Finish[i]`, `Request_i`. Never lowercase `max`, `alloc`, or `avail`.
- `Need = Max − Allocation`; `Available = Total − Σ Allocation`; vector order is always `(A, B, C)`.
- Processes `P0 … P4`.

## 11. End-of-video quiz

1. **(Numerical)** For the state in §6, what is `Available` after P0's request (0,2,0) is granted?
   *Solution:* Subtract the request from Available: (3−0, 3−2, 1−0) = **(3, 1, 1)**.
2. **(MCQ)** From the original state, P1 requests (1,0,2). The OS:
   (a) grants it (b) makes P1 wait because it exceeds Available (c) makes P1 wait because the result is unsafe (d) raises an error because it exceeds Need
   *Solution:* Check ①: (1,0,2) ≤ Need₁ (1,2,2), so ✓. Check ②: in component C, 2 > Available 1, so ✗. The process waits before the safety test is even reached. **(b)**.
3. **(MCQ)** From the original state, P3 requests (1,1,1). In the pretend state, which process passes the safety check first?
   (a) P0 (b) P1 (c) P3 (d) none, the state is unsafe
   *Solution:* The pretend Available is (3,3,1) − (1,1,1) = (2,2,0), and Need₃ = (0,0,0). P0, P1 and P2 fail. P3's Need of (0,0,0) ≤ (2,2,0) passes, and Work becomes (2,2,0) + (2,2,2) = (4,4,2). From there the original sequence continues. **(c)**, and the request is granted.
4. **(MCQ, concept)** Which statement is TRUE?
   (a) Every unsafe state is a deadlock (b) Banker's algorithm needs each process's maximum claim in advance (c) Detection algorithms require `Max` (d) Prevention runs a safety test per request
   *Solution:* (a) is false: unsafe ⊃ deadlocked. (c) is false: detection uses current Requests. (d) is false: prevention restricts the request patterns and runs no test. **(b)**.
5. **(Numerical)** In the safety run of §6.4, what is `Work` immediately after P2 finishes?
   *Solution:* Before P2, Work = (6,4,2). Add Allocation₂ (3,0,2) to get **(9, 4, 4)**.

## 12. References

- Silberschatz 8e, Ch. 7: prevention p.291; avoidance, safe state and Banker's algorithm pp.294–300 (worked safety example pp.298–300); detection p.301; recovery p.304.
- Stallings 6e, Ch. 6: prevention §6.2 p.272; avoidance, safe state and Banker's §6.3 p.273; detection §6.4 p.279; integrated strategy §6.5 p.281.
- Bovet & Cesati 3e: not cited, because its index records no Banker's-algorithm treatment.

## 13. Production notes

- **Pacing:** scenes 11 and 13 are the core of the video. Give each check 8–12 s with the component ticks visible. The safety runs in the request scenes can play at ×2.
- **Cut if over length:** shorten scene 17 to 30 s, then scene 18. Drop checkpoint 3. Never cut scene 15.
- **Recording:** record the animation silent, then narrate over it. Keep `Work` pinned top-right throughout.
- **Accessibility:** captions on. Show pass/fail as ✓/✗ glyphs *and* colour (blue/orange, not red/green). Colour the A/B/C columns from the Okabe–Ito palette. Read every changing number aloud.
- **Hinglish variant:** keep all matrix labels in English and change only the narration.

### Timing budget and cut priority

| # | Scene | Seconds | Cumulative | Priority |
|---|---|---|---|---|
| 1 | Hook | 40 | 0:40 | keep (core) |
| 2 | Recap from V17 (20 s) | 20 | 1:00 | keep (core) |
| 3 | Three strategies | 40 | 1:40 | shorten first |
| 4 | Vectors and matrices | 40 | 2:20 | shorten first |
| 5 | Comparing vectors | 30 | 2:50 | keep (core) |
| 6 | Max and Allocation | 50 | 3:40 | keep (core) |
| 7 | Available | 40 | 4:20 | keep (core) |
| 8 | Need = Max − Allocation | 50 | 5:10 | keep (core) |
| 9 | Safe state, defined | 40 | 5:50 | keep (core) |
| 10 | Safety algorithm setup | 40 | 6:30 | keep (core) |
| 11 | Safety pass 1 | 60 | 7:30 | keep (core) |
| 12 | **Pause-and-predict 1** | 30 | 8:00 | keep (core) |
| 13 | Safety pass 2 | 60 | 9:00 | keep (core) |
| 14 | Request granted: P0 asks (0,2,0) | 70 | 10:10 | keep (core) |
| 15 | Request denied: P4 asks (3,2,0) (hook payoff) | 70 | 11:20 | keep (core) |
| 16 | **Pause-and-predict 2** | 30 | 11:50 | shorten first |
| 17 | Prevention in 60 s | 60 | 12:50 | cut first |
| 18 | Detection and recovery in 60 s | 60 | 13:50 | cut first |
| 19 | Recap + bridge to V19 | 50 | 14:40 | keep (core) |

Core (keep) scenes total 10:50; shortenable 1:50; cuttable 2:00. Halving every "shorten first" scene and dropping every "cut first" scene saves 2:55.
