# V20 · L23 — Paging: Cutting an Address in Two

> **Lecture:** CS502 L23 `23-paging` · **Unit:** III · **Target length:** 11–14 min · **Series position:** 20/28
> **Format:** screen-capture animation (bit-level) · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

Address translation is a data path. The key event is that the *offset bits pass through unchanged*, while only the page-number bits are replaced by a frame number. A slide shows the address before and after. An animation shows 16 bits splitting apart, the high 6 bits travelling to the page table, a 5-bit frame number coming back, and the low 10 offset bits sliding straight across. Students who learn paging from slides alone tend to add the frame number to the logical address, multiply the page number by the page size without the lookup, or forget that the split lands on a bit boundary only because the page size is a power of two.

## 2. Learning outcomes

After watching, the student can:
- [Remember] Define page, frame, page table, page number `p`, offset `d`, and frame number `f`.
- [Apply] Given an address width and a page size, compute the number of offset bits, page-number bits, and page-table entries.
- [Apply] Translate a logical address (hex or decimal) to a physical address using a given page table, or detect an invalid page.
- [Understand] Explain why paging removes external fragmentation but leaves internal fragmentation, and compute the internal fragmentation for a given process size.

## 3. Prerequisites

- **Computer fundamentals:** memory is an array of numbered bytes; a program's instructions and data sit in memory; numbers can be written in binary and hex.
- **V19 Memory basics:** logical vs. physical address, the MMU, external fragmentation. Recapped on screen in scene 2 (20 s).
- **V05 Process:** each process has its own memory image (the reason there is one page table per process).
- Binary, hex and powers of two are refreshed on screen in scene 4. No classroom lecture is assumed.

## 4. Hook (first 30–45 s)

> *A program asks for the byte at address `0x1F05`. The memory chip does have a byte at `0x1F05`, but the CPU actually fetches the byte at `0x2F05`. Only the first hex digit changed, and the last three stayed exactly the same. Why?*
>
> "By the end of this video you'll be able to do this translation in your head, and you'll see exactly which part of an address can never change."

