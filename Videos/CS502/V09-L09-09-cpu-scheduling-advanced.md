# V09 · L09 — Who Runs Next When Not All Work Is Equal? Priority, Round Robin and MLFQ

> **Lecture:** CS502 L09 `09-cpu-scheduling-advanced` · **Unit:** I · **Target length:** 12–14 min · **Series position:** 09/28
> **Format:** screen-capture animation (Gantt charts + live queues; MLFQ queue ladder) · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

Round Robin and MLFQ are defined by *events over time*: a quantum expires, a
process goes to the tail of a queue, a newcomer slips in ahead of it, a
process that used its full slice drops one level. The final Gantt chart on a
slide hides all of that, and the tie rule ("an arrival at the instant a
quantum expires joins before the preempted process") is exactly where
students' hand-drawn charts diverge from the answer key. Animating the
circular queue, the timer, and the MLFQ ladder shows each decision, and a
quantum slider makes the response-time vs. overhead trade-off visible in a
way a table cannot.

## 2. Learning outcomes

After watching, the student can:
- [Apply] Build non-preemptive and preemptive Priority schedules and compute average WT and TAT.
- [Apply] Build a Round Robin schedule for a given quantum `q`, applying the stated tie rule, and compute average WT, TAT and RT.
- [Analyze] Explain how the choice of `q` changes response time, waiting time and the number of context switches, and when RR degenerates to FCFS.
- [Apply] Trace a three-level MLFQ (`Q0` RR `q = 2`, `Q1` RR `q = 4`, `Q2` FCFS) and explain how it favours short jobs without knowing burst lengths.
- [Understand] Define starvation and aging, and compute an aging bound.

## 3. Prerequisites

- Computer fundamentals: one CPU runs one program at a time; a hardware timer can interrupt it.
- V06 *Process states, queues and the context switch*: ready queue, context switch, timer interrupt.
- V08 *FCFS, SJF, SRTF and the convoy effect*: Gantt charts; TAT, WT, RT; preemption; starvation.
- All of these are recapped on screen in scene 2 (≈ 40 s).

## 4. Hook (first 30–45 s)

> *On screen: a laptop with three windows — a video call, a compiler, a
> backup. The video call freezes for a second.*
>
> "Your video call just froze because a backup job grabbed the CPU. Last
> time we saw that FCFS lets a long job block everyone, and SJF fixes it only
> if you know how long every burst will take — which no OS does. So how does
> a real scheduler keep the call smooth, finish the compile, *and* never
> forget the backup? Today one workload of five processes goes through four
> policies, and you'll see one of them figure out, all by itself, which job
> is short."

(Deck frame **Welcome: Who Should Run Next When Not All Work Is Equal?**.)

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:40 | Hook | Laptop with three windows; the call freezes; five process cards appear. | Real systems mix important and unimportant, short and long work. |
| 2 | 0:40–1:20 | Recap from V06 and V08 (≈ 40 s) | Ready-queue lane; a timer icon; the three formulas TAT = CT − AT, WT = TAT − τ, RT = ST − AT over a mini Gantt bar. | **Ready queue** = processes waiting only for the CPU. **Context switch** = swap one process's saved state for another's — pure overhead. **Timer interrupt** = hardware alarm that hands control to the kernel. Metrics: **TAT** (turnaround), **WT** (waiting), **RT** (response); τ = CPU burst. **Preemptive** = the OS may take the CPU away mid-burst. **Starvation** = a ready process that never gets the CPU. |
| 3 | 1:20–2:00 | The workload `W` | Table of five processes with arrival, burst, priority, and a character icon (deck **The Running Workload W**). Total work bar "28 ms". FCFS baseline badge: avg WT 11.4, TAT 17.0. | Lower priority number = more important (Silberschatz convention), ties to earlier arrival. Every schedule ends at 28. |
| 4 | 2:00–3:20 | Priority, two ways | Scheduler cam: non-preemptive chart P1 0–8, P2 8–12, P4 12–17, P3 17–26, P5 26–28. Then replay preemptively: at t = 1 P2 (priority 1) arrives, flash, P1 (3) pushed back with 7 ms left; t = 5 P4 (2) beats P1; P1 10–17. | **Priority scheduling** = run the most important ready process. Non-preemptive: avg WT 10.6, TAT 16.2 — couldn't help P2 because P1 already held the CPU. Preemptive: avg WT 9.6, TAT 15.2; P2 waits 0. SJF is priority with priority = predicted burst. |
| 5 | 3:20–4:10 | Who paid: starvation and aging | Table: WT of P5 = 22 under FCFS and both priorities. Then a stream of priority-1 jobs pushes P5 back forever. Aging dial: priority 63 → 0, one step per 10 ms; "630 ms" badge. | P5 always pays. **Aging** = raise a waiting process's priority over time. With 0–63 and −1 every 10 ms, the worst case reaches 0 in 63 × 10 = 630 ms — the wait is bounded. |
| 6 | 4:10–6:10 | Round Robin, `q = 3`, in slow motion | Circular ready queue drawn as a ring; a timer bar counts 3 ms over the running card. Key freezes: t = 3 queue [P2, P3, P4] then P1 rejoins → [P2, P3, P4, P1]; t = 6 P5 (arrived at 4) precedes P2 → [P3, P4, P1, P5, P2]; t = 15 [P5, P2, P3, P4, P1], P5 (2 ms) and P2 (1 ms left) leave early; t = 21 [P4, P1, P3] drain. Gantt grows underneath: P1 0–3, P2 3–6, P3 6–9, P4 9–12, P1 12–15, P5 15–17, P2 17–18, P3 18–21, P4 21–23, P1 23–25, P3 25–28. | **Round Robin (RR)** = FCFS plus a **time quantum** `q`: run at most `q`, then go to the tail. Tie rule used throughout: an arrival at the instant a quantum expires joins *before* the preempted process. A burst shorter than `q` gives up the CPU early. |
| 7 | 6:10–7:00 | RR's numbers | Table (deck **RR q=3: The Numbers**): first dispatch, CT, TAT, WT, RT per process; arrows FCFS → RR: RT 11.4 → 4.6 (green), WT 11.4 → 14.6 (red). | RR trades turnaround for responsiveness by design. Bound: with `n` ready and quantum `q`, nobody waits more than `(n − 1)q` for the next turn: 4 × 3 = 12 ms; largest RT here is P5's 11 ≤ 12. |
| 8 | 7:00–8:20 | The quantum slider | A slider `q` = 1, 2, 3, 4, 5, ≥ 9; the Gantt chart re-renders at each stop; a live table of avg WT / TAT / RT / switches (deck **How the Quantum Changes Everything**). Then an overhead overlay with `s = 0.5 ms`. | RT climbs steadily with `q` (1.2 → 11.4); WT does *not* move monotonically (13.0, 13.0, 14.6, 13.0, 12.6, 11.4). At `q ≥ 9` RR *is* FCFS. Small `q` costs switches: 25 switches × 0.5 ms = 12.5 ms on a 28 ms schedule at `q = 1`; 10 × 0.5 = 5 ms at `q = 3`. Useful fraction `q/(q+s)`: 66.7 %, 85.7 %, 95.2 % for `q` = 1, 3, 10. Rule of thumb: `q` ≫ switch time, but ~80 % of bursts finish within one quantum. |
| 9 | 8:20–9:00 | Multilevel queue (MLQ) | Two stacked lanes: *foreground (interactive) — RR* above *background (batch) — FCFS*; processes are permanently assigned; a "fixed priority" lock between lanes (deck **Multilevel Queue (MLQ)**, **Scheduling Among the Queues**). | **MLQ** = separate ready queues per class, each with its own policy; a process never changes queue. Simple, but a batch job can starve and a misclassified process is stuck. Fix: let processes move. |
| 10 | 9:00–11:00 | MLFQ on `W` | Three-rung ladder (deck **Multilevel Feedback Queue (MLFQ)** TikZ): `Q0` RR `q = 2`, `Q1` RR `q = 4`, `Q2` FCFS. Cards enter at `Q0`; each that uses a full quantum slides down a rung (gold arrow). Gantt shaded by level: 0–10 every process gets 2 ms in `Q0`, only P5 (τ = 2) finishes; 10–23 `Q1`: P1 10–14, P2 14–16 (done), P3 16–20, P4 20–23 (done); 23–28 `Q2`: P1 23–25, P3 25–28. | **MLFQ** = multilevel queue where processes *move*: use your whole quantum → demoted; higher queues preempt lower ones; promotion (aging) prevents starvation. The scheduler learns who is short. Numbers: P5 done at 10 (FCFS: 28), avg RT 2.0 vs RR's 4.6 with the same 10 switches; avg WT 12.8, TAT 18.4. |
| 11 | 11:00–11:50 | A late short arrival preempts | Second workload: P1 (0, 9), P2 (1, 3), P3 (2, 6), P4 (8, 2). At t = 8 P4 enters `Q0` and preempts P1 (in `Q1`); P1 goes to the tail of `Q1` behind P2 and P3. Gantt: P1 0–2, P2 2–4, P3 4–6, P1 6–8, P4 8–10, P2 10–11, P3 11–15, P1 15–19, P1 19–20 (`Q2`). | New short work is served at once: P4 TAT 2, WT 0. Averages WT 6.25, TAT 11.25. Gaming risk: a CPU hog that does tiny I/O before each quantum expires never gets demoted — defence: charge CPU time cumulatively per level (deck **Socratic Check 3**). |
| 12 | 11:50–12:40 | All policies on `W` | Summary table (deck **All Policies on W**) with the winning cell in each column highlighted; no row wins every column. | FCFS 11.4/17.0/11.4; Priority NP 10.6/16.2/10.6; Priority P 9.6/15.2/7.8; RR q=3 14.6/20.2/4.6; MLFQ 12.8/18.4/2.0 (avg WT / TAT / RT). The right policy depends on the criterion. |
| 13 | 12:40–13:20 | Bridge to V10 | Two processes on one CPU, switched by the RR timer in the middle of `counter++`; the counter shows a wrong value; a "?" | Preemption can strike between *any* two instructions. That's fine for fairness — but what if both processes were updating the same data? **Next, V10 — Race conditions and the critical-section problem.** |

## 6. Worked example (verified)

**Workload `W`** (deck **The Running Workload W**), times in ms, lower
number = higher priority:

| Process | AT | τ | Priority |
|---|---|---|---|
| P1 | 0 | 8 | 3 |
| P2 | 1 | 4 | 1 |
| P3 | 2 | 9 | 4 |
| P4 | 3 | 5 | 2 |
| P5 | 4 | 2 | 5 |

Total work 28 ms. FCFS baseline: `P1 0–8 | P2 8–12 | P3 12–21 | P4 21–26 |
P5 26–28`; avg WT 11.4, TAT 17.0, RT 11.4.

**Priority, non-preemptive:** `P1 0–8 | P2 8–12 | P4 12–17 | P3 17–26 | P5 26–28`
CT 8, 12, 26, 17, 28 · TAT 8, 11, 24, 14, 24 (avg 16.2) · WT 0, 7, 15, 9, 22 (avg **10.6**).

**Priority, preemptive:** `P1 0–1 | P2 1–5 | P4 5–10 | P1 10–17 | P3 17–26 | P5 26–28`
CT 17, 5, 26, 10, 28 · TAT 17, 4, 24, 7, 24 (avg 15.2) · WT 9, 0, 15, 2, 22
(avg **9.6**) · RT avg 7.8.

**RR, `q = 3`** (tie rule: arrival at a quantum expiry queues before the
preempted process):
`P1 0–3 | P2 3–6 | P3 6–9 | P4 9–12 | P1 12–15 | P5 15–17 | P2 17–18 | P3 18–21 | P4 21–23 | P1 23–25 | P3 25–28`

| | P1 | P2 | P3 | P4 | P5 | Avg |
|---|---|---|---|---|---|---|
| First dispatch | 0 | 3 | 6 | 9 | 15 | |
| CT | 25 | 18 | 28 | 23 | 17 | |
| TAT | 25 | 17 | 26 | 20 | 13 | 20.2 |
| WT | 17 | 13 | 17 | 15 | 11 | 14.6 |
| RT | 0 | 2 | 4 | 6 | 11 | **4.6** |

**Quantum sweep on `W`** (switches = changes of running process between
consecutive slices):

| `q` | avg WT | avg TAT | avg RT | switches |
|---|---|---|---|---|
| 1 | 13.0 | 18.6 | 1.2 | 25 |
| 2 | 13.0 | 18.6 | 2.8 | 13 |
| 3 | 14.6 | 20.2 | 4.6 | 10 |
| 4 | 13.0 | 18.6 | 6.0 | 8 |
| 5 | 12.6 | 18.2 | 7.4 | 6 |
| 9 (≥ longest burst) | 11.4 | 17.0 | 11.4 | 4 |

Overhead with `s = 0.5 ms`: useful fraction `q/(q+s)` = 66.7 % (`q = 1`),
85.7 % (`q = 3`), 95.2 % (`q = 10`); added time 25 × 0.5 = 12.5 ms (`q = 1`),
10 × 0.5 = 5 ms (`q = 3`). RR bound `(n − 1)q` = 4 × 3 = 12 ≥ max RT 11.

**MLFQ** (`Q0` RR 2, `Q1` RR 4, `Q2` FCFS; new arrivals enter `Q0`; higher
queues preempt lower; a preempted process returns to the tail of its own
queue):
`P1 0–2 | P2 2–4 | P3 4–6 | P4 6–8 | P5 8–10 | P1 10–14 | P2 14–16 | P3 16–20 | P4 20–23 | P1 23–25 | P3 25–28`
CT 25, 16, 28, 23, 10 · TAT 25, 15, 26, 20, 6 (avg 18.4) · WT 17, 11, 17,
15, 4 (avg 12.8) · RT 0, 1, 2, 3, 4 (avg **2.0**) · 10 switches.

**MLFQ, late short arrival:** P1 (0, 9), P2 (1, 3), P3 (2, 6), P4 (8, 2):
`P1 0–2 | P2 2–4 | P3 4–6 | P1 6–8 | P4 8–10 | P2 10–11 | P3 11–15 | P1 15–19 | P1 19–20`
WT 11, 7, 7, 0 → avg 6.25; TAT 20, 10, 13, 2 → avg 11.25.

**Aging:** priorities 0–63, −1 per 10 ms waited ⇒ worst case 63 × 10 = 630 ms.

**How the numbers were verified.** Every schedule, table and average above
was generated by the `python3` simulator in `scratchpad/videos/sched.py`
(driver `run.py`): unit-time Priority (preemptive and non-), event-driven RR
with the stated tie rule, and a unit-time three-level MLFQ with the stated
preemption/demotion rules. All outputs match the deck's tables exactly,
including the switch counts in the quantum sweep. Overhead percentages,
the aging bound and the RR bound were recomputed in `python3`.

## 7. Pause-and-predict checkpoints

1. **Scene 4, at t = 1 (preemptive replay):** "P2, priority 1, arrives while
   P1, priority 3, runs. What happens?" — *Answer:* P2 preempts; P1 returns to
   the ready queue with 7 ms left.
2. **Scene 6, frozen at t = 6:** "P2 was preempted at 6, and P5 arrived at 4.
   Who is ahead in the queue?" — *Answer:* P5 — it joined at 4, before P2
   re-queued at 6; queue = [P3, P4, P1, P5, P2].
3. **Scene 10, frozen at t = 10:** "Everyone has had 2 ms in `Q0`. Which
   processes are still in `Q0`, and who runs next?" — *Answer:* none — P5
   finished; P1–P4 were demoted to `Q1`; P1 runs next in `Q1` for up to 4 ms.

## 8. Misconceptions to address on screen

- **"A smaller quantum always gives better waiting time."** → Scene 8: WT
  goes 13.0, 13.0, 14.6, 13.0, 12.6 — not monotone; switches explode.
- **"RR and FCFS are unrelated."** → Scene 8's `q ≥ 9` stop: identical
  chart to FCFS.
- **"A preempted RR process goes to the front."** → Scene 6: it goes to the
  tail, and a simultaneous arrival goes ahead of it.
- **"Non-preemptive priority runs an important arrival immediately."** →
  Scene 4: P2 waits 7 ms behind P1.
- **"MLFQ needs to know burst lengths."** → Scene 10: P5 is discovered short
  purely from behaviour.
- **"Aging means old processes are removed."** → Scene 5's dial: aging
  *raises* priority.
- **"MLQ and MLFQ are the same."** → Scenes 9–10: locked lanes vs. sliding
  cards.

## 9. Visual assets

- **Reuse (from `Slides/CS502/09-cpu-scheduling-advanced.tex`):**
  - Frame **The Running Workload W** (table) — scene 3.
  - `\gantt` charts in **Worked Example: Non-Preemptive Priority on W**,
    **Worked Example: Preemptive Priority on W**, **Worked Example: RR with
    q = 3 ms on W**, **Worked Example: MLFQ on W** (level-shaded), and
    **Worked Example: A Late Short Arrival Preempts**.
  - Tables in **What Priority Bought, and Who Paid**, **RR q=3: The
    Numbers**, **How the Quantum Changes Everything**, **MLFQ on W: The
    Numbers**, **All Policies on W**.
  - Frames **Starvation and Aging**, **The Cost of a Small Quantum**,
    **RR's Guarantee: Bounded Response**, **Multilevel Queue (MLQ)**,
    **Scheduling Among the Queues**, **The Five Parameters of an MLFQ**,
    **Socratic Check 3**, **Common Misconceptions**.
  - TikZ queue ladder in **Multilevel Feedback Queue (MLFQ)** (`Q0`/`Q1`/`Q2`
    boxes with demotion arrows) — scene 10.
  - `Notes/CS502/09-cpu-scheduling-advanced-notes.tex` for prose on MLQ
    inter-queue scheduling.
- **New:**
  - Reuse V08's "scheduler cam" template (clock + ready queue + growing
    Gantt) for scenes 4, 6, 10, 11.
  - Circular-queue ring with a quantum timer bar (scene 6).
  - Quantum slider re-rendering the chart from simulator output for each
    `q` (scene 8) — pre-render six charts and cross-fade.
  - MLFQ ladder with cards sliding down a rung on demotion (scene 10).
  - Multiprocessor scheduling (deck Act 5) is *not* in this video; leave it
    to the notes or a later short.
