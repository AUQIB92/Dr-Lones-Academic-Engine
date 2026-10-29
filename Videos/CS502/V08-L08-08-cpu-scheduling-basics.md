# V08 · L08 — Five Jobs, One CPU: FCFS, SJF, SRTF and the Convoy Effect

> **Lecture:** CS502 L08 `08-cpu-scheduling-basics` · **Unit:** I · **Target length:** 12–14 min · **Series position:** 08/28
> **Format:** screen-capture animation (Gantt charts built in real time + metric tables) · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

A Gantt chart is a *process in time*: at every arrival or completion the
scheduler looks at the ready queue and makes a choice. A finished chart on a
slide hides those decision points, so students copy the bars without knowing
why `P3` ran at 6 or why `P1` was preempted at 2 — and then fail the GATE
question with a different arrival pattern. Animating the clock, the ready
queue and the chart together shows *each decision*. The convoy effect is
inherently dynamic (short jobs bunching up behind one long one, devices
idling) and essentially invisible in a static picture.

## 2. Learning outcomes

After watching, the student can:
- [Remember] State the per-process formulas TAT = CT − AT, WT = TAT − τ, RT = ST − AT.
- [Apply] Build FCFS, non-preemptive SJF and SRTF Gantt charts for a workload with staggered arrivals and compute average WT, TAT and RT.
- [Analyze] Explain the convoy effect and quantify it on a CPU-bound + I/O-bound mix.
- [Understand] Explain why SJF minimizes average WT, why it cannot be implemented exactly, and how exponential averaging predicts the next burst.
- [Analyze] Compare the three policies on one workload and identify the starvation risk of short-first scheduling.

## 3. Prerequisites

- Computer fundamentals: one CPU executes one instruction stream at a time; programs wait for disks and keyboards.
- V06 *Process states, queues and the context switch*: ready queue, scheduler, dispatcher, context switch.
- V07 *fork, exec, wait*: processes are created and end.
- Basic arithmetic means; reading a time axis. Recapped on screen in scene 1b.

## 4. Hook (first 30–45 s)

> *On screen: a lab-server dashboard. Five compile jobs pop into a queue over
> five milliseconds, each with a "CPU needed" badge: 6, 8, 2, 5, 4 ms.*
>
> "Five students hit 'compile' within five milliseconds. The server has one
> CPU. Every job will get exactly the CPU time it needs — the total work is
> 25 milliseconds no matter what. And yet, depending on *the order*, the
> average student waits more than 9 milliseconds, or just 6. Same work, same
> finish time, 50% more waiting. Which order is best — and can the OS even
> know it?"

