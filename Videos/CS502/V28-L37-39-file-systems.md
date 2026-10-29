# V28 · L37–39 — File Systems, and the Whole OS in One Picture

> **Lecture:** CS502 L37–39 `37-file-concept-access` · `38-file-directories` · `39-file-sharing-implementation` · **Unit:** IV · **Target length:** 18–20 min · **Series position:** 28/28
> **Format:** mixed (screen-capture animation of disk blocks and directory trees + short terminal clip) · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

A file looks like one continuous thing to a user, but on disk it is scattered blocks plus a small record that knows where they are. That gap between the user's view and the disk's view is exactly what an animation can bridge: the same five-block file can be laid down three different ways, and the viewer can watch how many disk reads each layout costs to reach one byte. Directory structures are graphs that grow and get tangled (links, cycles), which a still picture flattens. Inode arithmetic is a tree of pointers that fans out by a factor of 1024 per level; seeing it fan out is what makes "4 TiB from one small record" believable. Finally, as the last video of the series, it is the place to put every earlier idea onto one map.

## 2. Learning outcomes

After watching, the student can:
- [Remember] List file attributes and operations, and explain the per-process and system-wide open-file tables.
- [Understand] Distinguish sequential, direct and indexed access; single-level, two-level, tree and acyclic-graph directories; hard vs. symbolic links; mounting.
- [Apply] Read and set Unix `rwx` permissions and explain an access-control list.
- [Apply] Lay out one file with contiguous, linked/FAT and indexed allocation and count the disk reads needed to reach a given byte; compute the maximum file size of an inode with direct and indirect pointers.
- [Apply] Compute the size of a free-space bit vector and find the first free block; contrast it with a free list.

## 3. Prerequisites

- Basic computer fundamentals: files and folders as a user sees them; bytes, KiB/MiB/GiB/TiB; powers of two.
- **V03** — system calls (`open`, `read`, `write`, `close`).
- **V05** — the PCB (the per-process open-file table lives with it).
- **V20** — paging: a table of block/frame numbers, internal fragmentation.
- **V19** — contiguous allocation and external fragmentation (first/best fit).
- **V26** — block devices, buffering and caching, the life of one `read()`.
- **V27** — disk structure: blocks, seek time, why sequential is faster than scattered.

## 4. Hook (first 30–45 s)

> *You save a 4.5 KB note. The disk does not know what a "note" is. It knows numbered blocks, millions of them. So where does the name live, where do the bytes live, and how does the OS find byte 3500 without reading the whole disk?*
>
> "There are three classic answers. One of them needs one disk read to get there, one needs four, and one needs two. And the same small record that answers it can describe a file of over four terabytes. Let's build a file system, and then, since this is the last video, let's put the entire operating system on one page."

