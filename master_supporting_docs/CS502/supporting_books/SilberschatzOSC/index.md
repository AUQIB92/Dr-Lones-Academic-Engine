# Index: Operating System Concepts, 8th Edition (SilberschatzOSC)

**Source:** `master_supporting_docs/CS502/supporting_books/SilberschatzOSC/book.pdf` (982 PDF pages; printed book ~920 pp + appendices)
**Indexed:** 2026-10-14
**Extraction method:** text-layer (PyMuPDF). No OCR needed — pages 3+ extract cleanly.
**Authors/edition:** A. Silberschatz, P. B. Galvin, G. Gagne, *Operating System Concepts*, 8th ed. (the file on disk — "8th_EditionA4" — is a reflowed A4 capture with the standard 8e chapter set).

## Pagination note (IMPORTANT)

**The PDF-page ↔ printed-page offset is non-constant.** For ordinary body pages the mapping is `printed ≈ PDF − 14` (verified at PDF 63→49, PDF 115→101, PDF 239→225, PDF 329→315, PDF 435→421, PDF 519→505, PDF 605→591, PDF 813→801). But **part-title pages and chapter openers are unnumbered**, so the offset shifts by a few pages across each part boundary. **Never apply a fixed offset.** All pages in this index are the book's own **printed** page numbers (from the printed Contents, PDF pp. 13-18). When a lecture cites a page, cite the printed page and give `(PDF N)` as the backup.

Text-layer quality: good, but with the usual OCR-era artifacts (`sh�ucture`, `Surnmary`, `Synchrozination`, `flle-corttml`). Chapter *titles* in the PDF outline are misspelled; the corrected titles below come from the printed Contents.

## Chapter Index

