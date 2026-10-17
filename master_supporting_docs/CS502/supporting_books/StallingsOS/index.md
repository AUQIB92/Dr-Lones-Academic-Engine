# Index: Operating Systems: Internals and Design Principles, 6th Edition (StallingsOS)

**Source:** `master_supporting_docs/CS502/supporting_books/StallingsOS/book.pdf` (841 PDF pages)
**Indexed:** 2026-10-14
**Extraction method:** text-layer (PyMuPDF). No OCR needed.
**Author/edition:** William Stallings, *Operating Systems: Internals and Design Principles*, **6th ed.** (Pearson). PDF metadata title confirms "Sixth edition".

## Pagination note

Body-page offset is **`printed ≈ PDF − 19`** (verified: §5.1 printed 207 = PDF 226; Ch 1 printed 7 = PDF ~26). As with the other CS502 books, part-title pages are unnumbered, so **cite the printed page** (from the printed Contents, PDF pp. 6–12) and give `(PDF N)` as backup. Never apply a fixed offset blindly.

## Chapter Index

| Chapter | Printed pages | Summary | Key Terms/Definitions | Named Algorithms/Figures |
|---------|---------------|---------|------------------------|---------------------------|
| 0 — Reader's Guide | 1–5 | Roadmap for readers and instructors | — | — |
| 1 — Computer System Overview | 7–49 | Hardware foundations the OS builds on | basic elements, processor registers, instruction execution, **interrupts** (§1.4, p.15), memory hierarchy (§1.5, p.26), cache (§1.6, p.29), **I/O communication techniques incl. DMA** (§1.7, p.33), two-level memory (App. 1A, p.39), procedure control (App. 1B, p.46) | Fig 1.x interrupt cycle; Fig 1.x cache/memory hierarchy |
| 2 — Operating System Overview | 50–106 | OS objectives, evolution, achievements, and major modern systems | OS objectives & functions (§2.1, p.51), **evolution of OSs** (§2.2, p.55: serial, batch, **resident monitor** p.56, multiprogramming, **time-sharing** p.62), major achievements (processes, memory mgmt, information protection, scheduling, system structure) (§2.3, p.64), modern OS developments (§2.4, p.77), Windows/UNIX/Linux overviews (§2.5–2.8) | — |
| 3 — Process Description and Control | 107–159 | The process as the OS's unit of work | what is a process (§3.1, p.108), **process states** — two-state, five-state, seven-state models (§3.2, p.111), **process description / process control block** (§3.3, p.126), process control & **context switch** (§3.4, p.135), execution of the OS (§3.5, p.140), security issues (§3.6, p.143), UNIX SVR4 process mgmt (§3.7, p.147) | Fig 3.x five-state process model; Fig 3.x process image |
| 4 — Threads, SMP, and Microkernels | 160–205 | Threads, symmetric multiprocessing, microkernels | processes and threads (§4.1, p.161), **SMP** (§4.2, p.175), **microkernels** (§4.3, p.179), Windows/Solaris/Linux thread & SMP management (§4.4–4.6) | Fig 4.x thread states; microkernel architecture |
| 5 — Concurrency: Mutual Exclusion and Synchronization | 206–261 | Software & hardware mutual exclusion, semaphores, monitors, message passing | **principles of concurrency / critical section** (§5.1, p.207), **mutual exclusion: hardware support** (§5.2, p.216), **semaphores** (§5.3, p.219), **monitors** (§5.4, p.232), **message passing** (§5.5, p.239), **readers/writers problem** (§5.6, p.245) | Fig 5.x semaphore signalling; Fig 5.x monitor; producer/consumer buffering |
| 6 — Concurrency: Deadlock and Starvation | 262–310 | Deadlock principles and the four handling approaches | **principles of deadlock / four conditions** (§6.1, p.263), **prevention** (§6.2, p.272), **avoidance / safe state / Banker's** (§6.3, p.273), **detection** (§6.4, p.279), integrated strategy (§6.5, p.281), **dining philosophers** (§6.6, p.282), UNIX/Linux/Solaris/Windows concurrency mechanisms (§6.7–6.10) | Fig 6.x resource allocation graph (note: book uses "resource allocation graph" wording, not the hyphenated "resource-allocation graph"); Fig 6.x Banker's safe/unsafe states |
| 7 — Memory Management | 311–344 | Requirements, partitioning, paging, segmentation | memory management requirements (§7.1, p.312), **memory partitioning** (§7.2, p.315: fixed/variable, **first-fit / best-fit / worst-fit**, **fragmentation**, **compaction**), **paging** (§7.3, p.326), **segmentation** (§7.4, p.330), security issues (§7.5, p.331), loading & linking (App. 7A, p.338) | Fig 7.x fixed/variable partitioning; Fig 7.x address translation with paging |
| 8 — Virtual Memory | 345–404 | Virtual memory hardware + OS software | **hardware and control structures** (§8.1, p.346: **TLB** p.356, **page replacement** policies p.368, resident-set management), **OS software** (§8.2, p.365: **demand paging** p.366, **thrashing** p.382, working set), UNIX/Solaris/Linux/Windows (§8.3–8.5), hash tables (App. 8A, p.400) | Fig 8.x TLB operation; Fig 8.x page-replacement (FIFO/LRU/Clock) |
| 9 — Uniprocessor Scheduling | 405–451 | Scheduling types and the classic algorithms | types of scheduling (§9.1, p.406), **scheduling algorithms** (§9.2, p.410: **FCFS**, **shortest-job-first**, **shortest remaining time**, **round-robin**, priority, multilevel feedback), traditional UNIX scheduling (§9.3, p.432), response time (App. 9A, p.438), queuing systems (App. 9B, p.440) | Fig 9.x Gantt charts (FCFS/SJF/RR); Fig 9.x multilevel feedback queues |
| 10 — Multiprocessor and Real-Time Scheduling | 452–494 | Multi-processor and real-time scheduling | **multiprocessor scheduling** (§10.1, p.453), **real-time scheduling** (§10.2, p.466), Linux/FreeBSD/Windows scheduling (§10.3–10.5) | Fig 10.x thread scheduling/priority |
| 11 — I/O Management and Disk Scheduling | 495–550 | I/O organization, buffering, disk scheduling, RAID | **I/O devices** (§11.1, p.496), **organization of the I/O function** (§11.2, p.497), **OS design issues** (§11.3, p.501), **I/O buffering** (§11.4, p.504), **disk scheduling** (§11.5, p.507: FCFS, SSTF, SCAN, C-SCAN), **RAID** (§11.6, p.514), disk cache (§11.7, p.523), UNIX/Linux/Windows I/O (§11.8–11.10), disk storage devices (App. 11A, p.540) | Fig 11.x disk-scheduling arm movements; Fig 11.x RAID levels |
| 12 — File Management | 551–601 | File organization, directories, sharing, secondary storage | **overview** (§12.1, p.552), **file organization and access** (§12.2, p.558), **file directories** (§12.3, p.562), **file sharing** (§12.4, p.567), record blocking (§12.5, p.568), secondary-storage management (§12.6, p.570), file-system security (§12.7, p.578), UNIX/Linux/Windows file systems (§12.8–12.10: **VFS/inode** p.587) | Fig 12.x file-allocation methods; Fig 12.x directory tree |
| 13 — Embedded Operating Systems | 602–633 | Embedded OS characteristics, eCOS, TinyOS | embedded systems (§13.1, p.603), characteristics of embedded OSs (§13.2, p.605), eCOS (§13.3, p.607), TinyOS (§13.4, p.622) | — |
| 14 — Computer Security Threats | 635–666 | Security concepts and threats | security concepts (§14.1, p.636), threats/attacks/assets (§14.2, p.638), intruders (§14.3, p.643), malicious software (§14.4–14.6) | — |
| 15 — Computer Security Techniques | 667–706 | Defenses | authentication (§15.1, p.668), access control (§15.2, p.675), intrusion detection (§15.3, p.680), malware defense (§15.4, p.686), buffer overflow (§15.5, p.692) | — |
| 16 — Distributed Processing, Client/Server, and Clusters | 710–743 | Distributed systems | client/server (§16.1, p.711), distributed message passing (§16.2, p.722), RPC (§16.3, p.724), clusters (§16.4, p.728) | — |
| 17 — Networking *(online)* | — | Protocol architecture, sockets | TCP/IP architecture, sockets, Linux networking | — |
| 18 — Distributed Process Management *(online)* | — | Process migration, distributed mutual exclusion/deadlock | — | — |
| Appendix A — Topics in Concurrency | 744–764 | **Software mutual exclusion approaches** | **Mutual exclusion: software approaches** (§A.1, **p.745** — Dekker's/Peterson's-style), race conditions & semaphores (§A.2, p.751), barbershop problem (§A.3, p.758) | Peterson's algorithm (App. A.1) |
| Appendix B — Object-Oriented Design | 765–776 | OO concepts, CORBA | — | — |
| Appendix C — Programming and OS Projects | 777–783 | Projects, BACI/Nachos | — | — |

