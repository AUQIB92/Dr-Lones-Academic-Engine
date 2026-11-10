#!/usr/bin/env bash
# Rebuild the Academic Engine promo (≈3:14, 1080p30 + 9:16 cut).
#   ./Videos/marketing/build.sh
# Voice: your cloned "Dr.Lone" voice via Prompt2Render's VoiceStudio backend
# (start the VoiceStudio app on 127.0.0.1:3900 first). Override with
#   P2R_HOME=/path/to/Prompt2Render  P2R_VOICESTUDIO_PROFILE=<name>
# Vetted takes live in my-voice/l1..l9.wav (gitignored); delete one to re-generate it.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"; M="$ROOT/Videos/marketing"; W="${WORK:-$M/.work}"
P2R_HOME="${P2R_HOME:-$HOME/Prompt2Render}"
mkdir -p "$W"/{assets/raw,build/img/br,build/xterm,aud,voice,node,mirror/Slides} "$M/out"
[ -x "$W/venv/bin/python" ] || { python3 -m venv "$W/venv"; "$W/venv/bin/pip" -q install pymupdf pillow numpy; }
[ -d "$W/node/node_modules/@xterm/xterm" ] || (cd "$W/node" && npm init -y >/dev/null && npm i -s puppeteer-core@23.11.1 @xterm/xterm@5.5.0)
cp "$M"/src/*.js "$W/node/"
# 1. lecture artifacts (real PDFs) -> images
(cd "$ROOT" && "$W/venv/bin/python" -I "$M/src/raster.py" "$W/assets/raw")
for f in slides_01 slides_04 slides_15 slides_20 slides_23 slides_32 slides_35 notes_01 notes_02 assign_01 gate_01 handout_01; do cp "$W/assets/raw/$f.png" "$W/build/img/"; done
# 2. REAL terminal session, recorded in a mirror (symlinks + a scratch copy of the slides, so the repo is untouched)
cd "$W/mirror"; for d in master_supporting_docs syllabi scripts Assignments CompetitiveExam InstructorHandouts Notes Preambles Bibliography_base.bib; do ln -sfn "$ROOT/$d" "$d"; done
rm -rf Slides/CS502 && cp -R "$ROOT/Slides/CS502" Slides/CS502
python3 "$M/src/rec_term.py" "$W/term.raw.cast" "$W/mirror"
python3 "$M/src/speed_cast.py" "$W/term.raw.cast" "$W/build/term.cast" "$W/build/term.cast.js"
cp "$W/node/node_modules/@xterm/xterm/lib/xterm.js" "$W/node/node_modules/@xterm/xterm/css/xterm.css" "$W/build/xterm/"
# 3. REAL browser session on the live GitHub Pages site
(cd "$W/node" && node brec.js "$W/assets")
ffmpeg -y -loglevel error -i "$W/assets/browser.webm" -vf "fps=30,scale=1500:844:flags=lanczos" -q:v 2 "$W/build/img/br/%04d.jpg"
BRN=$(ls "$W/build/img/br" | wc -l | tr -d ' ')
sed "s/get('brn')||457/get('brn')||$BRN/" "$M/src/video.html" > "$W/build/video.html"
# 3b. Video Engine scene: excerpts of the PROPRIETARY Prompt2Render explainer for the same lecture
EXPL="${EXPLAINER:-$P2R_HOME/projects/CS502/05 From Program to Process/out/final.mp4}"
mkdir -p "$W/build/img/ex"; rm -f "$W/build/img/ex/"*.jpg
ffmpeg -y -loglevel error -i "$EXPL" -filter_complex "[0:v]trim=99.5:102,setpts=PTS-STARTPTS[a];[0:v]trim=199:201.5,setpts=PTS-STARTPTS[b];[0:v]trim=348:350.5,setpts=PTS-STARTPTS[c];[0:v]trim=497:499.5,setpts=PTS-STARTPTS[d];[0:v]trim=597:599.5,setpts=PTS-STARTPTS[e];[0:v]trim=645.2:653.0,setpts=PTS-STARTPTS[f];[a][b][c][d][e][f]concat=n=6:v=1:a=0,fps=30,scale=1300:731:flags=lanczos[o]" -map "[o]" -q:v 2 "$W/build/img/ex/%04d.jpg"
ffmpeg -y -loglevel error -ss 645.2 -to 653.0 -i "$EXPL" -vn -ac 2 -ar 48000 "$W/aud/explainer.wav"
ffmpeg -y -loglevel error -i "$W/aud/explainer.wav" -af "adelay=121100|121100,apad=whole_dur=194.8,atrim=0:194.8" -ar 48000 "$W/aud/expl.wav"
# 4. picture (headless Chrome, frame-exact, 1080p30)
(cd "$W/node" && node cap.js "$W/build/video.html" video "$W/visual.mp4")
# 5. music + SFX (synthesised; key clicks follow the real keystrokes)
"$W/venv/bin/python" -I "$M/src/audio.py" "$W/aud" "$W/build/term.cast"
# 6. voice: cloned voice via VoiceStudio (Prompt2Render), trimmed
(cd "$P2R_HOME" && PYTHONPATH="$P2R_HOME" P2R_VOICESTUDIO_PROFILE="${P2R_VOICESTUDIO_PROFILE:-Dr.Lone}" .venv/bin/python "$M/src/vs_gen.py" "$M/src/voiceover-lines.txt" "$M/my-voice")
for i in $(seq 1 14); do ffmpeg -y -loglevel error -i "$M/my-voice/l$i.wav" -af "silenceremove=start_periods=1:start_threshold=-45dB,areverse,silenceremove=start_periods=1:start_threshold=-45dB,areverse" -ar 48000 "$W/voice/t$i.wav"; cp "$W/voice/t$i.wav" "$W/aud/l$i.wav"; done
python3 "$M/src/mixvo.py" "$W/aud" "$W/aud" wav
# 7. mix + master (-14 LUFS) + mux, plus a 9:16 cut
ffmpeg -y -loglevel error -i "$W/aud/vo.wav" -i "$W/aud/music.wav" -i "$W/aud/sfx.wav" -i "$W/aud/expl.wav" -filter_complex "[0:a]highpass=f=80,acompressor=threshold=-20dB:ratio=3:attack=5:release=120,equalizer=f=3500:t=q:w=1:g=1.5,volume=1.6[v0];[3:a]highpass=f=80,acompressor=threshold=-20dB:ratio=3,volume=1.5[ex];[v0][ex]amix=inputs=2:normalize=0,asplit=2[vo][sc];[1:a]aecho=0.8:0.6:120|240:0.25|0.15,volume=0.55[m];[m][sc]sidechaincompress=threshold=0.03:ratio=6:attack=40:release=500[md];[2:a]volume=0.75[fx];[vo][md][fx]amix=inputs=3:normalize=0,alimiter=limit=0.95,loudnorm=I=-14:TP=-1.5:LRA=11[o]" -map "[o]" -ar 48000 "$W/aud/mix.wav"
ffmpeg -y -loglevel error -i "$W/visual.mp4" -i "$W/aud/mix.wav" -map 0:v -map 1:a -c:v copy -c:a aac -b:a 256k -shortest -movflags +faststart "$M/out/academic-engine-promo.mp4"
ffmpeg -y -loglevel error -i "$M/out/academic-engine-promo.mp4" -filter_complex "[0:v]scale=1080:1920:force_original_aspect_ratio=increase,crop=1080:1920,boxblur=40:2,eq=brightness=-0.12[bg];[0:v]scale=1080:-2[fg];[bg][fg]overlay=0:(H-h)/2" -c:v libx264 -crf 18 -c:a copy -movflags +faststart "$M/out/academic-engine-promo-vertical.mp4"
echo "✓ $M/out/academic-engine-promo.mp4"
