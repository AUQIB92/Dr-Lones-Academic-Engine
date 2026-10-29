# V24 · L27 — FIFO, Optimal, LRU, and the Anomaly That Shouldn't Exist

> **Lecture:** CS502 L27 `27-page-replacement-basic` · **Unit:** III · **Target length:** 14–16 min · **Series position:** 24/28
> **Format:** screen-capture animation (frame-strip traces) · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

A replacement trace is a process that unfolds over time: a page arrives, a victim is chosen, a slot changes, and a fault counter ticks. Printed as a finished grid, the trace hides the one thing each algorithm does differently, which is *how it chooses the victim*: FIFO's queue pointer, OPT's look into the future, LRU's look into the past. Belady's anomaly in particular has to be *watched*. The 4-frame run starts out ahead of the 3-frame run and then falls apart at reference 7. Students who only read "9 vs. 10" can recite the anomaly but cannot explain it.

## 2. Learning outcomes

After watching, the student can:
- [Apply] Trace FIFO, Optimal and LRU on a reference string with a given number of frames, and count page faults.
- [Analyze] Explain how each algorithm chooses its victim, and why OPT is a lower bound that cannot be implemented.
- [Analyze] Demonstrate Belady's anomaly for FIFO on 1,2,3,4,1,2,5,1,2,3,4,5 (3 vs. 4 frames).
- [Understand] State why LRU and OPT (stack algorithms) cannot exhibit Belady's anomaly.

## 3. Prerequisites

- **Computer fundamentals:** a program's data lives in memory; memory is limited; disk is slower but larger.
- **V20 Paging:** page and frame.
- **V23 Virtual memory and demand paging:** page fault, valid–invalid bit, "a fault means the page is not in a frame". Recapped on screen in scene 2 (20 s).
- *Queue* (first-in, first-out) is defined on screen in scene 5. No classroom lecture is assumed.

## 4. Hook (first 30–45 s)

> *You buy more RAM. You give a program four frames instead of three. Same program, same input. It page-faults **more**.*
>
> "That sounds like a bug in the measurement, but it's a property of one of the most natural algorithms there is. In about ten minutes you'll watch it happen, one reference at a time."

