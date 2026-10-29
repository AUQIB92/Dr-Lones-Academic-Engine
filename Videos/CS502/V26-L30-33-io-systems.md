# V26 · L30–33 — I/O Systems: How the OS Talks to Every Device

> **Lecture:** CS502 L30–33 `30-io-hardware` · `31-io-techniques` · `32-io-interface-kernel` · `33-io-buffering-performance` · **Unit:** IV · **Target length:** 17–19 min · **Series position:** 26/28
> **Format:** mixed (screen-capture animation of buses and data flow + short terminal clip) · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

I/O is the part of the OS where *timing* is the whole story. Polling is a CPU asking "ready yet?" in a tight loop; an interrupt is a device tapping the CPU on the shoulder; DMA is the CPU walking away while a helper moves the data. These three are indistinguishable on a static block diagram, but obvious when you watch the CPU's time bar fill with useful work or with waiting. Buffering is the same: a fast producer and a slow consumer only make sense as two rates moving side by side. And a single `read()` touches every layer in this video; following one request end to end ties the four lectures together.

## 2. Learning outcomes

After watching, the student can:
- [Remember] Name the parts of I/O hardware: device, port, controller, bus, device driver.
- [Apply] Compare polling, interrupt-driven I/O and DMA, and compute the CPU fraction each costs for a given device rate.
- [Understand] Classify devices as block, character or network, and explain blocking, non-blocking and asynchronous I/O.
- [Apply] Compute per-block time with no buffer, a single buffer and a double buffer; explain circular buffers, caching, spooling and error handling.
- [Analyze] Trace the life cycle of one `read()` request from user code to hardware and back.

## 3. Prerequisites

- Basic computer fundamentals: CPU, RAM, disk, keyboard; clock rate in GHz; bytes and KB.
- **V02** — interrupts, dual mode, the timer.
- **V03** — system calls (`read`, `write`) and the trap into the kernel.
- **V04** — layered OS structure; drivers as kernel modules.
- **V06** — blocked/waiting state and the ready queue.
- **V10–V16** — producer–consumer and the bounded buffer (for circular buffers).
- **V23, V25** — page faults and dirty-page write-back, which are I/O requests.

## 4. Hook (first 30–45 s)

> *Your CPU runs a billion instructions a second. A disk delivers 4 megabytes a second. If the CPU personally handled every 16 bytes the disk produced, how much of your computer's time would go just to babysitting the disk?*
>
> "The answer is about ten percent if it asks, twelve and a half if it's interrupted, and about a sixth of one percent if it delegates. Same disk, same data. Today we watch all three, then follow one `read()` all the way down to the hardware and back."

