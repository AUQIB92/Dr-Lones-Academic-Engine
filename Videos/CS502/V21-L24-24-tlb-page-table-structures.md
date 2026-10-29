# V21 · L24 — The TLB and the 4 MB Problem

> **Lecture:** CS502 L24 `24-tlb-page-table-structures` · **Unit:** III · **Target length:** 13–15 min · **Series position:** 21/28
> **Format:** mixed (screen-capture animation + whiteboard for the EAT algebra) · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

Two ideas in this lecture only make sense when you watch them happen. The first is the TLB *miss path*: a detour through the TLB, then the page table in memory, then filling the TLB, then memory, while the hit path skips the detour. The second is the *size* of a page table. Students badly underestimate it until they watch 1,048,576 entries pile up into 4 MB per process. On a static slide the EAT formula looks like algebra with no meaning behind it; animated, each term matches a path a request actually takes. Students also routinely forget that a hit still costs one memory access, and that a miss in a two-level table costs *three*.

## 2. Learning outcomes

After watching, the student can:
- [Understand] Trace a memory reference through the TLB on a hit and on a miss.
- [Apply] Compute EAT for a given hit ratio `α`, TLB time and memory time, for single-level and two-level page tables.
- [Apply] Solve for the minimum `α` that meets a target EAT.
- [Apply] Compute the size of a single-level page table from the address width, page size and PTE size.
- [Analyze] Explain how a two-level table saves space for a sparse address space, and count the tables a given process needs.

## 3. Prerequisites

- **Computer fundamentals:** memory is an array of bytes, and reading memory takes time (nanoseconds).
- **V20 Paging:** page, frame, page table, the `p | d` split, `f × page size + d`. Recapped on screen in scene 2 (20 s).
- **V19 Memory basics:** the MMU (named in the recap).
- *Cache*, *hit*, *miss*, *hit ratio* and *locality* are defined on screen in scene 3. Binary units are on a reference card in scene 11. No classroom lecture is assumed.

## 4. Hook (first 30–45 s)

> *In the last video, every load needed two trips to memory: one to the page table and one for the data. That would make paging roughly halve the speed of every program, yet real machines lose only a few percent. There's also a second problem nobody mentioned. On a 32-bit machine with 4 KB pages, every process's page table is 4 megabytes, so a hundred processes means 400 MB of RAM just for tables.*
>
> "That's two problems, speed and size. There's one fix for each, and we'll put numbers on both."

