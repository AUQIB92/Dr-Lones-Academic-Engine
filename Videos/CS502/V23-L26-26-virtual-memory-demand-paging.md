# V23 · L26 — Virtual Memory and Demand Paging: Loading Only What You Touch

> **Lecture:** CS502 L26 `26-virtual-memory-demand-paging` · **Unit:** III · **Target length:** 14–16 min · **Series position:** 23/28
> **Format:** screen-capture animation (page-table + disk + CPU pipeline) · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

A page fault is a story told in time. The CPU issues an address, the hardware discovers the page is absent, control jumps into the kernel, a disk spins, a table entry flips from invalid to valid, and the very same instruction runs again as if nothing had happened. On a slide this is a numbered list of six steps; students memorise the list but cannot say *who* does each step (hardware or OS) or *why* the instruction must restart. The second thing a static slide hides is scale: a single fault costs as much time as tens of thousands of memory accesses, and only an animated "time bar" that stretches as the fault rate rises makes that visceral.

## 2. Learning outcomes

After watching, the student can:
- [Understand] Explain why virtual memory lets a program larger than RAM run, and lets more programs share RAM at once.
- [Understand] Describe the valid–invalid bit and what it means for a page to be "not resident".
- [Apply] List, in order, the steps of page-fault handling, and say which steps are done by hardware and which by the OS.
- [Apply] Compute the effective access time (EAT) for a given fault rate and fault-service time, and the largest fault rate that keeps slowdown under a target.
- [Analyze] Explain how copy-on-write makes `fork()` cheap and when a real copy finally happens.

## 3. Prerequisites

- Basic computer fundamentals: CPU, RAM vs. disk, the idea that disk is far slower than RAM.
- **V02** — interrupts and traps, dual mode (user/kernel).
- **V06** — process states (running, waiting/blocked, ready) and the context switch.
- **V07** — `fork()` creates a child that is a copy of the parent.
- **V19** — logical vs. physical address, the MMU.
- **V20** — pages, frames, the page table, page number + offset.
- **V21** — TLB and memory-access time as the baseline cost.

## 4. Hook (first 30–45 s)

> *Your laptop has 8 GB of RAM. Right now it is running a browser, an editor, a music player and a video call, and their address spaces add up to far more than 8 GB. Nothing crashed. Where is the rest?*
>
> "The answer is that most of every program isn't in RAM at all. It's sitting on disk, and the OS brings a piece in only at the instant the program touches it. Today we watch that instant, step by step, and then we measure what it costs. Spoiler: one miss in a thousand makes memory about forty times slower."