On screen: a file icon "notes.txt, 4500 B" dissolves into five blocks scattered on a disk grid; three counters "1 read / 4 reads / 2 reads" blurred until scene 10.

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:40 | Hook | File icon dissolving into disk blocks; blurred counters | Disk knows blocks, not files; three layouts; one record, 4 TiB |
| 2 | 0:40–1:00 | Recap: syscalls & blocks (V03, V26, V27) | `open`/`read` trap; a block device lane; a disk arm | 15–20 s: programs ask the kernel via system calls; disks move fixed-size blocks; scattered blocks mean extra seeks |
| 3 | 1:00–2:30 | File concept, attributes, operations | A file card with fields: name, identifier, type, location, size, protection, timestamps, owner. Operation buttons: create, open, read, write, seek (reposition), delete, truncate, close | **Term card — File:** a named collection of related data on secondary storage, the OS's logical storage unit. **Attributes** = metadata. Operations are system calls |
| 4 | 2:30–3:45 | Open-file tables | Two processes `open` the same file. Per-process table (in/near the PCB, V05) holds a file pointer and mode; system-wide table holds location, size, an open count = 2. `close` decrements the count | **Term card — Open-file table:** kernel memory caching info about open files so each `read` need not search the directory again. **File descriptor:** index into the per-process table |
| 5 | 3:45–5:00 | Access methods | A tape head moving forward (sequential); a jump directly to block n (direct / relative); an index card pointing into the file (indexed) | **Sequential:** read next, write next. **Direct:** `read n` for any n; needs fixed-size records/blocks. **Indexed:** an index of keys → block numbers; search the small index, then jump |
| 6 | 5:00–7:00 | Directory structures | Single-level (one flat list, name clash), two-level (one directory per user), tree (paths, current directory, absolute vs. relative), acyclic graph (one file under two directories) | **Term card — Directory:** a symbol table mapping names to file records. Show the name clash that breaks single-level; show `/home/ali/os/notes.txt` as a path |
| 7 | 7:00–7:45 | Links | Hard link: two names → same inode, link count 2; delete one, file survives. Symbolic link: a small file containing a path; delete the target, link dangles | **Term cards:** *Hard link*, *Symbolic (soft) link*. Acyclic graphs need a reference count to know when to free a file; cycles (general graph) need garbage collection, so most systems forbid hard links to directories |
| 8 | 7:45–8:30 | Mounting | A USB drive's own tree snaps onto an empty directory `/media/usb` in the main tree | **Term card — Mounting:** attaching a file system to a mount point in the existing tree; the old contents of the mount point are hidden until unmount |
| 9 | 8:30–10:00 | Sharing & protection | `ls -l` line `-rw-r-----` broken into owner/group/others; octal 640; then an ACL listing named users | **Term card — Access control:** who may do what. Unix: 3 classes × (r, w, x). **ACL:** per-file list of (user, rights), finer but longer. Directory `x` = may enter |
| 10 | 10:00–13:00 | Allocation, worked on one file | "notes.txt" = 4500 B, 1 KiB blocks → 5 blocks. **Contiguous:** blocks 9–13, directory stores (start 9, length 5). **Linked:** 9 → 16 → 1 → 10 → 25 → end; **FAT** column shows the same chain in a table. **Indexed:** index block 19 = [9, 16, 1, 10, 25]. Byte 3500 → logical block 3, offset 428. Counters: contiguous 1 read (disk block 12), linked 4 reads (9, 16, 1, 10), indexed 2 reads (19, then 10) | **Term card — File-control block (FCB) / inode:** the per-file record holding attributes and block locations. Contiguous: fast, but external fragmentation and hard to grow (recap V19). Linked: no external fragmentation, but direct access is slow and a broken pointer loses the rest. FAT: the chain moved into a table, which can be cached. Indexed: direct access without fragmentation, at the cost of an index block. Last block wastes 620 B (internal fragmentation, V20) |
| 11 | 13:00–13:30 | Pause-and-predict 1 | "Linked allocation: how many reads to reach logical block 4?" | Answer §7 |
| 12 | 13:30–15:30 | Inode max-file-size arithmetic | Inode drawn with 12 direct pointers, then single, double, triple indirect; block size 4 KiB, pointer 4 B → 1024 pointers per block; a fan-out tree grows level by level with running totals: 48 KiB → ≈ 4.05 MiB → ≈ 4.004 GiB → ≈ 4.004 TiB | Small files use only direct pointers (fast). Each indirect level multiplies reach by 1024. Total 1,074,791,436 blocks = 4,402,345,721,856 B |
| 13 | 15:30–16:45 | Free-space management | Bit vector for 17 blocks `11100110000111110` (1 = allocated, 0 = free): first free = block 3. Then a 1 TiB disk with 4 KiB blocks → 268,435,456 bits = 32 MiB bitmap. Free list: free blocks chained 3 → 4 → 7 → … | **Term card — Bit vector (bitmap):** one bit per block. Easy to find contiguous runs; must be kept in memory to be fast. **Free list:** each free block points to the next; no extra space, but slow to traverse. Mention grouping/counting in one line |
| 14 | 16:45–17:15 | Pause-and-predict 2 | "Which blocks are free in the bitmap?" | Answer §7 |
| 15 | 17:15–19:30 | Series-closing recap map | One-page map builds outward from "hardware": V01–V04 foundations ring; V05–V09 process & CPU; V10–V18 concurrency & deadlock; V19–V25 memory; V26–V28 I/O, disk & files. Arrows: a `read()` from V26 passes through the file system (V28) to the disk scheduler (V27); a page fault (V23) uses the same disk; a blocked process (V06) waits on a semaphore-like queue (V13) | The OS as three illusions: every process has its own CPU (scheduling), its own memory (virtual memory), its own permanent storage (files), all safely shared (synchronisation, protection). Close the series |

### Scene-by-scene beat detail

**Scene 1 — Hook.**
- "notes.txt, 4500 B" floats in; it breaks into five tiles that fly to scattered spots on a disk grid.
- Three blurred counters wait in the corner.
- Beat: "The disk knows numbered blocks. The file system is the translator."

**Scene 2 — Recap (V03, V26, V27), 15–20 s.**
- `open()` trap arrow (V03); a block-device lane with 1 KiB tiles (V26); the disk arm moving between far-apart tracks (V27).
- Beat: "Fewer, closer block reads are faster."

