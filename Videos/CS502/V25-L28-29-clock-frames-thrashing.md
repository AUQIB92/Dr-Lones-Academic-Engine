# V25 · L28–29 — The Clock, the Frame Budget, and the Thrashing Cliff

> **Lecture:** CS502 L28–29 `28-page-replacement-advanced` + `29-frame-allocation-thrashing` · **Unit:** III · **Target length:** 16–18 min · **Series position:** 25/28
> **Format:** screen-capture animation (circular clock buffer, frame strips, utilisation curve) · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

The clock algorithm is literally a rotating hand: it sweeps, clears bits, skips pages, and stops. Drawn as a static table, students lose the hand position and get the trace wrong at the first wrap-around. Thrashing is a feedback loop that runs away over time: the CPU looks idle, the OS adds more processes, faults rise, the CPU looks idler still. Only an animation of that loop, next to the falling utilisation curve, shows why "more multiprogramming" turns from cure into disease. The working set is a sliding window, and a window has to slide to make sense.

## 2. Learning outcomes

After watching, the student can:
- [Understand] Explain why exact LRU is too expensive and how a hardware reference bit approximates it.
- [Apply] Trace the second-chance (clock) algorithm on a reference string, tracking the hand and the reference bits, and count faults.
- [Apply] Rank pages into the four enhanced second-chance classes (ref, dirty) and pick the victim.
- [Apply] Compute equal and proportional frame allocations; distinguish global vs. local replacement.
- [Analyze] Explain thrashing with the CPU-utilisation curve, compute a working set for a window Δ, and decide whether a system will thrash from total demand D vs. available frames m; describe page-fault-frequency control.

## 3. Prerequisites

- Basic computer fundamentals: RAM, disk, a "bit" in hardware.
- **V06** — process states, degree of multiprogramming, long-term (admission) scheduling.
- **V08–V09** — CPU utilisation as a scheduling goal.
- **V20** — pages, frames, page-table entries.
- **V23** — demand paging, page fault, fault service time, locality.
- **V24** — FIFO, Optimal, LRU; reference strings; the cost of a fault; Belady's anomaly.

## 4. Hook (first 30–45 s)

> *A server's CPU is at 20 % utilisation. The OS sees idle cycles and does the obvious thing: it admits more programs to keep the CPU busy. Utilisation drops to 10 %. It admits more. It drops to 3 %. The disk light never turns off.*
>
> "The operating system is making this worse, and it is following a perfectly reasonable rule. By the end of this video you'll know the rule, why it fails, and the one number the OS should have looked at instead."