On screen: two counters, "3 frames: 9 faults" and "4 frames: 10 faults", with "?!" between them. The counters stay blurred until after the hook line.

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:40 | Hook | Two fault counters, 9 vs. 10, "?!" | More frames, more faults? We'll watch it happen live |
| 2 | 0:40–1:00 | Recap from V23 (20 s) | A request misses: valid bit 0 → trap → page loaded from disk into a free frame | Page, frame, page fault, demand paging: one line each |
| 3 | 1:00–1:40 | The problem | Three frame slots, all full; a new page knocks; "Who leaves?" | Define *page replacement* and *victim*. Goal: as few faults as possible |
| 4 | 1:40–2:10 | The running string | Reference string `1 2 3 2 4 1 2 5 1 2 3 4 2 5 1` as a tape across the top; 3 empty frames | Define *reference string*. Same string, same 3 frames, three algorithms. The first loads into empty frames count as faults |
| 5 | 2:10–3:10 | FIFO refs 1–8 | A frame strip fills column by column; a queue arrow marks the oldest slot; fault = orange "F", hit = blue dot; evicted pages slide out | Define *queue* and FIFO (first in, first out): the victim is the page that arrived earliest. Ref 6: page 2, used at ref 4, is evicted anyway |
| 6 | 3:10–4:00 | FIFO refs 9–15 | The strip continues; the fault counter reaches **12** | FIFO never looks at how pages are used, only at when they arrived |
| 7 | 4:00–5:00 | OPT refs 1–8 | At each fault a "telescope" draws arrows forward along the tape to each resident page's next use; the page whose next use is farthest away is evicted | Define OPT (Optimal): evict the page not needed for the longest time. Ref 5 (`4`) evicts 3, whose next use is ref 11 |
| 8 | 5:00–5:30 | **Pause-and-predict 1** | OPT at ref 12 (`4`), frames {3, 2, 5}; freeze | See §7 |
| 9 | 5:30–6:10 | OPT refs 12–15 | Telescope arrows; ref 15 has a tie (no resident page is used again) and the leftmost slot is chosen; total **8** | 8 is the minimum possible. OPT needs to know the future, so it serves as a benchmark |
| 10 | 6:10–7:10 | LRU refs 1–8 | A "rear-view mirror" draws arrows backwards to each page's last use; the page used longest ago is evicted | Define LRU (least recently used): use the past to predict the future. Ref 6 (`1`) evicts 3, last used at ref 3 |
| 11 | 7:10–8:00 | LRU refs 9–15 | Counter reaches **10** | Between FIFO and OPT on this string |
| 12 | 8:00–8:40 | Scoreboard | Bars: FIFO 12 · LRU 10 · OPT 8; faded 4-frame bars: 11 · 9 · 6 | OPT ≤ every other algorithm, always. LRU usually lands between, but not always (see §8) |
| 13 | 8:40–9:40 | Belady refs 1–6 | Split screen: FIFO with 3 frames on top, FIFO with 4 frames below, on `1 2 3 4 1 2 5 1 2 3 4 5`; both strips advance in lock-step | After 6 references the 4-frame run leads, 4 faults against 6 |
| 14 | 9:40–10:20 | Belady ref 7 | Both fault on `5`; the top strip evicts 4, the bottom strip evicts 1 | The extra frame changed *which* page is oldest |
| 15 | 10:20–10:50 | **Pause-and-predict 2** | Freeze after ref 8; "What happens at ref 9 (`2`) in each?" | See §7 |
| 16 | 10:50–11:50 | Belady refs 9–12 | The top strip hits at refs 8–9; the bottom strip faults on every reference from 8 to 12; final counters **9 vs. 10** | Watch the cascade: each new page evicts exactly the page that is needed next |
| 17 | 11:50–12:50 | Why LRU and OPT are immune | Table for 1–5 frames: FIFO 12,12,9,10,5 (bump highlighted) · LRU 12,12,10,8,5 · OPT 12,9,7,6,5. Nested sets: pages in n frames ⊆ pages in n+1 frames | Define *stack algorithm*: what is resident with n frames is always a subset of what is resident with n+1, so adding a frame can never hurt. FIFO lacks this property |
| 18 | 12:50–13:40 | Recap + bridge to V25 | Icons: queue (FIFO), telescope (OPT), mirror (LRU); "LRU is expensive. Can we approximate it?"; end card "Next: V25 Clock, frame allocation, thrashing" | LRU needs bookkeeping on *every* reference. Bridge: V25 approximates it with one reference bit and a rotating clock hand, then asks how many frames a process really needs, and what happens when it gets too few (thrashing) |

## 6. Worked example (verified)

**Running string (15 refs), 3 frames:** `1 2 3 2 4 1 2 5 1 2 3 4 2 5 1`. Frames are listed by slot, and a victim is replaced in its own slot. F = fault, h = hit.

### 6.1 FIFO, 3 frames

| # | Ref | Frames | Result | Victim (arrival order) |
|---|---|---|---|---|
| 1 | 1 | 1 – – | F | – |
| 2 | 2 | 1 2 – | F | – |
| 3 | 3 | 1 2 3 | F | – |
| 4 | 2 | 1 2 3 | h | – |
| 5 | 4 | 4 2 3 | F | 1 (oldest) |
| 6 | 1 | 4 1 3 | F | 2 |
| 7 | 2 | 4 1 2 | F | 3 |
| 8 | 5 | 5 1 2 | F | 4 |
| 9 | 1 | 5 1 2 | h | – |
| 10 | 2 | 5 1 2 | h | – |
| 11 | 3 | 5 3 2 | F | 1 |
| 12 | 4 | 5 3 4 | F | 2 |
| 13 | 2 | 2 3 4 | F | 5 |
| 14 | 5 | 2 5 4 | F | 3 |
| 15 | 1 | 2 5 1 | F | 4 |
| | | | **12 faults** | |

### 6.2 OPT, 3 frames