On screen: a speedometer reading "2×", and a stack of 100 page-table blocks labelled "4 MB each".

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:40 | Hook | Speedometer at "2×"; 100 stacked tables, "4 MB each" | Speed and size; promise numbers |
| 2 | 0:40–1:00 | Recap from V20 (20 s) | V20's bit-split replayed at ×3: `p \| d` → page table → `f \| d` | Page, frame and page table, one line each; "every translation reads the table, and the table lives in memory" |
| 3 | 1:00–1:40 | Caches in one minute | A desk (small, fast) beside a bookshelf (big, slow); a recently used book stays on the desk | Define *cache*, *hit*, *miss*, *hit ratio α*, *locality* (programs reuse recent and nearby addresses) |
| 4 | 1:40–2:20 | The TLB | A small box beside the CPU with 4 (page, frame) pairs; on a lookup all four keys light at once | Define the *TLB* (translation lookaside buffer): a cache of recent `p → f` translations. Define *associative*: all entries are compared in parallel |
| 5 | 2:20–3:00 | Hit path | A packet goes CPU → TLB (10 ns ✓) → memory (80 ns); timer shows 90 | Even a hit reads memory once, for the data |
| 6 | 3:00–3:50 | Miss path | A packet goes CPU → TLB (10 ns ✗) → page table in memory (80 ns) → TLB filled → memory (80 ns); timer shows 170 | A miss adds one table read. The TLB is then refilled, so the next access to this page hits |
| 7 | 3:50–4:50 | EAT derivation (whiteboard) | `EAT = α(t_TLB + t_mem) + (1−α)(t_TLB + 2·t_mem)`; the two timers from scenes 5–6 drop into the brackets; substitute α = 0.90 → 0.9·90 + 0.1·170 = **98 ns** | Define *EAT* (effective access time): the average of the two paths, weighted by how often each happens |
| 8 | 4:50–5:20 | **Pause-and-predict 1** | Slider set to α = 0.99; freeze | See §7 |
| 9 | 5:20–6:10 | The α line | Plot of EAT against α: a straight line from 170 at α = 0 to 90 at α = 1; dashed lines at 160 (no TLB) and 100; points marked at 0.875, 0.90, 0.99 | Solve on screen for EAT ≤ 100 ⇒ α ≥ 0.875. At α = 0 the TLB makes things *worse* than having none (170 > 160) |
| 10 | 6:10–6:30 | Halfway card | Title card: "Problem 2: size" | A clear break between the two halves |
| 11 | 6:30–7:20 | Single-level size | Counter: 2^32 / 2^12 = 2^20 entries ticking up; each entry 4 B; a bar grows to 4 MB = 1024 pages of table; reference card of binary units | Define *PTE* (page-table entry). Every process needs its own table, contiguous, and most of its entries map nothing |
| 12 | 7:20–7:50 | 64-bit is hopeless | The bar runs off screen, switches to log scale: 2^52 × 8 B = 2^55 B = 32 PiB; a 48-bit marker at 512 GiB | A flat table is impossible at this size |
| 13 | 7:50–8:40 | Two-level idea | The 32-bit bar re-split `10 \| 10 \| 12`; an outer table of 1024 entries, most greyed (null); a few point to inner tables | Define *outer table* and *inner table*. Page the page table: regions that are never used need no inner table |
| 14 | 8:40–9:30 | Walk an address | `0x00403ABC` → `p1 = 1`, `p2 = 3`, `d = 0xABC`; the walk goes outer[1] → inner table → entry 3 → frame | Each inner table covers 1024 × 4 KB = 4 MB |
| 15 | 9:30–10:00 | **Pause-and-predict 2** | Process layout: 12 MB at the bottom + a 1 MB stack at the top; "How many 4 KB tables?" | See §7 |
| 16 | 10:00–10:30 | Answer + saving | 3 inner + 1 inner + 1 outer = 5 tables = 20 KB vs. 4 MB; "204.8× smaller" | When the address space is sparse, the saving is huge |
| 17 | 10:30–11:30 | Price of two levels | The miss path now has two table reads, giving 10 + 3·80 = 250 ns; recomputed EAT 106 ns (α = 0.90) and 91.6 ns (α = 0.99) | Space traded for a costlier miss; a high hit ratio hides it |
| 18 | 11:30–12:10 | Other structures (teaser) | Thumbnails: *hashed* page table (hash → chain) and *inverted* page table (one entry per frame, `<pid, p>`) | Define each in one line |
| 19 | 12:10–13:00 | Recap + bridge to V22 | Two-row card: speed → TLB (EAT); size → multilevel (count tables); end card "Next: V22 Segmentation" | Bridge: paging cuts memory into equal pieces that ignore the program's structure. What if we cut along the program's own parts (code, data, stack)? |

## 6. Worked example (verified)

**Timings:** `t_TLB` = 10 ns, `t_mem` = 80 ns. The TLB lookup happens before the memory access (sequential).

### 6.1 Path costs

| Path | Single-level | Two-level |
|---|---|---|
| TLB hit | 10 + 80 = 90 ns | 90 ns |
| TLB miss | 10 + 80 + 80 = 170 ns | 10 + 3·80 = 250 ns |
| No TLB at all | 2·80 = 160 ns | 3·80 = 240 ns |

### 6.2 Single-level EAT, step by step for α = 0.90