On screen: a CPU gauge falling as process icons are added; a disk LED stuck on; the label "thrashing" is held back until scene 10.

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:40 | Hook | Falling CPU gauge, stuck disk LED | The OS adds load and gets less work. Promise: the rule, why it fails, the fix |
| 2 | 0:40–1:00 | Recap: V24 | FIFO / OPT / LRU fault counters on the V24 string: 12 / 8 / 10 | 15 s: same string, three victims rules; LRU is close to OPT and has no Belady's anomaly. Today's string is that same one |
| 3 | 1:00–2:15 | The cost of exact LRU | Every memory reference writes a timestamp into a page-table entry (counter ticks) or moves a node to the top of a stack | Exact LRU needs work on **every** reference, not just on faults: billions per second. Hardware won't do it |
| 4 | 2:15–3:00 | The reference bit | A page-table entry grows a bit `R`; any access sets it to 1; only the OS clears it | **Term card — Reference bit (R):** hardware sets it on any read or write. Cheap, one bit; tells "used since last cleared", not "when" |
| 5 | 3:00–6:30 | Second-chance / clock, traced | 3 frames drawn on a circle; a hand arrow; each slot shows page + R. Reference tape `1 2 3 2 4 1 2 5 1 2 3 4 2 5 1` plays; at each fault the hand sweeps: R=1 → clear to 0 and skip; R=0 → evict | **Term card — Second-chance (clock):** FIFO order, but a page with R=1 gets one more lap. Loaded pages start with R=1. Highlight ref 5 (full sweep, all bits cleared, back to frame 0) and ref 11 (a full lap clears every bit, then page 1 is evicted). Result: **12 faults** |
| 6 | 6:30–7:00 | Pause-and-predict 1 | Freeze at ref 5 with all three R=1 | Which page goes? (Answer §7) |
| 7 | 7:00–8:15 | Enhanced second chance; LFU/MFU | 2×2 grid (R, D): (0,0) best victim → (1,1) worst; four sample pages drop into cells; LFU/MFU counters shown briefly | **Term card — Dirty (modify) bit D:** set on write; a dirty victim must be written to disk first, so it costs two I/Os. Victim = lowest non-empty class. **LFU** evicts smallest count; **MFU** evicts largest. Both are rarely used: counts remember old history |
| 8 | 8:15–10:00 | Frame allocation | A pool of 64 frames splits into three processes of 20, 60, 120 pages: equal 21/21/21 + 1 spare vs. proportional 6/19/38 + 1 spare | **Term card — Equal allocation:** m/n each. **Proportional:** aᵢ = (sᵢ / S)·m. Show formula, then the numbers. Mention a minimum per process set by the instruction set |
| 9 | 10:00–10:45 | Global vs. local | Victim search circle spans all frames (global) vs. only own frames (local) | **Global:** take from anyone (better throughput, but one process's behaviour hurts others). **Local:** only own frames (predictable, may waste frames) |
| 10 | 10:45–12:45 | Thrashing | Utilisation vs. degree of multiprogramming curve rises then falls off a cliff; animated loop: low CPU → admit more → fewer frames each → more faults → processes wait on disk → lower CPU | **Term card — Thrashing:** spending more time paging than executing. The OS's rule "low CPU means admit more" reads disk wait as idleness. With global replacement, processes steal frames from each other and the fault rate climbs for everyone |
| 11 | 12:45–13:05 | Recap: locality (V23) | V23 heat-map bands return | 15 s: programs use a small, slowly moving set of pages |
| 12 | 13:05–15:00 | Working-set model | String of 20 refs; window Δ = 4 slides; current set printed under it; size graph 3 → 4 → 3 → 2 → 1 | **Term card — Working set WS(t, Δ):** distinct pages referenced in the last Δ references. Δ too small misses the locality; too large merges localities. Total demand D = Σ WSSᵢ; if D > m, suspend a process |
| 13 | 15:00–15:30 | Pause-and-predict 2 | Three processes WSS 3, 4, 5; m = 10 | Thrash? (Answer §7) |
| 14 | 15:30–16:30 | Page-fault frequency | Fault-rate axis with an upper and lower band; a process dot above the band gains a frame, below it gives one up | **Term card — PFF:** control the fault rate directly. Rate above upper bound → give frames; below lower bound → take frames; no free frames → suspend a process |
| 15 | 16:30–17:30 | Recap + bridge | Summary chain: R bit → clock → (R, D) → allocation → thrashing → WS / PFF. Then the "write dirty page to disk" arrow zooms into a device controller | Bridge to V26: every one of these fixes ends with a disk transfer. How does the OS actually talk to a disk, a keyboard, a network card? Next: I/O systems |

### Scene-by-scene beat detail

**Scene 1 — Hook.**
- Process icons drop into a "RAM" box; with each one, the CPU gauge needle falls.
- Disk LED flashes faster until solid.
- Beat: "The OS is following a sensible rule." Withhold the word "thrashing".

**Scene 2 — Recap V24, 15–20 s.**
- Three counters from V24 on the same 15-reference string, with LRU marked "best you can do without seeing the future".

**Scene 3 — Cost of exact LRU.**
- Two implementations side by side: per-entry counters (a clock value copied on every access) and a doubly-linked stack (node moved to top on every access).
- A "references per second" counter spins into billions.
- Beat: "Paying on every access, to save work on rare faults, is a bad trade."

**Scene 4 — Reference bit.**
- PTE drawn as a row: frame #, `v/i` (recap V23), new `R`.
- An access lights R; then the OS "periodically" sweeps and clears it.
- Beat: "One bit tells us *whether*, not *when*. Clock turns that bit into an ordering."

**Scene 5 — Clock trace.**
- Circle of 3 slots, hand starts at slot 0.
- Rule card pinned: "On fault: while R(hand) = 1 → set R = 0, advance. Replace at hand, set R = 1, advance."
- Rule card 2: "On hit: set R = 1; hand does not move."
- Refs 1–3: fill slots 0, 1, 2; hand back at 0.
- Ref 4 (page 2): hit; R already 1.
- Ref 5 (page 4): hand clears all three bits in one lap, returns to slot 0, evicts page 1.
- Ref 11 (page 3): hand at slot 1. Page 1 (R = 1) → clear; page 2 (R = 1) → clear; page 5 (R = 1) → clear; back at slot 1, page 1 now R = 0 → evict. Narrate from §6.1 row 11.
- Running counter ends at 12.
- Beat: "On this string clock ties with FIFO (12) and LRU wins (10). Clock is an approximation, and here its bits were all 1 too often, so it fell back to FIFO."

**Scene 7 — Enhanced second chance.**
- Grid cells labelled with cost: (0,0) "not used, clean: free to drop"; (0,1) "not used, dirty: needs a write"; (1,0) "used recently, clean"; (1,1) "used and dirty: worst".
- Pages A (0,1), B (1,0), C (0,0), D (1,1) fall in; victim C glows.
- LFU/MFU: two counters on three pages; 5-second treatment only.

**Scene 8 — Frame allocation.**
- 64 frame tiles; equal split colours them in thirds; one grey spare.
- Proportional: formula aᵢ = (sᵢ/S)·m types out; S = 200; values 6.4, 19.2, 38.4 truncate to 6, 19, 38; one spare.
- Beat: "The big process gets most frames, as it should; the small one still gets enough to run."

**Scene 6 — Pause-and-predict 1.**
- Freeze the circle at ref 5 with all three R dots filled; countdown ring 5 s.
- Reveal: hand sweeps one full lap, hollowing every dot, then lands on page 1.

**Scene 9 — Global vs. local.**
- Same 64-tile pool; process P1 faults.
- Global: a search ring spans all tiles and picks a tile owned by P3 (tile changes colour).
- Local: the ring shrinks to P1's own 6 tiles.
- Beat: "Global is faster overall but lets one bad process hurt its neighbours; local isolates them."

**Scene 10 — Thrashing.**
- Curve drawn live as the hook's process icons are re-added.
- Feedback loop drawn as four arrows in a circle, each lighting in turn.
- Beat: "The CPU is not idle for lack of work; it's idle because everyone is waiting for the disk."

**Scene 12 — Working set.**
- Window bracket of width 4 slides along the 20-reference tape.
- Under it: WS printed; a small bar chart of |WS| builds at the bottom.
- Pause at t = 12 and toggle Δ = 2, 4, 8 to show {5, 6}, {4, 5, 6}, {1, 2, 4, 5, 6}.
- D vs. m gauge: a bar of total demand against a line at m.

**Scene 11 — Recap: locality (V23), 15 s.**
- The V23 heat-map plays once; one band is outlined and labelled "current locality".

**Scene 13 — Pause-and-predict 2.**
- Three bars (3, 4, 5) stack against a line at 10; viewer predicts before the stack crosses the line.
- Reveal: D = 12; one process fades to "suspended"; remaining D = 7, 8 or 9 (depending on which is suspended) fits.

**Scene 14 — PFF.**
- A vertical rate axis with green band; a process dot measured at 40 faults / 2000 refs = 0.02 sits inside; another at 90 / 2000 = 0.045 is above and gains a frame; one at 4 / 2000 = 0.002 is below and loses one.

**Scene 15 — Recap + bridge.** Chain animates; final zoom on a disk-write arrow into a controller box labelled "?".

## 6. Worked example (verified)

### 6.1 Clock (second-chance) trace

Reference string (same as V24): `1 2 3 2 4 1 2 5 1 2 3 4 2 5 1`, 3 frames. Convention: a newly loaded page gets R = 1; on a hit R is set to 1 and the hand does not move; on a fault the hand clears and skips every R = 1 page, replaces the first R = 0 page, sets its R = 1 and advances one slot. Frames are listed slot 0, 1, 2; R bits in the same order; "hand" = slot the hand points to *after* the reference.

| # | Ref | Result | Frames | R bits | Hand after |
|---|---|---|---|---|---|
| 1 | 1 | F | 1 – – | 1 0 0 | 1 |
| 2 | 2 | F | 1 2 – | 1 1 0 | 2 |
| 3 | 3 | F | 1 2 3 | 1 1 1 | 0 |
| 4 | 2 | hit | 1 2 3 | 1 1 1 | 0 |
| 5 | 4 | F, evict 1 | 4 2 3 | 1 0 0 | 1 |
| 6 | 1 | F, evict 2 | 4 1 3 | 1 1 0 | 2 |
| 7 | 2 | F, evict 3 | 4 1 2 | 1 1 1 | 0 |
| 8 | 5 | F, evict 4 | 5 1 2 | 1 0 0 | 1 |
| 9 | 1 | hit | 5 1 2 | 1 1 0 | 1 |
| 10 | 2 | hit | 5 1 2 | 1 1 1 | 1 |
| 11 | 3 | F, evict 1 | 5 3 2 | 0 1 0 | 2 |
| 12 | 4 | F, evict 2 | 5 3 4 | 0 1 1 | 0 |
| 13 | 2 | F, evict 5 | 2 3 4 | 1 1 1 | 1 |
| 14 | 5 | F, evict 3 | 2 5 4 | 0 1 0 | 2 |
| 15 | 1 | F, evict 4 | 2 5 1 | 0 1 1 | 0 |
| | | **12 faults** | | | |

Comparison on the same string, 3 frames: FIFO 12, LRU 10, clock 12 (matches V24's FIFO and LRU numbers).
Row 11 detail for narration: hand at slot 1 (page 1, R = 1) → clear, advance to slot 2 (page 2, R = 1) → clear, advance to slot 0 (page 5, R = 1) → clear, advance to slot 1 (page 1, now R = 0) → evict page 1. All three bits were cleared on the way, so pages 5 and 2 end with R = 0.

### 6.2 Enhanced second chance

Pages with (R, D): A (0,1), B (1,0), C (0,0), D (1,1). Class order (0,0) < (0,1) < (1,0) < (1,1). Victim: **C**.

### 6.3 Frame allocation

m = 64 frames; process sizes s = 20, 60, 120 pages; S = 200.
- Equal: 64 / 3 = 21.33 → 21 each, 1 frame spare.
- Proportional: aᵢ = sᵢ/S · m = 6.4, 19.2, 38.4 → 6, 19, 38 (sum 63), 1 frame spare.

### 6.4 Working set, Δ = 4

String (20 refs): `1 2 1 3 2 1 4 5 6 4 5 6 4 4 4 7 7 7 7 7`.

| t | Window (last 4) | WS | Size |
|---|---|---|---|
| 4 | 1 2 1 3 | {1,2,3} | 3 |
| 7 | 3 2 1 4 | {1,2,3,4} | 4 |
| 9 | 1 4 5 6 | {1,4,5,6} | 4 |
| 12 | 6 4 5 6 | {4,5,6} | 3 |
| 15 | 6 4 4 4 | {4,6} | 2 |
| 16 | 4 4 4 7 | {4,7} | 2 |
| 19 | 7 7 7 7 | {7} | 1 |

At t = 12 with different Δ: Δ = 2 → {5,6}; Δ = 4 → {4,5,6}; Δ = 8 → {1,2,4,5,6}.
Thrashing check: three processes with WSS 3, 4, 5 → D = 12 > m = 10 → thrashing likely; suspend one.

### 6.5 Page-fault frequency

Band: lower 0.005, upper 0.03 (faults per reference, chosen for the example). 40/2000 = 0.02 (inside, no change); 90/2000 = 0.045 (above → add a frame); 4/2000 = 0.002 (below → remove a frame).

**Verification.** Every number was computed by `python3` in `/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/`. `v25.py` simulates clock (with the conventions above), FIFO and LRU, and computes the allocations and working sets; `v25ws.py` prints every working-set window for Δ = 4, the Δ = 2/4/8 comparison and the D vs. m check; `v25b.py` computes the PFF rates, the enhanced-class victim and the quiz traces.

## 7. Pause-and-predict checkpoints

1. **Scene 6, ref 5 (page 4), all R = 1: which page is evicted?** Answer: the hand clears all three bits in one full lap, returns to slot 0, finds page 1 with R = 0 and evicts it. When every bit is 1, clock degenerates to FIFO.
2. **Scene 13: WSS 3, 4, 5 with m = 10 — thrash?** Answer: yes, D = 12 > 10. Suspend (swap out) one process so the rest fit.
3. **Scene 8, before the reveal: under proportional allocation, how many frames does the 120-page process get?** Answer: 38.4 → 38.

## 8. Misconceptions to address on screen

- "Clock is just LRU." → Side-by-side counters on the running string: clock 12, LRU 10.
- "The hand moves on every reference." → Ref 4 (hit): hand stays still, only the R bit is touched.
- "Thrashing means the CPU is overloaded." → Utilisation gauge near zero while the disk queue overflows.
- "Adding processes always raises CPU utilisation." → The curve's falling edge.
- "Bigger Δ is always better." → Δ = 8 at t = 12 includes pages 1 and 2 that the program left behind.
- "A dirty page is a bad victim because it is 'in use'." → Dirty means *modified*, not *recently used*; it costs an extra disk write.

## 9. Visual assets

- **Reuse:** no `Slides/CS502` decks for L28/L29 yet. Reuse the frame-strip style and F/hit marks from the V24 brief, the V23 locality heat-map, and the palette of `Slides/CS502/08-cpu-scheduling-basics.tex`.
- **New:**
  - Circular clock buffer with rotating hand and per-slot R bits (Manim).
  - (R, D) 2×2 class grid.
  - 64-tile frame pool splitting equal vs. proportional (PowerPoint morph).
  - Utilisation vs. degree-of-multiprogramming curve plus feedback-loop circle.
  - Sliding Δ window with live |WS| bar chart.
  - PFF band diagram.
- **Live demo (optional, 40 s):** on Linux, `vmstat 1` while running a memory-hungry program shows `si/so` (swap in/out) climbing and `us` CPU falling.

## 10. On-screen text & notation

- Reference bit R; dirty (modify) bit D; second-chance / clock; hand; enhanced second chance classes (R, D).
- LFU, MFU.
- m = total frames; sᵢ = size of process i; S = Σ sᵢ; aᵢ = (sᵢ/S)·m.
- Global vs. local replacement.
- Thrashing; degree of multiprogramming.
- WS(t, Δ), WSSᵢ = |WSᵢ|, D = Σ WSSᵢ; condition D > m.
- PFF upper/lower bounds.

## 11. End-of-video quiz

1. **(Numerical)** Clock with 3 frames on `2 3 2 1 5 2 4 5 3 2 5 2` (same conventions as §6.1). How many faults? **Answer: 8.**
2. **(MCQ)** Under enhanced second chance, which page is replaced first? (a) R=1, D=0 (b) R=0, D=1 (c) R=1, D=1 (d) R=0, D=0. **Answer: (d).**
3. **(Numerical)** 100 frames, processes of 15, 45 and 90 pages, proportional allocation. **Answer:** 10, 30, 60.
4. **(MCQ)** Thrashing is best detected by: (a) low CPU utilisation alone (b) high page-fault rate together with low CPU utilisation (c) a high number of ready processes (d) a large TLB. **Answer: (b).**
5. **(Short)** Why can global replacement spread thrashing from one process to others? **Answer:** a faulting process takes frames from other processes, which then fault more themselves.

*All quiz numbers computed in `v25b.py` (same directory as §6).*

## 12. References

- Silberschatz, *Operating System Concepts* 8e, Ch 9: LRU (p.375), LRU approximation / reference and second-chance bits (p.378), allocation of frames (p.382), thrashing (p.386), working-set model (p.388).
- Stallings, *Operating Systems* 6e, Ch 8: page replacement policies (p.368), thrashing (p.382).
- Bovet & Cesati, *Understanding the Linux Kernel*, Ch 17 (PFRA, LRU lists) as the production counterpart; chapter-level only.

## 13. Production notes

- Scene 5 is the longest and the most error-prone: script the hand positions from §6.1 and do not improvise. Hold 1 s on every sweep step.
- Colour R = 1 and R = 0 with shape as well as colour (filled vs. hollow dot) for colour-blind viewers.
- If over length: cut LFU/MFU to a single card, cut the live demo, and drop scene 9 to a voice-over over scene 8's tiles.
- Captions on; read every formula symbol by symbol.
- Keep the 15-reference string identical to V24 so viewers can compare.
