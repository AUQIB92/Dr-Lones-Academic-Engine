# V17 · L18 — Deadlock: When Everyone Waits for Everyone

> **Lecture:** CS502 L18 `18-deadlock-model-characterization` · **Unit:** II · **Target length:** 13–15 min · **Series position:** 17/28
> **Format:** screen-capture animation (traffic grid + resource-allocation graphs) · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

Deadlock is a state that builds up over time: each process takes one resource, then asks for
another, and only at the last request does the system freeze. A slide shows the final frozen
picture; an animation can show the arrows being added one at a time, so the viewer *sees* the
cycle close. The second thing students get wrong is the rule "cycle means deadlock". It is true
only when every resource has one instance. Showing two graphs with the same cycle shape, one
frozen and one that untangles when a process finishes, is the clearest way to teach that rule.

## 2. Learning outcomes

After watching, the student can:
- [Remember] Define deadlock and state the request–use–release model of resource use.
- [Remember] State the four necessary conditions for deadlock (mutual exclusion, hold and wait, no preemption, circular wait).
- [Understand] For each condition, give an example of a rule that breaks it.
- [Apply] Draw a resource-allocation graph from a description of holdings and requests.
- [Analyze] Decide whether a resource-allocation graph is deadlocked, both for single-instance resources (cycle ⇔ deadlock) and for multiple-instance resources (cycle is necessary but not sufficient).

## 3. Prerequisites

- Basic computer fundamentals: what a printer, a file and memory are; the idea of a program waiting for a device.
- V05–V06: process, process states, the `waiting` (blocked) state.
- V10–V11: critical section, mutual exclusion.
- V13: semaphores; `wait` blocks a process when the value is not positive.
- V15–V16: classical problems; the dining-philosophers deadlock (every philosopher holds one chopstick).

## 4. Hook (first 30–45 s)

Top-down animation of a four-way junction. Four cars, one from each side, each pulls halfway
into the box.