| Step | Expression | Value |
|---|---|---|
| 1 | α · hit cost | 0.90 · 90 = 81 |
| 2 | (1 − α) · miss cost | 0.10 · 170 = 17 |
| 3 | EAT | 81 + 17 = **98 ns** |

### 6.3 Single-level EAT across α

| α | EAT (ns) | Slowdown vs. 80 ns |
|---|---|---|
| 0.00 | 170.0 | 112.5 % |
| 0.875 | 100.0 | 25 % |
| 0.90 | 98.0 | 22.5 % |
| 0.99 | 90.8 | 13.5 % |
| 1.00 | 90.0 | 12.5 % |

Minimum α for EAT ≤ 100 ns: 90 + (1 − α)·80 ≤ 100 ⇒ **α ≥ 0.875**.

### 6.4 Two-level EAT

| α | Hit part | Miss part | EAT |
|---|---|---|---|
| 0.90 | 0.90·90 = 81 | 0.10·250 = 25 | **106.0 ns** |
| 0.99 | 0.99·90 = 89.1 | 0.01·250 = 2.5 | **91.6 ns** |

### 6.5 Page-table sizes

| Address space | Page | PTE | Entries | Table size |
|---|---|---|---|---|
| 32-bit | 4 KB | 4 B | 2^20 = 1,048,576 | 4,194,304 B = **4 MB** (1024 pages) |
| 48-bit | 4 KB | 8 B | 2^36 | **512 GiB** |
| 64-bit | 4 KB | 8 B | 2^52 | 2^55 B = **32 PiB** |

### 6.6 Two-level walk and table count

| Field | Bits | Value for `0x00403ABC` |
|---|---|---|
| p1 (outer index) | 10 | 1 |
| p2 (inner index) | 10 | 3 |
| d (offset) | 12 | 2748 = 0xABC |

| Region | Size | Inner tables (4 MB each) |
|---|---|---|
| Code + data + heap (bottom) | 12 MB | 3 |
| Stack (top) | 1 MB | 1 |
| Outer table | — | 1 |
| **Total** | | **5 tables × 4 KB = 20 KB** (vs. 4 MB flat, 204.8× smaller) |

A process that touches all 4 GB would need 1024 inner tables + 1 outer = 4100 KB, slightly *more* than the flat 4096 KB.

### 6.7 EAT grid (for the scene 9 plot and a description-box table)

| α | Single-level EAT (ns) | Two-level EAT (ns) |
|---|---|---|
| 0.80 | 106.0 | 122.0 |
| 0.85 | 102.0 | 114.0 |
| 0.90 | 98.0 | 106.0 |
| 0.95 | 94.0 | 98.0 |
| 0.98 | 91.6 | 93.2 |
| 0.99 | 90.8 | 91.6 |
| 1.00 | 90.0 | 90.0 |

### 6.8 Sensitivity to TLB speed (α = 0.95, single-level, t_mem = 80 ns)

| t_TLB (ns) | EAT (ns) |
|---|---|
| 2 | 86.0 |
| 5 | 89.0 |
| 10 | 94.0 |
| 20 | 104.0 |

Take-away for narration: a slow TLB pays its cost on *every* access, so TLBs are kept tiny and fast.

### 6.9 Two-level table counts for other process shapes (each inner table covers 4 MB)

| Process layout | Inner tables | + outer | Total memory for tables |
|---|---|---|---|
| Tiny: 1 MB code + 64 KB stack | 2 | 3 | 12 KB |
| Video example: 12 MB + 1 MB stack | 4 | 5 | 20 KB |
| Medium: 40 MB heap + 8 MB stack | 12 | 13 | 52 KB |
| Uses the full 4 GB | 1024 | 1025 | 4100 KB (> 4096 KB flat) |

Narration point: the saving comes entirely from regions that are never touched. A dense address space gains nothing.

