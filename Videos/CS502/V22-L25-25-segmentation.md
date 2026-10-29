# V22 · L25 — Segmentation: Memory the Way Programmers See It

> **Lecture:** CS502 L25 `25-segmentation` · **Unit:** III · **Target length:** 12–14 min · **Series position:** 22/28
> **Format:** screen-capture animation (segment table + address translation) · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

Paging (V20–V21) cuts a program into equal pieces that mean nothing to the programmer.
Segmentation cuts it into the pieces the programmer actually thinks in: code, data, stack, heap,
library. The translation itself is short, but students make the same two mistakes again and
again: they add the offset to the base *before* checking the limit, and they treat a segment
number as if it were an address. A video can run five addresses through the hardware one after
another, with the comparator lighting green or red each time, so the order "check, then add"
becomes a habit. It can also show sharing (two processes pointing to one code segment) as two
arrows meeting in memory, which is hard to read on a static figure.

## 2. Learning outcomes

After watching, the student can:
- [Understand] Describe the programmer's view of memory as a collection of variable-size segments and contrast it with paging.
- [Apply] Translate a two-part logical address ⟨segment, offset⟩ to a physical address using a segment table, or detect the trap.
- [Understand] Explain how segments are shared between processes and protected with per-segment bits.
- [Understand] Explain why pure segmentation suffers external fragmentation and how segmentation with paging (Intel-style) removes it.
- [Apply] Translate an address in a segmented-paged scheme: segment → page table → frame + offset.

## 3. Prerequisites

- Basic computer fundamentals: memory addresses, binary place value, a C program with functions and variables.
- V02: traps; user vs kernel mode.
- V19: logical vs physical address; MMU with relocation and limit registers; external fragmentation; first/best fit.
- V20: paging: page, frame, page table, page number + offset.
- V21: TLB, multilevel page tables.

## 4. Hook (first 30–45 s)

A C program on the left: `main()`, a function `sqrt()`, a global array `marks[60]`, a local
variable inside a recursive function. On the right, a blank memory bar.

