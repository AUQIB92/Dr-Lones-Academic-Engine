# V27 · L34–L36 — Where the Time Goes on a Disk, and Six Ways to Move the Arm

> **Lecture:** CS502 L34–L36 `34-disk-structure-performance`, `35-disk-scheduling-basic`, `36-disk-scheduling-advanced-raid` (scheduling part) · **Unit:** IV · **Target length:** 16–18 min · **Series position:** 27/28
> **Format:** screen-capture animation (platter cut-away + head-movement chart) · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

Disk scheduling is about motion. What gets optimised is the distance a mechanical arm travels. A static head-movement chart shows the zig-zag but not what it costs. Watching the arm swing 158 cylinders for one request while another request sits 7 cylinders away makes FCFS's waste obvious. Watching SSTF bounce near the head while a far request starves shows its trap. Six algorithms on one queue only stick in memory as six visibly different paths over the same pins. Before any of that, students need to see *why* the arm is the bottleneck, and a worked access-time example where seek is about two-thirds of the total settles it. Students who learn from slides alone sum the wrong legs, forget SCAN's trip to the edge, confuse SCAN with LOOK, and argue over whether C-SCAN's return jump counts.

## 2. Learning outcomes

After watching, the student can:
- [Remember] Name the parts of a hard disk (platter, track, sector, cylinder, arm, head) and the three components of access time.
- [Apply] Compute average access time from seek time, RPM, transfer rate and block size.
- [Apply] Produce the service order and total head movement for FCFS, SSTF, SCAN, C-SCAN, LOOK and C-LOOK on one queue, stating the direction and the return-jump convention.
- [Analyze] Compare the six algorithms on movement and fairness, and explain why SSTF can starve requests and is not optimal.

## 3. Prerequisites

- **Computer fundamentals:** a file is stored on a disk; a program asks to read or write a block of it; reading a disk is much slower than reading memory.
- **V26 I/O:** a device, its controller, and an I/O request queued by the OS. Recapped on screen in scene 2 (20 s).
- **V05 Process:** processes issue I/O requests and wait for them, which is why many requests queue at once.
- No classroom lecture is assumed. Every disk term is defined on screen in scenes 3–5.

## 4. Hook (first 30–45 s)

> *The disk's read head is resting on track 63, and eight requests are waiting. Serve them in arrival order and the arm travels 699 tracks. Reorder them and it travels 239. Same requests, same disk, about a third of the arm travel.*
>
> "Today we'll find out why the arm is the part worth saving, and race six ways of ordering those eight requests. One of the cleverest-looking ones can leave a request waiting forever."