- **Live demo (optional, 30 s):** `python3 run.py` in `scratchpad/videos/`
  prints every `W` schedule; show the `q` sweep scrolling past.

## 10. On-screen text & notation

- First-use definitions required on screen: priority (and the
  lower-number-is-higher convention), preemptive/non-preemptive, starvation,
  aging, time quantum, Round Robin, multilevel queue, multilevel feedback
  queue, demotion/promotion, context switch, response time.
- Algorithm names exactly `Priority`, `RR`, `MLQ`, `MLFQ`; quantum `q`;
  switch cost `s`; queues `Q0`, `Q1`, `Q2`.
- Formulas: `TAT = CT − AT`, `WT = TAT − τ`, `RT = ST − AT`; useful fraction
  `q/(q + s)`; RR bound `(n − 1)q`.
- Gantt rules as in V08: time left → right, arrivals marked on the axis,
  MLFQ bars shaded by level *and* labelled `Q0`/`Q1`/`Q2`.
- State the tie rule in a lower-third caption whenever an RR chart is shown.

## 11. End-of-video quiz

Workload for Q1–Q3 (new, original): `A` (AT 0, τ 7, priority 3), `B`
(2, 4, 1), `C` (4, 1, 4), `D` (5, 4, 2); times in ms; lower number = higher
priority. All answers from the simulator.

