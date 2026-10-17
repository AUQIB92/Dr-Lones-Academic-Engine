# Index: Understanding the Linux Kernel, 3rd Edition (LinuxKernel)

**Source:** `master_supporting_docs/CS502/supporting_books/LinuxKernel/book.pdf` (944 PDF pages)
**Indexed:** 2026-10-14
**Extraction method:** text-layer (PyMuPDF). No OCR needed.
**Authors/edition:** Daniel P. Bovet & Marco Cesati, *Understanding the Linux Kernel*, **3rd ed.**, O'Reilly, 2006 (covers Linux 2.6). PDF metadata title confirms "UnderStanding The Linux Kernel 3rd Edition".

**Status:** **auxiliary/advanced reference — NOT a prescribed CS502 textbook.** The official CS502 book list is Silberschatz, Bach, and Stallings (`syllabi/CS502.md`). This book is an implementation deep-dive used for *concrete-UNIX/Linux* colour and optional depth, exactly as Bach's *Design of the UNIX Operating System* is used. Cite it only where a lecture explicitly wants to show how a real kernel implements a mechanism.

## Pagination note

Body-page offset is **`printed ≈ PDF − 18`** (verified: TOC "1. Introduction" printed 1 = outline PDF 19; "2. Memory Addressing" printed 35 = PDF 53). Cite the **printed** page (from the printed Contents, PDF pp. 7–11) and give `(PDF N)` as backup.

## Chapter Index

| Chapter | Printed pages | Summary | Key Terms/Definitions | Linux specifics |
|---------|---------------|---------|------------------------|-----------------|
| 1 — Introduction | 1–34 | Linux vs. other UNIX kernels; basic OS concepts; filesystem and kernel overview | multiuser systems, users/groups, processes, **kernel architecture**, filesystem overview, **file descriptor and inode**, process/kernel model, reentrant kernels, process address space, synchronization & critical regions (semaphores, spin locks), signals & IPC, process management (zombies), memory management, device drivers | monolith with loadable modules; softirqs/tasklets (see Ch 4) |
| 2 — Memory Addressing | 35–78 | Hardware + Linux address translation | memory addresses, **segmentation in hardware** (segment selectors, descriptors), segmentation in Linux, **paging in hardware**, paging in Linux (page tables, TLB) | Linux uses segmentation minimally (flat model); 3-/4-level page tables; TLB handling |
| 3 — Processes | 79–130 | Process representation and lifecycle | processes / lightweight processes / threads, **process descriptor (`task_struct`)**, **process switch**, creating processes (`fork`, `clone`, `vfork`), destroying processes (`exit`) | `task_struct` fields; `copy_on_write`; `do_fork` |
| 4 — Interrupts and Exceptions | 131–188 | How Linux handles interrupts/exceptions | interrupt signals role, interrupts vs. exceptions, nested handlers, **IDT**, exception handling, interrupt handling, **softirqs and tasklets**, work queues | top-half/bottom-half; IRQ descriptors; deferred work |
| 5 — Kernel Synchronization | 189–226 | Kernel concurrency primitives | how the kernel services requests, **synchronization primitives** (per-CPU variables, atomic ops, spin locks, read/write spin locks, seqlocks, RCU, semaphores, rwsem, completions), synchronizing accesses to data structures, race-condition examples | spin lock vs. semaphore selection; RCU |
| 6 — Timing Measurements | 227–257 | Clocks, timers, timekeeping | clock/timer circuits, Linux timekeeping architecture, updating time/date, software timers & delay functions, timing system calls | jiffies, timer wheel, `nanosleep` |
| 7 — Process Scheduling | 258–293 | The Linux scheduler (2.6) | scheduling policy, **the scheduling algorithm** (O(1) scheduler: active/expired arrays, priority, timeslice), data structures, scheduler functions, **runqueue balancing in SMP**, scheduling system calls | `nice`/priority; load balancing; SMP affinity |
| 8 — Memory Management | 294–350 | Page-frame and memory-area allocation | **page frame management** (buddy allocator), **memory area management** (slab allocator), noncontiguous memory area management | buddy system; slab/`kmalloc` |
| 9 — Process Address Space | 351–397 | A process's virtual memory | the process address space, memory descriptor (`mm_struct`), **memory regions (VMAs)**, **page fault exception handler**, creating/deleting an address space, managing the heap (`brk`) | demand paging by the kernel; `do_page_fault`; COW |
| 10 — System Calls | 398–419 | How a system call enters/exits the kernel | POSIX APIs and system calls, system-call handler & service routines, entering/exiting a system call, **parameter passing**, kernel wrapper routines | `syscall` table; registers-as-parameters on x86 |
| 11 — Signals | 420–455 | POSIX signals in Linux | role of signals, generating a signal, delivering a signal, signal-handling system calls | signal masks; `sigaction` |
| 12 — The Virtual Filesystem | 456–518 | The VFS abstraction | role of the VFS, **VFS data structures** (superblock, inode, file, dentry objects), filesystem types, filesystem handling, **pathname lookup**, VFS system calls, file locking | inode/dentry caches; `open`/`read`/`write` internals |
| 13 — I/O Architecture and Device Drivers | 519–559 | Linux I/O model and drivers | **I/O architecture**, the device-driver model, device files, **device drivers**, character device drivers | `/dev`, major/minor numbers; `file_operations` |
| 14 — Block Device Drivers | 560–598 | Block layer and I/O scheduling | block devices handling, **the generic block layer**, **the I/O scheduler**, block device drivers, opening a block device file | **elevator I/O schedulers (elevator, deadline, anticipatory)** — the concrete basis for CS502's disk-scheduling lecture |
| 15 — The Page Cache | 599–630 | Caching file pages in RAM | the page cache, storing blocks in the page cache, writing dirty pages to disk, `sync`/`fsync`/`fdatasync` | address_space; writeback |
| 16 — Accessing Files | 631–675 | Reading/writing files and mmap | reading and writing a file, **memory mapping (`mmap`)**, direct I/O transfers, asynchronous I/O | page-cache-backed reads; `mmap` |
| 17 — Page Frame Reclaiming | 676–737 | Reclaiming memory under pressure | **the page frame reclaiming algorithm (PFRA)**, reverse mapping, implementing the PFRA, **swapping** | LRU lists; kswapd; swap area |
| 18 — The Ext2 and Ext3 Filesystems | 738–774 | A concrete on-disk filesystem | general characteristics of Ext2, **Ext2 disk data structures**, Ext2 memory data structures, creating the filesystem, Ext2 methods, managing disk space, the Ext3 filesystem | inodes, block bitmaps, journaling (Ext3) |
| 19 — Process Communication | 775–807 | Linux IPC mechanisms | **pipes**, FIFOs, **System V IPC** (semaphores, message queues, shared memory), POSIX message queues | the concrete IPC implementations CS502 L15/L18 gesture at |
| 20 — Program Execution | 808–834 | How Linux runs a program | executable files, executable formats (ELF), execution domains, the `exec` functions | `execve`; `load_elf_binary` |
| Appendix A — System Startup | 835–841 | Boot sequence | BIOS, boot loader, kernel initialization, `init` | — |
| Appendix B — Modules | 842–851 | Loadable kernel modules | module data structures, module loading/unloading | — |