On screen: a RAM bar labelled "8 GB" with four coloured program bars stacked above it overflowing the top; the overflow drains into a disk icon. A counter "×41" is blurred in the corner and revealed at the spoiler line.

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:40 | Hook | RAM bar overflowing into disk; blurred "×41" | Programs together exceed RAM, yet run. Promise: watch a fault, then price it |
| 2 | 0:40–1:00 | Recap: paging (V20/V21) | Logical address splits into page # and offset; page-table lookup arrow to a frame; TLB box beside it | 15 s recap: address = (page, offset); table maps page → frame; TLB caches entries. Memory access time = our baseline, call it t_mem |
| 3 | 1:00–2:15 | Why virtual memory | A program's code drawn as 10 pages; error-handler pages and an unused big array greyed out; only 3 pages lit while running | **Term card — Virtual memory:** separating the address space a program *sees* from the physical RAM it *gets*. Programs use little of their code at once. Two wins: program bigger than RAM; more programs in RAM at once (higher degree of multiprogramming, recap V06) |
| 4 | 2:15–3:15 | Demand paging + valid–invalid bit | Page table gains a column "v/i"; resident pages show `v` + frame #; others show `i` and an arrow to a slot on disk (backing store) | **Term card — Demand paging:** load a page only when it is first referenced. **Term card — Valid–invalid bit:** `v` = page is legal and in a frame; `i` = not in RAM (or not part of the program at all). **Term card — Backing store / swap space:** the disk area holding non-resident pages |
| 5 | 3:15–3:35 | Recap: traps (V02) | Mini-replay: user-mode bar, a trap arrow into kernel-mode bar, return arrow | 15 s recap: a trap is a hardware-forced jump into the kernel in kernel mode; the OS handles it and returns |
| 6 | 3:35–6:15 | Page-fault handling, animated | Six-step pipeline drawn as lanes "Hardware" and "OS". (1) CPU issues `load` of page 3 → MMU finds `i` → red flash, trap. (2) OS checks the process's memory map: legal? (illegal → kill, "segmentation fault"). (3) OS finds a free frame (free-frame list pops a frame). (4) Disk read scheduled; process moves to *waiting* (V06 state diagram lights up); CPU given to another process. (5) Disk interrupt: frame filled; table entry set to `v` + frame #. (6) Process back to *ready*; instruction restarted from the beginning | **Term card — Page fault:** the trap raised when a referenced page is marked invalid. Emphasise lane colours: steps 1 & restart are hardware; 2–5 are OS. Why restart, not continue: the instruction never completed; it must see the now-valid entry. If no frame is free, a page must be evicted (forward pointer to V24/V25) |
| 7 | 6:15–6:45 | Pause-and-predict 1 | Freeze at step 4: CPU idle icon with "?" | Ask: does the CPU sit idle during the disk read? (Answer in §7) |
| 8 | 6:45–7:45 | Pure demand paging | Process starts with an all-`i` page table; reference string `0 1 2 0 1 3 0 1 2 3 0 1` plays; first touch of each page flashes red, later touches green | **Term card — Pure demand paging:** start with *zero* pages in memory. 12 references, only 4 faults (one per distinct page), given enough frames. Faults bunch at startup, then quiet down |
| 9 | 7:45–8:45 | Locality | Heat-map of addresses over time: bright narrow bands that drift (a loop, then a function, then another loop) | **Term card — Locality of reference:** programs keep touching a small, slowly moving set of pages. This is *why* demand paging works; without it, every reference would fault |
| 10 | 8:45–11:00 | EAT with page faults, worked | Formula builds term by term: EAT = (1 − p)·t_mem + p·(fault service time). Plug t_mem = 200 ns, service = 8 ms = 8,000,000 ns. Slider on p: 0 → 200 ns; 1/100,000 → 280 ns; 1/10,000 → 1000 ns; 1/1000 → 8199.8 ns. A bar stretches to 41× its original length | **Term card — p (page-fault rate):** fraction of memory accesses that fault, 0 ≤ p ≤ 1. **Term card — Effective access time:** the average cost of one memory reference. Point: the 8 ms term dominates even when p is tiny. Unit hygiene: convert ms to ns first |
| 11 | 11:00–11:45 | Pause-and-predict 2 | "Keep slowdown under 10 %: p < ?" | Viewer solves 220 = (1 − p)·200 + p·8,000,000 → p < 2.5 × 10⁻⁶, about 1 fault per 400,000 accesses |
| 12 | 11:45–12:05 | Recap: fork (V07) | Parent box duplicates into child box with a copying progress bar | 15 s recap: `fork()` makes a child with a copy of the parent's address space; often followed by `exec()` which throws that copy away |
| 13 | 12:05–13:45 | Copy-on-write | Parent and child page tables both point at the same frames, each entry stamped "read-only, COW". Child writes to page 2 → protection trap → OS copies only page 2 into a new frame, child's entry repointed and made writable; other pages stay shared | **Term card — Copy-on-write (COW):** share pages after fork; copy a page only when one side writes it. With fork-then-exec, almost nothing is ever copied. The write triggers a trap just like a page fault |
| 14 | 13:45–14:30 | Misconception sweep | Three crossed-out statements fly in (see §8) | Quick corrections |
| 15 | 14:30–15:30 | Recap + bridge | One-screen summary: v/i bit → fault → 6 steps → EAT → COW. Then step 3 of the pipeline zooms: "free-frame list is empty" | Bridge to V24: a fault when RAM is full means someone must leave. Which page? Next video traces FIFO, Optimal and LRU, and an anomaly where more frames give more faults |