On screen: the two addresses stacked, with the first digit pulsing in orange and `F05` in a calm blue. Scene 9 gives the precise rule: the low **10 bits** never change. The hex digits only appear to survive whole in this particular address. §8 covers the exception.

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:40 | Hook | `0x1F05 → 0x2F05`; the first hex digit pulses | Pose the puzzle; promise a rule you can apply in your head |
| 2 | 0:40–1:00 | Recap from V19 (20 s) | CPU emits a *logical address* → MMU box → *physical address* on the RAM chip; a memory map with scattered free holes | Define logical address (what the program uses), physical address (what RAM sees), MMU (the hardware that translates), external fragmentation (enough free memory in total, but no single hole big enough) |
| 3 | 1:00–1:50 | Why paging | The V19 memory map; a process block fails to fit any hole; the block shatters into equal tiles that drop into scattered free slots | Define *page* (fixed-size piece of a process) and *frame* (same-size slot of physical memory). Contiguity is no longer needed |
| 4 | 1:50–2:30 | Binary refresher | Counter: 1 bit → 2 values, 2 bits → 4 …, 10 bits → 1024; one hex digit shown as 4 bits | 2^10 = 1024, so 1 KB = 1024 B. One hex digit is exactly 4 bits |
| 5 | 2:30–3:20 | The running machine | Spec card: 16-bit logical address · page size 1 KB · 32 frames. A 16-cell bit-ruler; the low 10 cells shade blue (offset), the high 6 orange (page number) | Define *offset* `d` (position inside the page) and *page number* `p`. 10 offset bits; 6 page bits ⇒ 64 pages; 32 frames ⇒ 5-bit frame number `f`; 32 KB ⇒ 15-bit physical address |
| 6 | 3:20–4:00 | The page table | Table with 8 visible rows: 0→5, 1→9, 2→7, 3→invalid, 4→2, 5→invalid, 6→27, 7→11; a valid/invalid column; rows 8–63 greyed | Define *page table*: one per process, indexed by `p`, each entry holding `f`. Define *valid–invalid bit*: is this page actually in memory? |
| 7 | 4:00–4:40 | Split | `0x0ABC` in hex → expands to `0000 1010 1011 1100` → a cut line slides in 10 bits from the right → `000010 \| 1010111100` | Step 1 of 3: split. `p = 2`, `d = 700` |
| 8 | 4:40–5:20 | Look up | The orange block flies to the table and lands on row 2; the row glows; `7` (binary `00111`) flies back | Step 2: look up. `f = 7` |
| 9 | 5:20–6:00 | Glue | `00111` snaps onto the untouched blue block → `00111 1010111100` → `0x1EBC` = 7868; a formula line appears: `7 × 1024 + 700 = 7868` | Step 3: glue. The MMU does no arithmetic on the offset; it just rewires bits. Physical = `f × page size + d` |
| 10 | 6:00–6:30 | **Pause-and-predict 1** | `0x1F05` (the hook) with the cut already drawn; freeze | See §7 |
| 11 | 6:30–7:10 | Hook resolved | Same three steps at ×1.5: `000111 \| 1100000101`, `p = 7`, `d = 773`, `f = 11` → `0x2F05` = 12037 | Payoff: frame 11 starts at 11 × 0x400 = 0x2C00, plus 0x305 gives 0x2F05 |
| 12 | 7:10–7:50 | Invalid page | `0x0C10` → `p = 3`; row 3 flashes "invalid" → a lightning bolt to an OS icon | Define *trap* (the hardware hands control to the OS). Preview: V23 turns this trap into a page fault |
| 13 | 7:50–8:30 | Edge of a page | `0x13FF` → `p = 4`, `d = 1023` (the last byte) → `f = 2` → `0x0BFF` | The last byte of a page maps to the last byte of its frame; the offset carries no surprise |
| 14 | 8:30–9:20 | Decimal (exam) method | Long-division overlay: `5000 ÷ 1024` → quotient 4, remainder 904 → row 4 → f = 2 → `2·1024 + 904 = 2952` | `p = ⌊LA / page size⌋`, `d = LA mod page size`. Same result as the bit-split, with no binary required |
| 15 | 9:20–9:50 | **Pause-and-predict 2** | Decimal 7500; freeze | See §7 |
| 16 | 9:50–10:50 | What paging costs | A 5420-B process tiled into 6 pages; the last page is filled 300/1024; the 724-B tail is hatched | Define *internal fragmentation*: waste inside the last page, at most page size − 1. External fragmentation is gone |
| 17 | 10:50–11:30 | Recap + bridge to V21 | Rule card: split → look up → glue. A stopwatch icon over the table lookup; end card "Next: V21 TLB and page-table structures" | Every load/store now costs two memory reads, and the table itself is large. Bridge: V21 fixes the speed with a tiny cache (the TLB) and the size with multilevel tables |

## 6. Worked example (verified)

### 6.1 Machine

| Quantity | Value | How |
|---|---|---|
| Logical address width | 16 bits | given |
| Page size | 1 KB = 1024 B | given |
| Offset bits | 10 | 1024 = 2^10 |
| Page-number bits | 6 | 16 − 10 |
| Pages per process | 64 | 2^6 |
| Frames | 32 | given |
| Physical memory | 32768 B | 32 × 1024 |
| Physical address bits | 15 | 32768 = 2^15 |
| Frame-number bits | 5 | 2^5 = 32 |

### 6.2 Page table (visible rows)

| p | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|---|---|---|---|---|---|---|---|---|
| f | 5 | 9 | 7 | invalid | 2 | invalid | 27 | 11 |

### 6.3 Translation 1: `0x0ABC` (scenes 7–9)

| Step | Action | Value |
|---|---|---|
| 1 | Hex → binary | 0000 1010 1011 1100 |
| 2 | Split 6 \| 10 | 000010 \| 1010111100 |
| 3 | p, d | p = 2, d = 700 (0x2BC) |
| 4 | Look up row 2 | f = 7 (00111) |
| 5 | Glue | 00111 1010111100 |
| 6 | Physical | 7·1024 + 700 = **7868 = 0x1EBC** |