(Deck frame **Welcome: Who Runs Next on the Lab Server?** — the running
workload *R*.)

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:40 | Hook | Five job cards drop into a queue at their arrival times; table of AT and τ (deck welcome frame). | Same work, order changes waiting; preview "9.2 vs 6.0". |
| 1b | 0:40–1:00 | Recap from V06–V07 (20 s) | Mini state diagram with `ready` and `running` lit; a ready-queue lane of PCB cards; a gold "switch" box between two bars. | A **process** is **ready** when it needs only the CPU. Ready processes wait in the **ready queue**. The **scheduler** chooses which runs next; the **dispatcher** actually hands it the CPU via a **context switch** (save one process's registers, load another's). |
| 2 | 1:00–1:40 | Bursts and the scheduler | CPU–I/O burst strip (deck **The CPU–I/O Burst Cycle**); ready queue → scheduler → dispatcher → CPU. Four decision points light up: running → waiting, running → ready, waiting → ready, terminates (deck **When Does the Scheduler Run?**). | Processes alternate CPU and I/O bursts. Non-preemptive: decide only when the CPU is given up. Preemptive: also on arrivals/interrupts. |
| 3 | 1:40–2:50 | Yardsticks | Five criteria list; then the three formulas (deck **The Three Per-Process Formulas**) with a mini timeline showing AT, ST, CT for one bar. Idle-gap example: P1(0,3), P2(5,4), P3(6,2) FCFS: bars 0–3, idle 3–5, 5–9, 9–11; table fills. | TAT = CT − AT, WT = TAT − τ, RT = ST − AT. Idle example: averages TAT 4, WT 1, RT 1; utilization 9/11 ≈ 81.8 %. Idle time is marked, not hidden. |
| 4 | 2:50–4:00 | FCFS on three jobs: order matters | Two Gantt charts stacked: 24, 3, 3 (avg WT 17) vs. 3, 3, 24 (avg WT 3). A "17 ÷ 3 ≈ 5.7×" badge. | Same jobs, same finish at 30; 5.7× less average waiting simply by order (deck **Worked Example: One Long Job First** / **Same Jobs, Different Arrival Order**). |
| 5 | 4:00–5:30 | The convoy effect, animated | Two-row timeline: CPU row and an "I/O-bound devices busy" row. One CPU-bound job `C` (10 ms CPU, 2 ms I/O) and three I/O-bound jobs `I1–I3` (1 ms CPU, 3 ms I/O). FCFS: long gold `C` bars with three short bars squashed behind; device row mostly empty. Then short-first: CPU row a tight rhythm of 1 ms bars; device row nearly full. Counters at 48 ms. | Short jobs queue behind the long one; their devices idle; everyone moves at the speed of the slowest (deck **The Convoy Effect**). Over 48 ms: I/O-bound devices busy 27 of 144 device-ms (18.75 %) under FCFS vs. 102 of 144 (≈ 70.8 %) letting short bursts go first; I/O-bound CPU bursts completed: 9 vs. 36. CPU stays 100 % busy in both — the difference is the *devices*. |
| 6 | 5:30–6:50 | FCFS on the lab server | Clock ticks 0 → 25; at each completion the ready-queue panel shows who is waiting; bars P1 0–6, P2 6–14, P3 14–16, P4 16–21, P5 21–25. Table fills row by row. | P3 needs 2 ms but waits 12 behind P2's 8. Avg WT 46/5 = 9.2, TAT 14.2, RT 9.2. |
| 7 | 6:50–7:40 | SJF idea + why optimal | Four jobs at 0 (6, 8, 7, 3): SJF bars P4, P1, P3, P2, avg WT 7 vs FCFS 10.25. Then the swap argument: two adjacent bars, long-then-short, swap — the short one's wait drops by more than the long one's rises (deck **Why SJF Is Optimal**). | Moving a short job ahead lowers its wait more than it raises the long one's. So shortest-first minimizes average WT. |
| 8 | 7:40–9:00 | Non-preemptive SJF on the lab server | Clock at 0: only P1 ready → runs to 6. Freeze at t = 6: ready queue panel shows P2(8), P3(2), P4(5), P5(4) sorted; P3 picked, then P5, P4, P2. Bars 0–6, 6–8, 8–12, 12–17, 17–25. | Decision only when the CPU is free. Avg WT 32/5 = 6.4, TAT 11.4, RT 6.4. |
| 9 | 9:00–10:00 | The catch: predicting τ | Exponential-average chart: measured bursts 6, 4, 6, 4, 13, 13, 13 as bars, predictions 10, 8, 6, 6, 5, 9, 11, 12 as a line lagging the jump. Formula `τ̂_{n+1} = α τ_n + (1 − α) τ̂_n`, α = 1/2. | Nobody knows the next burst; predict it from history. When behaviour jumps from 4 to 13, the prediction lags (9, 11, 12) then converges. |
| 10 | 10:00–11:20 | SRTF on the lab server | Clock with remaining-time badges on each card. t = 2: P3 arrives with 2, P1 has 4 left → preempt flash, P1 back to queue. t = 3: P4(5) vs running P3 (1 left) — no preempt. t = 4: P3 done; P1 (4 left) beats P4 (5) and P2 (8). t = 5: P5(4) vs P1 (3 left) — no preempt. Bars P1 0–2, P3 2–4, P1 4–8, P5 8–12, P4 12–17, P2 17–25. | Preemptive SJF: compare the newcomer with the *remaining* time of the running job. Avg WT 30/5 = 6.0, TAT 11.0, RT 5.6. |
| 11 | 11:20–12:20 | Three policies, one workload | Three Gantt charts aligned on one axis; comparison table (deck **Three Policies, One Workload**). Then a 0.5 ms dispatch-latency overlay: utilization 25/28 ≈ 89.3 % (SRTF, 6 dispatches) vs. 25/27.5 ≈ 90.9 % (FCFS, 5). | All end at 25; only order changes. SRTF best averages, one extra dispatch. |
| 12 | 12:20–13:10 | Starvation + bridge | A stream of 2 ms jobs keeps arriving; P2's card is pushed back again and again; its WT counter climbs. "Aging" teaser label. | P2 already has the largest WT (16) under SJF and SRTF; a steady stream of short jobs would postpone it forever. FCFS never starves. **Next, V09 — Priority, Round Robin, MLFQ:** how do real schedulers stop this starvation and stay responsive without knowing burst lengths? Same idea of a running workload, new policies. |

