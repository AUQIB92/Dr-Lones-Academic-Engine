"""Synthesize original (royalty-free) music bed + SFX for the marketing video."""
import numpy as np, wave, sys
SR = 48000
DUR = 194.8
N = int(SR * DUR)
out = sys.argv[1]
rng = np.random.default_rng(3)
S = dict(s1=0, s2=8.4, s3=22.8, s3k=33.6, s4=49.8, s5=70.8, s6=82.6, s6h=92.2, s7v=107.6, s8=130.0, s8d=146.0, s8x=165.0, s9=174.0, end=194.0)
import json
CAST=[json.loads(l) for l in open(sys.argv[2]).read().strip().splitlines()[1:]]

def mtof(m): return 440 * 2 ** ((m - 69) / 12)
def env_adsr(n, a, d, s, r, sus_len):
    a, d, r = int(a*SR), int(d*SR), int(r*SR); sl = max(0, int(sus_len*SR) - a - d)
    e = np.concatenate([np.linspace(0,1,max(a,1)), np.linspace(1,s,max(d,1)), np.full(sl,s), np.linspace(s,0,max(r,1))])
    return e[:n] if len(e) >= n else np.pad(e,(0,n-len(e)))
def add(buf, start, sig, gain=1.0, pan=0.0):
    i = int(start*SR)
    if i >= N: return
    sig = sig[:N-i]
    l, r = np.cos((pan+1)*np.pi/4), np.sin((pan+1)*np.pi/4)
    buf[0, i:i+len(sig)] += sig*gain*l*1.414
    buf[1, i:i+len(sig)] += sig*gain*r*1.414
def tone(f, dur, harm=6, roll=1.6, det=0.0):
    t = np.arange(int(dur*SR))/SR; s = np.zeros_like(t)
    for h in range(1, harm+1):
        s += np.sin(2*np.pi*f*h*t*(1+det) + h) / h**roll
    return s
def lowpass(x, a):  # one-pole, vectorised via cumulative trick in blocks (fast enough)
    y = np.empty_like(x); acc = 0.0
    for i in range(0, len(x), 1):
        acc += a*(x[i]-acc); y[i] = acc
    return y

music = np.zeros((2, N)); sfx = np.zeros((2, N))
BPM = 100; beat = 60/BPM; bar = 4*beat
# A minor: Am F C G  (two bars each)
prog = [(57,[57,60,64,69]), (53,[53,57,60,65]), (48,[55,60,64,67]), (55,[55,59,62,67])]
t0 = 0.0
chord_len = 2*bar
k = 0
while t0 < S['end']:
    root, ch = prog[k % 4]
    intensity = 1.0
    # pad: detuned, slow attack
    for m in ch:
        for det, pan in ((-0.004,-0.6),(0.004,0.6)):
            sig = tone(mtof(m), chord_len+1.5, harm=7, roll=1.7, det=det)*env_adsr(int((chord_len+1.5)*SR),0.9,0.5,0.8,1.5,chord_len)
            add(music, t0, sig, 0.035, pan)
    # sub bass (from s2 onward), pulsing on beats
    if t0 >= S['s2']-0.1 and t0 < S['s9']:
        for b in range(8):
            bt = t0 + b*beat
            sig = np.sin(2*np.pi*mtof(root-12)*np.arange(int(beat*SR))/SR)*env_adsr(int(beat*SR),0.01,0.2,0.5,0.2,beat*0.6)
            add(music, bt, sig, 0.16)
    # arpeggio plucks (8ths) with echo, from s2 to s9
    if t0 >= S['s2']-0.1 and t0 < S['s9']:
        arp = ch + [ch[1]+12, ch[2]+12, ch[3]+12, ch[2]+12]
        for j in range(16):
            m = arp[j % len(arp)] + 12
            d = 0.5
            sig = tone(mtof(m), d, harm=4, roll=2.2)*np.exp(-np.arange(int(d*SR))/SR*9)
            for e, (dl, g) in enumerate(((0,1),(0.45,0.35),(0.9,0.12))):
                add(music, t0 + j*beat/2 + dl, sig, 0.05*g, 0.5 if (j+e)%2 else -0.5)
    # drums from s2 to s9
    if t0 >= S['s2']-0.1 and t0 < S['s9']:
        for b in range(8):
            bt = t0 + b*beat
            if b % 2 == 0:  # kick
                n = int(0.35*SR); tt = np.arange(n)/SR
                f = 50 + 90*np.exp(-tt*35)
                kick = np.sin(2*np.pi*np.cumsum(f)/SR)*np.exp(-tt*9)
                add(music, bt, kick, 0.38)
            else:  # soft clap/snare
                n = int(0.25*SR); tt = np.arange(n)/SR
                sn = rng.standard_normal(n)*np.exp(-tt*22)
                sn = np.diff(np.concatenate([[0], sn]))  # highpass-ish
                add(music, bt, sn, 0.05)
            for h in (0.5,):  # offbeat hat
                n = int(0.06*SR); hat = np.diff(np.concatenate([[0], rng.standard_normal(n)]))*np.exp(-np.arange(n)/SR*60)
                add(music, bt + h*beat, hat, 0.03, 0.3)
    t0 += chord_len; k += 1