On screen: a horizontal axis 0–199, the head at 63, eight request pins, and a big counter that spins to 699 and then rewinds to 239.

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:40 | Hook | Axis 0–199; head at 63; pins 112, 41, 176, 18, 95, 150, 33, 70 labelled with arrival order 1–8; counter 699 → 239 | Same work, very different cost; tease starvation |
| 2 | 0:40–1:00 | Recap from V26 (20 s) | Process → system call → OS → device queue → controller → disk; several requests stack in the queue | Device, controller and request queue, one line each. Many pending requests mean the OS gets to choose the order |
| 3 | 1:00–1:50 | Platters, tracks, sectors | Cut-away: platters spin; one concentric ring highlights (*track*); a pie-slice of that ring highlights (*sector*) | Define *platter*, *track* and *sector* (the smallest unit read) |
| 4 | 1:50–2:30 | Cylinders, arm, heads | The same track on every platter lights up together (*cylinder*); the arm assembly moves its heads in and out together | Define *cylinder*, *arm* and *head*. Cylinders are numbered 0–199 from the outer edge. From here on, "track" and "cylinder" both mean the head position |
| 5 | 2:30–3:30 | Three costs of one read | The arm swings (*seek*); the head waits while the sector rotates underneath (*rotational latency*); bits stream off (*transfer*); a stacked bar builds | Define all three. Access time = seek + rotational latency + transfer |
| 6 | 3:30–4:40 | Numeric example (whiteboard) | Spec card: 7200 RPM, average seek 9 ms, 100 MB/s, 4 KB block; four lines of arithmetic (§6.1); pie chart with seek ≈ 68 % | Define *RPM*. Seek dominates and transfer is negligible. Seek time grows with the number of cylinders crossed |
| 7 | 4:40–5:00 | Setup card | Cylinders 0–199; head 63; queue in arrival order; SCAN-family direction = towards 199 | Define *total head movement* = the sum of the cylinder distances moved. Fix the conventions once |
| 8 | 5:00–6:20 | FCFS | Head-movement chart (cylinders across, time downward). Path 63→112→41→176→18→95→150→33→70; each leg labelled; running sum to **699** | Define FCFS. It is fair, but the arm criss-crosses the disk; the 176→18 leg alone is 158 |
| 9 | 6:20–7:40 | SSTF | Before each move, rays fan out to every pending pin and the shortest one lights; path 63→70→95→112→150→176→41→33→18; **271** | Define SSTF (shortest seek time first: always go to the nearest request). Note the 135-cylinder jump back once the high side is exhausted |
| 10 | 7:40–8:10 | **Pause-and-predict 1** | Freeze at head = 63 with rays to 70 (7) and 41 (22) | See §7 |
| 11 | 8:10–9:00 | SSTF starvation | New ghost pins keep dropping in near 150–180; the head bounces among them; the pin at 18 dims as its "waiting…" timer grows | Define *starvation*. Greedy also isn't optimal, as scene 13 will show |
| 12 | 9:00–10:20 | SCAN | Head sweeps up 63→70→95→112→150→176→**199** (the edge flashes with no request) → reverses → 41→33→18; **317** | Define SCAN (the elevator algorithm). Waiting is bounded, but here it moves *more* than SSTF because of the trip to the edge |
| 13 | 10:20–11:00 | Direction matters | Ghost replay of SCAN going *down* first: 63→41→33→18→0→70→95→112→150→176 = **239** | 317 vs. 239. Always read the stated direction. 239 < 271, so SSTF is not optimal |
| 14 | 11:00–12:10 | C-SCAN | Sweep up to 199; a dashed fly-back 199→0 that serves nothing; then 18→33→41; counters **376** (return counted) and **177** (service only) | Define C-SCAN (circular SCAN): it serves in one direction only, which evens out waiting times. Our convention counts the return unless a question says otherwise |
| 15 | 12:10–12:40 | **Pause-and-predict 2** | "SCAN up, but reverse at the last request (176) instead of 199. New total?" | See §7. This sets up LOOK |
| 16 | 12:40–13:30 | LOOK | SCAN's path with the 176→199→176 overshoot erased → **271** | Define LOOK: SCAN that turns at the last request instead of the edge. On this queue it happens to equal SSTF |
| 17 | 13:30–14:20 | C-LOOK | Up to 176, a dashed jump to 18 (the lowest request), then 33, 41 → **294** with the jump, **136** without | Define C-LOOK: C-SCAN that turns at the last request and jumps only as far as the lowest pending request |
| 18 | 14:20–15:10 | Scoreboard | Bars: FCFS 699 · SSTF 271 · SCAN 317 · C-SCAN 376 · LOOK 271 · C-LOOK 294 (dashed sub-bars 177/136); fairness row underneath | Least movement and fairness are separate criteria. The SCAN family bounds waiting time, and the C- variants even it out |
| 19 | 15:10–16:00 | Recap + bridge to V28 | Six-row card; the hook counter again; end card "Next: V28 File systems" | The arm is the cost, and order is the lever. Bridge: these blocks belong to files. V28, the final video, shows how the OS organises blocks into files and directories |

## 6. Worked example (verified)

### 6.1 Access time (scene 6)

| Step | Quantity | Calculation | Value |
|---|---|---|---|
| 1 | One rotation | 60000 ms / 7200 | 8.333 ms |
| 2 | Average rotational latency | ½ × 8.333 | 4.167 ms |
| 3 | Transfer of 4 KB | 4096 B ÷ (100 · 2^20 B/s) | 0.039 ms |
| 4 | Seek | given | 9 ms |
| 5 | **Average access time** | 9 + 4.167 + 0.039 | **13.206 ms** |
| 6 | Seek share | 9 / 13.206 | 68.15 % |

### 6.2 Scheduling input

Cylinders 0–199, head at **63**, queue in arrival order **112, 41, 176, 18, 95, 150, 33, 70**. The SCAN family moves towards 199 unless stated otherwise.

### 6.3 FCFS: each head move