### 6.4 Translation 2: `0x1F05` (hook, scene 11)

| Step | Action | Value |
|---|---|---|
| 1 | Binary | 0001 1111 0000 0101 |
| 2 | Split | 000111 \| 1100000101 |
| 3 | p, d | p = 7, d = 773 (0x305) |
| 4 | Look up | f = 11 (01011) |
| 5 | Physical | 11·1024 + 773 = **12037 = 0x2F05** |

### 6.5 Remaining translations

| Logical | Decimal | Split `p \| d` | p | d | f | Physical |
|---|---|---|---|---|---|---|
| 0x0C10 | 3088 | 000011 \| 0000010000 | 3 | 16 | — | **invalid → trap** |
| 0x13FF | 5119 | 000100 \| 1111111111 | 4 | 1023 | 2 | 3071 = 0x0BFF |
| 0x1C00 | 7168 | 000111 \| 0000000000 | 7 | 0 | 11 | 11264 = 0x2C00 |
| 0x0000 | 0 | 000000 \| 0000000000 | 0 | 0 | 5 | 5120 = 0x1400 |

### 6.6 Decimal method

| LA | ÷ 1024 → p | mod 1024 → d | f | f·1024 + d |
|---|---|---|---|---|
| 5000 | 4 | 904 | 2 | **2952** |
| 7500 | 7 | 332 | 11 | **11596** |

### 6.7 Internal fragmentation

A process of 5420 B (5 KB + 300 B) needs ⌈5420/1024⌉ = 6 pages = 6144 B, which wastes **724 B**.

### 6.8 Hex shortcut (frame base + offset)

With 1 KB pages, frame `f` starts at `f × 0x400`, and the physical address is that base plus the offset in hex.

| Logical | p | d (hex) | f | Frame base | Physical |
|---|---|---|---|---|---|
| 0x0ABC | 2 | 0x2BC | 7 | 0x1C00 | 0x1EBC |
| 0x1F05 | 7 | 0x305 | 11 | 0x2C00 | 0x2F05 |
| 0x13FF | 4 | 0x3FF | 2 | 0x0800 | 0x0BFF |
| 0x1C00 | 7 | 0x000 | 11 | 0x2C00 | 0x2C00 |
| 0x0000 | 0 | 0x000 | 5 | 0x1400 | 0x1400 |

### 6.9 Same addresses with 2 KB pages (for the "what if the page size changes?" aside in scene 16)

With 2 KB pages there are 11 offset bits, 5 page bits and 32 pages. The split moves, so the *same* logical address lands on a different page.

| Logical | p (2 KB pages) | d |
|---|---|---|
| 0x0ABC | 1 | 700 (0x2BC) |
| 0x1F05 | 3 | 1797 (0x705) |
| 0x13FF | 2 | 1023 (0x3FF) |

### 6.10 Geometry drill (on-screen card for scene 5; also homework)

The number of offset bits is log₂(page size). The page-number bits are whatever remains of the address.

| Address bits | Page size | Offset bits | Page bits | Page-table entries |
|---|---|---|---|---|
| 16 | 512 B | 9 | 7 | 128 |
| 16 | 1 KB | 10 | 6 | 64 |
| 16 | 4 KB | 12 | 4 | 16 |
| 20 | 1 KB | 10 | 10 | 1024 |
| 32 | 4 KB | 12 | 20 | 1,048,576 |
| 32 | 8 KB | 13 | 19 | 524,288 |

Narration point: doubling the page size moves the cut one bit to the left, so it halves the number of table entries and doubles the worst-case internal fragmentation.

**Verification.** All translations, bit strings, sizes and the fragmentation figure were computed by `paging.py` (§6.8–6.10 by `extras.py`), and the quiz values by `quiz.py`. All three scripts are in `/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/` (python3; supplementary tables from `extras.py` in the same folder; uses shift/mask and `divmod`, and prints both the binary split and the hex result). The page table is original.

## 7. Pause-and-predict checkpoints

1. **Scene 10 (`0x1F05`).** "Which row, and what physical address?"
   *Answer:* `p = 7`, `d = 773`; row 7 gives `f = 11`, so 12037 = 0x2F05.