On screen: a CPU time bar split into "useful" (green) and "I/O babysitting" (orange); three bars labelled Polling / Interrupts / DMA with their orange portions hidden until scene 6.

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:40 | Hook | Three hidden CPU-time bars | Same disk, three ways to handle it, wildly different cost |
| 2 | 0:40–1:00 | Recap: interrupts & syscalls (V02, V03) | Device bolt → interrupt vector → handler; user `read()` → trap → kernel | 15–20 s: an interrupt is a device-initiated jump to a kernel handler; a system call is a program-initiated one |
| 3 | 1:00–2:45 | I/O hardware | Motherboard schematic: CPU, memory, PCIe bus, a disk controller with registers (status, control, data-in, data-out), a USB controller with keyboard and mouse | **Term cards:** *Device*, *Port* (connection point), *Bus* (shared wires + protocol), *Controller* (the chip that runs the device; has registers), *Device driver* (kernel code that knows one controller's registers). Port-mapped vs. memory-mapped I/O in one line |
| 4 | 2:45–4:15 | Polling, animated | CPU loop: read status register → "busy" → read again… → "ready" → move 16 bytes → repeat. CPU bar fills orange | **Term card — Polling (busy-waiting):** the CPU repeatedly reads a status bit. Fine for a device that is almost always ready, wasteful otherwise |
| 5 | 4:15–5:30 | Interrupts, then DMA | (a) CPU runs other work (green); device raises an interrupt per 16 bytes; context saved, handler runs, back. (b) CPU writes a DMA command (memory address, count, direction) and leaves; the DMA controller moves a whole 4 KB block over the bus; one interrupt at the end | **Term card — Interrupt-driven I/O:** device signals when ready. **Term card — DMA (direct memory access):** a controller copies blocks between device and RAM without the CPU, interrupting once per block. Mention cycle stealing on the bus |
| 6 | 5:30–7:15 | CPU-cost comparison, worked | Table builds: 1 GHz CPU; mouse 30 polls/s × 400 cycles = 0.0012 %; disk 250,000 transfers/s × 400 = 10 %; interrupts × 500 = 12.5 %; DMA 976.6 blocks/s × 1500 = 0.146 % | Polling is cheap for a slow, rarely-ready mouse; for a fast disk it's a tax. Interrupts *per small unit* can be worse than polling. DMA wins for bulk data. All from §6.1 |
| 7 | 7:15–7:45 | Pause-and-predict 1 | "Why are interrupts *more* expensive than polling for the disk here?" | Answer in §7 |
| 8 | 7:45–9:30 | Application I/O interface | Three device families as lanes: block (disk: `read`/`write`/`seek` blocks), character (keyboard: `get`/`put` bytes), network (socket: `send`/`recv`). Then three clocks for a `read`: blocking (process sleeps), non-blocking (returns "0 bytes yet" immediately), asynchronous (returns now; a signal/callback later) | **Term cards:** *Block device*, *Character device*, *Network device / socket*; *Blocking*, *Non-blocking*, *Asynchronous I/O*. The kernel hides each controller's details behind a uniform interface (the driver layer from V04) |
| 9 | 9:30–10:15 | Kernel I/O subsystem: scheduling | A queue of disk requests reorders to reduce arm travel (preview arrow to V27) | **Term card — I/O scheduling:** reorder pending requests for efficiency and fairness. Detailed algorithms in V27 |
| 10 | 10:15–10:35 | Recap: bounded buffer (V10–V16) | Producer and consumer around a ring of slots | 15 s: the producer–consumer ring you synchronised earlier is exactly the kernel's circular buffer |
| 11 | 10:35–13:00 | Buffering, worked | Timeline lanes for one block stream with T = 10 ms (device fills block), C = 6 ms (process uses block), M = 1 ms (copy buffer → user). No buffer: 16 ms/block. Single: 11. Double: 10. Then a speed-mismatch: a modem at 1 KB/s fills a 4 KB buffer in 4 s; the disk writes it in 20 ms; with two buffers the modem never stops. Circular: a ring of N buffers | **Term card — Buffer:** memory that holds data in transit. Reasons: speed mismatch, transfer-size mismatch, copy semantics. Double buffering overlaps device and process; the slower of T and C sets the pace |
| 12 | 13:00–14:15 | Caching, spooling, error handling | Cache: a second `read` of the same block served from RAM (no disk icon). Spooling: three processes "print" at once; each job lands as a file in a spool directory; one daemon feeds the printer in order. Error: a failed disk read retries, then returns an error code / `errno` | **Term cards:** *Cache* (fast copy of data that also lives elsewhere; buffer may be the only copy), *Spool* (a queue of whole jobs for a device that can't interleave), *Error handling* (retry, report, log) |
| 13 | 14:15–14:45 | Pause-and-predict 2 | "Two processes print at once with no spooling. What does the paper look like?" | Answer in §7 |
| 14 | 14:45–17:00 | The life of one `read()` | Vertical stack: user process → syscall trap → kernel: check cache → (miss) driver issues command, process **blocked** → controller + DMA move data → interrupt → handler marks I/O complete → data copied to user buffer → process **ready** → returns from `read()` | Every earlier piece appears once: syscall (V03), blocked state (V06), driver, DMA, interrupt, buffer, cache. Ask the viewer to name each layer as it lights |
| 15 | 17:00–18:00 | Recap + bridge | Summary: hardware → three techniques → interface → kernel subsystem → one `read()`. Zoom into the disk: platters and an arm | Bridge to V27: the slowest step was the disk itself. How is a disk organised, and in what order should the arm serve a queue of requests? |

### Scene-by-scene beat detail

**Scene 1 — Hook.**
- Three empty bars; the question types out; a "1 GHz" chip and a "4 MB/s" disk appear.
- Beat: "Same disk, same bytes, three costs." Do not reveal numbers yet.

**Scene 2 — Recap, 15–20 s.**
- Left: device bolt → vector table → handler → return (V02).
- Right: `read()` → trap → kernel → return (V03).
- Beat: "Hardware-initiated vs. program-initiated, both land in kernel mode."

**Scene 3 — I/O hardware.**
- Build the schematic piece by piece; each piece gets its term card as it lands.
- Zoom into the disk controller: four labelled registers.
- Beat: "The driver is the only code that knows what these registers mean. Everything above it speaks a common language."
- One line on memory-mapped I/O: controller registers appear at memory addresses, so ordinary loads/stores reach them.

**Scene 4 — Polling.**
- The CPU repeatedly reads the status register; a loop counter spins.
- A "ready" bit finally flips; 16 bytes move; loop restarts.
- The CPU bar fills orange the entire time.

**Scene 4b — When polling is the right call.**
- Quick cut back to the mouse row: 30 polls a second costs almost nothing.
- Beat: "Polling is not evil. It is wrong only when the device is fast *and* the CPU has better things to do, or when the device is rarely ready."

**Scene 5 — Interrupts, then DMA.**
- (a) CPU bar mostly green; each interrupt carves a thin orange slice (save state, handler, restore).
- Slices come 250,000 times a second: the bar turns striped.
- (b) CPU writes three fields into the DMA controller, then the CPU bar goes fully green; a data packet animates over the bus 4 KB at a time; one orange slice per block.

**Scene 6 — Cost table.** Rows appear one by one with the arithmetic shown, from §6.1.

**Scene 8 — Application I/O interface.**
- Three lanes, each with its own verbs.
- Then three timelines for one `read`: blocking (process icon sleeps until data), non-blocking (returns at once, app polls again later), async (returns at once, a callback bell rings later).
- Beat: "Non-blocking: 'give me what you have now.' Async: 'do it all and tell me when.'"

**Scene 9 — I/O scheduling.** A queue of five requests shuffles into a sweep order; arrow labelled "V27".

**Scene 9b — Device-independent naming.**
- `/dev/sda`, `/dev/tty1`, `eth0` labels hover over the three lanes.
- Beat: "Applications open a name; the kernel finds the driver. Swap the disk brand and the program does not change."

**Scene 10 — Recap bounded buffer.** Ring of slots with `in` and `out` pointers chasing each other.

**Scene 11 — Buffering.**
- Three timelines stacked; each shows T (device) and C (process) blocks; M (copy) in grey.
- No buffer: T and C strictly in sequence → 16 ms per block.
- Single buffer: device reads the next block while process works on a copied block → max(T, C) + M = 11.
- Double buffer: device fills one buffer while the process consumes the other → max(T, C) = 10.
- For 100 blocks: 1600, 1100, 1000 ms.
- Speed mismatch: modem trickles into buffer A for 4 s; when full, disk writes A in 20 ms while modem fills B.
- Circular: extend to a ring; this is the bounded buffer from scene 10.

**Scene 12 — Caching, spooling, errors.**
- Cache: second read shows a "hit" stamp and no disk animation.
- Spooling: three jobs become three files; daemon prints them whole, one after another.
- Error: a red X on a disk read, a retry arrow, then `-1` with `errno = EIO` returned to user space.

**Scene 7 — Pause-and-predict 1.**
- Freeze on rows 2 and 3 of the cost table, both highlighted; countdown 5 s.
- Reveal: per-event overhead comparison (400 vs. 500 cycles) with the note "disk almost always ready".

**Scene 13 — Pause-and-predict 2.**
- Two documents, "Report" and "Invoice", sent at once; printer icon with a blank page.
- Reveal: page shows alternating lines from each job, then the spooled version shows two clean documents.

**Scene 14 — Life of one `read()`.**
- 10 layers light in sequence; the process icon changes state colour (running → blocked → ready → running).
- Hold 1 s on the "blocked" moment to connect to V06.

**Scene 15 — Recap + bridge.** Five-word chain; the disk icon expands into platters and an arm.

## 6. Worked example (verified)

### 6.1 CPU cost of polling, interrupts and DMA

Assumptions (stated on screen): CPU clock 1 GHz = 10⁹ cycles/s. Polling one transfer costs 400 cycles. Handling one interrupt costs 500 cycles. DMA costs 1000 cycles to set up a block plus 500 for the completion interrupt = 1500 cycles per block. Disk rate 4 MB/s = 4 × 10⁶ B/s; non-DMA unit = 16 B; DMA block = 4096 B.

| Case | Events per second | Cycles per event | Cycles per second | CPU fraction |
|---|---|---|---|---|
| Mouse, polling (30 polls/s) | 30 | 400 | 12,000 | 0.0012 % |
| Disk, polling (16 B units) | 250,000 | 400 | 1.0 × 10⁸ | 10 % |
| Disk, interrupts (16 B units) | 250,000 | 500 | 1.25 × 10⁸ | 12.5 % |
| Disk, DMA (4 KB blocks) | 976.5625 | 1500 | 1,464,843.75 | 0.146 % |

### 6.2 Buffering timeline

T = 10 ms (device delivers one block), C = 6 ms (process consumes one block), M = 1 ms (copy from system buffer to user space).

| Scheme | Time per block | 100 blocks |
|---|---|---|
| No buffer | T + C = 16 ms | 1600 ms |
| Single buffer | max(C, T) + M = 11 ms | 1100 ms |
| Double buffer | max(C, T) = 10 ms | 1000 ms |

Speed mismatch: a modem at 1 KB/s (1024 B/s) fills a 4 KB (4096 B) buffer in 4 s; the disk writes that buffer in 20 ms, so with double buffering the modem never waits.

### 6.3 The life of one `read(fd, buf, 4096)` (no numbers; layer trace used in scene 14)

| Step | Layer | What happens | Process state |
|---|---|---|---|
| 1 | User code | Calls `read(fd, buf, 4096)` | running |
| 2 | Syscall interface (V03) | Trap into kernel mode; arguments checked | running (kernel mode) |
| 3 | File/VFS layer | Maps file offset to a disk block number (preview of V28) | running |
| 4 | Buffer cache | Is the block already in RAM? If yes, skip to step 10 | running |
| 5 | I/O scheduler | Request added to the device queue, possibly reordered (V27) | blocked |
| 6 | Device driver | Writes command, block number and memory address into controller registers | blocked |
| 7 | Controller + DMA | Disk seeks, reads; DMA copies the block into a kernel buffer | blocked (CPU runs others) |
| 8 | Interrupt | Controller raises an interrupt at completion | blocked |
| 9 | Interrupt handler / driver | Marks request done; wakes the process | ready |
| 10 | Kernel | Copies data from the kernel buffer into `buf` | running (kernel mode) |
| 11 | Syscall return | Mode switch back to user; returns byte count (or −1 + `errno`) | running |

**Verification.** All values were computed by `python3` in `/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/`: `v26.py` (cost table, buffering times, modem fill time) and `v26q.py` (interrupt counts per second and the quiz values). The buffering formulas follow Stallings' I/O-buffering model (Ch 11 §11.4).

## 7. Pause-and-predict checkpoints

1. **Scene 7: why do interrupts cost more than polling for this disk?** Answer: the disk is almost always ready, so a poll rarely wastes a loop, while every interrupt pays a fixed save/restore overhead (500 vs. 400 cycles per 16 B). Interrupts win when the device is *rarely* ready; DMA wins for bulk data.
2. **Scene 11, before the reveal: double-buffer time per block with T = 10, C = 6?** Answer: 10 ms, the slower of the two.
3. **Scene 13: two processes print at once with no spooling.** Answer: lines from both jobs interleave on the same pages. Spooling keeps each job whole.

## 8. Misconceptions to address on screen

- "Interrupts are always better than polling." → Cost table: 12.5 % vs. 10 % for the always-ready disk.
- "With DMA the CPU does nothing for I/O." → One orange slice per block remains (setup + completion interrupt); the bus is shared (cycle stealing).
- "Non-blocking and asynchronous mean the same thing." → Side-by-side timelines: non-blocking returns *partial or zero* data now; async returns now and delivers *everything* later.
- "A buffer and a cache are the same." → Cache shows "a copy also lives on disk"; the buffer in the modem example may be the *only* copy.
- "Spooling is just a bigger buffer." → Spool holds *whole jobs as files* and hands them out one at a time; a buffer smooths a byte stream.
- "A device driver is part of the hardware." → Schematic: the driver sits in the kernel box, the controller on the board.
- "The process keeps the CPU while the disk works." → The `read()` timeline: the process is blocked and another runs.

## 9. Visual assets

- **Reuse:** no `Slides/CS502` decks for L30–L33 yet. Reuse the V06 state-diagram colours for the blocked/ready changes and the V02 interrupt-vector drawing. Match the palette of `Slides/CS502/08-cpu-scheduling-basics.tex`.
- **New:**
  - Motherboard schematic with controller registers (Motion Canvas).
  - CPU time bars for polling / interrupts / DMA that fill live.
  - DMA packet animation over the bus.
  - Three-lane buffering timelines (no / single / double) with T, C, M blocks.
  - Spool directory and printer daemon.
  - Ten-layer `read()` stack with a travelling request token.
- **Live demo (optional, 45 s):** on Linux, `cat /proc/interrupts` (interrupt counts per device), `ls -l /dev/sda /dev/tty` (the `b` and `c` in the first column mark block vs. character devices), and `strace -e trace=read cat /etc/hostname` to show the `read()` call.

## 10. On-screen text & notation

- Device, port, bus, controller, device driver; memory-mapped I/O.
- Polling (busy-waiting), interrupt-driven I/O, DMA, cycle stealing.
- Block / character / network devices; blocking / non-blocking / asynchronous I/O.
- I/O scheduling, buffering (single, double, circular), caching, spooling, error handling.
- T (device time per block), C (compute time per block), M (copy time).
- CPU fraction = events/s × cycles/event ÷ clock rate.
- Units on screen: 1 GHz = 10⁹ cycles/s; 4 MB/s here means 4 × 10⁶ B/s (decimal), while 4 KB blocks are 4096 B (binary). Say this once, explicitly, in scene 6.
- Colour code used throughout:
  - Green = CPU doing useful program work.
  - Orange = CPU spent on I/O overhead (polls, interrupt handling, DMA setup).
  - Blue = data moving on the bus.
  - Grey = copy between kernel and user buffers (M).
- State names reuse V06 exactly: running, ready, blocked (waiting).
- Write `read()` with parentheses and in monospace every time.

## 11. End-of-video quiz

1. **(Numerical)** A 2 GHz CPU polls a network card delivering 1 MB/s in 8-byte units, 400 cycles per poll. What fraction of CPU time is spent polling? **Answer:** 125,000 polls/s × 400 = 5 × 10⁷ cycles/s → 2.5 %.
2. **(Numerical)** Same CPU and card, but DMA in 2048-byte blocks costing 2000 cycles each. CPU fraction? **Answer:** ≈ 0.0488 %.
3. **(Numerical)** T = 8 ms, C = 12 ms, M = 1 ms. Time per block with no buffer, single and double buffering? **Answer:** 20, 13, 12 ms.
4. **(MCQ)** A keyboard is best described as a (a) block device (b) character device (c) network device (d) spooled device. **Answer: (b).**
5. **(MCQ)** Which technique stores whole jobs for a device that cannot interleave them? (a) caching (b) double buffering (c) spooling (d) DMA. **Answer: (c).**

*Quiz numbers computed in `v26q.py` (same directory as §6).*

## 12. References

- Silberschatz, *Operating System Concepts* 8e, Ch 13: overview (p.555), I/O hardware (p.556), DMA (p.563), application I/O interface (p.565), kernel I/O subsystem (p.571), transforming I/O requests to hardware (p.578), I/O performance (p.582). DMA also in Ch 1 (p.44).
- Stallings, *Operating Systems* 6e, Ch 11: I/O devices (p.496), organisation of the I/O function (p.497), OS design issues (p.501), I/O buffering (p.504). Ch 1 §1.7 I/O communication techniques (p.33).
- Bovet & Cesati, *Understanding the Linux Kernel*, Ch 13 I/O architecture and device drivers (pp.519–559), Ch 14 block device drivers (pp.560–598).

## 13. Production notes

- This brief covers four lectures; the risk is rushing. Scenes 5, 6 and 14 must not be cut.
- If over length: compress scene 9 to a 20 s preview, merge scenes 12–13, and drop the live demo.
- Show the arithmetic in scene 6 row by row; let the viewer see units cancel.
- Use colour plus pattern (solid vs. striped) for CPU "useful" vs. "I/O" time.
- Captions on; spell out "DMA" as "direct memory access" the first three times it is spoken.
- Timing budget by lecture block (keep roughly balanced so no lecture is shortchanged):
  - L30 hardware: scenes 2–3, about 2 min.
  - L31 techniques: scenes 4–7, about 5 min.
  - L32 interface + kernel subsystem: scenes 8–9, about 2.5 min.
  - L33 buffering, caching, spooling, performance: scenes 10–13, about 4 min.
  - Synthesis (`read()` + bridge): scenes 14–15, about 3 min.
- Record the `read()` stack (scene 14) last, after all other scenes are cut, so its layer icons match exactly.
- Narrator tip: say "per second" every time a rate is read aloud; most viewer errors in scene 6 come from mixing rates and totals.
- Accessibility: every animated bus transfer also gets a text label ("4 KB → RAM") for viewers with motion sensitivity; provide a reduced-motion export.