| Chapter | Printed pages | Summary | Key Terms/Definitions | Named Theorems/Algorithms/Figures |
|---------|---------------|---------|------------------------|-----------------------------------|
| 1 — Introduction | ~1–47 | What an OS is and does; computer-system organization and architecture; OS operations (dual-mode); a tour of the major subsystems | OS, kernel, bootstrap, interrupt, trap, DMA (p.44), multiprogramming (pp.19–21), time-sharing (p.20), job scheduling, dual-mode operation / mode bit (pp.21–22), timer, protection & security, computing environments | Fig 1.x computer-system organization; multiprogramming memory layout; dual-mode (user/kernel) diagram; §1.2.3 DMA |
| 2 — System Structures | ~48–99 | The OS as a service provider; user interfaces; system calls and their types; system programs; design/implementation; the five OS structures | OS services (p.49), CLI/GUI (p.52), **system call** (p.55), types of system calls (p.58: process control, file mgmt, device mgmt, info maintenance, communication, protection), system programs (p.66), **monolithic / layered / microkernel / modular** structures (pp.70–76), virtual machines (p.76), system boot (p.89) | Fig 2.x system-call parameter passing; Fig 2.x layered & microkernel structures; Fig — device-driver interface (p.~29) |
| 3 — Process Concept | ~100–152 | The process as the unit of work; states, PCB, scheduling, operations, and interprocess communication | process concept (p.101), **process control block / PCB** (p.103), process states (p.103), process scheduling / queues (p.105), **context switch** (p.110), process creation/termination & `fork`/`exec` (p.110), **IPC**: shared memory & message passing (p.116), pipes (p.123), sockets (p.128) | Fig 3.x process state diagram; Fig 3.x PCB; Fig 3.x ready/device queues; producer–consumer with bounded buffer (p.118) |
| 4 — Multithreaded Programming | 153–182 | Threads as lightweight processes; models and libraries | thread concept (p.153), multithreading models (p.157: many-to-one, one-to-one, many-to-many), thread libraries (p.159), threading issues (p.165) | Fig 4.x multithreading models |
| 5 — Process Scheduling | 183–224 | CPU scheduling concepts, criteria, the classic algorithms, and their evaluation | basic concepts (p.183), CPU–I/O burst cycle, preemptive vs non-preemptive, dispatcher, dispatch latency, scheduling criteria (p.187: CPU utilization, throughput, turnaround, waiting, response), **FCFS** (p.188), **SJF / SRTF** (p.189), **Priority** (p.192), **Round Robin** (p.194), **Multilevel Queue** (p.196), **Multilevel Feedback Queue** (p.197), thread scheduling (p.199), multiple-processor scheduling (p.200) | Gantt-chart worked examples (Figs 5.1–5.5); Fig 5.6 multilevel queue; deterministic modeling / queueing evaluation (p.213) |
| 6 — Synchronization | 224–282 | The critical-section problem, its software/hardware solutions, and high-level constructs | background / race condition (p.225), **critical-section problem** (p.227; requirements: mutual exclusion, progress, bounded waiting), **Peterson's solution** (p.229), synchronization hardware (p.231: test-and-set, compare-and-swap), **semaphores** (p.234: `wait`/`signal`, counting vs binary), **classic problems** (p.239: bounded-buffer, readers–writers, dining-philosophers), **monitors** (p.244), atomic transactions (p.257) | Peterson's two-process algorithm (Fig 6.2); bounded-buffer semaphore solution; dining-philosophers (Fig 6.13); monitor construct |
| 7 — Deadlocks | 283–313 | How deadlock arises and the four handling strategies | system model (p.283), **resource-allocation graph** (p.287), deadlock characterization / **four necessary conditions** (p.285: mutual exclusion, hold-and-wait, no preemption, circular wait), methods for handling (p.290), **prevention** (p.291), **avoidance / safe state / Banker's algorithm** (pp.294–300), **detection** (p.301), **recovery** (p.304) | Fig 7.x resource-allocation graph with a cycle; Banker's-algorithm safety worked example (pp.298–300) |
| 8 — Memory-Management Strategies | 314–356 | Address binding, contiguous allocation, and the two non-contiguous schemes | background / address binding / MMU (p.315), swapping (p.322), **contiguous allocation** (p.324), **first-fit / best-fit / worst-fit** (pp.326–327), **fragmentation** (external/internal, p.327), **compaction** (p.327), **paging** (p.328), **TLB** (p.333), structure of the page table (p.337: hierarchical, hashed, inverted), **segmentation** (p.342), Intel Pentium example (p.345) | Fig 8.x paging hardware & address translation; Fig 8.x TLB; Fig 8.x segmentation hardware; first/best/worst-fit worked allocation |
| 9 — Virtual-Memory Management | 357–419 | Demand paging, page replacement, frame allocation, and thrashing | background (p.357), **demand paging** (p.361), **page fault** handling (p.364), **copy-on-write** (p.367), **page replacement** (p.369), **FIFO** (p.371), **Optimal/OPT** (p.374), **LRU** (p.375), LRU-approximation / reference & second-chance bits (p.378), **Belady's anomaly** (p.374), **allocation of frames** (p.382), global vs local allocation, **thrashing** (p.386), **working-set model** (p.388), page-fault frequency, memory-mapped files (p.390), allocating kernel memory / slab (p.396) | Fig 9.x demand-paging steps; Fig 9.x FIFO vs OPT vs LRU on one reference string; Fig 9.18 thrashing; Fig 9.x working-set model |
| 10 — File System | 420–459 | The file abstraction, access methods, directories, sharing, and protection | file concept / attributes / operations (p.421), **access methods** (p.430: sequential, direct, indexed), **directory & disk structure** (p.433: single-level, two-level, tree, acyclic-graph, general-graph), **file-system mounting** (p.444), **file sharing** (p.446), protection / access control (p.451) | Fig 10.x file-system layout; tree / acyclic-graph directory diagrams |
| 11 — Implementing File Systems | 460–503 | On-disk structures, allocation methods, free-space management, and performance | file-system structure (p.461), **file-control block (FCB)** (pp.463–465), in-memory structures, **directory implementation** (p.470), **allocation methods** (p.471: contiguous, linked, indexed / **inode** p.443 & 464), **free-space management** (p.479: bitmap, linked list), efficiency & performance (p.482), recovery / consistency checking (p.486), NFS (p.490) | Fig 11.2 a typical file-control block; Fig 11.x inode & indexed allocation; FAT example |
| 12 — Secondary-Storage Structure | 504–553 | Disk hardware, scheduling, management, and RAID | overview of mass storage (p.505), **disk structure** (p.508), disk attachment (p.509: HDD, host-attached, network-attached, SAN), **disk scheduling** (p.510): **FCFS** (p.511), **SSTF** (p.512), **SCAN** (p.512), **C-SCAN** (p.514), **LOOK** & **C-LOOK** (p.515); disk management (p.516), swap-space management (p.520), **RAID** structure (p.522), stable-storage implementation (p.533) | Figs 12.2–12.8: FCFS / SSTF / SCAN / C-SCAN / LOOK / C-LOOK worked arm-queue examples (head movement totals); Fig 12.x RAID levels 0–6 |
| 13 — I/O Systems | 554–589 | I/O hardware, the application I/O interface, the kernel I/O subsystem, and performance | overview (p.555), **I/O hardware** (p.556: controllers, device drivers, polling, interrupts, **DMA** p.563), application I/O interface (p.565), **kernel I/O subsystem** (p.571: scheduling, **buffering**, caching, spooling, error handling), transforming I/O requests to hardware (p.578), STREAMS (p.580), I/O performance (p.582) | Fig 13.x interrupt-driven I/O cycle; Fig 13.x DMA; Fig 13.x device-driver / kernel-I/O stack; Fig 13.x STREAMS |
| 14 — System Protection | 590–619 | Goals and mechanisms of protection | goals of protection (p.591), principles (p.592), domain of protection (p.593), **access matrix** (p.598), implementation (p.602: global table, access lists, capability lists), access control (p.605), revocation (p.606), capability-based systems (p.607), language-based protection (p.610) | Fig 14.x access matrix; Fig 14.x capability lists |
| 15 — System Security | 620–671 | Threats and defenses | the security problem (p.621), program threats (p.625), system/network threats (p.633), cryptography as a tool (p.638), user authentication (p.649), security defenses (p.654), firewalls (p.661), Windows XP example (p.664) | Fig 15.x cryptography / RSA; security-classification tables |
| 19 — Real-Time Systems | ~757–777 | Hard/soft real-time characteristics and scheduling | overview (p.759), system characteristics (p.760), features of real-time kernels (p.762), implementing RTOS (p.764), **real-time CPU scheduling** (p.768: rate-monotonic, EDF), VxWorks (p.774) | rate-monotonic & earliest-deadline-first scheduling diagrams |
| 23 — Influential Operating Systems | ~903–921 | Historical systems | feature migration (p.903), **early systems / resident monitor** (pp.904–910), Atlas, XDS-940, THE, RC 4000, CTSS, MULTICS, OS/360, TOPS-20, CP/M-MSDOS, Mach | **Fig 23.2 memory layout for a resident monitor (p.907)**; Fig 23.3 card-deck batch setup (p.908) |