**Scene 3 — File concept.**
- Attribute fields fill in one by one on a file card.
- Operation buttons appear; each one, when pressed, shows a tiny trap arrow: they are system calls.
- Beat: "Seek does not touch the disk; it only moves the file pointer."

**Scene 4 — Open-file tables.**
- Process A and Process B (two PCB boxes) each get a row in their own table: (fd 3, pointer, mode).
- Both rows point to one system-wide entry with "open count = 2".
- A closes: count drops to 1; B still reads.
- Beat: "Why cache? Searching the directory on every read would cost extra disk reads."

**Scene 5 — Access methods.**
- Sequential: a head steps through records 0, 1, 2, …
- Direct: a jump arrow to record 37.
- Indexed: a small index (A–F → block 4, G–M → block 9, …), a lookup, then a direct jump.

**Scene 6 — Directory structures.**
- Single-level: two users both create `test.c`; red clash.
- Two-level: master directory → user directories; clash resolved; but users cannot easily share.
- Tree: grow `/home/ali/os/notes.txt`; current directory marker; absolute vs. relative path shown for the same file.
- Acyclic graph: a shared `project/` directory appears under two users.

**Scene 7 — Links.**
- Hard link: second name arrow to the same inode; link count 1 → 2; delete one → 1; file stays.
- Symlink: a tiny file containing the text `/home/ali/os/notes.txt`; delete the target; symlink turns red ("dangling").
- Beat: "Hard links to directories could make cycles; most systems forbid them."

**Scene 8 — Mounting.**
- The USB tree hovers, then snaps onto `/media/usb`; an old file in that directory greys out ("hidden until unmount").

**Scene 9 — Sharing & protection.**
- `-rw-r-----` splits into four colour groups (type, owner, group, others); octal 6 4 0 typed underneath.
- ACL list: `ali: rw`, `sara: r`, `everyone: –`.
- Beat: "Unix bits are compact but coarse; ACLs are precise but longer to store and check."

**Scene 10 — Allocation (the centrepiece).**
- Same disk grid; three passes over the same 5-block file.
- Contiguous: tiles 9–13 light; directory entry (9, 5); byte 3500 → block 9 + 3 = 12 → 1 read.
- Linked: tiles 9, 16, 1, 10, 25 light with arrows; a read-head token follows 4 arrows to reach block 10.
- FAT: same chain shown as a table column: entry 9 → 16, 16 → 1, 1 → 10, 10 → 25, 25 → EOF.
- Indexed: index block 19 shows the list; token reads 19, then 10 → 2 reads.
- Last tile shows 620 B shaded "unused".
- Counters from the hook unblur.

**Scene 11 — Pause-and-predict 1.** Freeze on the linked chain; countdown 5 s.

**Scene 12 — Inode arithmetic.**
- Inode rectangle; 12 short arrows to data blocks: "12 × 4 KiB = 48 KiB".
- Single indirect: one arrow to a pointer block holding 1024 arrows: total ≈ 4.05 MiB.
- Double: 1024 × 1024 blocks; tree fans out; total ≈ 4.004 GiB.
- Triple: 1024³; total ≈ 4.004 TiB.
- Beat: "Each level multiplies by 1024 because one 4 KiB block holds 1024 four-byte pointers."

**Scene 13 — Free space.**
- 17 bit cells; scan left to right; stop at the first 0 (block 3).
- Scale-up card: 1 TiB / 4 KiB = 2²⁸ blocks → 2²⁸ bits = 32 MiB.
- Free list: chain arrows through the zero cells.

**Scene 14 — Pause-and-predict 2.** Bitmap frozen; reveal the free set.

**Scene 15 — Series-closing recap map.** See §5a.

### 5a. Series-closing recap map (scene 15 layout)

Build the map in five rings, each ring appearing with a 10–15 s narration beat and the video numbers printed on each node:

1. **Core — hardware & protection (V01, V02):** CPU, memory, devices; interrupts, dual mode, timer.
2. **Interface & structure (V03, V04):** system calls as the only door; monolithic / layered / microkernel.
3. **Processes & CPU (V05–V09):** PCB, states, context switch, `fork`/`exec`, scheduling. Label: "illusion 1: everyone has a CPU".
4. **Concurrency (V10–V18):** critical sections, locks, semaphores, monitors, IPC, deadlock, RAG, Banker's. Label: "sharing safely".
5. **Memory (V19–V25):** MMU, paging, TLB, segmentation, demand paging, replacement, thrashing. Label: "illusion 2: everyone has a private memory".
6. **I/O, disk, files (V26–V28):** drivers, DMA, buffering; disk scheduling; files, directories, inodes. Label: "illusion 3: permanent named storage".