> "North car wants to go straight. It needs the square in front of it, but the East car is
> sitting there. East needs the square held by South. South needs West's. West needs North's.
> Nobody is broken. Nobody is breaking a rule. And nobody will ever move.
> This is gridlock. In an operating system it is called *deadlock*, and it happens with
> printers, files and locks exactly the way it happens with cars.
> Question: what has to be true for this to happen, and if we take away one ingredient, does
> it go away?"

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:45 | Hook | Junction with four quadrants Q1–Q4; four cars advance one at a time and each takes one quadrant; dotted "wants" arrows appear; a clock icon spins | Each car holds one square and wants the next one. Nobody moves. Name it: deadlock |
| 2 | 0:45–1:05 | Recap V13, V16 | Two semaphores `S` and `Q` both = 1; P0 `wait(S)` then `wait(Q)`, P1 `wait(Q)` then `wait(S)`; both tokens end up in each other's queue. Then the dining table with five philosophers each holding one chopstick | **Recap (15–20 s).** We already met this twice: two semaphores taken in opposite order (V13), and five philosophers each holding a left chopstick (V16). Today we give it a precise model |
| 3 | 1:05–2:00 | Definition | Text card: "A set of processes is **deadlocked** when every process in the set is waiting for an event that only another process in the set can cause" | Define **deadlock**. Stress "set": one process waiting for a slow disk is *not* deadlock. Contrast with **starvation** (one process waits indefinitely while others progress) |
| 4 | 2:00–3:15 | System model | Resource types as boxes with dots: `R1` printer (1 dot), `R2` tape drive (2 dots), `R3` mutex lock (1 dot). Process P1 walks through three gates: **request** → **use** → **release**; if the request cannot be granted, P1 moves to the `waiting` queue | Define **resource type** and **instance** (identical units; any instance satisfies a request). The protocol: request (wait if unavailable), use, release. Requests and releases are system calls (`open`/`close`, `wait`/`signal`, `malloc`/`free`) |
| 5 | 3:15–4:15 | The four conditions | Four tiles light one by one over the frozen junction: mutual exclusion (one car per square); hold and wait (each car holds a square while waiting); no preemption (no tow truck); circular wait (arrows form a ring) | Define each **necessary condition**. All four must hold at the same time for deadlock to happen. "Necessary" means: remove any one and deadlock cannot occur |
| 6 | 4:15–6:15 | Breaking each condition | Same junction, four mini-replays, each with one tile turned off. (1) Mutual exclusion: a read-only file shared by many readers, no waiting. (2) Hold and wait: a car must reserve *both* squares before entering; process requests all resources at start. (3) No preemption: a tow truck removes a waiting car; OS takes resources back from a waiting process. (4) Circular wait: number the squares Q1–Q4, every car must take squares in increasing order; the ring cannot close | One concrete breaker per condition; state the cost of each (some resources cannot be shared; requesting all at start wastes resources; preempting a printer mid-page is unsafe; ordering needs discipline). This is the seed of *prevention* in V18 |
| 7 | 6:15–6:45 | Pause & predict 1 | Card: "A system makes every process request all its resources before it starts. Which condition is broken?" | Pause. Answer: hold and wait |
| 8 | 6:45–8:00 | Resource-allocation graph notation | Legend builds: circle = process; rectangle = resource type, dots = instances; **request edge** P → R (arrow to the rectangle); **assignment edge** R → P (arrow from a dot). Animation: P1 requests R1 (request edge appears), the grant flips it to an assignment edge | Define **resource-allocation graph (RAG)**, its two vertex kinds and two edge kinds. A request edge becomes an assignment edge when granted; the edge disappears on release |
| 9 | 8:00–9:30 | RAG A: single instance, deadlock | Graph A (§6) built edge by edge: R1→P1, R2→P2, R3→P3 (assignments), then P1→R2, P2→R3, P3→R1 (requests). The cycle P1 → R2 → P2 → R3 → P3 → R1 → P1 is highlighted in a moving pulse | Every resource has one instance, every process in the cycle waits for the next. **With single instances, a cycle means deadlock**, and no cycle means no deadlock. Try to "finish" any process: none can, all three are stuck |
| 10 | 9:30–11:30 | RAG B: multiple instances, cycle but no deadlock | Graph B (§6): R1 has 2 dots, R2 has 1. R2→P1, R1→P2, R1→P3; P1→R1, P2→R2. Cycle P1 → R1 → P2 → R2 → P1 pulses. Then the *reduction*: P3 requests nothing, finishes, its R1 dot returns (P3 fades); P1's request is granted, P1 finishes, releases R1 and R2; P2 gets R2 and finishes. Banner: "cycle, but no deadlock" | Same cycle shape as A. Difference: R1 has a second instance held by P3, who is *outside* the cycle and can finish. **With multiple instances, a cycle is necessary but not sufficient.** Method: repeatedly remove a process whose requests can all be met, return its resources; if every process is removed, no deadlock. Order here: P3, P1, P2 |
| 11 | 11:30–12:15 | Pause & predict 2 | Graph B with one new edge: P3 → R2. "Deadlock now?" | Pause. Answer: yes. Now nobody can finish: P1 waits for R1 (both instances held by P2 and P3), and P2 and P3 both wait for R2 (held by P1). Two cycles; reduction removes nobody (§6) |
| 12 | 12:15–13:15 | Four ways to handle deadlock | Four doors: **prevention** (make one condition impossible, scene 6), **avoidance** (check each request and refuse ones that could lead to deadlock; needs advance knowledge of maximum needs), **detection + recovery** (let it happen, find the cycle, kill or roll back a process), **ignore** (assume it is rare, reboot if it happens; used by most general-purpose OSs) | Define each strategy in one sentence and the price: prevention lowers resource use; avoidance needs future knowledge; detection costs CPU time and recovery is messy; ignoring is cheap but leaves the problem to users |
| 13 | 13:15–14:00 | Recap + bridge | Summary cards: definition · request/use/release · four conditions + breakers · RAG rules (single vs multiple). Final frame: a table with columns `Allocation`, `Max`, `Need`, `Available` sliding in, labelled "Banker's algorithm" | Four conditions; cycles are the evidence; single vs multiple instances. Next, V18: avoidance in practice, with the Banker's algorithm |

## 6. Worked example (verified)

All graphs below were checked by a cycle-finder and by graph reduction (repeatedly remove a
process whose outstanding requests can be met from available instances, returning its holdings).

**Graph A — single instance of each resource (deadlock).**

| Resource | Instances | Assigned to | Requested by |
|---|---|---|---|
| R1 | 1 | P1 | P3 |
| R2 | 1 | P2 | P1 |
| R3 | 1 | P3 | P2 |

Edges: R1→P1, R2→P2, R3→P3, P1→R2, P2→R3, P3→R1.
Cycle found: **P1 → R2 → P2 → R3 → P3 → R1 → P1** (one cycle).
Available = (0, 0, 0). Reduction: **no process can finish; P1, P2, P3 deadlocked.**

**Graph B — R1 has 2 instances, R2 has 1 (cycle, no deadlock).**