### Scene-by-scene beat detail

**Scene 1 — Hook.**
- Open on the overflowing RAM bar; let the overflow visibly "drain" to disk for 2 s before any narration.
- Beat: "Four programs, more than 8 GB between them, and nothing crashed."
- Beat: promise two things only: watch one page fault, then price it.
- Reveal "×41" on the word "forty"; leave it in the corner until scene 10 pays it off.

**Scene 2 — Recap: paging (V20/V21), 15–20 s.**
- Logical address slides in and splits into a blue page # and grey offset.
- The page # drops into a page-table row; an arrow lands on a frame in RAM.
- TLB box flashes once: "this lookup is usually cached".
- Beat: "Every reference costs about one memory access; call that t_mem."

**Scene 3 — Why virtual memory.**
- Ten page tiles of a program; colour by use: hot (lit), cold (greyed: error handlers, an oversized array, rarely used menu code).
- Term card: *Virtual memory*. Keep it on screen 4 s.
- Beat 1: "A program never needs all of itself at once."
- Beat 2: two payoffs appear as ticks: "bigger than RAM" and "more programs in RAM".
- Beat 3: more programs ready → CPU less likely to sit idle (one-line callback to V06's ready queue).

**Scene 4 — Demand paging and the v/i bit.**
- The page table from scene 2 grows a third column.
- Rows 0, 1, 4 show `v` and a frame number; rows 2, 3, 5–9 show `i` with dotted arrows to a disk grid.
- Term cards in order: *Demand paging* → *Valid–invalid bit* → *Backing store*.
- Beat: "Lazy loading: don't fetch until asked."
- Beat: `i` is a question mark for the OS, not an error by itself.

**Scene 5 — Recap: traps (V02), 15 s.**
- Two horizontal bands, user mode and kernel mode; a red trap arrow drops down, a green return arrow goes back up.
- Beat: "Hardware forces the jump; the OS decides what happens next."

**Scene 6 — Page-fault handling (the centrepiece).**
- Step 1: CPU sends `load` for page 3; MMU reads `i`; red flash; trap arrow into the OS lane.
- Step 2: OS opens the process's memory-map card: "page 3 is part of the program" → green tick. Side branch shown faded: "not part of the program → terminate".
- Step 3: free-frame list (a stack of empty boxes) pops frame 7.
- Step 4: a request travels to the disk icon; the process token walks from *running* to *waiting* on the V06 state diagram; another process token moves onto the CPU.
- Step 5: disk-interrupt bolt; frame 7 fills with page 3's contents; row 3 flips `i` → `v`, frame 7.
- Step 6: process token moves *waiting → ready*; when dispatched, the PC arrow jumps back to the same `load` and it succeeds.
- Beat: name the lane of every step as it happens.
- Beat: "Restart, because the instruction never finished."

**Scene 7 — Pause-and-predict 1.** Freeze on step 4 for 5 s with a countdown ring.

**Scene 8 — Pure demand paging.**
- Page table starts fully `i`.
- Reference tape `0 1 2 0 1 3 0 1 2 3 0 1` scrolls; counter "faults: 4 / refs: 12" ticks up.
- Beat: "Every fault here is a first touch; after that the page stays."

**Scene 9 — Locality.**
- Heat-map (time on x, page # on y): three bright horizontal bands, each lasting a while, then shifting.
- Term card: *Locality of reference*.
- Beat: "The band is the set of pages the program currently cares about. Hold on to that idea; it returns in V25 as the working set."

**Scene 10 — EAT with faults.**
- Formula builds: first the hit term, then the fault term, each underlined in its colour.
- Unit card: 8 ms = 8,000,000 ns.
- Slider moves through four p values; time bar stretches; table rows from §6 appear one by one.
- Beat: "With p = 1/1000, 99.9 % of accesses are fast, yet average cost is 41 times worse."

**Scene 11 — Pause-and-predict 2.** Inequality shown blank; reveal after 6 s.

**Scene 12 — Recap: fork (V07), 15 s.**
- Parent box clones into child; a progress bar labelled "copy all pages?" fills slowly, then a question mark appears.

**Scene 13 — Copy-on-write.**
- Two page tables, both rows pointing at shared frames, a small lock icon "read-only" on each.
- Child writes page 2 → protection trap (same red flash as scene 6) → new frame appears → child's row repoints.
- Counter "frames copied: 0 → 1".
- Term card: *Copy-on-write*.

**Scene 14 — Misconception sweep.** Three cards from §8, each struck through with its fixing visual.

**Scene 15 — Recap + bridge.**
- Summary chain animates left to right.
- Zoom into step 3: free-frame list now empty; a frame-full RAM with a question mark: "who leaves?"
- Bridge line to V24.

## 6. Worked example (verified)

**Running parameters.** Memory access time t_mem = 200 ns. Page-fault service time = 8 ms = 8,000,000 ns (trap + disk read + restart, dominated by the disk).

**Formula (state on screen exactly):** EAT = (1 − p) · t_mem + p · (fault service time).

| p (fault rate) | (1 − p)·200 | p·8,000,000 | EAT | Slowdown vs. 200 ns |
|---|---|---|---|---|
| 0 | 200 | 0 | 200.0 ns | 1.00× |
| 1/100,000 | 199.998 | 80 | 280.0 ns | 1.40× |
| 1/10,000 | 199.98 | 800 | 1000.0 ns | 5.00× |
| 1/1000 | 199.8 | 8000 | 8199.8 ns | ≈ 41× (40.999) |

**Sensitivity (optional on-screen inset for scene 10).** How much the answer depends on the fault service time, at two fault rates (t_mem = 200 ns):

| Fault service time | EAT at p = 1/1000 | Slowdown | EAT at p = 1/100,000 | Slowdown |
|---|---|---|---|---|
| 2 ms | 2199.8 ns | ≈ 11.0× | 220.0 ns | 1.10× |
| 5 ms | 5199.8 ns | ≈ 26.0× | 250.0 ns | 1.25× |
| 8 ms | 8199.8 ns | ≈ 41.0× | 280.0 ns | 1.40× |
| 10 ms | 10199.8 ns | ≈ 51.0× | 300.0 ns | 1.50× |

Narration beat for the inset: even a fast (2 ms) fault service leaves a 1-in-1000 fault rate at about 11× slower; the fault *rate* is the lever the OS controls, which is why V24 and V25 are about keeping faults rare.

**Target question.** For slowdown under 10 % (EAT < 220 ns):
220 > 200 + p·(8,000,000 − 200) ⇒ p < 20 / 7,999,800 ≈ 2.5 × 10⁻⁶, i.e. fewer than 1 fault per about 400,000 accesses (399,990 exactly).

**Pure demand paging trace.** Reference string `0 1 2 0 1 3 0 1 2 3 0 1` (12 references), process starts with no pages resident and has at least 4 frames: faults at the first touch of 0, 1, 2, 3 = **4 faults**, 8 hits.

**Note on formulas.** This brief uses the page-fault EAT, not the TLB EAT of V21. The baseline t_mem here already stands for "one memory reference with translation", so TLB effects are folded in; say so in one line on screen.

**Verification.** All values were computed by `python3` in `/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/v23.py`. That covers the EAT table rows, the ≈ 41× factor (40.999), the p bound (2.50006 × 10⁻⁶ → 1 in 399,990) and the distinct-page count (4) for the pure-demand string; `v23b.py` computes the sensitivity table.

## 7. Pause-and-predict checkpoints

1. **Scene 7: while the disk reads the page, is the CPU idle?** Answer: no. The faulting process is moved to the waiting state and the scheduler dispatches another ready process (V06/V08). The disk interrupt later makes the faulting process ready again.
2. **Scene 11: what fault rate keeps slowdown under 10 %?** Answer: p < 2.5 × 10⁻⁶, about 1 in 400,000.
3. **Scene 13, before the child writes: how many frames are copied by `fork()` under COW?** Answer: zero. Copies happen only page by page, on the first write.

4. **Scene 4, optional: a page's entry shows `i`. Must the OS read it from disk?** Answer: only if the memory map says the page belongs to the program; otherwise the access is illegal and the process is terminated.

## 8. Misconceptions to address on screen

- "A page fault is an error / a crash." → Show the two branches at step 2: *illegal* address → process killed; *legal but not resident* → normal, handled silently. Most faults are the second kind.
- "After a fault the CPU continues with the next instruction." → Replay step 6 with the program counter arrow jumping *back* to the faulting instruction.
- "Virtual memory means using the disk as RAM, so it's just slow RAM." → EAT slider: with good locality p is tiny and EAT stays near 200 ns; the disk is touched rarely.
- "Invalid bit means the address is illegal." → Show both meanings of `i`: not part of the program, *or* part of it but on disk; the OS's memory map tells them apart.
- "Demand paging means no page is ever loaded in advance." → Pure demand paging is the extreme; real systems may prefetch neighbours. Show a dotted "prefetch" arrow, labelled optional.
- "A page fault is the same as a TLB miss." → Split screen: TLB miss → walk the page table in memory (nanoseconds, V21); page fault → go to disk (milliseconds). Different rows of the EAT table.
- "More RAM always removes page faults." → Pure-demand trace: the first touch of each page faults no matter how much RAM there is.
- "COW copies the whole address space on the first write." → Only the written page (page 2) gets a new frame; the rest stay shared.

## 9. Visual assets

- **Reuse:** no `Slides/CS502` deck for L26 exists yet. Reuse the page-table / frame drawing style and colour palette of `Slides/CS502/08-cpu-scheduling-basics.tex` and the V20/V21 briefs (page # / offset split; TLB box) so the series looks continuous. Reuse the V06 state diagram for scene 6 step 4.
- **New:**
  - Two-lane (Hardware / OS) page-fault pipeline with six numbered steps (Manim or Motion Canvas).
  - Page table with v/i column and arrows to a disk "backing store" grid.
  - Locality heat-map (address vs. time) generated from a toy loop trace.
  - EAT slider with stretching time bar (PowerPoint morph is enough).
  - COW split: two page tables sharing frames, one entry peeling off on write.
- **Diagram checklist for the animator:**
  - Same page-table geometry as V20 (rows = page numbers, top to bottom).
  - Disk grid on the right, RAM frames in the middle, CPU on the left.
  - Every arrow that crosses into the OS lane is drawn red at the instant of the trap.
  - Frame numbers used: free frame 7 for the fault; frames 2, 5, 8 for the shared COW pages.
- **Live demo (optional, 30 s):** on Linux, `/usr/bin/time -v ls` shows "Minor/Major page faults"; run twice to show the second run has fewer major faults because pages are already cached.

## 10. On-screen text & notation

- Virtual memory; demand paging; pure demand paging; page fault; valid–invalid bit (`v` / `i`); backing store (swap space); free-frame list; locality of reference; copy-on-write (COW).
- p = page-fault rate; t_mem = memory access time; EAT = (1 − p)·t_mem + p·(fault service time).
- Units: always show ns and ms together, with the conversion 1 ms = 1,000,000 ns.
- Keep "frame" (physical) and "page" (logical) in distinct colours, consistent with V20.
- Colour code for the fault pipeline:
  - Blue lane = hardware (MMU, trap, restart).
  - Orange lane = OS (check, find frame, schedule disk read, update table).
  - Red flash = trap raised.
  - Green = valid entry / successful access.
- Process states use V06 wording exactly: running, ready, waiting (blocked).
- Write `fork()` and `exec()` in monospace with parentheses.
- Show the formula in one fixed position (top right) for scenes 10–11 so viewers can refer back.
- Avoid the symbol "ma" for memory access; use t_mem consistently with V21.

## 11. End-of-video quiz

1. **(MCQ)** Which step of page-fault handling is done by hardware, not the OS? (a) choosing a free frame (b) scheduling the disk read (c) detecting the invalid bit and trapping (d) updating the page table. **Answer: (c).**
2. **(Numerical)** t_mem = 100 ns, fault service time = 5 ms, p = 1/2000. Find EAT. **Answer:** (1 − 0.0005)·100 + 0.0005·5,000,000 = 99.95 + 2500 = 2599.95 ns (about 26× slower). *(Computed with the same script pattern; see §6.)*
3. **(MCQ)** Under pure demand paging, a process that touches 7 distinct pages in total, with enough frames, takes at least how many faults? (a) 0 (b) 1 (c) 7 (d) depends on the reference order. **Answer: (c)** — one per first touch, independent of order.
4. **(Short)** A parent forks, then the child immediately calls `exec()`. Why is COW a big saving here? **Answer:** the child never writes the parent's pages before `exec()` replaces its address space, so no page is copied at all.
5. **(MCQ)** Why must the faulting instruction be restarted? (a) to flush the TLB (b) because it did not complete when the page was missing (c) to switch to user mode (d) to recompute p. **Answer: (b).**

*Quiz Q2 check:* 0.9995·100 + 0.0005·5e6 = 2599.95 ns, computed with `python3` in the scratchpad directory above.

## 12. References

- Silberschatz, *Operating System Concepts* 8e, Ch 9: background (p.357), demand paging (p.361), page fault handling (p.364), copy-on-write (p.367).
- Stallings, *Operating Systems* 6e, Ch 8 §8.2: demand paging (p.366).
- Bovet & Cesati, *Understanding the Linux Kernel*, Ch 9 (pp.351–397): page fault exception handler, COW in the process address space.

## 13. Production notes

- Pacing: scene 6 is the heart; give each of the six steps its own beat with a held frame. Do not compress it to make time.
- Keep hardware vs. OS lanes in two colour-blind-safe hues (blue / orange), and also label them in text.
- Scene 10: put the unit conversion on screen *before* plugging numbers; most student errors are ms/ns slips.
- If over length: cut the live demo and shorten scene 9 (locality) to one heat-map pass.
- Captions on throughout; read every formula aloud term by term.
- Avoid the phrase "disk as RAM" in narration; it seeds misconception 3.
- Timing budget: why VM + demand paging (scenes 2–5) about 3.5 min; fault handling (6–7) about 3 min; pure demand paging + locality (8–9) about 2 min; EAT (10–11) about 3 min; COW (12–13) about 2 min; wrap-up (14–15) about 1.5 min.
- Record scene 6 in one continuous take of the animation, then add narration; re-timing a six-step animation in edit is error-prone.
- Reduced-motion export: replace the stretching time bar in scene 10 with a static bar chart of the four EAT values.
- Hinglish version: keep all term cards in English; translate narration only.
- Pronounce "COW" as letters the first time (C-O-W, copy-on-write), then as a word.
- Check before export: every number on screen matches §6 exactly (8199.8, 2.5 × 10⁻⁶, 4 faults).
- Keep total runtime under 16 min; scene 6 alone should be 2.5–3 min.
- Thumbnail: the red-flash page-table row with the text "×41".