Cross-links drawn last (each an animated arrow):
- Page fault (V23) → disk I/O (V26) → disk scheduler (V27).
- `read()` (V03) → file system (V28) → buffer cache (V26).
- Blocked process (V06) ↔ waiting queue of a semaphore (V13) ↔ device queue (V26).
- Copy-on-write (V23) ↔ `fork` (V07).
- Protection: mode bit (V02) ↔ page-table bits (V20) ↔ `rwx` (V28).

Final line on screen: "An operating system is a resource manager that hands out illusions safely." Hold 5 s; end card with the series index.

## 6. Worked example (verified)

### 6.1 One file, three allocation methods

File `notes.txt`: 4500 B; block size 1 KiB = 1024 B → ⌈4500 / 1024⌉ = **5 blocks**; last block wastes 5 × 1024 − 4500 = **620 B**.
Target byte 3500 → logical block ⌊3500 / 1024⌋ = **3**, offset 3500 mod 1024 = **428**.

| Method | Layout on disk | Directory / FCB stores | Disk reads to reach logical block 3 |
|---|---|---|---|
| Contiguous | 9, 10, 11, 12, 13 | start 9, length 5 | **1** (block 9 + 3 = 12) |
| Linked | 9 → 16 → 1 → 10 → 25 | first 9, last 25 | **4** (9, 16, 1, 10) |
| FAT | table: 9→16, 16→1, 1→10, 10→25, 25→EOF | first 9 | 1 data read if the FAT is cached in memory; otherwise FAT reads + 1 |
| Indexed | index block 19 = [9, 16, 1, 10, 25] | index block 19 | **2** (19, then 10) |

(Assumes the FCB/inode itself is already in memory via the open-file table.)

### 6.2 Inode maximum file size

Block size 4 KiB = 4096 B; pointer size 4 B → 1024 pointers per block. 12 direct pointers, 1 single, 1 double, 1 triple indirect.

| Reach | Blocks | Cumulative size |
|---|---|---|
| 12 direct | 12 | 48 KiB |
| + single indirect | 12 + 1024 | ≈ 4.047 MiB |
| + double indirect | + 1024² | ≈ 4.004 GiB |
| + triple indirect | + 1024³ | ≈ 4.004 TiB |
| **Total** | **1,074,791,436 blocks** | **4,402,345,721,856 B** |

A 100 KiB file needs 25 blocks: 12 direct + 13 via the single indirect block.

### 6.3 Free-space management

Bit vector for blocks 0–16, convention 1 = allocated, 0 = free: `11100110000111110` → free blocks **3, 4, 7, 8, 9, 10, 16**; first free = **3**. (State the convention on screen; some textbooks use 1 = free.)
Bitmap size for a 1 TiB disk with 4 KiB blocks: 2⁴⁰ / 2¹² = 268,435,456 blocks → 268,435,456 bits = **32 MiB**. For comparison, a FAT with 4-byte entries for the same disk would be 1024 MiB.

**Verification.** Every number above was computed by `python3` in `/private/tmp/claude-501/-Users-auqib92-Academic-Command-Centre/4c985779-d8d6-40da-997c-96f474fa719d/scratchpad/videos/`: `v28.py` (block count, internal fragmentation, byte → block/offset, inode reach at each level and total, bitmap and FAT sizes, free blocks from the bit string) and `v28q.py` (read counts per allocation method, 100 KiB example, and quiz values).

## 7. Pause-and-predict checkpoints

1. **Scene 11: linked allocation, reads to reach logical block 4?** Answer: 5 (9, 16, 1, 10, 25); every earlier block must be read to find the next pointer.
2. **Scene 14: free blocks in `11100110000111110`?** Answer: 3, 4, 7, 8, 9, 10, 16.
3. **Scene 7, before deleting: after `rm` of one hard-link name, does the file still exist?** Answer: yes; link count drops from 2 to 1, and data is freed only at 0.

## 8. Misconceptions to address on screen

- "A file's name is stored in the file." → Directory entry holds the name; the inode/FCB holds attributes and block pointers; the data blocks hold only data.
- "Deleting a hard link deletes the file." → Link-count animation 2 → 1, file still readable.
- "Indexed allocation wastes a whole block for tiny files, so it's always worse." → Show the inode's direct pointers: small files need no separate index block.
- "Contiguous allocation is obsolete." → It is the fastest for reads and is still used for read-only media; its problem is growth and external fragmentation.
- "`x` on a directory means you can run it." → Directory `x` = permission to enter/traverse.
- "Mounting copies files." → Mount just grafts a tree; unmount and the old contents reappear.

