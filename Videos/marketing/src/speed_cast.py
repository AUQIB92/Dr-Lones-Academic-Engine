"""Speed up only the typing in a recorded asciicast (output keeps real timing; idle gaps capped)."""
import json, sys
L = open(sys.argv[1]).read().splitlines(); out = []; prev = 0; nt = 0
for t, k, d in (json.loads(l) for l in L[1:]):
    dt = t - prev; prev = t
    nt += dt / 2.8 if (len(d) <= 2 and d.isprintable()) else min(dt, 0.9)
    out.append([round(nt, 4), k, d])
open(sys.argv[2], 'w').write(L[0] + '\n' + '\n'.join(json.dumps(e) for e in out) + '\n')
txt = open(sys.argv[2]).read()
open(sys.argv[3], 'w').write('window.CASTTXT=' + json.dumps(txt) + ';')