**Also present (outside the syllabus's four units):** Ch 16–18 Distributed Systems (~673–754), Ch 20 Multimedia Systems (~779–797), Ch 21 The Linux System (~801–845), Ch 22 Windows XP (~847–901), Appendices A (BSD UNIX) / B (Mach) / C (Windows 2000).

## Syllabus-coverage check (Step 0.5)

Cross-checked `syllabi/CS502.md`'s four units against actual book content. The book covers **all four units** — with three citation corrections and two genuine gaps.

### Citation corrections (syllabus anchor-readings were wrong; now fixed in `syllabi/CS502.md`)

| Week | Was (wrong) | Correct (verified) |
|------|-------------|--------------------|
| 13 (I/O management) | "Silberschatz Ch. 12 §12.1-12.5" | **Ch 13 §13.1–13.4** (I/O Systems, pp.555–578) |
| 14 (Disk scheduling) | "Silberschatz Ch. 11 §11.1-11.2" | **Ch 12 §12.1–12.4** (Secondary-Storage Structure; §12.4 Disk Scheduling, pp.510–515) |
| 15 (File systems) | "Silberschatz Ch. 13 §13.1-13.4, Ch. 14" | **Ch 10 §10.1–10.6** (File System, pp.421–456) + **Ch 11 §11.1–11.4** (Implementing File Systems) |

Weeks 1–12 citations were confirmed correct against the printed Contents.

### Gaps — syllabus topics NOT covered where expected (use general treatment)

| Syllabus topic (unit) | Status in this book | Guidance for `/create-lecture` |
|-----------------------|--------------------|--------------------------------|
| **resident monitor** (Unit III, memory) | **Not** in the memory chapter (Ch 8). Present only in **Ch 23 §23.2, p.907** (Fig 23.2 = resident-monitor memory layout, p.907). | Cite **Ch 23 §23.2 (p.907)** for the historical definition; there is no "resident monitor" treatment in the memory-management chapter. |
| **overlays** (Unit III, memory) | **Not** in Ch 8; "overlay" does not appear as a memory-management technique anywhere in Ch 8. | Phrase as **general/standard treatment** (or cover under Ch 23 early-systems context). Do not page-cite Ch 8 for overlays. |
| **"file control block (FCB)"** naming | Present, but the book uses **"file-control block"** (pp.463–465, Fig 11.2) and **"inode"** interchangeably — not the bare acronym "FCB" in the Ch 10 file-system chapter. | Cite **Ch 11 §11.2, pp.463–465** for the FCB concept; prefer "inode"/"file-control block" phrasing in student-facing text. |

### Confirmed-present named topics (page evidence)

- **Ch 1:** multiprogramming (pp.19–21), time-sharing (p.20), dual-mode/mode bit (pp.21–22), DMA (p.44).
- **Ch 2:** system call (p.55), types of system calls (p.58), system programs (p.66), monolithic/layered/microkernel/modular (pp.70–76).
- **Ch 3:** PCB (p.103), process states (p.103), context switch (p.110), IPC (p.116).
- **Ch 5:** FCFS (p.188), SJF/SRTF (p.189), Priority (p.192), Round Robin (p.194), Multilevel Queue (p.196), Multilevel Feedback Queue (p.197), multiple-processor scheduling (p.200).
- **Ch 6:** critical-section problem (p.227), Peterson's solution (p.229), synchronization hardware (p.231), semaphores (p.234), classic problems (p.239), monitors (p.244).
- **Ch 7:** resource-allocation graph (p.287), four necessary conditions (p.285), prevention (p.291), avoidance/Banker's (pp.294–300), detection (p.301), recovery (p.304).
- **Ch 8:** first-fit (p.326), best-fit (p.327), worst-fit (p.327), fragmentation (p.327), compaction (p.327), paging (p.328), TLB (p.333), segmentation (p.342).
- **Ch 9:** demand paging (p.361), page fault (p.364), page replacement (p.369), FIFO (p.371), **Optimal** (p.374), **LRU** (p.375), **Belady's anomaly (p.374)**, allocation of frames (p.382), thrashing (p.386), working set (p.388).
- **Ch 12:** SSTF (p.512), SCAN (p.512), C-SCAN (p.514), LOOK/C-LOOK (p.515).
- **Ch 13:** DMA (p.563), kernel I/O buffering (p.571).
- **Ch 10/11:** directory structure (p.433), file sharing (p.446), inode (p.443/464).

**Notable strengths vs. other CS courses' anchors:** unlike the CS401 Stallings text, this book **does** cover **Optimal page replacement and Belady's anomaly** (both p.374) and **LOOK/C-LOOK** disk scheduling (p.515), so those can be page-cited rather than phrased as general treatment.