## 6. Worked example (verified)

**Running workload R (lab server)** — deck welcome frame:

| Process | `P1` | `P2` | `P3` | `P4` | `P5` |
|---|---|---|---|---|---|
| AT (ms) | 0 | 1 | 2 | 3 | 5 |
| τ (ms) | 6 | 8 | 2 | 5 | 4 |

**FCFS:** `| P1 | P2 | P3 | P4 | P5 |` over `0 6 14 16 21 25`

| Proc | AT | τ | CT | TAT | WT | RT |
|---|---|---|---|---|---|---|
| P1 | 0 | 6 | 6 | 6 | 0 | 0 |
| P2 | 1 | 8 | 14 | 13 | 5 | 5 |
| P3 | 2 | 2 | 16 | 14 | 12 | 12 |
| P4 | 3 | 5 | 21 | 18 | 13 | 13 |
| P5 | 5 | 4 | 25 | 20 | 16 | 16 |
| **Avg** | | | | **14.2** | **9.2** | **9.2** |

**Non-preemptive SJF:** `| P1 | P3 | P5 | P4 | P2 |` over `0 6 8 12 17 25`

| Proc | CT | TAT | WT | RT |
|---|---|---|---|---|
| P1 | 6 | 6 | 0 | 0 |
| P2 | 25 | 24 | 16 | 16 |
| P3 | 8 | 6 | 4 | 4 |
| P4 | 17 | 14 | 9 | 9 |
| P5 | 12 | 7 | 3 | 3 |
| **Avg** | | **11.4** | **6.4** | **6.4** |

**SRTF:** `| P1 | P3 | P1 | P5 | P4 | P2 |` over `0 2 4 8 12 17 25`

| Proc | CT | TAT | WT | RT |
|---|---|---|---|---|
| P1 | 8 | 8 | 2 | 0 |
| P2 | 25 | 24 | 16 | 16 |
| P3 | 4 | 2 | 0 | 0 |
| P4 | 17 | 14 | 9 | 9 |
| P5 | 12 | 7 | 3 | 3 |
| **Avg** | | **11.0** | **6.0** | **5.6** |

**Comparison:** FCFS 9.2 / 14.2 / 9.2 (5 dispatches); SJF 6.4 / 11.4 / 6.4
(5); SRTF 6.0 / 11.0 / 5.6 (6) — avg WT / TAT / RT. With 0.5 ms dispatch
latency: SRTF 25/28 ≈ 89.3 %, FCFS 25/27.5 ≈ 90.9 %.

**Supporting examples (scenes 3, 4, 7, 9):**
- Idle gap, FCFS on P1(0,3), P2(5,4), P3(6,2): `P1 0–3 | idle 3–5 | P2 5–9 |
  P3 9–11`; averages TAT 4, WT 1, RT 1; utilization 9/11 ≈ 81.8 %.
- FCFS 24, 3, 3 at t = 0: WT 0, 24, 27 → avg 17. Order 3, 3, 24: WT 6
  (for P1), 0, 3 → avg 3. Ratio 17/3 ≈ 5.67.
- SJF on 6, 8, 7, 3 at t = 0: order P4, P1, P3, P2; WT 3, 16, 9, 0 → avg 7;
  FCFS in index order avg 10.25.
- Exponential average, α = 1/2, τ̂₀ = 10, bursts 6, 4, 6, 4, 13, 13, 13 →
  τ̂ = 10, 8, 6, 6, 5, 9, 11, 12.

**Convoy simulation (scene 5, new for the video):** one CPU; `C` = CPU-bound
(10 ms CPU, 2 ms I/O, repeating), `I1–I3` = I/O-bound (1 ms CPU, 3 ms I/O,
repeating); each process has its own I/O device (no device contention); all
start ready at 0 in the order C, I1, I2, I3; 1 ms time steps; window 0–48 ms.