> "Ask a programmer to draw the memory of this program and you get this:"
> (Five labelled blobs float up: *code*, *global data*, *heap*, *stack*, *math library*.)
> "Different sizes, no particular order, and each one grows or shrinks on its own. Ask the
> paging hardware from V20 and you get this:" (the same program chopped into identical squares).
> "Nobody writes a program in 4 KB squares. What if the hardware spoke the programmer's
> language instead? And what would that cost us?"

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:45 | Hook | C program → five labelled blobs vs grid of identical pages | Two views of the same program: logical pieces vs equal pages |
| 2 | 0:45–1:05 | Recap V19–V20 | V19's MMU (limit check, then add relocation; trap on failure). V20's page-number/offset split and page table | **Recap (15–20 s).** One relocation/limit pair per process in V19; a page table per process in V20. Segmentation is "V19's pair, but one pair per *piece* of the program" |
| 3 | 1:05–2:15 | The user's view of memory | The five blobs with names and lengths: code 1000, stack 400, data 1100, heap 1100, library 1200 (bytes). Each blob gets a number 0–4. An instruction `load marks[53]` becomes "the 53rd byte of segment 2" | Define **segment**: a logical unit (code, global data, stack, heap, library) of variable length. Define **segmented logical address**: a pair ⟨segment number `s`, offset `d`⟩. The compiler builds segments automatically; the programmer never picks segment numbers by hand |
| 4 | 2:15–3:15 | The segment table | Table slides in: rows 0–4, columns `base` and `limit` (§6). Memory bar on the right with the five segments placed at their bases, gaps between them. A register labelled **STBR** (segment-table base register) points at the table; **STLR** (segment-table length register) = 5 | Define **segment table**: one entry per segment, holding **base** (start in physical memory) and **limit** (length). Define STBR and STLR. Each process has its own table; switching processes switches tables (V06 context switch) |
| 5 | 3:15–6:45 | Translation, five worked addresses | Hardware diagram: ⟨s, d⟩ enters; step 1 `s < STLR`?; step 2 look up row s; step 3 comparator `d < limit`?; step 4 adder `base + d`. Run in turn: ⟨0, 430⟩ → 1830; ⟨2, 53⟩ → 4353; ⟨3, 852⟩ → 4052; ⟨4, 1112⟩ → 8112; ⟨1, 399⟩ → 6699. A dot travels to the right place in the memory bar each time | Always the same order: segment number valid? offset less than limit? then add. Say the comparison aloud for each one ("53 is less than 1100, so add 4300") |
| 6 | 6:45–7:15 | Pause & predict 1 | Freeze with ⟨1, 400⟩ at the input. "Stack segment, limit 400. Physical address?" | Pause. Answer: none: 400 is not less than 400, so trap. Offsets run 0 to limit − 1 |
| 7 | 7:15–8:00 | The traps | Two red runs: ⟨1, 400⟩ fails at the comparator; ⟨5, 10⟩ fails at the STLR check (only segments 0–4 exist). Red arrow to the OS: "addressing error, trap" | Two ways to fail. Off-by-one at the limit is the classic exam trap. The OS usually terminates the process (a "segmentation fault" got its name here) |
| 8 | 8:00–9:15 | Sharing | Two processes P1 and P2, each with its own segment table. Both row-0 entries (code, e.g. an editor) carry the same base 1400; two arrows meet in one code block. Their data segments point to different places | Sharing is natural: share a *segment* by putting the same base/limit in both tables. Only one copy of the code in memory. Caveat: the shared segment should have the same segment number in every process that uses it if it contains references to itself |
| 9 | 9:15–10:00 | Protection | Each table row gains bits `R W X` (read, write, execute): code `R-X`, data `RW-`, stack `RW-`. A `store` into segment 0 is stopped: red "protection trap" | Define **protection bits**. Because a segment is a meaningful unit, permissions fit it naturally: make code read-only, stop the data from being executed. Paging protects pages, which may mix code and data |
| 10 | 10:00–10:45 | The cost: external fragmentation | Memory bar from scene 4 after several processes come and go: varied gaps; a new 1500-byte segment cannot be placed; V19 hole-list badge | Segments are variable-size, so placing them is V19's dynamic-allocation problem again: first/best fit, external fragmentation, compaction. Pure segmentation brings back the problem paging solved |
| 11 | 10:45–12:15 | Segmentation with paging (Intel-style, simplified) | Two-stage pipeline: ⟨s = 2, d = 700⟩ → segment table row 2 (limit 1100: 700 < 1100 ✓) → instead of a base, a pointer to segment 2's **page table**. Offset 700 split with 256-byte pages: page 2, offset 188 → page table row 2 → frame 17 → physical 17 × 256 + 188 = 4540 | Combine both: segments for the programmer's view and protection, pages for placement. Each segment is itself paged, so no external fragmentation. Intel x86 (32-bit) used this: a segmentation unit produced a *linear address* that the paging unit then translated. Keep it simple: two lookups, checked in order |
| 12 | 12:15–12:45 | Pause & predict 2 | "Pages of 256 bytes. Offset 700 inside a segment. Which page and which offset within it?" | Pause. Answer: page 2, offset 188 (700 = 2 × 256 + 188) |
| 13 | 12:45–13:15 | Segmentation vs paging summary | Two-column table: unit size (variable / fixed), visible to programmer (yes / no), fragmentation (external / internal, last page), sharing & protection unit (logical piece / page), table entry (base + limit / frame number) | One table to remember |
| 14 | 13:15–14:00 | Recap + bridge | Summary cards. Final frame: the page table from scene 11 with a new column "valid / in memory?", and one page drawn on disk instead of in RAM | Segments = user's view; translate by check-then-add; share and protect per segment; page each segment to escape external fragmentation. Next, V23: what if some pages are not in memory at all? Virtual memory and demand paging |

## 6. Worked example (verified)

**Segment table (one process).** Lengths in bytes. STLR = 5.

| s | Segment | base | limit | Occupies |
|---|---|---|---|---|
| 0 | code | 1400 | 1000 | 1400–2399 |
| 1 | stack | 6300 | 400 | 6300–6699 |
| 2 | data | 4300 | 1100 | 4300–5399 |
| 3 | heap | 3200 | 1100 | 3200–4299 |
| 4 | library | 7000 | 1200 | 7000–8199 |

The script checks that no two segments overlap (heap ends at 4299, data starts at 4300).

**Translations.** Rule: if `s ≥ STLR` trap; else if `d ≥ limit[s]` trap; else physical = `base[s] + d`.