1. **(Numerical)** Preemptive Priority: Gantt chart and average WT? —
   **Answer:** `A 0–2 | B 2–6 | D 6–10 | A 10–15 | C 15–16`; WT A 8, B 0,
   C 11, D 1 → **avg 5.0 ms**.
2. **(Numerical, GATE-style)** RR with `q = 2` (same tie rule): average WT
   and average RT? — **Answer:** `A 0–2 | B 2–4 | A 4–6 | C 6–7 | B 7–9 |
   D 9–11 | A 11–13 | D 13–15 | A 15–16`; avg WT **5.0 ms**, avg RT
   **1.5 ms**.
3. **(Numerical)** MLFQ (`Q0` RR 2, `Q1` RR 4, `Q2` FCFS): when does `C`
   finish, and what is the average RT? — **Answer:** `C` finishes at **5**;
   every process is first dispatched on arrival, so avg RT = **0**.
4. **(Numerical)** Six processes are ready, `q = 4 ms`, switch time ignored.
   Maximum wait before a process gets its next quantum? —
   **Answer:** (6 − 1) × 4 = **20 ms**.
5. **(MCQ)** In MLFQ, a CPU-bound program issues a tiny I/O just before
   every quantum expires. Without countermeasures it will
   (a) be demoted faster (b) stay in the top queue (c) be killed
   (d) move to `Q2` immediately — **Answer: (b).**