| # | Ref | Frames | Result | Victim (why) |
|---|---|---|---|---|
| 1 | 1 | 1 – – | F | – |
| 2 | 2 | 1 2 – | F | – |
| 3 | 3 | 1 2 3 | F | – |
| 4 | 2 | 1 2 3 | h | – |
| 5 | 4 | 1 2 4 | F | 3 (next use ref 11; 1 @6, 2 @7) |
| 6 | 1 | 1 2 4 | h | – |
| 7 | 2 | 1 2 4 | h | – |
| 8 | 5 | 1 2 5 | F | 4 (next use ref 12; 1 @9, 2 @10) |
| 9 | 1 | 1 2 5 | h | – |
| 10 | 2 | 1 2 5 | h | – |
| 11 | 3 | 3 2 5 | F | 1 (next use ref 15; 2 @13, 5 @14) |
| 12 | 4 | 4 2 5 | F | 3 (never used again) |
| 13 | 2 | 4 2 5 | h | – |
| 14 | 5 | 4 2 5 | h | – |
| 15 | 1 | 1 2 5 | F | 4 (tie: none reused; leftmost slot) |
| | | | **8 faults** | |

### 6.3 LRU, 3 frames

| # | Ref | Frames | Result | Victim (last used) |
|---|---|---|---|---|
| 1 | 1 | 1 – – | F | – |
| 2 | 2 | 1 2 – | F | – |
| 3 | 3 | 1 2 3 | F | – |
| 4 | 2 | 1 2 3 | h | – |
| 5 | 4 | 4 2 3 | F | 1 (@1) |
| 6 | 1 | 4 2 1 | F | 3 (@3) |
| 7 | 2 | 4 2 1 | h | – |
| 8 | 5 | 5 2 1 | F | 4 (@5) |
| 9 | 1 | 5 2 1 | h | – |
| 10 | 2 | 5 2 1 | h | – |
| 11 | 3 | 3 2 1 | F | 5 (@8) |
| 12 | 4 | 3 2 4 | F | 1 (@9) |
| 13 | 2 | 3 2 4 | h | – |
| 14 | 5 | 5 2 4 | F | 3 (@11) |
| 15 | 1 | 5 2 1 | F | 4 (@12) |
| | | | **10 faults** | |

With 4 frames on the same string: FIFO 11, OPT 6, LRU 9.

### 6.4 Belady string `1 2 3 4 1 2 5 1 2 3 4 5`, FIFO

| # | Ref | 3 frames | | 4 frames | | Running faults (3 / 4) |
|---|---|---|---|---|---|---|
| 1 | 1 | 1 – – | F | 1 – – – | F | 1 / 1 |
| 2 | 2 | 1 2 – | F | 1 2 – – | F | 2 / 2 |
| 3 | 3 | 1 2 3 | F | 1 2 3 – | F | 3 / 3 |
| 4 | 4 | 4 2 3 | F (−1) | 1 2 3 4 | F | 4 / 4 |
| 5 | 1 | 4 1 3 | F (−2) | 1 2 3 4 | h | 5 / 4 |
| 6 | 2 | 4 1 2 | F (−3) | 1 2 3 4 | h | 6 / 4 |
| 7 | 5 | 5 1 2 | F (−4) | 5 2 3 4 | F (−1) | 7 / 5 |
| 8 | 1 | 5 1 2 | h | 5 1 3 4 | F (−2) | 7 / 6 |
| 9 | 2 | 5 1 2 | h | 5 1 2 4 | F (−3) | 7 / 7 |
| 10 | 3 | 5 3 2 | F (−1) | 5 1 2 3 | F (−4) | 8 / 8 |
| 11 | 4 | 5 3 4 | F (−2) | 4 1 2 3 | F (−5) | 9 / 9 |
| 12 | 5 | 5 3 4 | h | 4 5 2 3 | F (−1) | **9 / 10** |

### 6.5 Fault counts for 1–5 frames (Belady string)

| Frames | 1 | 2 | 3 | 4 | 5 |
|---|---|---|---|---|---|
| FIFO | 12 | 12 | 9 | **10** | 5 |
| LRU | 12 | 12 | 10 | 8 | 5 |
| OPT | 12 | 9 | 7 | 6 | 5 |

**Counter-example (for §8):** on `1 2 3 2 4 1 3 2 4 5 2 3 1 4 2` with 3 frames, FIFO = 11, OPT = 8, LRU = **13**.

### 6.6 Running string across 1–5 frames (no anomaly here)