| Move | From → To | Distance | Running total |
|---|---|---|---|
| 1 | 63 → 112 | 49 | 49 |
| 2 | 112 → 41 | 71 | 120 |
| 3 | 41 → 176 | 135 | 255 |
| 4 | 176 → 18 | 158 | 413 |
| 5 | 18 → 95 | 77 | 490 |
| 6 | 95 → 150 | 55 | 545 |
| 7 | 150 → 33 | 117 | 662 |
| 8 | 33 → 70 | 37 | **699** |

### 6.4 SSTF: each head move (no distance ties occurred)

| Move | From → To | Distance | Nearest alternative | Running total |
|---|---|---|---|---|
| 1 | 63 → 70 | 7 | 41 (22) | 7 |
| 2 | 70 → 95 | 25 | 41 (29) | 32 |
| 3 | 95 → 112 | 17 | 41 (54) | 49 |
| 4 | 112 → 150 | 38 | 176 (64) | 87 |
| 5 | 150 → 176 | 26 | 41 (109) | 113 |
| 6 | 176 → 41 | 135 | 33 (143) | 248 |
| 7 | 41 → 33 | 8 | 18 (23) | 256 |
| 8 | 33 → 18 | 15 | – | **271** |

### 6.5 SCAN (up): each head move

| Move | From → To | Distance | Running total |
|---|---|---|---|
| 1 | 63 → 70 | 7 | 7 |
| 2 | 70 → 95 | 25 | 32 |
| 3 | 95 → 112 | 17 | 49 |
| 4 | 112 → 150 | 38 | 87 |
| 5 | 150 → 176 | 26 | 113 |
| 6 | 176 → 199 (edge) | 23 | 136 |
| 7 | 199 → 41 | 158 | 294 |
| 8 | 41 → 33 | 8 | 302 |
| 9 | 33 → 18 | 15 | **317** |

SCAN (down): 63→41 (22), →33 (8), →18 (15), →0 (18), →70 (70), →95 (25), →112 (17), →150 (38), →176 (26) = **239**.

### 6.6 C-SCAN (up): each head move

| Move | From → To | Distance | Running total |
|---|---|---|---|
| 1–5 | 63 → 70 → 95 → 112 → 150 → 176 | 7, 25, 17, 38, 26 | 113 |
| 6 | 176 → 199 (edge) | 23 | 136 |
| 7 | 199 → 0 (return, no service) | 199 | 335 |
| 8 | 0 → 18 | 18 | 353 |
| 9 | 18 → 33 | 15 | 368 |
| 10 | 33 → 41 | 8 | **376** (177 excluding the return) |

### 6.7 LOOK (up) and C-LOOK (up)

| Algorithm | Path | Legs | Total |
|---|---|---|---|
| LOOK | 63, 70, 95, 112, 150, 176, 41, 33, 18 | 7, 25, 17, 38, 26, 135, 8, 15 | **271** |
| C-LOOK | 63, 70, 95, 112, 150, 176, →18, 33, 41 | 7, 25, 17, 38, 26, 158, 15, 8 | **294** (136 excluding the jump) |

### 6.8 Summary

| FCFS | SSTF | SCAN up | SCAN down | C-SCAN | LOOK | C-LOOK |
|---|---|---|---|---|---|---|
| 699 | 271 | 317 | 239 | 376 / 177 | 271 | 294 / 136 |

### 6.9 Fairness: cylinders travelled before each request is served

The columns are in arrival order. This is the waiting-time proxy behind the scene 18 fairness row.

| Algorithm | 112 | 41 | 176 | 18 | 95 | 150 | 33 | 70 | Mean | Worst |
|---|---|---|---|---|---|---|---|---|---|---|
| FCFS | 49 | 120 | 255 | 413 | 490 | 545 | 662 | 699 | 404.125 | 699 |
| SSTF | 49 | 248 | 113 | 271 | 32 | 87 | 256 | 7 | 132.875 | 271 |
| SCAN | 49 | 294 | 113 | 317 | 32 | 87 | 302 | 7 | 150.125 | 317 |
| C-SCAN | 49 | 376 | 113 | 353 | 32 | 87 | 368 | 7 | 173.125 | 376 |
| LOOK | 49 | 248 | 113 | 271 | 32 | 87 | 256 | 7 | 132.875 | 271 |
| C-LOOK | 49 | 294 | 113 | 271 | 32 | 87 | 286 | 7 | 142.375 | 294 |

