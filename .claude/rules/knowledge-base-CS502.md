---
paths:
  - "Slides/CS502/**/*.tex"
  - "Notes/CS502/**/*.tex"
  - "Assignments/CS502/**/*.tex"
  - "InstructorHandouts/CS502/**/*.tex"
  - "CompetitiveExam/CS502/**/*.tex"
---

# Course Knowledge Base: Operating Systems (PCC CS-502)

<!-- Bootstrapped 2026-10-14 while scaffolding the CS502 course. Expanded the same
     day from 15 weeks to the 39-lecture (Unit I 9, Units II-IV 10 each) structure, and the anchor
     textbook (Silberschatz, 8th ed.) was indexed. Lecture numbers below are
     L01-L39. Notation rows marked (projected) are filled in as each lecture is
     authored; rows confirmed by an authored lecture say so. -->

## Course Profile

| Field | Value |
|---|---|
| Audience level | `undergrad` (B.Tech CSE, semester 5) |
| Programme position | semester 5 of 8 |
| Default difficulty | **`core`** — derived from `undergrad` + semester 5 per `.claude/rules/difficulty-levels.md` |
| Exam target | GATE CS (via `/competitive-exam-questions`) |
| Course length | **39 lectures**, Unit I 9 + Units II-IV 10 each, 3 sessions/week |
| Implementation language | C for user/kernel-interface examples (`fork`, `wait`, `exec`, `pipe`); language-neutral pseudocode for algorithms |

## Anchor Textbooks