| Frames | 1 | 2 | 3 | 4 | 5 |
|---|---|---|---|---|---|
| FIFO | 15 | 14 | 12 | 11 | 5 |
| LRU | 15 | 14 | 10 | 9 | 5 |
| OPT | 15 | 11 | 8 | 6 | 5 |

Hit ratios with 3 frames: FIFO 0.20, LRU 0.3333, OPT 0.4667. Use this for the scene 12 aside: on *this* string FIFO behaves normally, so the anomaly depends on the string.

**Verification.** Every frame state, victim and fault count above, and the quiz traces in §11, were produced by `replace.py` (§6.6 by `extras.py`, which reuses the same simulator) in `/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/` (python3; supplementary tables from `extras.py` in the same folder; one simulator for FIFO/OPT/LRU that prints the trace slot by slot). The running string and the quiz string are original. The Belady string is the classic one, as requested.

## 7. Pause-and-predict checkpoints

1. **Scene 8 (OPT at ref 12, frames {3, 2, 5}).** "Which page does OPT evict?"
   *Answer:* Page 2 is next used at ref 13 and page 5 at ref 14, but page 3 is never used again. Evict **3**.
2. **Scene 15 (Belady, ref 9 = `2`).** "Hit or fault in each strip?"
   *Answer:* With 3 frames {5,1,2} it is a **hit**. With 4 frames {5,1,3,4} it is a **fault**, and page 3 is evicted.
3. *(Optional, scene 11)* "LRU at ref 11 (`3`), frames {5,2,1}. Which page is the victim?"
   *Answer:* The last uses were 5 @8, 1 @9 and 2 @10, so **5** is evicted.

## 8. Misconceptions to address on screen

- **"More frames always means fewer faults."** → Scenes 13–16, plus the highlighted bump in the scene 17 table.
- **"Belady's anomaly affects LRU too."** → Scene 17: LRU's row never increases (nested-set picture).
- **"LRU always beats FIFO."** → A lower-third in scene 12 with the counter-example: FIFO 11 vs. LRU 13.
- **"FIFO evicts the least-used page."** → Scene 5, ref 6: page 2 was just used and is evicted anyway.
- **"The first loads are free."** → Scene 4: cold-start loads count as faults.

## 9. Visual assets

- **Reuse:** the demand-paging graphic from V23 for the recap. `Slides/CS502/27-page-replacement-basic.tex` is not built yet. Style the tables after `Slides/CS502/08-cpu-scheduling-basics.tex`.
- **New:**
  - Frame-strip animation (Manim) with a chooser overlay per algorithm: queue arrow, forward arrows, backward arrows.
  - Lock-step split-screen strips for Belady, with synced counters.
  - Faults-vs.-frames line chart.
  - A candidate for `/render-lecture-video` once the L27 deck exists with a step sequence.
- **Live demo (optional, 40 s):** `python3 replace.py | grep -A13 "FIFO frames=4"`.

### Instructor whiteboard version

1. Write the reference string across the top of the board, one number per column, with wide spacing. Draw 3 rows of empty boxes under it, one row per frame.
2. Work left to right, filling in one column per reference. Copy the previous column, change only the victim's box, and write F or ✓ underneath.
3. **FIFO:** keep a small "queue" list in the margin. Cross off the head and append on every fault.
4. **OPT:** before each eviction, run a finger *right* along the string to each resident page's next occurrence and say the reference numbers aloud.
5. **LRU:** run a finger *left* to each page's last use instead.
6. **Belady:** use two boards side by side, one with 3 rows and one with 4. Fill them in alternately, column by column, and keep a tally under each.
7. Finish by writing the 1–5-frame table and circling FIFO's 9 → 10.

## 10. On-screen text & notation

- **Defined on screen at first use:** page, frame, page fault, demand paging (2); page replacement, victim (3); reference string (4); queue, FIFO, hit (5); OPT (7); LRU (10); Belady's anomaly (13); stack algorithm (17).
- Use **FIFO**, **OPT** (Optimal, also MIN) and **LRU**. Say "page fault" rather than "miss". Keep a legend for F / h on screen.

## 11. End-of-video quiz

Quiz string: `0 1 2 0 3 0 4 2 3 0 3 2`, 3 frames, all initially empty.