# intro riser into s2
n = int(3.2*SR); tt = np.arange(n)/SR
ris = rng.standard_normal(n); ris = np.diff(np.concatenate([[0], ris]))
ris *= (tt/tt[-1])**2.5
add(music, S['s2']-3.2, ris, 0.06)

# CTA: big impact + swelling final chord
n = int(2.5*SR); tt = np.arange(n)/SR
boom = np.sin(2*np.pi*np.cumsum(38+60*np.exp(-tt*8))/SR)*np.exp(-tt*1.8)
add(music, S['s9']+0.9, boom, 0.55)
cym = np.diff(np.concatenate([[0], rng.standard_normal(n)]))*np.exp(-tt*1.4)
add(music, S['s9']+0.9, cym, 0.05)
# warm final Am(add9) chord with soft piano-like hits
for i, m in enumerate([45, 57, 60, 64, 71, 76]):
    d = S['end'] - S['s9'] - 0.6
    sig = tone(mtof(m), d, harm=5, roll=2.0)*env_adsr(int(d*SR), 0.02, 2.0, 0.35, 4.0, d-4.0)
    add(music, S['s9']+0.9 + i*0.04, sig, 0.05, (i-2.5)/4)

# ---------- SFX ----------
def whoosh(center, dur=0.7, g=0.10):
    n = int(dur*SR); tt = np.arange(n)/SR
    x = rng.standard_normal(n)
    # sweep: blend lowpassed/highpassed noise
    e = np.sin(np.pi*tt/dur)**2
    lp = np.convolve(x, np.ones(18)/18, mode='same')
    hp = x - lp
    mix = lp*(1-tt/dur) + hp*(tt/dur)*0.6
    add(sfx, center-dur*0.6, mix*e, g, -0.4)
    add(sfx, center-dur*0.55, mix[::-1]*e, g*0.6, 0.4)
def ding(t, m, g=0.09, pan=0.0):
    d = 1.0; tt = np.arange(int(d*SR))/SR
    s = (np.sin(2*np.pi*mtof(m)*tt) + 0.35*np.sin(2*np.pi*mtof(m)*2.01*tt) + 0.12*np.sin(2*np.pi*mtof(m)*3.98*tt))*np.exp(-tt*5)
    add(sfx, t, s, g, pan)
def tick(t, g=0.05, f=2400, d=0.02):
    n = int(d*SR); tt = np.arange(n)/SR
    s = (rng.standard_normal(n)*0.6 + np.sin(2*np.pi*f*tt))*np.exp(-tt*300)
    add(sfx, t, s, g, rng.uniform(-.3,.3))
def thump(t, g=0.25):
    n = int(0.3*SR); tt = np.arange(n)/SR
    add(sfx, t, np.sin(2*np.pi*np.cumsum(70+80*np.exp(-tt*40))/SR)*np.exp(-tt*14), g)

for key in ('s2','s3','s3k','s4','s5','s6','s6h','s7v','s8','s8d','s8x','s9'):
    whoosh(S[key])