## 9. Visual assets

- **Reuse:** no `Slides/CS502` decks for L37–L39 yet. Reuse the V19 first/best-fit hole picture for contiguous allocation, the V20 page-table look for the index block, the V27 disk-arm drawing, and the palette of `Slides/CS502/08-cpu-scheduling-basics.tex`.
- **New:**
  - Disk-block grid with three allocation passes and a travelling read-head token (Manim).
  - FAT table column synchronised with the linked chain.
  - Inode fan-out tree with running totals.
  - Directory structures morphing single → two-level → tree → acyclic graph (PowerPoint morph).
  - Mount animation: subtree snapping on.
  - Five-ring series recap map (Motion Canvas), exported also as a still PDF for the series landing page.
- **Live demo (optional, 60 s):** `ls -li notes.txt`, `ln notes.txt n2; ls -li` (same inode number, link count 2), `ln -s notes.txt n3; rm notes.txt; cat n3` (dangling), `stat n2`, `chmod 640 n2; ls -l n2`, `df -h` (mount points).

## 10. On-screen text & notation

- File, attributes, operations; open-file table (per-process, system-wide); file descriptor; open count.
- Sequential, direct (relative), indexed access.
- Directory; single-level, two-level, tree, acyclic graph, general graph; path (absolute, relative); current directory.
- Hard link, symbolic (soft) link, link count; mount, mount point.
- `rwx`, owner/group/others, octal mode; ACL.
- File-control block (FCB) / inode; contiguous, linked, FAT, indexed allocation; direct / single / double / triple indirect.
- Bit vector (bitmap), free list; internal vs. external fragmentation.
- Units: KiB = 1024 B, MiB = 1024 KiB, GiB, TiB, binary throughout this video.

## 11. End-of-video quiz

1. **(Numerical)** Block size 8 KiB, 4-byte pointers, inode with 10 direct, 1 single and 1 double indirect pointer. Maximum file size? **Answer:** 10 + 2048 + 2048² = 4,196,362 blocks = 34,376,597,504 B ≈ 32.02 GiB.
2. **(Numerical)** A 500 GiB disk with 4 KiB blocks uses a bit vector. How large is it? **Answer:** 131,072,000 bits = 15.625 MiB.
3. **(MCQ)** Which allocation method gives the fastest direct access but suffers external fragmentation? (a) linked (b) FAT (c) contiguous (d) indexed. **Answer: (c).**
4. **(MCQ)** Permission string `-rwxr-x---` in octal is (a) 750 (b) 755 (c) 640 (d) 570. **Answer: (a).**
5. **(Short)** Why do most systems forbid hard links to directories? **Answer:** they could create cycles, breaking traversal and making reference counts unable to decide when to free a directory.

*Quiz numbers computed in `v28q.py` (same directory as §6).*

## 12. References

- Silberschatz, *Operating System Concepts* 8e, Ch 10: file concept (p.421), access methods (p.430), directory and disk structure (p.433), mounting (p.444), file sharing (p.446), protection (p.451). Ch 11: file-system structure (p.461), file-control block (pp.463–465), directory implementation (p.470), allocation methods (p.471), free-space management (p.479). Inode (pp.443, 464).
- Stallings, *Operating Systems* 6e, Ch 12: overview (p.552), file organisation and access (p.558), file directories (p.562), file sharing (p.567), secondary-storage management (p.570), VFS/inode (p.587).
- Bovet & Cesati, *Understanding the Linux Kernel*, Ch 12 the VFS (pp.456–518), Ch 18 Ext2/Ext3 (pp.738–774).

## 13. Production notes

- Scene 10 is the centrepiece; keep the same disk grid for all three passes so only the layout changes.
- Scene 15 must not be cut; it is the series finale. If over length, cut the live demo, shorten scene 5 to one sentence per method, and merge scenes 7–8.
- Read inode totals as "about four mebibytes / gibibytes / tebibytes" and show exact block counts on screen.
- Distinguish allocated/free cells by fill pattern as well as colour.
- Captions on; provide the recap map as a downloadable still.
- End card: link back to V01 so the series loops.
- Timing budget: file concept + tables (scenes 2–4) about 3.5 min; access + directories + links + mount (5–8) about 4.5 min; protection (9) 1.5 min; implementation (10–14) about 7 min; recap map (15) 2–2.5 min.
- Record the recap map with a slow, steady build; do not cut between rings.
- Thumbnail: the inode fan-out tree with "4 TiB" on it.