| Logical ⟨s, d⟩ | Check | Result |
|---|---|---|
| ⟨0, 430⟩ | 430 < 1000 | **1830** |
| ⟨2, 53⟩ | 53 < 1100 | **4353** |
| ⟨3, 852⟩ | 852 < 1100 | **4052** |
| ⟨4, 1112⟩ | 1112 < 1200 | **8112** |
| ⟨1, 399⟩ | 399 < 400 | **6699** (last byte of the stack) |
| ⟨1, 400⟩ | 400 ≥ 400 | **trap** (checkpoint 1) |
| ⟨5, 10⟩ | 5 ≥ STLR = 5 | **trap** (no segment 5) |

Note that ⟨3, 852⟩ → 4052 lies *below* ⟨2, 53⟩ → 4353 in memory: segment numbers say nothing
about physical order.

**Segmentation with paging.** Page size 256 bytes. Segment 2 (data, limit 1100) has page table
{0 → 9, 1 → 4, 2 → 17, 3 → 6, 4 → 12} (page → frame).
Address ⟨2, 700⟩: 700 < 1100 ✓; page = 700 div 256 = **2**, offset = 700 mod 256 = **188**;
frame 17; physical = 17 × 256 + 188 = **4540**.

**Quiz arithmetic.** Table {0: base 2000, limit 600; 1: base 3500, limit 250}:
⟨0, 599⟩ → **2599**; ⟨1, 250⟩ → **trap**; ⟨1, 100⟩ → **3600**. A 16-bit offset field gives a
maximum segment size of 2^16 = **65536** bytes. With 512-byte pages, offset 1300 → page **2**,
offset **276**; if page 2 is in frame 7, physical = 7 × 512 + 276 = **3860**.

**Memory map of this process** (for the scene 4 bar and the scene 10 fragmentation shot).
Sorted by base: code 1400–2399, heap 3200–4299, data 4300–5399, stack 6300–6699,
library 7000–8199. Gaps between the segments:

| Gap | From | To | Size |
|---|---|---|---|
| G1 | 2400 | 3199 | 800 |
| G2 | 5400 | 6299 | 900 |
| G3 | 6700 | 6999 | 300 |

The five segments total 4800 bytes. A new 1500-byte segment fits none of the gaps even though
they add up to 2000 bytes: the same external fragmentation as V19, now with segments
(scene 10 uses exactly this picture).

**How it was verified.** `v22_seg.py` holds the table, asserts the segments do not overlap (an
earlier draft placed the library segment overlapping the data segment; the assert caught it and
the base was changed to 7000), and computes every translation and trap above, the gap list, plus the paged
example and quiz values. File in
`/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/`.

## 7. Pause-and-predict checkpoints

1. **(Scene 6, ~6:45)** ⟨1, 400⟩ with limit 400? → **Trap; valid offsets are 0–399.**
2. **(Scene 12, ~12:15)** 256-byte pages, offset 700. → **Page 2, offset 188.**
3. **(Scene 8, quick)** P1 and P2 share the code segment. How many copies of the code are in memory? → **One; both tables hold the same base and limit.**

## 8. Misconceptions to address on screen

- "Add the base, then compare with the limit." → Scene 5: the comparator sits *before* the adder and takes the offset `d`, not the sum.
- "Offset equal to the limit is fine." → Scene 6/7: ⟨1, 400⟩ traps; label "0 … limit − 1".
- "Segment 3 must be above segment 2 in memory." → §6 note: ⟨3, 852⟩ lands below ⟨2, 53⟩.
- "Segmentation has no fragmentation." → Scene 10: variable sizes bring back external fragmentation.
- "Segmentation with paging means two page tables." → Scene 11: a segment table *then* a page table per segment; the segment entry holds a limit and a page-table pointer.
- "The segment table is stored inside the CPU." → Scene 4: the table lives in memory; only STBR and STLR are registers, which is why a TLB-like cache helps here too (V21).
- "Sharing a segment copies it." → Scene 8: one block in memory, two arrows.
- "Every process numbers its segments the same way." → Scene 8: each process has its own table; numbering is per process (shared code works best with the same number in each).
- "A segment is the same as a page, just bigger." → Scene 13 table: variable vs fixed size, visible vs invisible to the programmer.

## 9. Visual assets

