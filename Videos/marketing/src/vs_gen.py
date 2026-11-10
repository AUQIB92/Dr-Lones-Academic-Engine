import sys, pathlib
from prompt2render import voicestudio_tts as vs
lines = pathlib.Path(sys.argv[1]).read_text().strip().splitlines()
out = pathlib.Path(sys.argv[2])
vs.require_available()
prof = vs.resolve_profile(vs.default_profile())
ref = vs.reference(prof)
for i, t in enumerate(lines, 1):
    p = out / f"l{i}.wav"
    if p.exists(): continue
    p.write_bytes(vs._generate(t, ref, "en", 1.0)); print("done", i, flush=True)