## Syllabus-coverage check (Step 0.5)

This book is **not on the CS502 syllabus**; it is an auxiliary reference. Mapping to the 39 lectures where it can add concrete-Linux depth:

| CS502 lectures | LinuxKernel chapter(s) | Use |
|----------------|------------------------|-----|
| L01–L03 (Intro, system calls) | Ch 1 (basic OS concepts, file descriptor/inode), Ch 10 (system calls: entering/exiting, parameter passing) | Concrete system-call boundary & parameter passing |
| L05–L07 (Processes) | Ch 3 (process descriptor, process switch, `fork`/`exec`/`exit`), Ch 20 (program execution) | How `fork`/`exec` are actually implemented |
| L08–L09 (Scheduling) | Ch 7 (Linux O(1) scheduler, SMP balancing), Ch 6 (timing/jiffies) | A real scheduler (note: implementation, *not* the classical FCFS/SJF theory) |
| L10–L17 (Concurrency, IPC) | Ch 5 (spin locks, semaphores, RCU, completions), Ch 4 (softirqs/tasklets), Ch 19 (pipes, System V IPC, shared memory) | Real synchronization primitives & IPC |
| L18–L19 (Deadlock) | Ch 5 (spin-lock deadlock avoidance), Ch 1 (avoiding deadlocks) | Practical deadlock avoidance in a kernel |
| L20–L25 (Memory, paging, segmentation) | Ch 2 (segmentation + paging hardware/Linux), Ch 8 (buddy/slab allocators), Ch 9 (address space, page faults, COW) | Concrete address translation and page-fault handling |
| L26–L29 (Virtual memory, reclaim, thrashing) | Ch 9 (page-fault handler), Ch 15 (page cache), Ch 17 (PFRA, LRU lists, swapping) | How demand paging and reclaim work in production |
| L37–L39 (File systems) | Ch 12 (VFS: inode/dentry/file objects, pathname lookup), Ch 15 (page cache), Ch 18 (Ext2/Ext3 on-disk structures) | The concrete filesystem behind the abstract model |
| L15, L30–L36 (I/O, disk scheduling) | Ch 13 (I/O architecture, drivers), Ch 14 (block layer + **elevator I/O schedulers**) | Concrete disk-I/O scheduling in Linux |

### Gaps / cautions

| Topic | Status here | Guidance |
|-------|-------------|----------|
| Classical algorithm **theory** (FCFS/SJF/priority, Banker's, Belady, thrashing theory, First/Best/Worst-fit) | **Not present** — this is an implementation book, not a theory text. | Do **not** cite this book for algorithm-definition pages; use Silberschatz. |
| **Deadlock** (Banker's, RAG) | Only practical avoidance mentions. | Use Silberschatz Ch 7. |
| Portability of line-level detail | Covers Linux **2.6 only**. | Keep references structural ("the page cache", "the O(1) scheduler"), not version-specific claims, in student-facing material. |