Narration point: on a fixed queue SSTF looks fair. Its starvation only appears when new requests keep arriving (scene 11), which a static queue cannot show. That is exactly why the animation adds ghost pins.

**Verification.** The access times were computed by `access.py`. Every order, leg and total was computed by `disk.py` (§6.9 fairness table by `extras.py`), which covers FCFS, SSTF with tie detection, SCAN in both directions, C-SCAN, LOOK and C-LOOK, each with and without the return jump. The running totals and SSTF alternatives in §6.3–6.6 are partial sums of the script's printed legs. Both scripts are in `/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/` (python3; supplementary tables from `extras.py` in the same folder). The quiz values come from the same scripts. All queues and parameters are original.

## 7. Pause-and-predict checkpoints

1. **Scene 10 (SSTF from 63).** "Which pending request is nearest?"
   *Answer:* 70 (distance 7). The next nearest, 41, is 22 away.
2. **Scene 15 (SCAN up, reversing at 176).** "What is the new total?"
   *Answer:* 317 − 2·23 = **271**. That is LOOK.
3. *(Optional, scene 6)* "If the RPM doubles to 14400, what happens to the average rotational latency?"
   *Answer:* It halves to about 2.083 ms. Seek is unchanged, so it still dominates.

## 8. Misconceptions to address on screen

- **"Transfer time is the big cost."** → Scene 6 pie chart: 0.039 of 13.206 ms.
- **"SSTF is optimal."** → Scene 13: SCAN down gives 239, below SSTF's 271.
- **"SCAN reverses at the last request."** → Scene 12: the edge flashes. Reversing at the last request is LOOK.
- **"Total head movement = max − min."** → Scene 8: leg labels plus a running sum.
- **"Start counting from the first request."** → The first leg starts at the head: 63→112 = 49.
- **"C-SCAN's return is free" / "always counted".** → Scene 14 shows both counts and states the convention.

## 9. Visual assets

- **Reuse:** the device-queue graphic from V26 for the recap. The L34–L36 decks are not built yet; when they are, reuse their head-movement TikZ charts. Match the table style of `Slides/CS502/08-cpu-scheduling-basics.tex`, whose "The Price of Short-First: Starvation" frame echoes scene 11.
- **New:**
  - Platter/arm cut-away with track, sector and cylinder highlights.
  - Seek/rotation/transfer bar and pie chart (matplotlib).
  - Head-movement chart animation (Manim / Motion Canvas): arm dot, downward path, leg labels, running-sum counter, ghost layers for the variants, dashed arcs for return jumps.
- **Live demo (optional, 30 s):** `python3 disk.py | head -16`, or on Linux, `cat /sys/block/sda/queue/scheduler`.

### Instructor whiteboard version

1. Draw a long horizontal line labelled 0 at the left and 199 at the right. Mark the eight request cylinders as short ticks, with the head position 63 as a filled circle.
2. For each algorithm, draw the path *downward* from the head: one diagonal segment per move. Write the leg length on each segment and keep a running total in the right margin.
3. Draw FCFS, SSTF and SCAN in three colours on one chart, or on three stacked copies of the axis if the board is small.
4. For SCAN, draw an explicit dot at 199 before the reversal. For C-SCAN, draw the return as a dashed line and write "199 (counted?)" beside it.
5. Do the access-time arithmetic as a four-line column in a corner before starting the charts.
6. Finish with a hand-drawn bar row of the totals.

## 10. On-screen text & notation

- **Defined on screen at first use:** device, controller, request queue (2); platter, track, sector (3); cylinder, arm, head (4); seek time, rotational latency, transfer time, access time (5); RPM (6); total head movement (7); FCFS (8); SSTF (9); starvation (11); SCAN (12); C-SCAN (14); LOOK (16); C-LOOK (17).
- Algorithm names are exactly **`FCFS`, `SSTF`, `SCAN`, `C-SCAN`, `LOOK`, `C-LOOK`**. Always report "total head movement = N cylinders". State the head position and direction on every chart. Binary MB for transfer rates, stated once.

## 11. End-of-video quiz

Quiz disk: cylinders 0–199, head at **120**, queue in arrival order **47, 133, 10, 88, 190, 61, 145, 25**, moving towards 199 where relevant.