2. **Scene 15 (decimal 7500).** "Page, offset, physical?"
   *Answer:* 7500 = 7·1024 + 332, so `p = 7`, `d = 332`; `f = 11`, giving 11·1024 + 332 = **11596**.
3. *(Optional)* "How many entries does this process's page table have, even though we drew only eight?"
   *Answer:* 2^6 = 64.

## 8. Misconceptions to address on screen

- **"Physical = logical + frame number"** (relocation-register thinking from V19). → Scene 9: the frame bits *replace* the page bits, and nothing is added.
- **"Physical = p × page size + d"** (skipping the lookup). → Scene 9: `2·1024 + 700 = 2748` is shown crossed out beside the correct 7868.
- **"The last hex digits always survive."** → Only the *offset bits* survive. Here the offset is 10 bits, which is 2.5 hex digits. In `0x0ABC → 0x1EBC` the third digit changes from A to E, because two of its bits belong to `p`.
- **"Pages and frames can differ in size."** → Scene 3: the tiles are equal, and the definition card says "same size, always".
- **"Bigger pages are always better (smaller tables)."** → §6.10 card: a bigger page halves the table but doubles the worst-case internal fragmentation. Show both bars moving in opposite directions.
- **"The page table lives inside the MMU."** → Scene 6 caption: the table is in ordinary memory, and the MMU only knows where it starts. This sets up V21's "two memory reads" problem.
- **"Paging has no fragmentation."** → Scene 16: the hatched 724-B tail.

## 9. Visual assets

- **Reuse:** `Slides/CS502/23-paging.tex` is not built yet. Once it is, reuse its paging-hardware figure (CPU → p|d → page table → f|d → memory). Reuse the memory-map graphic from V19. Style tables after `Slides/CS502/08-cpu-scheduling-basics.tex`.
- **New:**
  - Manim bit-ruler: 16 cells, cut line, a travelling page block, a returning frame block. V21 reuses it for the 10|10|12 split.
  - Memory-tile shatter for scene 3 (a PowerPoint morph is enough).
  - Long-division overlay for scene 14.
- **Live demo (optional, 20 s):** in a Python REPL, `la = 0x1F05; la >> 10, la & 0x3FF` → `(7, 773)`, then `hex(11*1024 + 773)` → `'0x2f05'`.

### Instructor whiteboard version

1. Draw a 16-box strip across the top of the board. Write the 16 bits of `0x0ABC` into it in nibble groups.
2. Draw a thick vertical line after the 6th box. Colour the left 6 boxes with one marker and the right 10 with another, and keep those two colours for the rest of the recording.
3. Draw the 8-row page table on the right. Draw an arrow from the left block to row 2, and circle the 7.
4. Below the strip, draw a 15-box strip. Write `00111` in the left 5 boxes, then *copy* the 10 offset bits straight down into the right boxes, with vertical arrows to show they are unchanged.
5. Convert to hex beside it. Then rub out only the two strips and redo them for `0x1F05` and `0x0C10` (for the invalid case, draw a lightning bolt and write "trap → OS").
6. Do the decimal method as ordinary long division in a corner of the board.

## 10. On-screen text & notation

- **Defined on screen at first use:** logical/physical address, MMU, external fragmentation (2); page, frame (3); bit, KB, hex digit (4); offset, page number, frame number (5); page table, valid–invalid bit (6); trap (12); internal fragmentation (16).
- Page number **`p`**, offset **`d`**, frame number **`f`**. Never reuse `p` for the frame.
- Physical address = **`f × (page size) + d`**, equivalently `f · 2^10 + d` here, where the exponent is the number of offset bits. Never write `2^d`.
- Hex with the `0x` prefix. Show the binary split with a vertical bar `p | d`.

## 11. End-of-video quiz