| Policy | CPU busy | I/O-bound device-ms busy (of 144) | CPU bursts done: C / each I |
|---|---|---|---|
| FCFS | 48/48 | 27 (18.75 %) | 3 / 3 |
| preemptive shortest-remaining-first | 48/48 | 102 (≈ 70.8 %) | 1 / 12 |

FCFS CPU timeline: `C 0–10 | I1 10–11 | I2 11–12 | I3 12–13 | C 13–23 |
I1 23–24 | I2 24–25 | I3 25–26 | C 26–36 | I1 I2 I3 36–39 | C 39–48`.

**How the numbers were verified.** All schedules were generated by a
`python3` unit-time simulator (`scratchpad/videos/sched.py`, driver `run.py`)
implementing FCFS, SJF (ties: earlier arrival) and SRTF (running job keeps
the CPU on ties); every Gantt chart and every average above is its output
and matches the deck's tables exactly. The exponential-average sequence and
the utilization ratios were recomputed in `python3`. The convoy figures come
from a separate simulator (`scratchpad/videos/convoy2.py`) under the stated
assumptions; they are illustrative of the deck's qualitative claim, not from
the textbook — if you change the burst lengths, re-run it.

## 7. Pause-and-predict checkpoints

1. **Scene 8, frozen at t = 6:** "Ready: P2(8), P3(2), P4(5), P5(4). Who runs
   next, and in what order after that?" — *Answer:* P3, then P5, P4, P2.
2. **Scene 10, frozen at t = 2:** "P3 arrives with τ = 2; P1 has 4 ms left.
   Preempt?" — *Answer:* yes, 2 < 4. Follow-up freeze at t = 4 (P3 done):
   ready P1 (4 left), P2 (8), P4 (5) → P1 resumes; at t = 5 P5 (4) does not
   beat P1's remaining 3.
3. **Scene 11, before the table:** "Which policy gives the lowest average
   response time?" — *Answer:* SRTF, 5.6 ms (SJF 6.4, FCFS 9.2).

## 8. Misconceptions to address on screen

- **"The order doesn't matter — total work is the same."** → Scene 4's
  stacked charts: same finish time 30, avg WT 17 vs. 3.
- **"WT = start time."** → Scene 3's mini timeline: WT = TAT − τ, and for a
  preempted job (SRTF P1) WT = 2 but RT = 0.
- **"SRTF compares the newcomer with the running job's *original* burst."**
  → Scene 10's remaining-time badges.
- **"Non-preemptive SJF can switch to a shorter job that arrives."** →
  Scene 8: P1 keeps the CPU to 6 although P3 (2) arrived at 2.
- **"FCFS is fair, so it's efficient."** → Scene 5's empty device row.
- **"SJF is always the best choice."** → Scene 9 (unknown τ) and scene 12
  (starvation).
- **"Idle time can be left out of the chart."** → Scene 3's marked idle
  block (knowledge-base Gantt rule).

## 9. Visual assets

- **Reuse (from `Slides/CS502/08-cpu-scheduling-basics.tex`; Gantt TikZ was
  generated from the verified simulator):**
  - Gantt TikZ in **Worked Example: Metrics From a Schedule With an Idle
    Gap** (scene 3).
  - Gantt TikZ in **Worked Example: One Long Job First** and **Same Jobs,
    Different Arrival Order** (scene 4).
  - Frame **The Convoy Effect** (bullet text for scene 5 narration).
  - Gantt TikZ + tables in **The Lab Server Under FCFS**, **The Lab Server
    Under Non-preemptive SJF**, **The Lab Server Under SRTF** (scenes 6, 8,
    10) — arrival arrows already drawn on the axis.
  - Gantt TikZ in **Worked Example: SJF With Simultaneous Arrivals**
    (scene 7) and frame **Why SJF Is Optimal**.
  - Table in **Worked Example: Predicting Bursts With α = 1/2** (scene 9).
  - Frames **Three Policies, One Workload**, **The Price of Short-First:
    Starvation**, **Common Misconceptions**.
  - `Notes/CS502/08-cpu-scheduling-basics-notes.tex` for the optimality
    argument in prose.