**Verification.** All EAT values, the α threshold, every table size, the bit split and the table counts were computed by `eat.py` (§6.7–6.9 by `extras.py`). The quiz values were computed by `quiz.py`. Both scripts are in `/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/` (python3; supplementary tables from `extras.py` in the same folder). The 4100 vs. 4096 KB check was run in python3 as well. The timings (10/80 ns) are original and differ from the textbook's 20/100 ns example.

## 7. Pause-and-predict checkpoints

1. **Scene 8 (α = 0.99).** "Predict the EAT."
   *Answer:* 0.99·90 + 0.01·170 = **90.8 ns**.
2. **Scene 15 (sparse process).** "How many 4 KB tables in total?"
   *Answer:* 3 + 1 inner tables + 1 outer table = **5** (20 KB).
3. *(Optional, scene 17)* "Two levels, α = 0.90: is the EAT above or below 100 ns?"
   *Answer:* Above: **106 ns**.

## 8. Misconceptions to address on screen

- **"A TLB hit costs only the TLB time."** → Scene 5: the hit packet still travels to memory for the data.
- **"A miss costs one memory access."** → Scenes 6 and 17: a miss costs 2 memory accesses with one level and 3 with two.
- **"Adding a TLB can never hurt."** → Scene 9: at α = 0 the cost is 170, above the 160 no-TLB line.
- **Formula drift:** a common slip is `EAT = α·t_TLB + (1−α)(t_TLB + t_mem)`, which drops the data access. → Scene 7 boxes the full form and checks it against the path timers. *(The CS502 knowledge-base example row contains this incomplete form. Flag it to the course author, and do not show it on screen.)*
- **"Two-level tables are always smaller."** → §6.6: a process using the whole 4 GB needs 4100 KB, versus 4096 KB flat.
- **"Table size depends on physical memory."** → Scene 11 caption: it depends on the *virtual* address width, the page size and the PTE size.

## 9. Visual assets

- **Reuse:** V20's bit-ruler and page-table animation (re-split as 10|10|12). `Slides/CS502/24-tlb-page-table-structures.tex` is not built yet. Once it is, reuse its TLB and two-level figures. Match the table style of `Slides/CS502/08-cpu-scheduling-basics.tex`.
- **New:**
  - Packet-path animation with running timers (Motion Canvas).
  - EAT vs. α line plot (matplotlib or Manim `Axes`).
  - Growing bar with a log-scale switch.
  - Two-level tree with greyed null entries.
- **Live demo (optional, 30 s):** `python3 eat.py`, or on Linux `getconf PAGESIZE` → `4096`.

### Instructor whiteboard version

1. Left half of the board: draw CPU, TLB and Memory boxes in a row. Trace the hit path in blue, writing "10" and "80" on the arrows. Trace the miss path in orange with its extra loop to the page table. Total each path in a circle (90, 170).
2. Write the EAT formula underneath, and literally copy the two circled totals into the brackets. Substitute α = 0.90 and then 0.99.
3. Sketch the α line by hand. Plot only the two end points (0 → 170, 1 → 90), join them, and mark 160 with a dashed line.
4. Right half: write 2^32 / 2^12 = 2^20 and × 4 B = 4 MB as a column of arithmetic. Then draw the 10|10|12 strip.
5. Draw the outer table as a tall rectangle with 1024 "slots". Shade three slots at the bottom and one at the top, and draw small inner tables only for those. Count 5 aloud.
6. Finish by re-drawing the miss path with a second table hop and recomputing 106 ns.

## 10. On-screen text & notation

- **Defined on screen at first use:** page, frame, page table (2); cache, hit, miss, hit ratio, locality (3); TLB, associative (4); EAT (7); PTE, binary units (11); outer/inner table (13); hashed and inverted page table (18).
- **`EAT`**, hit ratio **`α`**, `t_TLB`, `t_mem`, all stated explicitly. Always show the full formula.
- `p1`, `p2`, `d`. Binary units: KB = 2^10, MB = 2^20, GiB = 2^30, PiB = 2^50.

## 11. End-of-video quiz