- **Reuse:** `Slides/CS502/25-segmentation.tex` does not exist yet; follow the block/exampleblock/alertblock roles of `Slides/CS502/08-cpu-scheduling-basics.tex`. Reuse the V19 MMU comparator/adder graphic and V20's page-table graphic so the viewer recognises them.
- **New:**
  - Program → blobs vs program → pages split (hook).
  - Segment table with STBR/STLR and synchronized memory bar (scene 4).
  - Translation pipeline with green/red comparator states; one run per address (scenes 5–7), Manim recommended.
  - Two processes sharing a code segment (scene 8); R/W/X bits column (scene 9).
  - Two-stage segmented-paging pipeline (scene 11).
- **Live demo:** none. (Optional: a C program writing past an array on Linux to show "Segmentation fault"; not run for this brief, and note that modern Linux faults come from paging, not segment limits.)

**Keyframe list for the animator** (one line = one click; values from §6 only):

- Hook — K1 C program; K2 five blobs float up with names; K3 the same program as a grid of
  identical pages; K4 question card.
- Scene 3 — K1 blobs with lengths: code 1000, stack 400, data 1100, heap 1100, library 1200;
  K2 numbers 0–4 attached; K3 `load marks[53]` → ⟨2, 53⟩.
- Scene 4 — K1 empty table rows 0–4; K2 fill base/limit row by row; K3 memory bar: code at
  1400, heap at 3200, data at 4300, stack at 6300, library at 7000; K4 STBR arrow, STLR = 5.
- Scene 5 — per address four clicks (STLR check, row highlight, limit comparator, adder):
  ⟨0, 430⟩: 0 < 5, row 0, 430 < 1000, 1400 + 430 = 1830;
  ⟨2, 53⟩: row 2, 53 < 1100, 4300 + 53 = 4353;
  ⟨3, 852⟩: row 3, 852 < 1100, 3200 + 852 = 4052;
  ⟨4, 1112⟩: row 4, 1112 < 1200, 7000 + 1112 = 8112;
  ⟨1, 399⟩: row 1, 399 < 400, 6300 + 399 = 6699.
- Scene 7 — K1 ⟨1, 400⟩: comparator red, adder never runs; K2 ⟨5, 10⟩: STLR check red;
  K3 trap arrow to the OS, "segmentation fault" caption.
- Scene 8 — K1 P1's table; K2 P2's table; K3 both row 0 = base 1400, limit 1000; K4 two
  arrows meet in one code block; K5 their data rows point to different places.
- Scene 9 — K1 `R W X` column added: code R-X, data RW-, stack RW-; K2 a `store` to
  segment 0 hits the W bit, red protection trap.
- Scene 10 — K1 memory with several gaps; K2 1500-byte segment searches, finds none;
  K3 V19 badge "external fragmentation".
- Scene 11 — K1 ⟨2, 700⟩ enters; K2 row 2: 700 < 1100 ✓, entry points to a page table;
  K3 700 splits into page 2, offset 188; K4 page-table row 2 → frame 17;
  K5 17 × 256 + 188 = 4540; K6 caption "segmentation unit → linear address → paging unit".

**Narration checks** (facts to state on screen):

- "Check first, add second. The adder never sees an offset that failed the check."
- "Valid offsets in a segment of length L are 0 to L − 1."
- "Segment numbers are names, not positions: segment 3 sits below segment 2 in memory here."
- "With paging inside each segment, a segment no longer needs one unbroken hole."
- "Sharing a segment means two tables holding the same base and limit, not two copies."
- "Protection bits live in the segment-table entry, so they cover the whole logical unit."

## 10. On-screen text & notation

- **segment**, **segmented logical address** ⟨`s`, `d`⟩ (scene 3).
- **segment table**, **base**, **limit**, **STBR**, **STLR** (scene 4).
- Rule card: `if (s >= STLR) trap; if (d >= limit[s]) trap; physical = base[s] + d;` (scene 5).
- **sharing**, **protection bits** `R W X` (scenes 8–9).
- **segmentation with paging**, **linear address** (Intel term; scene 11).
- Sizes in bytes; segment numbers 0–4; processes `P1`, `P2`. Keep V20's notation for paging: page `p`, offset `d`, frame `f`.

## 11. End-of-video quiz