- **New:**
  - Manim/Motion Canvas "scheduler cam": a clock, a sorted ready-queue panel
    with remaining-time badges, and a Gantt bar that grows in real time; one
    template reused for scenes 6, 8 and 10.
  - Convoy two-row timeline (CPU row + device-busy row) driven by
    `convoy2.py` output.
  - Exponential-average line chart (scene 9).
  - `/render-lecture-video` can render the three lab-server frames as a
    step sequence for a quick cut.
- **Live demo (optional, 40 s):** `python3 run.py` in
  `scratchpad/videos/` prints all three lab-server schedules — show it as
  "you can check any Gantt chart with 60 lines of Python".

## 10. On-screen text & notation

- Processes `P1`–`P5`; times in ms.
- First-use definitions required on screen: CPU burst, I/O burst, preemptive vs. non-preemptive, dispatch latency, utilization, throughput, convoy effect, starvation, exponential averaging.
- Metrics spelled out at first use: **turnaround time (TAT)**, **waiting
  time (WT)**, **response time (RT)**; burst **τ**; **AT / ST / CT**.
- Formulas: `TAT = CT − AT`, `WT = TAT − τ`, `RT = ST − AT`;
  `τ̂_{n+1} = α τ_n + (1 − α) τ̂_n`.
- Algorithm names exactly `FCFS`, `SJF`, `SRTF` (no punctuation).
- Gantt charts: time left → right; process label in the bar; arrivals
  marked on the axis with arrows, never inside the bars; idle gaps labelled.

## 11. End-of-video quiz

Workload for Q1–Q3 (new, original): `A` (AT 0, τ 7), `B` (2, 4), `C` (4, 1),
`D` (5, 4), times in ms. All answers computed with the simulator.

1. **(Numerical)** FCFS average WT? — **Answer: 4.75 ms** (WT 0, 5, 7, 7).
2. **(Numerical)** Non-preemptive SJF average WT? — **Answer: 4.0 ms**
   (order A, C, B, D; WT 0, 6, 3, 7 for A, B, C, D).
3. **(Numerical, GATE-style)** SRTF average WT and Gantt chart? —
   **Answer: 3.0 ms**; `A 0–2 | B 2–4 | C 4–5 | B 5–7 | D 7–11 | A 11–16`
   (WT A 9, B 1, C 0, D 2).
4. **(Numerical)** α = 1/2, τ̂₀ = 10 ms, measured bursts 6, 4, 6 ms. What is
   τ̂₃? — **Answer: 6 ms** (8, then 6, then 6).
5. **(MCQ)** Which statement is true?
   (a) SRTF can starve a long process (b) FCFS can starve a long process
   (c) non-preemptive SJF preempts on a shorter arrival (d) SJF needs no
   knowledge of burst lengths — **Answer: (a).**

## 12. References

- Silberschatz, Galvin & Gagne, *Operating System Concepts*, 8e — Ch. 5
  §5.1 basic concepts, p. 183; §5.2 scheduling criteria, p. 187; §5.3.1
  FCFS, p. 188 (convoy effect, pp. 188–189); §5.3.2 SJF and SRTF, p. 189.
- Stallings, *Operating Systems: Internals and Design Principles*, 6e —
  Ch. 9 §9.1 types of scheduling, p. 406; §9.2 scheduling algorithms (FCFS,
  SPN, SRT), p. 410.

## 13. Production notes

- **Highest-value video in Unit I.** Spend the animation budget on the
  "scheduler cam" template; it is reused in V09.
- **Freeze-frames at decision points** (t = 6 in SJF; t = 2, 3, 4, 5 in
  SRTF) with a 2 s hold and a sound cue — this is the skill being taught.
- **Pacing warning:** three full metric tables is a lot; fill each table row
  as the process completes rather than all at the end, and keep only the
  averages on screen afterwards.
- **Checkpoint 2 nuance:** at t = 3 P1 is *not running* (P3 is), so the
  comparison is P4 vs. the running P3 (remaining 1) — keep the freeze at
  t = 2 and t = 4/5 to avoid confusing viewers.
- **If over length:** cut scene 7's optimality swap to 20 s and scene 9 to
  one sentence plus the chart; keep scene 5 (convoy) — it's the title topic.
- **Accessibility:** captions; Gantt bars labelled with process names (do
  not rely on colour); colour-blind-safe palette (blue/gold/grey as in the
  deck); preemption flash also shown as a vertical dashed line.