| Resource | Instances | Assigned to | Requested by |
|---|---|---|---|
| R1 | 2 | P2, P3 | P1 |
| R2 | 1 | P1 | P2 |

Edges: R2→P1, R1→P2, R1→P3, P1→R1, P2→R2.
Cycle found: **P1 → R1 → P2 → R2 → P1**.
Available = (R1: 0, R2: 0). Reduction:
1. P3 has no outstanding request → finishes, releases one R1 → Available R1 = 1.
2. P1's request for R1 can be met → finishes, releases R1 and R2 → Available R1 = 2, R2 = 1.
3. P2's request for R2 can be met → finishes.
Finish order **P3, P1, P2**: **no deadlock**, although the graph has a cycle.

**Graph B′ — Graph B plus request edge P3 → R2 (checkpoint 2).**
Cycles found: **P1 → R1 → P2 → R2 → P1** and **P1 → R1 → P3 → R2 → P1**.
Reduction: no process can finish (P1 needs R1, none free; P2 and P3 need R2, none free):
**deadlock of P1, P2, P3.**

**Graph C — no cycle (used as a contrast in scene 8).** R1, R2 single instance; R1→P1, R2→P2,
P1→R2. No cycle; reduction order **P2, P1**: no deadlock.

**Graph B as a table** (shown for 10 s at the end of scene 10, as a preview of V18's matrices).
Columns are (R1, R2).

| Process | Allocation | Request |
|---|---|---|
| P1 | (0, 1) | (1, 0) |
| P2 | (1, 0) | (0, 1) |
| P3 | (1, 0) | (0, 0) |

`Available` = (0, 0). Reduction reads the table row by row: P3's request (0, 0) ≤ (0, 0), so P3
finishes and `Available` becomes (1, 0); then P1's request (1, 0) ≤ (1, 0); P1 finishes and
`Available` becomes (2, 1); then P2's request (0, 1) fits. The same steps run on tables
are the detection algorithm, and with `Max` and `Need` added they become the Banker's safety
check in V18.

**Quiz arithmetic.** n processes each needing at most k instances of one resource type: with
n(k − 1) instances every process may hold k − 1 and wait (deadlock possible); with n(k − 1) + 1
instances someone can always finish. n = 3, k = 2 → deadlock possible with **3**, impossible
with **4**. n = 4, k = 3 → possible with **8**, impossible with **9**.

**How it was verified.** `v17_rag.py` builds each graph as an edge list, enumerates the simple
cycles by depth-first search, and runs the reduction; it also prints the Graph B tables; the printed cycles and finish orders are
the ones above. The quiz bound was computed by the same script. File in
`/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/`.

## 7. Pause-and-predict checkpoints

1. **(Scene 7, ~6:15)** All resources requested before a process starts. Which condition is broken? → **Hold and wait.**
2. **(Scene 11, ~11:30)** Graph B plus P3 → R2. Deadlock? → **Yes: two cycles and no process can finish.**
3. **(Scene 9, quick)** In Graph A, if R2 had a second free instance, would there still be deadlock? → **No: P1 gets R2, finishes, releases R1; then P3, then P2.** (Same reduction logic; say it, do not animate it.)

## 8. Misconceptions to address on screen

- "A cycle in a RAG always means deadlock." → Scene 10: Graph B, same cycle shape as A, untangles.
- "No cycle could still be a deadlock." → Scene 9 narration and Graph C: no cycle ⇒ no deadlock, for any number of instances.
- "Deadlock and starvation are the same." → Scene 3: deadlock is a frozen set; starvation is one process left behind while others run.
- "Any one of the four conditions causes deadlock." → Scene 5: all four must hold together; they are necessary, not sufficient individually.
- "Request edges point from resources to processes." → Scene 8: legend stays in the corner for scenes 8–11: request P → R, assignment R → P (from a dot).
- "Holding a resource and waiting is always a bug." → Scene 6 (2): hold and wait is normal; it only becomes dangerous together with the other three conditions.
- "Preempting any resource is safe." → Scene 6 (3): a CPU or memory can be preempted and restored; a half-printed page or a half-updated file cannot.
- "Deadlock needs at least three processes." → Scene 2 recap: two processes with two semaphores already deadlock; a cycle needs only two processes and two resources.
- "Ignoring deadlock is a lazy textbook joke." → Scene 12: it is what most general-purpose systems actually do for application-level resources.

## 9. Visual assets

- **Reuse:** `Slides/CS502/18-deadlock-model-characterization.tex` does not exist yet; follow the block/exampleblock/alertblock roles of `Slides/CS502/08-cpu-scheduling-basics.tex`. Reuse the semaphore S/Q vignette from V13 scene 11 and the dining table from V16.
- **New:**
  - Four-way junction with quadrants Q1–Q4 and four cars; four replay variants (scenes 1, 5, 6).
  - RAG builder: circles, rectangles with instance dots, request and assignment arrows; one keyframe per edge (Manim or PowerPoint morph).
  - Cycle "pulse" animation along the edges of a cycle (scenes 9–11).
  - Reduction animation: process fades, its dots fly back into the rectangle (scene 10).
  - Four-door strategy graphic (scene 12).
- **Live demo:** none. (Optional: a two-thread C program locking two mutexes in opposite order would hang; not run for this brief, so do not include its output.)

**Keyframe list for the animator** (one line = one click; graphs exactly as in §6):

- Scene 1 — K1 empty junction, quadrants Q1–Q4; K2–K5 north, east, south, west cars each take
  one quadrant; K6 four dotted "wants" arrows; K7 clock icon spins, nothing moves.
- Scene 4 — K1 rectangles R1 (1 dot), R2 (2 dots), R3 (1 dot); K2 P1 at "request" gate;
  K3 granted, moves to "use"; K4 "release", dot returns; K5 replay with R1 busy: P1 goes to the
  waiting queue.
- Scene 6 — four replays, each: K1 the tile for the broken condition greys out; K2 the cars
  move through; K3 cost caption.
  (1) shared read-only file, many readers; (2) car reserves both squares before entering;
  (3) tow truck removes a waiting car; (4) squares numbered Q1–Q4, every car takes them in
  increasing order, the ring cannot close.
- Scene 8 — K1 legend: circle, rectangle with dots; K2 request edge P1 → R1 (dashed);
  K3 grant: edge flips to solid R1 → P1 from a dot; K4 release: edge vanishes.
- Scene 9 (Graph A) — K1 P1, P2, P3 and R1, R2, R3 placed in a triangle; K2 R1 → P1;
  K3 R2 → P2; K4 R3 → P3; K5 P1 → R2; K6 P2 → R3; K7 P3 → R1; K8 cycle pulse
  P1 → R2 → P2 → R3 → P3 → R1 → P1; K9 "try to finish": each process greys and returns, stuck.
- Scene 10 (Graph B) — K1 R1 with 2 dots, R2 with 1; K2 R2 → P1; K3 R1 → P2; K4 R1 → P3
  (P3 drawn outside the loop); K5 P1 → R1; K6 P2 → R2; K7 cycle pulse P1 → R1 → P2 → R2 → P1;
  K8 P3 fades, its dot flies back (R1 available 1); K9 P1's request flips to assignment,
  P1 fades, returns R1 and R2; K10 P2 gets R2, fades; K11 banner "cycle, but no deadlock".
- Scene 11 (Graph B′) — K1 Graph B frozen at K7; K2 new dashed edge P3 → R2;
  K3 two cycles pulse in turn; K4 "try to finish" fails for all three.
- Scene 12 — K1–K4 one door per strategy opens with its one-line definition and price tag.

**Narration checks for the graph scenes** (say these sentences exactly as facts):

- Graph A: "Three resources, one instance each. Every process holds one and waits for the next."
- Graph B: "R1 has two instances. One of them belongs to P3, and P3 is not in the cycle."
- Graph B′: "Now P3 also waits, for R2. Every way out is blocked."
- Rule card after scene 10: "No cycle: no deadlock. Cycle with single instances: deadlock.
  Cycle with multiple instances: maybe; check by trying to finish processes."

## 10. On-screen text & notation

- **deadlock**, **starvation** (scene 3).
- **resource type**, **instance**, **request / use / release** (scene 4).
- **mutual exclusion**, **hold and wait**, **no preemption**, **circular wait**, "necessary condition" (scene 5).
- **resource-allocation graph (RAG)**: process = circle `Pi`; resource = rectangle `Rj` with one dot per instance; **request edge** `Pi → Rj`; **assignment edge** `Rj → Pi` (scene 8).
- **prevention**, **avoidance**, **detection and recovery**, **ignore** (scene 12).
- Matrices named as in the course: `Available`, `Max`, `Allocation`, `Need` (teaser only).
- Process names `P1`, `P2`, `P3`; resources `R1`, `R2`, `R3`.

## 11. End-of-video quiz

1. **(MCQ)** Which condition is broken by forcing every process to acquire locks in increasing lock-number order?
   (a) mutual exclusion (b) hold and wait (c) no preemption (d) circular wait
   **Answer: (d).**
2. **(MCQ)** A RAG in which every resource type has exactly one instance contains a cycle. Then:
   (a) deadlock is possible but not certain (b) the processes in the cycle are deadlocked (c) no conclusion (d) the system is in a safe state
   **Answer: (b).**
3. **(Numerical)** Three processes share 3 identical tape drives; each needs at most 2 at once. Is deadlock possible? What is the minimum number of drives that makes it impossible? **Answer: yes (each holds 1 and waits); 4 drives.**
4. **(Analyze)** R1 has 2 instances, both assigned (to P2 and P4). R2 has 1 instance, assigned to P1. P1 requests R1; P2 requests R2; P4 requests nothing. Is there a cycle? Is there deadlock? **Answer: cycle P1 → R1 → P2 → R2 → P1; no deadlock (P4 finishes, then P1, then P2).**
5. **(Short answer)** Name one reason why most desktop operating systems choose to ignore deadlock. **Answer:** prevention and avoidance reduce resource use or need future knowledge, and detection costs CPU time, while deadlocks are rare.

## 12. References

- Silberschatz, Galvin & Gagne, *Operating System Concepts*, 8th ed., Ch. 7 — system model (p.283); four necessary conditions (p.285); resource-allocation graph (p.287); methods for handling deadlocks (p.290); prevention (p.291).
- Stallings, *Operating Systems: Internals and Design Principles*, 6th ed., Ch. 6 §6.1 — principles of deadlock, four conditions, resource allocation graph (p.263).

## 13. Production notes

- **Pacing:** scenes 9 and 10 are the core. Build each graph one edge per beat and say the edge aloud ("R2 is assigned to P1") before it appears. Keep Graph A and Graph B in the same screen positions so the viewer sees that the shapes match.
- Draw instance dots large; the whole lesson of scene 10 sits in R1's second dot.
- **Over length?** Shorten scene 6 to two breakers on screen (hold and wait, circular wait) and list the other two as text; drop Graph C.
- **Under length?** Add a real-world vignette: two bank transfers locking accounts A, B in opposite order.
- **Accessibility:** request and assignment edges differ by line style (dashed request, solid assignment) as well as colour; cycle pulse uses thickness, not only colour; Okabe–Ito palette; captions on.
- **Recap scene (2):** keep it to 20 s; show the S/Q semaphore picture for 8 s and the dining table for 8 s, then cut. Viewers who skipped V13/V16 only need "we have seen everyone-waits-for-everyone before".
- **Consistency with V18:** use the same circle/rectangle/dot style and the same edge styles, since V18 opens on a RAG with claim edges.
- **Hook audio:** a single soft horn at the freeze; no music under the four-condition tiles so the definitions are clear.
- **Bridge line (final 10 s):** "Avoidance means saying no to a request that is safe *now* but could trap us later. To do that, the OS needs a ledger of who holds what and who may ask for more. That ledger is the Banker's algorithm: V18."

**Time budget** (computed from the storyboard times; see the over-length notes above for what to cut):

| Scene | Title | Seconds | Running total |
|---|---|---|---|
| 1 | Hook | 45 | 0:45 |
| 2 | Recap V13, V16 | 20 | 1:05 |
| 3 | Definition | 55 | 2:00 |
| 4 | System model | 75 | 3:15 |
| 5 | The four conditions | 60 | 4:15 |
| 6 | Breaking each condition | 120 | 6:15 |
| 7 | Pause & predict 1 | 30 | 6:45 |
| 8 | Resource-allocation graph notation | 75 | 8:00 |
| 9 | RAG A: single instance, deadlock | 90 | 9:30 |
| 10 | RAG B: multiple instances, cycle but no deadlock | 120 | 11:30 |
| 11 | Pause & predict 2 | 45 | 12:15 |
| 12 | Four ways to handle deadlock | 60 | 13:15 |
| 13 | Recap + bridge | 45 | 14:00 |

**Viewer questions to expect in comments** (answers for a pinned reply):

- Is a single process waiting for a slow printer deadlocked? No: the printer will finish; deadlock needs a set waiting on each other.
- Does a cycle in a graph with multiple instances ever mean deadlock? It can (Graph B′); use reduction to decide.
- Is the dining-philosophers deadlock a RAG cycle? Yes: five philosophers, five single-instance chopsticks, a cycle of length ten edges.
- Why not just always prevent deadlock? Each prevention rule costs utilisation or flexibility (scene 6).
