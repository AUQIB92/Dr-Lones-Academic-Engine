# V07 · L07 — One Call, Two Returns: `fork`, `exec`, `wait`, Zombies and Orphans

> **Lecture:** CS502 L07 `07-process-operations` · **Unit:** I · **Target length:** 11–13 min · **Series position:** 07/28
> **Format:** code + terminal, with animated process-tree overlays · **Language:** English (Hinglish version optional)

## 1. Why this deserves a video

`fork()` is the one call in the course that returns *twice*, into two
processes that then run at the same time. On paper the code reads top to
bottom, so students mentally run one copy and get every "how many lines does
this print?" question wrong. Watching the process tree *grow* — a node
splitting into two at each `fork`, branches pruned by `&&`/`||` — makes
counting a tree problem, not a guess. The zombie and orphan states only
exist for a moment in time; a live `ps` showing `Z` and a `getppid()` that
changes to 1 is far more convincing than a definition box.

## 2. Learning outcomes

After watching, the student can:
- [Remember] State what `fork()` returns in the child (`0`), in the parent (the child's pid), and on failure (`-1`).
- [Apply] Count the processes and output lines of a program with sequential forks, forks inside a loop, and forks combined with `&&` / `||`, by drawing the process tree.
- [Understand] Explain why `exec` never returns on success and why code after it does not run.
- [Analyze] Explain why the same `fork` loop prints a different number of lines on a terminal and when redirected to a file (the copied `stdio` buffer).
- [Understand] Define zombie and orphan, and say who reaps each.

## 3. Prerequisites

- Computer fundamentals: CPU, memory, a program, a file; what "output to the screen" vs. "output to a file" means.
- V03 *The journey of one system call*: system calls; `printf` buffers text and calls `write` later.
- V05 *The process and its PCB*: process, PCB, process id (pid).
- V06 *Process states, queues and the context switch*: the `waiting` state.
- Helpful: reading simple C (`if/else`, `for`). The short-circuit rule for `&&` / `||` is explained on screen (scene 9). All of the above is recapped in scene 1b.

## 4. Hook (first 30–45 s)

> *On screen: an editor with four lines of C.*
> ```c
> int main(void) {
>     fork(); fork(); fork();
>     printf("hi\n");
> }
> ```
> "Three lines of real code. How many times does `hi` get printed? Write a
> number down — right now, before I run it. … Most people say three. Some say
> four. Let's run it."
>
> *Terminal:* `gcc f3.c -o f3 && ./f3` — eight `hi` lines scroll out.
>
> "Eight. By the end of this video you'll get that right every time — and
> you'll also see a program that prints 14 lines on my screen but 24 lines
> into a file, with no change to the code."

## 5. Storyboard

| # | Time | Scene | On screen (visuals / animation) | Narration beats (not a script) |
|---|------|-------|---------------------------------|--------------------------------|
| 1 | 0:00–0:45 | Hook | Code of `f3.c`; viewer prompt "your guess?"; terminal run prints 8 lines. | Commit to a guess; reveal 8; promise a method. |
| 1b | 0:45–1:05 | Recap from V03, V05, V06 (20 s) | A PCB card with a `pid` badge; a `printf` dropping text into a buffer box that later empties into a `write` arrow; the state `waiting` lit on a mini state diagram. | A **process** = a running program, recorded by the kernel in a **PCB**; each has a **pid** (process id). A **system call** asks the kernel for a service. `printf` does not write immediately — it fills a **buffer** in the program's own memory and calls `write` later. A process blocked until something happens is in the **waiting** state. |
| 2 | 1:05–1:40 | Who runs `ls`? | Shell prompt; `ls` typed; a process tree appears: `shell` → child → becomes `ls` → disappears; shell continues (deck **Welcome: Who Actually Runs `ls`?**, **Parents, Children, and the Process Tree**). | Every process except the first is created by another. Shell creates a child, child becomes `ls`, shell waits. Three calls: `fork`, `exec`, `wait`, plus `exit`. |
| 3 | 1:30–2:40 | One call, two returns | Single timeline splits at `fork()` into two lanes, *parent* and *child*. A return-value bubble appears over each: `child: 0`, `parent: 4211` (illustrative pid, labelled "e.g."). Table from deck **`fork`: One Call, Two Returns** fades in. | Both continue at the instruction after the call. The return value is the *only* way the copies can tell themselves apart. Failure: `-1`, no child. |
| 4 | 2:40–3:50 | The canonical program | Deck **Worked Example: The Canonical `fork`/`exec`/`wait`** code on the left; on the right the deck's **The Same Program as a Timeline** TikZ animates: child branch takes `pid == 0`, `exec("ls")` box replaces its image, `exit()` sends a status arrow up; parent sits in the `wait()` box until it arrives, then prints `Child complete`. | Highlight which branch each copy takes. The parent is *blocked* (state `waiting`, V06) while the child runs. |
| 5 | 3:50–4:40 | Two copies, two variables | Two memory boxes side by side, both showing `x = 10` at the same virtual address. Child's box changes to 15, parent's to 5. Terminal run of `x.c`: `child x=15`, `parent x=5`. | Same name, same virtual address, different physical copies. Shared: open file descriptors. So a child cannot hand back an answer via a global variable — use the exit status, or inter-process communication (IPC), covered later in the series. Define **virtual address** briefly: the address a program sees; the OS maps it to real memory separately for each process. |
| 6 | 4:40–6:00 | Counting: sequential forks | Deck **Three `fork` Calls in a Row** binary-tree TikZ built level by level: P → (P, C1) → 4 → 8 nodes, side labels "after 1st: 2 / after 2nd: 4 / after 3rd: 8". Then `f4.c` run prints 16. | Each `fork` doubles the number of processes that execute the *next* line. `n` sequential forks: `2^n` processes, `2^n − 1` new children. |
| 7 | 6:00–7:10 | Counting: `printf` inside the loop | Code `for (i=0;i<3;i++){ fork(); printf("A\n"); }`. Tree grows one level per iteration; at each level every leaf emits an "A" chip: 2, then 4, then 8. Running total 2 + 4 + 8 = 14. Terminal run on screen: 14 lines. | Count prints per level, not processes at the end: 14 lines from 8 processes (deck **Worked Example: `printf` Inside the Loop**). |
| 8 | 7:10–8:10 | The buffer that gets copied | Same binary, run as `./loop > out.txt; wc -l out.txt` → **24**. Then an animation: a `stdout` buffer box inside each process holding "A\n" chips that have not been written yet; at each `fork` the box is copied with its chips. Then `printf("hello "); fork(); printf("world\n");` prints `hello world` twice; with `fflush(stdout)` before `fork` it prints `hello world` then `world`. | On a terminal `stdout` is line-buffered, so each `A\n` is written immediately: 14. Into a file it's fully buffered, unwritten `A`s are copied into every child: 24. `fork` copies *state*, not just code (deck **The Buffer That Gets Copied**). Fix: `fflush(stdout)` before `fork`. |
| 9 | 8:10–9:20 | Short-circuit operators prune the tree | `(fork() && fork()) \|\| fork(); printf("y\n");` — tree drawn with each node labelled by the value it saw (non-zero / 0); branches that skip an operand are greyed out. Result P, C1, C2, C3, C4 → 5. Then `if (fork() && fork()) fork();` → 4 (deck **Socratic Check 2**). | `fork()` is non-zero in the parent and 0 in the child, so `&&`/`\|\|` decide *which copy* forks again. Rule: draw the tree, label each node with what it saw, follow the operator. |
| 10 | 9:20–10:10 | `exec`: same process, new program | Process box keeps its pid label while its contents (code, data, stack) are wiped and replaced with `echo`. The line `printf("never\n")` fades out with the old image. Terminal run of `ex.c`: `from-exec`, `parent`. | `exec` replaces the image; on success it never returns. Code after it was part of the old program. Why split `fork` + `exec`? The child can set up redirection in between; copy-on-write makes the copy cheap (deck **Why Split Creation Into `fork` + `exec`?**, **Is Copying the Whole Address Space Not Wasteful?**). |
| 11 | 10:10–11:20 | Zombies and orphans, live | Deck **The Life Cycle at the Boundary** TikZ (created → running → zombie → reaped; dashed path to "orphan of `init`"). Terminal: `./zomb` shows `ps` line with `STAT Z <defunct>`, then `reaped … status=7`. Then `./orph`: `child ppid before=<parent pid>`, parent exits, `child ppid after=1`. | Zombie = terminated, parent hasn't called `wait`: PCB entry (pid + status) kept. Orphan = parent died first: re-parented to pid 1, which waits for it. A server that never waits fills the pid table with zombies (deck **Socratic Check 3**). |
| 12 | 11:20–12:20 | The shell loop + bridge | Deck **Worked Example: The Shell Loop**: prompt → `fork` → child `execvp` → `exit` → parent `waitpid` → prompt, as a looping animation; a `&` variant skips the wait. Teaser: five process cards crowd a single CPU icon. | Four process-control calls per foreground command. **Next, V08 — FCFS, SJF, SRTF:** we can now create many processes; with one CPU, which one should run next? We'll build schedules live and measure them. |

## 6. Worked example (verified)

**Running example:** the shell running a command — the deck's canonical
`fork`/`exec`/`wait` program — plus the deck's fork-counting programs.
Every count below was produced by compiling and running a real C program.

| Program (body of `main`, then `while (wait(NULL) > 0);` to reap) | Processes | Lines printed |
|---|---|---|
| `fork(); fork(); fork(); printf("hi\n");` | 8 | **8** |
| `fork(); fork(); fork(); fork(); printf("hi\n");` | 16 | **16** |
| `for(i=0;i<3;i++){ fork(); printf("A\n"); }` — terminal / line-buffered | 8 | **14** (2 + 4 + 8) |
| same binary, `stdout` redirected to a file or pipe | 8 | **24** |
| `(fork() && fork()) \|\| fork(); printf("y\n");` | 5 | **5** |
| `if (fork() && fork()) fork(); printf("x\n");` | 4 | **4** |
| `printf("hello "); fork(); printf("world\n");` | 2 | `hello world` ×2 |
| same with `fflush(stdout);` before `fork` | 2 | `hello world`, `world` |

**Two copies, two variables (`x.c`):** `int x = 10; pid = fork();` child does
`x += 5`, parent `wait`s then `x -= 5`. Output: `child x=15`, `parent x=5`.

**Code after exec (`ex.c`):** child `execlp("echo","echo","from-exec",NULL);
printf("never\n");`, parent `wait(NULL); printf("parent\n");`. Output:
`from-exec`, `parent` — two lines.

**Zombie (`zomb.c`):** child `exit(7)`; parent sleeps 2 s, runs
`ps -o pid,ppid,stat,comm -p <child>` → `STAT Z`, `COMM <defunct>`; then
`waitpid` → `reaped <pid> status=7`; afterwards `ps` no longer lists it.

**Orphan (`orph.c`):** child prints `getppid()` (the parent's pid), parent
exits, child sleeps 2 s and prints `getppid()` again → `1`.

**Tree trace for `(fork() && fork()) || fork()`:**
1. First `fork`: P (non-zero), C1 (0).
2. P runs second `fork`: P (non-zero), C2 (0). P's `&&` true → skips `||`.
3. C1's `&&` false → C1 runs third `fork` → C3.
4. C2's `&&` false → C2 runs third `fork` → C4.
5. P, C1, C2, C3, C4 each print once → 5.

**How the numbers were verified.** Every row above was compiled with `gcc`
and run on the recording machine (macOS) in `scratchpad/videos/fork/`;
counts taken with `./prog | wc -l` and `sort | uniq -c`. The 14-line count
was measured with `setvbuf(stdout, NULL, _IOLBF, 0)` (line buffering, as on a
terminal) and the 24-line count with `stdout` going to a pipe and to a file
(`./loop > out.txt; wc -l`). The 14 vs. 24 discrepancy is a real finding:
the deck's "14 (verified)" is correct for a terminal; say so on screen and
show both. Zombie `Z` state and the orphan's `ppid = 1` were observed live.
pids in the video will differ from run to run — never quote a specific pid.

## 7. Pause-and-predict checkpoints

1. **Scene 5, before the run:** "Child adds 5, parent subtracts 5, both
   starting from `x = 10`. What does each print?" — *Answer:* `child x=15`,
   `parent x=5`; separate copies.
2. **Scene 7, before the run:** "`fork` then `printf` inside a 3-iteration
   loop — how many `A`s?" — *Answer:* 2 + 4 + 8 = 14 (on a terminal).
3. **Scene 9, before the tree is drawn:** "`if (fork() && fork()) fork();
   printf("x\n");` — how many lines?" — *Answer:* 4 (P, C1, C2, C3).

## 8. Misconceptions to address on screen

- **"`fork` returns once, the child starts at `main`."** → Scene 3: the
  timeline splits *after* the call; both lanes continue from the same point.
- **"n forks give n + 1 processes."** → Scene 6: the doubling tree, labels
  2, 4, 8.
- **"Child and parent share variables."** → Scene 5: two memory boxes, two
  values, same address.
- **"Code after `exec` runs when the new program finishes."** → Scene 10:
  the old image (including `printf("never")`) is wiped.
- **"Output count depends only on the code."** → Scene 8: the same binary
  prints 14 on a terminal and 24 into a file.
- **"A zombie is a process still using CPU/memory."** → Scene 11: `Z
  <defunct>` — only a table entry with an exit status remains.
- **"Orphans are killed."** → Scene 11: `ppid` becomes 1 and the process
  keeps running.

## 9. Visual assets

- **Reuse (from `Slides/CS502/07-process-operations.tex`):**
  - Frame **`fork`: One Call, Two Returns** (return-value table).
  - Frame **Worked Example: The Canonical `fork`/`exec`/`wait`** (code).
  - TikZ in **The Same Program as a Timeline** (parent/child lanes, `wait`
    and `exec` boxes) — scene 4.
  - Frame **Two Copies, Two Variables** — scene 5.
  - TikZ binary tree in **Three `fork` Calls in a Row** (8 leaves, coordinate
    map in source comments) — scene 6, also as the base for scene 7's
    per-level printing.
  - Frames **Worked Example: `printf` Inside the Loop**, **Short-Circuit
    Operators Prune the Tree**, **The Buffer That Gets Copied**,
    **Socratic Check 2**, **Worked Example: Code After `exec`**,
    **Zombies and Orphans**, **Worked Example: The Shell Loop**.
  - TikZ in **The Life Cycle at the Boundary** (created → running →
    zombie → reaped; dashed orphan path) — scene 11.
  - `Notes/CS502/07-process-operations-notes.tex` for the copy-on-write
    explanation.
- **New:**
  - Manim tree-growth animation: nodes split at each `fork`, each node
    labelled with the value it saw; greyed branches for skipped operands.
  - "stdout buffer" chip animation copying unwritten `A\n` chips into the
    child (scene 8).
  - Image-replacement wipe for `exec` with the pid label held constant.
- **Live demo (exact commands; sources in `scratchpad/videos/fork/`):**
  ```
  gcc -o f3 f3.c && ./f3                 # 8 lines
  gcc -o f4 f4.c && ./f4 | wc -l         # 16
  gcc -o loop loop.c && ./loop           # 14 on the terminal
  ./loop > out.txt; wc -l < out.txt      # 24 into a file
  gcc -o x x.c && ./x                    # child x=15 / parent x=5
  gcc -o ex ex.c && ./ex                 # from-exec / parent
  gcc -o zomb zomb.c && ./zomb           # STAT Z <defunct>, then reaped status=7
  gcc -o orph orph.c && ./orph; sleep 3  # ppid before=<pid>, after=1
  ```
  Each counting program ends with `while (wait(NULL) > 0);` so the shell
  prompt does not interleave with children's output. Run `./loop` with the
  terminal cleared so the 14 lines are countable on screen.

## 10. On-screen text & notation

- Processes in tree diagrams: `P` (original) and `C1, C2, …` in creation
  order, as in the deck's tree.
- Calls in monospace: `fork()`, `exec` family (`execlp`, `execvp`),
  `wait(NULL)`, `waitpid(pid, &status, 0)`, `exit(status)`, `getpid()`,
  `getppid()`.
- Formula on screen: `n` sequential forks → `2^n` processes, `2^n − 1`
  children.
- First-use definitions required on screen: parent, child, process tree, `fork`, `exec`, `wait`, `exit`, exit status, zombie, orphan, `init`/pid 1, copy-on-write, line- vs. fully-buffered output, short-circuit evaluation.
- Terms: **parent**, **child**, **process tree**, **zombie**, **orphan**,
  **reaping**, **copy-on-write**, **`init` (pid 1)**.
- Always label the buffering condition when showing a count: "(terminal)"
  or "(redirected)".

## 11. End-of-video quiz

All answers verified by compiling and running the programs (output to a
terminal / line-buffered unless stated).

1. **(Numerical)** `for (i = 0; i < 4; i++) fork(); printf("Q\n");` — how
   many lines? — **Answer: 16.**
2. **(Numerical)** `fork() || fork(); printf("z\n");` — how many lines?
   — **Answer: 3.** (Parent's first `fork` is non-zero, so it skips the
   second; only the first child forks again.)
3. **(Numerical)** `for (i = 0; i < 2; i++) if (fork() == 0) printf("C\n");`
   (children continue the loop) — how many `C` lines? — **Answer: 4.**
4. **(Numerical, GATE-style)** `fork(); fork() && fork(); printf("k\n");` —
   how many lines? — **Answer: 6.**
5. **(MCQ)** A child calls `exit(3)` and its parent is still running but has
   not called `wait`. The child is now
   (a) running (b) an orphan (c) a zombie (d) reaped
   — **Answer: (c).**

## 12. References

- Silberschatz, Galvin & Gagne, *Operating System Concepts*, 8e — Ch. 3
  §3.3 operations on processes: creation, termination, `fork`/`exec`,
  pp. 110–116.
- Stallings, *Operating Systems: Internals and Design Principles*, 6e —
  Ch. 3 §3.4 process control, p. 135.
- Bovet & Cesati, *Understanding the Linux Kernel*, 3e — Ch. 3 creating and
  destroying processes (`fork`, `clone`, `vfork`, `exit`), pp. 79–130; Ch. 20
  program execution, pp. 808–834.
- Bach, *The Design of the UNIX Operating System* — chapter-level only (not
  indexed; no page citation).

## 13. Production notes

- **The hook only works if viewers commit to a guess.** Hold the "your guess?"
  card for 3 full seconds before running.
- **Terminal hygiene:** large font (≥ 22 pt), clear screen before each run,
  and pipe long outputs through `wc -l` so counts are legible.
- **Buffering scene (8) is subtle** — slow down; show the terminal and the
  file run back to back with the same binary name visible.
- **Output order of forked children is nondeterministic**; never claim a
  specific interleaving, only counts.
- **If over length:** drop the second short-circuit example in scene 9 and
  compress scene 12 to 30 s. Keep scenes 6–8 intact.
- **If under length:** add the deck's **Another Design: One-Step Creation**
  contrast (`CreateProcess`/`posix_spawn`) as a 40 s aside after scene 10.
- **Accessibility:** captions; tree nodes distinguish parent/child by label
  and outline style, not colour only; greyed-out branches also dashed.