| Book (ShortName) | Index | Lectures Backed | Page-Verified? |
|-------------------|-------|-----------------|-----------------|
| Silberschatz, Galvin & Gagne, *Operating System Concepts*, **8th ed. on disk** (SilberschatzOSC) | `master_supporting_docs/CS502/supporting_books/SilberschatzOSC/index.md` — **built 2026-10-14**, all 23 chapters at chapter+section level with printed page numbers | **Primary anchor for all 39 lectures.** L01-04 (Ch 1-2), L05-07 (Ch 3), L08-09 (Ch 5), L10-17 (Ch 6), L18-19 (Ch 7), L20-25 (Ch 8), L26-29 (Ch 9), L30-33 (Ch 13), L34-36 (Ch 12), L37-39 (Ch 10-11); resident monitor = Ch 23 §23.2 | **Yes — page-cited** (printed page numbers from the book's own Contents; the PDF↔printed offset is *non-constant*, so always cite the printed page and give `(PDF N)` as backup) |
| Stallings, *Operating Systems: Internals and Design Principles*, **6th ed. on disk** (StallingsOS) | `master_supporting_docs/CS502/supporting_books/StallingsOS/index.md` — **built 2026-10-14**, 16 chapters + appendices at chapter+section level with printed page numbers | **Secondary anchor across all four units.** L01-04 (Ch 1-2), L05-07 (Ch 3), L08-09 (Ch 9-10), L10-17 (Ch 5 + App. A), L18-19 (Ch 6), L20-25 (Ch 7), L26-29 (Ch 8), L30-33 (Ch 11 §11.1-11.4), L34-36 (Ch 11 §11.5-11.6), L37-39 (Ch 12) | **Yes — page-cited** (printed pages from the book's own Contents; offset `printed ≈ PDF − 19`) |
| Bovet & Cesati, *Understanding the Linux Kernel*, **3rd ed.** (LinuxKernel) | `master_supporting_docs/CS502/supporting_books/LinuxKernel/index.md` — **built 2026-10-14**, 20 chapters + apps at chapter level with printed page numbers | **Auxiliary/advanced reference (NOT a syllabus text).** Concrete-Linux depth for L02-03 (Ch 1,10), L05-07 (Ch 3,20), L08-09 (Ch 7), L10-18 (Ch 4,5,19), L20-29 (Ch 2,8,9,15,17), L15/L30-36 (Ch 13,14), L37-39 (Ch 12,15,18) | **Yes — page-cited** (offset `printed ≈ PDF − 18`); implementation-focused, Linux 2.6 |
| Bach, *The Design of the UNIX Operating System* (Bach1986) | *not indexed* | Reference for the concrete UNIX view (processes, IPC, file system) | **No — chapter-level only** |

<!-- [LEARN:textbook-grounding] Silberschatz (the primary anchor) is now indexed
     and page-cited. Two Unit-III topics are NOT covered in its memory chapter and
     must stay general/standard treatment: "overlays" (absent from Ch 8), and
     "resident monitor" (only in Ch 23 §23.2, p.907). See the index's
     syllabus-coverage section. -->

## Notation Registry

| Rule | Convention | Example | Anti-Pattern |
|------|-----------|---------|-------------|
| Processes | `P1, P2, …` or a descriptive name in `\texttt{}`; never lowercase | `\texttt{P1}`, `\texttt{producer}` | `p1`, `Proc A` (inconsistent) |
| Semaphore ops | **`wait(S)` / `signal(S)`** (Silberschatz), with `P(S)`/`V(S)` named once as the historical alias | `wait(mutex); … signal(mutex)` | Mixing `P/V` and `wait/signal` on the same slide pair |
| Critical-section structure | lowercase section names: *entry section · critical section · exit section · remainder section* | "the process executes its exit section" | Capitalized "Entry Section" mid-sentence |
| Scheduling metrics | Spell out at first use, then abbreviate: **TAT** (turnaround), **WT** (waiting), **RT** (response), `τ` (burst/CPU time) | "average WT = 3.25 ms" | `T_A`, `W`, `W.T.` — invented abbreviations |
| Scheduling algorithms | Fixed acronyms, no punctuation: `FCFS`, `SJF`, `SRTF`, `RR`, `Priority`, `MLFQ` | "SRTF is preemptive SJF" | `S.J.F.`, `Round-Robin` (inconsistent hyphenation) |
| Gantt charts | Time horizontal, left→right; a process bar labeled with the process; **arrival marked on the axis**, not inside the bar | `| P1 | P2 | P1 |` over `0 2 4 7` | Bars with no time axis; idle gaps unmarked |
| Address translation | page number `p` + offset `d`; frame number `f`; physical address = `f · 2^d + d` | `p = 12, d = 0x2A` | Reusing `p` for both page and frame |
| Memory units | Binary units: 1 KB = 1024 B; state the page/frame size at first use | "4 KB pages ⇒ 12-bit offset" | Mixing 1000/1024 silently |
| Effective access time | **`EAT`**; state hit ratio `α` and access times explicitly | `EAT = α·t_TLB + (1−α)(t_TLB + t_mem)` | Bare "average time" with no formula |
| Deadlock matrices | `Available`, `Max`, `Allocation`, `Need` (capitalized as named objects) | `Need = Max − Allocation` | Lowercase `max`, `alloc` |
| Disk scheduling | Acronyms as named: `FCFS`, `SSTF`, `SCAN`, `C-SCAN`, `LOOK`, `C-LOOK`; report total head movement | "SSTF total = 236 cylinders" | `CScan`, `c-scan` |
| File system | `FCB` / `inode` (Silberschatz uses "file-control block" and "inode"); file names in `\texttt{}` | `\texttt{/usr/src/test.c}` | "File descriptor" used for the FCB |

## Symbol Reference

| Symbol | Meaning | Introduced |
|--------|---------|------------|
| OS | Operating system — the program that manages hardware and provides the environment for programs | **L01** |
| kernel / user mode | The privileged vs. unprivileged execution modes; a mode bit selects which instructions are legal | **L01 / L02** |
| trap / interrupt | A software-initiated (system call) vs. hardware-initiated transfer of control to the kernel | **L01 (overview) / L03** |
| interrupt vector table | Array of handler addresses indexed by interrupt number; how the CPU dispatches without searching | **CS401 prerequisite** |
| memory hierarchy / cache locality | Registers → cache → RAM → SSD → disk → tape; temporal and spatial locality make caching pay | **CS401 prerequisite** |
| system call | The programmatic request from a user program to a kernel service; the boundary of the OS API | **L03** |
| mode bit | Hardware bit distinguishing user (`1`) from kernel (`0`) mode | **L02** |
| PCB | Process Control Block — the kernel's record of a process: state, `PC`, registers, memory limits, open files | **L05** |
| Process states | `new · ready · running · waiting/blocked · terminated` | **L05 / L06** |
| Context switch | Saving the running process's context (PCB) and restoring another's | **L06** |
| Scheduler types | long-term (job → ready), short-term (ready → CPU), medium-term (swapping) | **L06** |
| $n$ (multiprogramming degree) | Number of processes resident in memory | **L01 (evolution) / CS401** |
| TAT / WT / RT | Turnaround / waiting / response time | **L08** |
| $\tau$ | CPU burst time of a process | **L08** |
| FCFS / SJF / SRTF | First-Come-First-Served; Shortest-Job-First (and its preemptive Shortest-Remaining-Time-First form) | **L08** |
| RR / Priority / MLQ / MLFQ | Round Robin; Priority; Multilevel Queue; Multilevel Feedback Queue | **L09** |
| race condition | Two processes' interleaved accesses to shared data producing a data-dependent outcome | **L10** |
| critical section | Code that accesses shared data and must not interleave | **L10** |
| mutual exclusion (mutex) | The requirement that at most one process be in its critical section | **L10** |
| progress / bounded waiting | The other two critical-section requirements (with mutual exclusion) | **L11** |
| Peterson's solution | Two-process mutual-exclusion software solution using `flag[]` and `turn` | **L11** |
| test-and-set / compare-and-swap | Atomic hardware primitives for mutual exclusion | **L12** |
| wait(S) / signal(S) | Atomic semaphore operations (historical `P`/`V`) | **L13** |
| counting vs. binary semaphore | A semaphore counting $[0,N]$ vs. constrained to $\{0,1\}$ | **L13** |
| monitor | A high-level synchronization construct encapsulating shared data with implicit mutual exclusion | **L17** |
| IPC | Inter-process communication — shared memory vs. message passing (`send`/`receive`), pipes, sockets | **L14 / L15** |
| bounded-buffer / readers-writers / dining-philosophers | The three classical concurrency problems | **L16 / L17** |
| RAG | Resource-Allocation Graph — process (circle) and resource (rectangle) vertices, request/assignment edges | **L18** |
| four necessary conditions | mutual exclusion, hold-and-wait, no preemption, circular wait | **L18** |
| Banker's algorithm | Deadlock *avoidance* via `Available`/`Max`/`Allocation`/`Need` safe-sequence test | **L19** |
| safe state / safe sequence | A state from which some ordering lets every process finish | **L19** |
| resident monitor | The earliest single-job memory layout: loader + program (Silberschatz Ch. 23 §23.2) | **L20** |
| contiguous allocation | Fixed/variable partitioning; first-fit / best-fit / worst-fit | **L21 / L22** |
| external / internal fragmentation | Wasted free space *between* vs. *within* partitions | **L22** |
| compaction | Relocating processes to merge holes (needs relocation/relocation registers) | **L22** |
| paging | Non-contiguous allocation: fixed-size pages → frames; page table maps `p → f` | **L23** |
| TLB | Translation Lookaside Buffer — associative cache of page-table entries | **L24** |
| EAT | Effective Access Time — `α`-weighted TLB/memory access cost | **L24** |
| segmentation | Logical variable-size segments with base + limit registers; segmentation with paging | **L25** |
| demand paging | Load a page only when first referenced; a reference to an absent page traps to a page-fault handler | **L26** |
| page fault / valid-invalid bit | The trap and the page-table bit that marks a page resident or not | **L26** |
| copy-on-write | Sharing a page read-only until one process writes (then copy) | **L26** |
| reference string | The sequence of page numbers a process references, used to evaluate replacement | **L27** |
| FIFO / Optimal (OPT/MIN) / LRU | The page-replacement algorithms; OPT is the unachievable lower bound | **L27** |
| Belady's anomaly | FIFO's page-fault count rising as frames increase | **L27** |
| Clock / second-chance / enhanced second-chance / counting | LRU-approximation replacement algorithms | **L28** |
| working set | The pages a process references in a recent window $\Delta$ | **L29** |
| thrashing | A process spending more time paging than executing | **L29** |
| device driver / controller | Kernel module vs. hardware controller that drives a device | **L01 (background) / L30 (driver interface)** |
| polling / interrupt-driven I/O / DMA | The three I/O techniques (busy-wait · trap on completion · block transfer without CPU) | **CS401 prerequisite / L31 (in depth)** |
| buffering / caching / spooling | Kernel I/O-subystem techniques that overlap and smooth I/O | **L33** |
| seek / rotational latency / transfer time | The three disk-access cost components | **L34** |
| SSTF / SCAN / C-SCAN | Disk-scheduling algorithms beyond FCFS | **L35 / L36** |
| LOOK / C-LOOK | Elevator variants that stop at the last request instead of the disk edge | **L36** |
| FCB / inode | File Control Block / UNIX index node — per-file metadata | **L39** |
| sequential / direct (relative) / indexed access | File access methods | **L37** |
| directory / mounting / file sharing / protection | File-system structure and sharing mechanisms | **L38 / L39** |

## Lecture Progression

| # | Title | Core Question | Key Notation | Key Method |
|---|-------|--------------|-------------|------------|
| 1 | Introduction to OS | What is an OS, and what does it do for a program? | OS, kernel/user mode, trap, system call | Trace a system call across the user/kernel boundary |
| 2 | OS Operations & Protection | How does the OS stay in control? | mode bit, dual-mode, timer | Dual-mode enforcement walkthrough |
| 3 | System Calls | How does a program enter the kernel? | system call, parameter passing | API → system-call mapping; block/register/stack |
| 4 | System Programs & OS Structures | How is the OS organized internally? | monolithic, layered, microkernel, modular, VM | Structure trade-off comparison |
| 5 | Process Concept & PCB | What is a running program? | PCB, process states | Draw + interpret the process state diagram |
| 6 | Process States, Schedulers, Context Switch | How does the OS track and switch processes? | scheduler types, context switch, queues | Context-switch trace |
| 7 | Operations on Processes | How are processes created and ended? | `fork`, `exec`, `wait`, `exit` | Parent/child `fork` trace |
| 8 | CPU Scheduling: Concepts, FCFS, SJF | Which process next, and how do we compare policies? | TAT, WT, RT, FCFS, SJF/SRTF | Gantt chart → average WT/TAT |
| 9 | Priority, RR, Multilevel & Multiprocessor | How do real schedulers combine policies? | RR, Priority, MLQ, MLFQ | Compare queue-ladder behavior |
| 10 | Concurrency & the Critical-Section Problem | What breaks when processes interleave? | race condition, critical section, mutual exclusion | Interleaving demonstration |
| 11 | Mutex Requirements & Peterson's Solution | How do we exclude with software only? | progress, bounded waiting, Peterson's solution | Prove mutual exclusion + progress + bounded waiting |
| 12 | Synchronization Hardware | How does hardware make exclusion atomic? | test-and-set, CAS, atomic variable | Spin-lock build |
| 13 | Semaphores | What is the standard synchronization primitive? | wait/signal, counting vs binary | Mutual-exclusion + ordering with semaphores |
| 14 | Semaphore Patterns & IPC | How do processes share and communicate? | IPC, shared memory, message passing | Producer–consumer with semaphores |
| 15 | IPC in Practice | What IPC mechanisms do real systems offer? | pipes, message queues, sockets | Compare IPC mechanisms for a workload |
| 16 | Classical Problem I: Producer–Consumer | How does bounded-buffer synchronize? | bounded buffer, in/out, empty/full semaphores | Full semaphore solution + trace |
| 17 | Classical Problems II & Monitors | How do readers–writers and philosophers fall to tools? | readers–writers, dining philosophers, monitor | Map each problem to semaphores/monitors |
| 18 | Deadlock: Model & Characterization | How does a set of processes deadlock? | RAG, four necessary conditions | RAG cycle check |
| 19 | Deadlock Handling | How do we prevent, avoid, detect, recover? | Banker's, safe sequence | Safety algorithm walkthrough |
| 20 | Memory-Management Background | How did early systems place processes in memory? | resident monitor, address binding, MMU | Binding-time walkthrough |
| 21 | Contiguous Allocation | Fixed vs. variable partitioning? | MFT, MVT, partitions | Partition-allocation walkthrough |
| 22 | Allocation Strategies & Fragmentation | Which hole do we pick? | first/best/worst-fit, fragmentation, compaction | Fit-strategy walkthrough; fragmentation count |
| 23 | Paging | How do we allocate memory non-contiguously? | page, frame, page table | Address translation |
| 24 | TLB & Page-Table Structures | How do we make translation fast? | TLB, EAT, hierarchical/hashed/inverted tables | EAT computation |
| 25 | Segmentation | What if memory is logically divided? | segment, base, limit, segmentation-with-paging | Address translation (segment + offset) |
| 26 | Virtual Memory & Demand Paging | How do we run programs larger than memory? | demand paging, page fault, COW | Page-fault handling trace; EAT with faults |
| 27 | Page Replacement: FIFO, OPT, LRU | Which page to evict? | reference string, FIFO/OPT/LRU, Belady's anomaly | Reference-string fault count; Belady check |
| 28 | Page Replacement: Clock & Enhanced | How do we approximate LRU cheaply? | reference bit, second-chance, clock | Clock-algorithm trace |
| 29 | Frames, Working Set, Thrashing | When does paging collapse? | working set, thrashing, PFF | Working-set vs. local/global allocation |
| 30 | I/O Hardware | How is the OS wired to devices? | controller, driver, bus, port | Device stack walkthrough |
| 31 | I/O Techniques | Which I/O technique, and at what CPU cost? | polling, interrupt, DMA | Polling vs interrupt vs DMA head-to-head |
| 32 | I/O Interface & Kernel Subsystem | What does the kernel do for I/O? | blocking/non-blocking/async, device-status table | Request-to-hardware trace |
| 33 | I/O Buffering & Performance | How do we hide I/O latency? | single/double buffering, caching, spooling | Buffering timing example |
| 34 | Disk Structure & Performance | What does a disk access cost? | seek, rotational latency, transfer, scheduling | Access-time computation |
| 35 | Disk Scheduling: FCFS, SSTF, SCAN | In what order should the arm serve requests? | FCFS, SSTF, SCAN | Arm-queue ordering; total head movement |
| 36 | Disk Scheduling: C-SCAN, LOOK, C-LOOK; RAID | Which scheduling/RAID trade-off? | C-SCAN, LOOK, C-LOOK; RAID 0-6 | Compare total head movement; RAID-level comparison |
| 37 | File Concept & Access Methods | What is a file, and how is it accessed? | file attributes/operations; sequential/direct/indexed | Access-method comparison |
| 38 | File Directories & Mounting | How do we organize and locate files? | directory structures, mounting, path resolution | Directory traversal |
| 39 | File Sharing, Protection & Implementation | How is the file system stored and shared? | FCB/inode, allocation methods, free space | Inode/allocation design; access control |

## Course Outcomes & Assessment Blueprint

**Source:** `syllabi/CS502.md`'s Learning Objectives (numbered here for the first time). Bloom levels are a first-pass draft from each objective's stated verb. Regenerate via `/accreditation` rather than hand-editing if the syllabus objectives change.

| CO | Statement | Bloom level | Lectures | Assessed in |
|----|-----------|------------|----------|-------------|
| CO1 | Describe OS functions, evolution, structure, services, and the system-call/system-program interface | Understand | L01-04 | Class Test 1, Assignment 1, End-sem |
| CO2 | Represent a program as a process; interpret the state diagram, PCB, schedulers, context switch, and process operations | Understand/Apply | L05-07 | Class Test 1, Assignment 2, End-sem |
| CO3 | Apply and compare CPU scheduling algorithms against criteria; describe multiprocessor scheduling | Apply/Analyze | L08-09 | Class Test 1, End-sem |
| CO4 | Explain concurrency, solve the critical-section problem, and apply software/hardware solutions, semaphores, and monitors | Apply | L10-14 | Class Test 2, Assignment 3, End-sem |
| CO5 | Explain inter-process communication and apply synchronization tools to the classical problems | Apply | L14-17 | Class Test 2, End-sem |
| CO6 | Characterize deadlock and apply prevention, avoidance (Banker's), detection, and recovery | Analyze | L18-19 | Class Test 2, End-sem |
| CO7 | Compare contiguous memory-allocation techniques and allocation strategies; quantify fragmentation | Analyze | L20-22 | Class Test 3, Assignment 4, End-sem |
| CO8 | Explain paging/segmentation address translation and TLB performance; demand paging | Apply | L23-26 | Class Test 3, End-sem |
| CO9 | Compare page-replacement algorithms; explain frame allocation and thrashing | Analyze | L27-29 | Class Test 3, End-sem |
| CO10 | Describe I/O organization, techniques, and buffering; apply disk scheduling; explain file-system organization, access, directories, sharing, and implementation | Apply/Understand | L30-39 | Assignment 5, End-sem |

## Design Principles

| Principle | Evidence | Lectures Applied |
|-----------|----------|-----------------|
| Motivate every mechanism with the concrete failure or cost it prevents (race → lost update, no-TLB → double memory access, no-scheduling → convoy) before naming it | Prevents "list of mechanisms to memorize" fatigue | L01-L39 |
| Thread one worked example across each unit (e.g. a 3-process workload through FCFS→SJF→RR→MLFQ; a reference string through FIFO→OPT→LRU→Clock) so trade-offs are quantitative, not adjectives | Cost/speed and correctness trade-offs are the recurring theme of OS content | Units I-IV |

## Anti-Patterns (Don't Do This)

| Anti-Pattern | What Happened | Correction |
|-------------|---------------|-----------|
| | | |

## R Code Pitfalls

| Bug | Impact | Fix |
|-----|--------|-----|
| | | |

<!-- CS502 has no R/statistical component; the R Code Pitfalls table is empty by
     design, not an oversight. -->