for i in range(1, 5): thump(S['s2']+0.5+i*0.45+0.5, 0.2); ding(S['s2']+0.5+i*0.45+0.5, [67,71,74,79][i-1], 0.035)
penta = [72, 74, 76, 79, 81, 84]
for i in range(1, 7): ding(S['s2']+6.0+i*0.5+0.15, penta[i-1], 0.06, 0.3)
# skills: pulse along the build-week stages, pops for each single-skill chip
for i in range(6): ding(S['s3k']+1.6+i*0.45+0.2, [72,74,76,79,81,84][i], 0.05)
for i in range(8): tick(S['s3k']+6.6+i*0.38+0.15, 0.08, 1400+i*120, 0.03)
for i in range(3): tick(S['s3k']+11.2+i*0.4+0.15, 0.08, 1100, 0.03)
# handout: a check per item
for i in range(1, 8): tick(S['s6h']+1.5+i*1.55+0.35, 0.1, 1000, 0.04); ding(S['s6h']+1.5+i*1.55+0.35, 79, 0.03)
for i in range(4): thump(S['s3']+0.5+i*0.3+0.45, 0.15)
for i in range(39): tick(S['s3']+2.2+i*0.12+0.1, 0.035, 1800+i*25)
# real terminal: a key click per typed character, enter on newline, chimes on results
T4 = S['s4'] + 1.0
acc = ''
done = set()
for t, _, d in CAST:
    if len(d) <= 2 and d.isprintable():
        tick(T4+t, 0.06, 1500, 0.022)
    elif d.startswith('\r\n') and len(d) <= 4:
        tick(T4+t, 0.1, 900, 0.045)
    acc += d
    for key, notes in (('Output written', (72, 76, 79)), ('Overall Score', (69, 72, 76, 81)), ('LinuxKernel', (79,)), ('PCB (p', (84,)), ('Unit IV', (81,))):
        if key in acc and key not in done:
            done.add(key)
            for j, m in enumerate(notes): ding(T4+t+j*0.06, m, 0.06)
for i in range(1, 7): whoosh(S['s5']+0.4+i*1.62, 0.4, 0.04)
for i in range(1, 5): thump(S['s6']+0.5+i*0.9+0.55, 0.14); ding(S['s6']+0.5+i*0.9+0.55, [76,79,81,84][i-1], 0.04)
# video engine scene: lock clunk on the PROPRIETARY badge, swooshes on montage cuts, chime on Request access
thump(S['s7v']+1.15, 0.22); tick(S['s7v']+1.2, 0.14, 700, 0.05); ding(S['s7v']+1.2, 57, 0.06)
for k in range(1, 6): whoosh(S['s7v']+1.0+2.5*k, 0.35, 0.035)
for m in (79, 84): ding(S['s7v']+9.3+(m-79)*0.01, m, 0.05)
for ct in (6.9, 15.0):  # real mouse clicks in the browser recording
    tick(S['s8']+0.5+ct, 0.16, 1200, 0.03); tick(S['s8']+0.5+ct+0.09, 0.08, 1600, 0.02)
# any-field orbit: core pop, CS & Math chimes, a pop per field, glints as each field is named
thump(S['s8d']+1.0, 0.22); ding(S['s8d']+1.0, 57, 0.05)
ding(S['s8d']+2.3, 76, 0.07); ding(S['s8d']+3.1, 79, 0.07)
for i in range(12): tick(S['s8d']+6.6+i*0.42+0.25, 0.07, 1300+i*90, 0.03); ding(S['s8d']+6.6+i*0.42+0.25, [72,74,76,79,81,84][i%6]+12, 0.018)
for k, tt in enumerate((12.6, 13.4, 14.1, 14.9, 15.7, 16.6)): ding(S['s8d']+tt+0.2, [81,84,86,88,91,93][k], 0.035)
for m in (69, 72, 76, 81, 84): ding(S['s8x']+0.6+(m-69)*0.02, m, 0.045)
for m in (88, 91, 93): ding(S['s8x']+3.0+(m-88)*0.03, m, 0.035)
for base in (5.0, 9.0):
    for m in (88, 91, 93): ding(S['s9']+base+(m-88)*0.03, m, 0.04)

for name, buf in (('music', music), ('sfx', sfx)):
    pk = np.abs(buf).max(); buf = buf / max(pk, 1e-9) * 0.89
    if name == 'music':  # gentle fade out
        f = int(3.0*SR); buf[:, int(S['end']*SR)-f:int(S['end']*SR)] *= np.linspace(1, 0, f); buf[:, int(S['end']*SR):] = 0
        buf[:, :int(1.0*SR)] *= np.linspace(0, 1, int(1.0*SR))
    pcm = (np.clip(buf.T, -1, 1)*32767).astype('<i2')
    with wave.open(f'{out}/{name}.wav', 'wb') as w:
        w.setnchannels(2); w.setsampwidth(2); w.setframerate(SR); w.writeframes(pcm.tobytes())
print('ok')
