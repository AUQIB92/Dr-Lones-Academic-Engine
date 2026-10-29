# CS502 Operating Systems — Complete Video Series

A self-contained, 28-video course on Operating Systems that covers the whole CS502 syllabus (39 lectures, 4 units). The **only prerequisite is computer fundamentals**: what a CPU, memory, disk, program and file are, plus binary numbers. Each video assumes nothing beyond the videos before it. Every OS term is defined on screen the first time it is used, and ideas from earlier videos get a short recap. A student can learn the subject from these videos alone.

Each `V<NN>-*.md` file is a production brief written to [`_TEMPLATE.md`](_TEMPLATE.md):

- why it should be a video
- learning outcomes
- a hook
- a timed storyboard with narration beats
- a verified worked example
- pause-and-predict checkpoints
- misconceptions
- visual assets
- a quiz
- page-cited references
- production notes

**Accuracy.** Every number in a brief was computed by a script or by running a real program. Each brief's §6 says how it was verified.

Once a video is published, add its link to [`syllabi/CS502.videos.yaml`](../../syllabi/CS502.videos.yaml), which drives the course hub's *Video Explainers* section.

## The series

### Unit I — Introduction to Operating Systems & Process Management

| V | Lectures | Title |
|---|---|---|
| [01](V01-L01-01-intro-os-structure.md) | L01 | **What Is an Operating System, and What Does It Do?** Definition, functions, services, evolution and types; the series roadmap |
| [02](V02-L02-02-os-operations-protection.md) | L02 | The Machine Underneath: interrupts, storage hierarchy, dual mode, timer, protection |
| [03](V03-L03-03-system-calls.md) | L03 | The Journey of a System Call |
| [04](V04-L04-04-system-programs-os-structure.md) | L04 | How an OS Is Built: system programs; monolithic, layered, microkernel, modular, VM |
| [05](V05-L05-05-process-concept-pcb.md) | L05 | From Program to Process: memory layout and the PCB |
| [06](V06-L06-06-process-states-schedulers.md) | L06 | Process States, Queues, and the Context Switch |
| [07](V07-L07-07-process-operations.md) | L07 | fork, exec, wait: Creating and Ending Processes |
| [08](V08-L08-08-cpu-scheduling-basics.md) | L08 | CPU Scheduling I: Criteria, FCFS, SJF, SRTF |
| [09](V09-L09-09-cpu-scheduling-advanced.md) | L09 | CPU Scheduling II: Priority, Round Robin, MLFQ, Multiprocessor |

### Unit II — Concurrent Processes & Deadlock

| V | Lectures | Title |
|---|---|---|
| [10](V10-L10-10-concurrency-critical-section.md) | L10 | Race Conditions and the Critical-Section Problem |
| [11](V11-L11-11-mutex-software-solutions.md) | L11 | Mutual-Exclusion Requirements and Peterson's Solution |
| [12](V12-L12-12-synchronization-hardware.md) | L12 | Hardware Help: Test-and-Set, Compare-and-Swap, Atomics |
| [13](V13-L13-13-semaphores.md) | L13 | Semaphores |
| [14](V14-L14-15-ipc.md) | L14–15 | Inter-Process Communication: Shared Memory, Messages, Pipes, Sockets |
| [15](V15-L16-16-classical-producer-consumer.md) | L16 | The Bounded-Buffer Producer–Consumer Problem |
| [16](V16-L17-17-classical-readers-writers-monitors.md) | L17 | Readers–Writers, Dining Philosophers, Monitors |
| [17](V17-L18-18-deadlock-model-characterization.md) | L18 | Deadlock: Resource-Allocation Graphs and the Four Conditions |
| [18](V18-L19-19-deadlock-handling.md) | L19 | Handling Deadlock: Prevention, Banker's Algorithm, Detection, Recovery |

### Unit III — Memory Management & Virtual Memory

| V | Lectures | Title |
|---|---|---|
| [19](V19-L20-22-memory-basics-contiguous.md) | L20–22 | Memory Basics: Binding, the MMU, Contiguous Allocation, Fragmentation |
| [20](V20-L23-23-paging.md) | L23 | Paging and Address Translation |
| [21](V21-L24-24-tlb-page-table-structures.md) | L24 | TLB, Effective Access Time, and Multilevel Page Tables |
| [22](V22-L25-25-segmentation.md) | L25 | Segmentation and Segmentation with Paging |
| [23](V23-L26-26-virtual-memory-demand-paging.md) | L26 | Virtual Memory and Demand Paging |
| [24](V24-L27-27-page-replacement-basic.md) | L27 | Page Replacement I: FIFO, Optimal, LRU, Belady's Anomaly |
| [25](V25-L28-29-clock-frames-thrashing.md) | L28–29 | Page Replacement II: Clock, Frame Allocation, Working Set, Thrashing |

### Unit IV — I/O Management, Disk Scheduling & File Systems

| V | Lectures | Title |
|---|---|---|
| [26](V26-L30-33-io-systems.md) | L30–33 | I/O Systems: Polling, Interrupts, DMA, Buffering, the Kernel I/O Subsystem |
| [27](V27-L34-36-disk-structure-scheduling.md) | L34–36 | Disks: Structure, Access Time, and All Six Scheduling Algorithms |
| [28](V28-L37-39-file-systems.md) | L37–39 | File Systems: Files, Directories, Allocation, Free Space; series recap |

Every one of the 39 lectures maps to a video. Some videos bundle several lectures, where those lectures tell a single story together.

## Recording plan

1. **Record in series order.** Each video's recaps and bridges assume the one before it.
2. **Unit I first.** Its decks (L01–L09) are already built, so their TikZ figures can be reused directly.
3. **Target length is 10–18 minutes each.** That gives roughly 6–7 hours of video for the whole course.

## Status

| Videos | Brief | Recorded | Published (link in `CS502.videos.yaml`) |
|---|---|---|---|
| V01–V28 | ✅ drafted | ☐ | ☐ |
