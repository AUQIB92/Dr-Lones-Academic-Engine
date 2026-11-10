"""Record a REAL shell session (pty) as an asciicast v2 file: commands are typed into bash, output captured with timing."""
import os, pty, json, time, select, sys, random
out, cwd = sys.argv[1], sys.argv[2]
CMDS = [
 "ls master_supporting_docs/CS502/supporting_books/",
 "grep -oE 'process concept \\(p.101\\)|PCB\\*\\* \\(p.103\\)|context switch\\*\\* \\(p.110\\)' master_supporting_docs/CS502/supporting_books/SilberschatzOSC/index.md | tr -d '*'",
 "grep '^### Unit' syllabi/CS502.md",
 "cd Slides/CS502 && TEXINPUTS=../../Preambles: xelatex -interaction=nonstopmode 05-process-concept-pcb.tex; cd ../..",
 "python3 scripts/quality_score.py Slides/CS502/05-process-concept-pcb.tex | grep -E 'Overall|Critical'",
 "ls */CS502/05-process-concept-pcb*.pdf",
]
pid, fd = pty.fork()
if pid == 0:
    os.chdir(cwd)
    os.execvpe("bash", ["bash", "--noprofile", "--norc", "-i"], {**os.environ, "PS1": "\\[\\e[38;5;179m\\]❯\\[\\e[0m\\] ", "TERM": "xterm-256color", "COLUMNS": "120", "LINES": "26", "BASH_SILENCE_DEPRECATION_WARNING": "1"})
import fcntl, termios, struct
fcntl.ioctl(fd, termios.TIOCSWINSZ, struct.pack("HHHH", 26, 120, 0, 0))
t0 = time.time(); ev = []
def pump(dur):
    end = time.time() + dur
    while time.time() < end:
        r, _, _ = select.select([fd], [], [], 0.02)
        if r:
            try: d = os.read(fd, 65536)
            except OSError: return
            ev.append([round(time.time()-t0, 4), "o", d.decode("utf-8", "replace")])
def wait_prompt(maxs=120):
    end = time.time() + maxs
    while time.time() < end:
        pump(0.1)
        tail = "".join(e[2] for e in ev[-3:])
        if tail.rstrip().endswith("❯") or tail.endswith("❯\x1b[0m "): return
pump(1.0); ev.clear(); t0 = time.time(); os.write(fd, b"clear\n"); pump(0.6); ev.clear(); t0=time.time()
os.write(fd, b"\n"); pump(0.5)
for c in CMDS:
    pump(0.7)
    for ch in c:
        os.write(fd, ch.encode()); pump(random.uniform(0.025, 0.06))
    pump(0.35); os.write(fd, b"\n"); wait_prompt()
pump(1.5)
with open(out, "w") as f:
    f.write(json.dumps({"version": 2, "width": 120, "height": 26}) + "\n")
    for e in ev: f.write(json.dumps(e) + "\n")
print("events", len(ev), "dur", ev[-1][0])