1. **(Numerical)** `t_TLB` = 5 ns, `t_mem` = 100 ns, α = 0.95, single-level. What is the EAT?
   *Solution:* Hit = 5 + 100 = 105 and miss = 5 + 200 = 205. EAT = 0.95·105 + 0.05·205 = 99.75 + 10.25 = **110 ns**.
2. **(Numerical)** Same values, but with a two-level page table. What is the EAT?
   *Solution:* Miss = 5 + 3·100 = 305. EAT = 0.95·105 + 0.05·305 = 99.75 + 15.25 = **115 ns**.
3. **(Numerical)** 40-bit virtual address, 8 KB pages, 4 B PTE. What is the size of a single-level page table?
   *Solution:* Entries = 2^40 / 2^13 = 2^27, and size = 2^27 × 4 = 2^29 B = **512 MiB**.
4. **(Numerical)** `t_TLB` = 5 ns, `t_mem` = 100 ns, single-level. What is the minimum α for EAT ≤ 115 ns?
   *Solution:* EAT = 105 + (1 − α)·100 ≤ 115, so (1 − α) ≤ 0.10 and **α ≥ 0.90**.
5. **(MCQ)** An inverted page table has one entry per: (a) virtual page of each process (b) physical frame (c) process (d) TLB slot
   *Solution:* It is indexed by frame and stores `<pid, p>`, so its size tracks physical memory. **(b)**.

## 12. References

- Silberschatz 8e, Ch. 8: TLB p.333 (within §8.4, pp.328–337); structure of the page table (hierarchical, hashed, inverted) p.337.
- Stallings 6e, Ch. 8: hardware and control structures §8.1 p.346; TLB p.356.
- Bovet & Cesati 3e, Ch. 2 "Memory Addressing" pp.35–78: multilevel page tables and TLB handling in Linux on x86.

## 13. Production notes

- **Pacing:** keep the two halves clearly separated by scene 10. The algebra in scene 7 is best written live by hand.
- **Cut if over length:** reduce scene 18 to a lower-third, then drop checkpoint 3. Keep scene 17, since the cost of a two-level miss is the point exams test most often.
- **Accessibility:** draw hits as solid blue lines and misses as dashed orange lines. Captions on. Read exponents aloud ("two to the twentieth").

### Timing budget and cut priority

| # | Scene | Seconds | Cumulative | Priority |
|---|---|---|---|---|
| 1 | Hook | 40 | 0:40 | keep (core) |
| 2 | Recap from V20 (20 s) | 20 | 1:00 | keep (core) |
| 3 | Caches in one minute | 40 | 1:40 | shorten first |
| 4 | The TLB | 40 | 2:20 | keep (core) |
| 5 | Hit path | 40 | 3:00 | keep (core) |
| 6 | Miss path | 50 | 3:50 | keep (core) |
| 7 | EAT derivation (whiteboard) | 60 | 4:50 | keep (core) |
| 8 | **Pause-and-predict 1** | 30 | 5:20 | keep (core) |
| 9 | The α line | 50 | 6:10 | shorten first |
| 10 | Halfway card | 20 | 6:30 | shorten first |
| 11 | Single-level size | 50 | 7:20 | keep (core) |
| 12 | 64-bit is hopeless | 30 | 7:50 | shorten first |
| 13 | Two-level idea | 50 | 8:40 | keep (core) |
| 14 | Walk an address | 50 | 9:30 | keep (core) |
| 15 | **Pause-and-predict 2** | 30 | 10:00 | shorten first |
| 16 | Answer + saving | 30 | 10:30 | keep (core) |
| 17 | Price of two levels | 60 | 11:30 | keep (core) |
| 18 | Other structures (teaser) | 40 | 12:10 | cut first |
| 19 | Recap + bridge to V22 | 50 | 13:00 | keep (core) |

Core (keep) scenes total 9:30; shortenable 2:50; cuttable 0:40. Halving every "shorten first" scene and dropping every "cut first" scene saves 2:05.