1. **(Numerical)** How many page faults under FIFO?
   *Solution:* Frames after each reference: 0F · 01F · 012F · 012h · 312F(−0) · 302F(−1) · 304F(−2) · 204F(−3) · 234F(−0) · 230F(−4) · 230h · 230h → **9**.
2. **(Numerical)** Under OPT?
   *Solution:* 0F · 01F · 012F · 012h · 032F(−1: never reused) · 032h · 432F(−0: next use @10, the farthest) · 432h · 432h · 032F(−4: never reused) · 032h · 032h → **6**.
3. **(Numerical)** Under LRU?
   *Solution:* 0F · 01F · 012F · 012h · 032F(−1) · 032h · 034F(−2) · 024F(−3) · 324F(−0) · 320F(−4) · 320h · 320h → **8**.
4. **(MCQ)** Which algorithm(s) can suffer from Belady's anomaly? (a) FIFO only (b) LRU only (c) FIFO and LRU (d) OPT
   *Solution:* LRU and OPT are stack algorithms: the resident set with n frames is always a subset of the set with n+1, so faults cannot rise. FIFO has no such property. **(a)**.
5. **(MCQ)** On `1 2 3 4 1 2 5 1 2 3 4 5`, how many FIFO faults with 5 frames? (a) 5 (b) 9 (c) 10 (d) 12
   *Solution:* There are only 5 distinct pages, so with 5 frames each faults once and nothing is ever evicted. **(a)**.

## 12. References

- Silberschatz 8e, Ch. 9: page replacement p.369; FIFO p.371; Optimal p.374; Belady's anomaly p.374; LRU p.375; LRU approximation (leads into V25) p.378.
- Stallings 6e, Ch. 8: page-replacement policies p.368 (§8.1); OS software §8.2 p.365. *Stallings does not cover Optimal or Belady's anomaly in its text, so cite Silberschatz p.374 for those.*
- Bovet & Cesati 3e, Ch. 17 "Page Frame Reclaiming" pp.676–737: Linux's LRU lists.

## 13. Production notes

- **Pacing:** narrate every column of FIFO. For OPT and LRU you can skip the narration on hits. Slow the Belady split screen to about 10 s per reference after ref 6.
- **Cut if over length:** run LRU refs 9–15 at ×2, then drop checkpoint 3. Never shorten scenes 13–16.
- **Accessibility:** mark F / h with a letter *and* a colour, and give each page number its own Okabe–Ito colour. Captions on. No fast flashing.

### Timing budget and cut priority

| # | Scene | Seconds | Cumulative | Priority |
|---|---|---|---|---|
| 1 | Hook | 40 | 0:40 | keep (core) |
| 2 | Recap from V23 (20 s) | 20 | 1:00 | keep (core) |
| 3 | The problem | 40 | 1:40 | keep (core) |
| 4 | The running string | 30 | 2:10 | keep (core) |
| 5 | FIFO refs 1–8 | 60 | 3:10 | keep (core) |
| 6 | FIFO refs 9–15 | 50 | 4:00 | keep (core) |
| 7 | OPT refs 1–8 | 60 | 5:00 | keep (core) |
| 8 | **Pause-and-predict 1** | 30 | 5:30 | keep (core) |
| 9 | OPT refs 12–15 | 40 | 6:10 | keep (core) |
| 10 | LRU refs 1–8 | 60 | 7:10 | keep (core) |
| 11 | LRU refs 9–15 | 50 | 8:00 | shorten first |
| 12 | Scoreboard | 40 | 8:40 | keep (core) |
| 13 | Belady refs 1–6 | 60 | 9:40 | keep (core) |
| 14 | Belady ref 7 | 40 | 10:20 | keep (core) |
| 15 | **Pause-and-predict 2** | 30 | 10:50 | keep (core) |
| 16 | Belady refs 9–12 | 60 | 11:50 | keep (core) |
| 17 | Why LRU and OPT are immune | 60 | 12:50 | shorten first |
| 18 | Recap + bridge to V25 | 50 | 13:40 | keep (core) |

Core (keep) scenes total 11:50; shortenable 1:50; cuttable 0:00. Halving every "shorten first" scene and dropping every "cut first" scene saves 0:55.