## 12. References

- Silberschatz, Galvin & Gagne, *Operating System Concepts*, 8e — Ch. 5
  §5.3: Priority p. 192, Round Robin p. 194, Multilevel Queue p. 196,
  Multilevel Feedback Queue p. 197; §5.5 multiple-processor scheduling,
  p. 200.
- Stallings, *Operating Systems: Internals and Design Principles*, 6e —
  Ch. 9 §9.2 scheduling algorithms (round-robin, priority, feedback),
  p. 410.
- Bovet & Cesati, *Understanding the Linux Kernel*, 3e — Ch. 7 Process
  Scheduling, pp. 258–293 (a real priority + timeslice scheduler; optional).

## 13. Production notes

- **The tie rule is the #1 source of wrong RR charts.** Show it as a
  lower-third caption at every quantum expiry in scene 6, and narrate it at
  t = 3 and t = 6.
- **Pacing warning:** scene 6 has 11 slices; animate at ~8 s per slice for
  the first four, then speed up to ~3 s once the pattern is clear.
- **Scene 8 (slider)** must not re-explain each chart — let the table numbers
  carry it; 6–8 s per stop.
- **If over length:** cut scene 9 (MLQ) to 20 s and scene 11 (late arrival)
  to a 20 s "one more thing" with only the t = 8 freeze. Keep scenes 6, 8
  and 10.
- **If under length:** add the deck's **Worked Example: Two-Queue MLQ** as
  a 40 s insert after scene 9 (re-run the simulator before quoting its
  numbers — they are not verified in this brief).
- **Accessibility:** captions; MLFQ levels distinguished by shading *and* a
  `Q0/Q1/Q2` label on every bar; green/red deltas in scene 7 also carry ↑/↓
  arrows; colour-blind-safe blue/gold/slate palette from the deck.