## Syllabus-coverage check (Step 0.5)

`StallingsOS` is prescribed textbook #3 for CS502 (`syllabi/CS502.md`, Books). It is a **secondary anchor** (Silberschatz is primary). Verified against the 39-lecture syllabus:

### Confirmed coverage

| CS502 lectures | Stallings coverage |
|----------------|--------------------|
| L01–L04 (Intro, OS operations, structures) | Ch 2 (OS Overview); Ch 1 (background: computer-system overview) |
| L05–L07 (Process concept, states, PCB, context switch) | Ch 3 §3.1–3.4 (process description, states, PCB, process control/context switch) |
| L08–L09 (Scheduling) | Ch 9 (uniprocessor scheduling: FCFS, SJF, SRT, RR, priority, multilevel feedback), Ch 10 (multiprocessor, real-time) |
| L10–L17 (Concurrency, semaphores, monitors, classical problems) | Ch 5 (mutual exclusion, semaphores, monitors, message passing, readers/writers), App. A |
| L18–L19 (Deadlock) | Ch 6 §6.1–6.5 (principles, prevention, avoidance/Banker's, detection), §6.6 dining philosophers |
| L20–L25 (Memory mgmt, paging, segmentation) | Ch 7 (partitioning, first/best/worst-fit, fragmentation, compaction, paging, segmentation) |
| L26–L29 (Virtual memory, replacement, thrashing) | Ch 8 (TLB, demand paging, page replacement, thrashing, working set) |
| L30–L33 (I/O management, buffering) | Ch 11 §11.1–11.4 (I/O devices, organization of the I/O function, OS design issues, buffering) |
| L34–L36 (Disk scheduling) | Ch 11 §11.5–11.6 (disk scheduling: FCFS/SSTF/SCAN/C-SCAN; RAID) |
| L37–L39 (File systems) | Ch 12 (overview, file organization/access, directories, file sharing, secondary-storage mgmt, UNIX/Linux/Windows file systems) |

### Gaps / cautions

| Topic | Status in Stallings 6e | Guidance |
|-------|------------------------|----------|
| **Belady's anomaly** | Only in the back-matter glossary (~p.796); **no textual treatment** of the anomaly or the Optimal page-replacement algorithm. | Cite **Silberschatz 8e p.374** for Belady's anomaly and Optimal replacement; do not page-cite Stallings for these. |
| **"resource-allocation graph"** | The exact hyphenated phrase is not used; Stallings presents deadlock via a **resource allocation graph** figure in Ch 6 §6.1. | Phrase as "resource-allocation graph (Silberschatz §7.2, p.287)" for the canonical form; Stallings supports the same idea. |
| **LOOK / C-LOOK** | §11.5 covers FCFS, SSTF, SCAN, C-SCAN; **LOOK/C-LOOK are not named**. | Cite **Silberschatz 8e p.515** for LOOK/C-LOOK. |
| **READERS/WRITERS** | Ch 5 §5.6 (p.245) — present. | Good secondary citation. |

*Overlap note:* Stallings and Silberschatz cover the same four units; where both are cited, prefer **Silberschatz** for the canonical definition/formula (page-verified) and **Stallings** for the engineering-architecture framing (Stallings emphasizes OS design/implementation more than Silberschatz).