1. **(Numerical)** Segment table: segment 0 base 2000 limit 600; segment 1 base 3500 limit 250. Give the physical address or "trap" for ⟨0, 599⟩, ⟨1, 250⟩ and ⟨1, 100⟩. **Answer: 2599; trap; 3600.**
2. **(MCQ)** In pure segmentation, which kind of fragmentation is the main problem?
   (a) internal (b) external (c) both equally (d) none
   **Answer: (b).**
3. **(Numerical)** A segmented logical address has a 16-bit offset field. What is the largest possible segment, in bytes? **Answer: 65536.**
4. **(Numerical)** Segmentation with paging, page size 512 bytes. Address ⟨0, 1300⟩, segment 0 limit 2000; segment 0's page 2 is in frame 7. Physical address? **Answer: page 2, offset 276; 7 × 512 + 276 = 3860.**
5. **(Short answer)** Why is it easier to protect code from being overwritten with segmentation than with pure paging? **Answer:** a segment holds exactly one logical unit, so one read/execute-only bit covers all the code; a page may mix code and data.

## 12. References

- Silberschatz, Galvin & Gagne, *Operating System Concepts*, 8th ed., Ch. 8 — segmentation: user's view, hardware, segment table (p.342); Intel Pentium segmentation with paging (p.345).
- Stallings, *Operating Systems: Internals and Design Principles*, 6th ed., Ch. 7 §7.4 — segmentation (p.330).
- Bovet & Cesati, *Understanding the Linux Kernel*, 3rd ed., Ch. 2 (pp.35–78) — segmentation in hardware (segment selectors, descriptors) and Linux's minimal use of segmentation (optional depth).

## 13. Production notes

- **Pacing:** scene 5 is the core. Same rhythm for each address: show ⟨s, d⟩, highlight row s, say the comparison, then the sum. Speed up for the fourth and fifth addresses once viewers have the pattern.
- Keep the segment table at the same screen position from scene 4 to scene 9.
- Scene 11 must stay simple: no selectors, descriptors, GDT/LDT or privilege rings; mention "Intel used a version of this" in one sentence.
- **Over length?** Shorten scene 9 to a single caption and drop scene 13's table to three rows.
- **Under length?** Add a 30 s aside on how 64-bit x86 effectively turns segmentation off (flat model) and relies on paging.
- **Accessibility:** pass/fail states shown with ✓/✗ icons and text, not just green/red; Okabe–Ito palette; captions on.
- **Recap scene (2):** 20 s maximum; reuse the exact V19 MMU and V20 page-table graphics so the viewer recognises them instantly, then morph the single relocation/limit pair into the first row of the segment table.
- **Number hygiene:** every address on screen comes from §6; do not improvise new addresses while recording.
- **Consistency with V23:** draw the page table in scene 11 with the same column layout V23 will extend with a valid bit.
- **Bridge line (final 10 s):** "So far every page of a running process had to be in memory. What if most of them stayed on disk until someone actually touched them? That's virtual memory and demand paging: V23."

**Time budget** (computed from the storyboard times; see the over-length notes above for what to cut):

| Scene | Title | Seconds | Running total |
|---|---|---|---|
| 1 | Hook | 45 | 0:45 |
| 2 | Recap V19–V20 | 20 | 1:05 |
| 3 | The user's view of memory | 70 | 2:15 |
| 4 | The segment table | 60 | 3:15 |
| 5 | Translation, five worked addresses | 210 | 6:45 |
| 6 | Pause & predict 1 | 30 | 7:15 |
| 7 | The traps | 45 | 8:00 |
| 8 | Sharing | 75 | 9:15 |
| 9 | Protection | 45 | 10:00 |
| 10 | The cost: external fragmentation | 45 | 10:45 |
| 11 | Segmentation with paging (Intel-style, simplified) | 90 | 12:15 |
| 12 | Pause & predict 2 | 30 | 12:45 |
| 13 | Segmentation vs paging summary | 30 | 13:15 |
| 14 | Recap + bridge | 45 | 14:00 |

**Viewer questions to expect in comments** (answers for a pinned reply):

- Is a segmentation fault on Linux caused by segment limits? Usually not today; it is raised by the paging hardware, but the name survives.
- How many entries does a segment table have? One per segment of that process; STLR holds the count.
- Does segmentation with paging have any fragmentation? Only internal, in the last page of each segment.
- Why keep segments at all if paging works? For the programmer's view: sharing and protection per logical unit.