1. **(Numerical)** Page size 2 KB, 16-bit logical address `0x3A7F`. Give `p` and `d`. If page `p` maps to frame 3, give the physical address in hex.
   *Solution:* 2 KB = 2^11, so there are 11 offset bits and 5 page bits. `0x3A7F` = 0011 1010 0111 1111 splits as 00111 | 01001111111, so `p = 7` and `d` = 0x27F = 639. Physical = 3·2048 + 639 = 6783 = **0x1A7F**.
2. **(Numerical)** A 32-bit logical address space uses 8 KB pages. How many offset bits, and how many page-table entries?
   *Solution:* 8 KB = 2^13, so there are **13** offset bits. Page bits = 32 − 13 = 19, so there are 2^19 = **524,288** entries.
3. **(Numerical)** Physical memory is 1 GB and the frame size is 4 KB. How many frames, and how many bits in a frame number?
   *Solution:* 2^30 / 2^12 = 2^18 = **262,144** frames, so a frame number needs **18** bits.
4. **(MCQ)** Paging eliminates: (a) internal fragmentation (b) external fragmentation (c) both (d) neither
   *Solution:* Any free frame can hold any page, so there are no unusable holes *between* allocations. The last page can still be partly empty. **(b)**.
5. **(MCQ)** In the video's machine, logical address `0x0C10` results in: (a) physical 0x1C10 (b) physical 0x0C10 (c) a trap to the OS (d) physical 0x2810
   *Solution:* 0x0C10 = 000011 | 0000010000, so `p = 3`. Row 3 is marked invalid, so the MMU traps to the OS. **(c)**.

## 12. References

- Silberschatz 8e, Ch. 8: background, logical/physical addresses and the MMU p.315; fragmentation p.327; paging p.328 (§8.4, pp.328–337).
- Stallings 6e, Ch. 7: partitioning and fragmentation §7.2 p.315; paging §7.3 p.326.
- Bovet & Cesati 3e, Ch. 2 "Memory Addressing" pp.35–78: paging in hardware (x86) and in Linux.

## 13. Production notes

- **Pacing:** the first translation (scenes 7–9) should take about 2 minutes across its three steps. Let each motion finish before you narrate the next one. Later translations can run at ×1.5.
- **Cut if over length:** drop checkpoint 3 first, then scene 13. Keep scene 12, because it sets up V23.
- **Recording:** use a monospaced font for bits, and keep the page/frame bits and offset bits in two consistent colours.
- **Continuity:** V21 opens by replaying scenes 7–9 at ×3, so export them as a separate clip.
- **Description box:** paste the §6.10 geometry table as practice, with the answers hidden behind a timestamp to the explanation.
- **Accessibility:** use Okabe–Ito orange for page/frame bits and blue for offset bits. Captions on. Read binary aloud in nibble groups.

### Timing budget and cut priority

| # | Scene | Seconds | Cumulative | Priority |
|---|---|---|---|---|
| 1 | Hook | 40 | 0:40 | keep (core) |
| 2 | Recap from V19 (20 s) | 20 | 1:00 | keep (core) |
| 3 | Why paging | 50 | 1:50 | keep (core) |
| 4 | Binary refresher | 40 | 2:30 | shorten first |
| 5 | The running machine | 50 | 3:20 | keep (core) |
| 6 | The page table | 40 | 4:00 | keep (core) |
| 7 | Split | 40 | 4:40 | keep (core) |
| 8 | Look up | 40 | 5:20 | keep (core) |
| 9 | Glue | 40 | 6:00 | keep (core) |
| 10 | **Pause-and-predict 1** | 30 | 6:30 | keep (core) |
| 11 | Hook resolved | 40 | 7:10 | keep (core) |
| 12 | Invalid page | 40 | 7:50 | keep (core) |
| 13 | Edge of a page | 40 | 8:30 | cut first |
| 14 | Decimal (exam) method | 50 | 9:20 | shorten first |
| 15 | **Pause-and-predict 2** | 30 | 9:50 | shorten first |
| 16 | What paging costs | 60 | 10:50 | keep (core) |
| 17 | Recap + bridge to V21 | 40 | 11:30 | keep (core) |

Core (keep) scenes total 8:50; shortenable 2:00; cuttable 0:40. Halving every "shorten first" scene and dropping every "cut first" scene saves 1:40.