1. **(Numerical)** What is the average access time for 5400 RPM, average seek 12 ms, 50 MB/s, and an 8 KB block?
   *Solution:* One rotation = 60000/5400 = 11.111 ms, so the average latency is 5.556 ms. Transfer = 8192 / (50·2^20) s = 0.156 ms. Total = 12 + 5.556 + 0.156 = **17.712 ms**.
2. **(Numerical)** What is the FCFS total head movement?
   *Solution:* Legs 73, 86, 123, 78, 102, 129, 84, 120 → **795**.
3. **(Numerical)** What is the SSTF total?
   *Solution:* 120→133 (13), →145 (12), →190 (45), →88 (102), →61 (27), →47 (14), →25 (22), →10 (15) → **250**.
4. **(Numerical)** What are the SCAN (up) and LOOK (up) totals?
   *Solution:* SCAN: 13, 12, 45, then 190→199 (9), 199→88 (111), 27, 14, 22, 15 → **268**. LOOK skips the edge: 13, 12, 45, 190→88 (102), 27, 14, 22, 15 → **250**.
5. **(Numerical)** What is the C-SCAN (up) total, counting the 199→0 return?
   *Solution:* 13, 12, 45, 9, return 199, then 0→10 (10), 15, 22, 14, 27 → **366** (167 excluding the return).

## 12. References

- Silberschatz 8e, Ch. 12: overview of mass storage p.505; disk structure p.508; disk scheduling p.510; FCFS p.511; SSTF p.512; SCAN p.512; C-SCAN p.514; LOOK & C-LOOK p.515.
- Stallings 6e, Ch. 11: disk scheduling §11.5 p.507 (FCFS, SSTF, SCAN, C-SCAN); disk storage devices App. 11A p.540. *Stallings does not name LOOK or C-LOOK, so cite Silberschatz p.515 for those.*
- Bovet & Cesati 3e, Ch. 14 "Block Device Drivers" pp.560–598: the Linux I/O scheduler.

## 13. Production notes

- **Pacing:** this is the longest video in the series, so open with a three-part chapter card ("anatomy · cost · six orders"). Narrate every FCFS leg. For the later algorithms, narrate only the move that sets each one apart.
- **Cut if over length:** shorten scenes 3–4 to 60 s, then drop checkpoint 3, then merge scene 13 into scene 12 as a caption (keep the 239 figure).
- **Recording:** use a fixed axis scale and identical pin colours in every scene.
- **Accessibility:** distinguish paths by line style as well as Okabe–Ito colour, and always draw return jumps as dashed grey. Captions on. Read FCFS leg lengths aloud.

### Timing budget and cut priority

| # | Scene | Seconds | Cumulative | Priority |
|---|---|---|---|---|
| 1 | Hook | 40 | 0:40 | keep (core) |
| 2 | Recap from V26 (20 s) | 20 | 1:00 | keep (core) |
| 3 | Platters, tracks, sectors | 50 | 1:50 | shorten first |
| 4 | Cylinders, arm, heads | 40 | 2:30 | shorten first |
| 5 | Three costs of one read | 60 | 3:30 | keep (core) |
| 6 | Numeric example (whiteboard) | 70 | 4:40 | keep (core) |
| 7 | Setup card | 20 | 5:00 | keep (core) |
| 8 | FCFS | 80 | 6:20 | keep (core) |
| 9 | SSTF | 80 | 7:40 | keep (core) |
| 10 | **Pause-and-predict 1** | 30 | 8:10 | keep (core) |
| 11 | SSTF starvation | 50 | 9:00 | shorten first |
| 12 | SCAN | 80 | 10:20 | keep (core) |
| 13 | Direction matters | 40 | 11:00 | shorten first |
| 14 | C-SCAN | 70 | 12:10 | keep (core) |
| 15 | **Pause-and-predict 2** | 30 | 12:40 | shorten first |
| 16 | LOOK | 50 | 13:30 | keep (core) |
| 17 | C-LOOK | 50 | 14:20 | keep (core) |
| 18 | Scoreboard | 50 | 15:10 | shorten first |
| 19 | Recap + bridge to V28 | 50 | 16:00 | keep (core) |

Core (keep) scenes total 11:40; shortenable 4:20; cuttable 0:00. Halving every "shorten first" scene and dropping every "cut first" scene saves 2:10.
