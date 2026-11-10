# Promo video — "Your textbooks in. A whole lecture out." (3:14, 1920×1080 + 9:16)

Narrated in the cloned **Dr.Lone** voice through Prompt2Render's VoiceStudio backend
(`prompt2render/voicestudio_tts.py`, local app on 127.0.0.1:3900). Every take was transcribed back
through VoiceStudio's `/transcribe` and re-generated where a word came out wrong. The vetted takes are in
`my-voice/l1..l14.wav` (gitignored). Rebuild everything with `./Videos/marketing/build.sh`.

| # | Starts | Scene | What's on screen | Line |
|---|---|---|---|---|
| 1 | 0:00.6 | Hook | "Your textbooks in. A whole lecture out." | What if one command could turn the textbooks you trust into a complete lecture… |
| 2 | 0:09.0 | **Inputs** | 4 source types: textbooks, your slides, technical papers, supporting material (+ their `master_supporting_docs/` folders); real book-index page numbers | It starts with your sources. Textbooks, your existing slides, technical papers, and any supporting material… |
| 3 | 0:23.3 | Curriculum | CS-502, 4 units, 39 lectures | From those books and your syllabus, it drafts the curriculum… |
| 4 | 0:34.2 | **Skills** | `/build-week CS502/05` → Slides → Notes → Assignment → Lab → GATE practice → Course hub; single-part skills `/create-lecture` `/lecture-notes` `/create-assignment` `/lab-manual` `/coding-assignment` `/competitive-exam-questions` `/instructor-handout` `/publish-course-hub`; input skills `/syllabus` `/index-textbook` `/teach-from-paper` | Then you choose. One skill, called build week, creates the entire lecture… or call a single skill for just the part you need… |
| 5 | 0:50.4 | **Real terminal** | Real shell session: book index → syllabus → XeLaTeX compile (36 pages) | Here is the engine at work, for real… |
| 6 | 1:01.8 | **Real terminal** | Real `quality_score.py` → 100/100, then the 7 real PDFs | Nothing ships until it compiles cleanly and clears the quality gate… |
| 7 | 1:11.3 | Deck | 7 of the 36 real Beamer slides | Out comes a Beamer slide deck… |
| 8 | 1:23.2 | Package | Real notes / assignment / GATE set / handout pages | Plus lecture notes, a graded assignment… |
| 9 | 1:32.8 | **Instructor handout** | Real handout page + its "Before you teach" checklist: Silberschatz §3.1 pp.101–105 · Stallings §3.1–3.3 pp.108/111/126 · Bovet & Cesati Ch.3 · counter.c demo + `ps` · state diagram from memory · prerequisites L2/L3 · exit ticket | And the instructor handout gets you ready to teach. Exactly which pages to read… |
| 10 | 1:48.2 | **Video Engine — PROPRIETARY** | Real excerpts of the Prompt2Render explainer for Lecture 05 (13 min), 🔒 Proprietary badge, "Request access", 7.8 s of its own narration | And for video, there is my Video Engine… The engine is proprietary, so reach out for access. |
| 11 | 2:10.8 | **Real screen recording** | Live GitHub Pages: home → courses → CS-502 → Lecture 05 → Slides | Then it goes live, on your own course website… |
| 12 | 2:26.6 | **Any field** | "Made for CS & Math. Ready for any field." Engine core with Computer Science + Mathematics in gold; 12 fields wire in (Physics, Chemistry, Biology, Electrical/Mechanical/Civil Eng., Statistics, Economics, Medicine, Business, Law, Humanities), each glowing as it is named; "Swap the books, syllabus & knowledge base, and the same skills build your course." | Today it is tailored for computer science and mathematics. But the best part: swap in your own books, syllabus, and knowledge base, and it adapts to any field… |
| 13 | 2:45.6 | **Shout-out** | "Standing on the shoulders of **Pedro H. C. Sant'Anna**", original author of the open-source `claude-code-my-workflow` template this engine is forked from; github.com/pedrohcgs/claude-code-my-workflow · MIT License; peer-review pipeline adapted from **clo-author** by Hugo Sant'Anna (with permission) | A big thank you to Pedro Sant'Anna. This engine is built on his open source Claude Code workflow template. |
| 14 | 2:55.6 | CTA | Open source; **CODE** github.com/AUQIB92/Dr-Lones-Academic-Engine; **SEE WHAT IT HAS BUILT** auqib92.github.io/Dr-Lones-Academic-Engine/courses/cs502; red 🔒 line: Video Engine is proprietary · request access via the site's #contact | I built this for my own students, and now it is open source. See everything it has produced so far on my course page, and get the code on GitHub… |

**Honesty notes.** The terminal recording is a real bash session (pty) against this repo; only the
*typing* is sped up 2.8×, command output keeps its real timing, and it says so on screen. The compile
runs in a scratch mirror so the repo is not modified. The browser segment is a real Chrome recording of
the live site. It does not show `/build-week` itself running (that pipeline pauses for approval between
stages and takes much longer than a promo).

**Open vs proprietary.** The Academic Engine (this repo: slides, notes, assignments, GATE sets, course
site) is open source. The **Video Engine (Prompt2Render) is proprietary**: the video shows its real
output but says so twice on screen (🔒 badge + "Request access" in the scene, red line on the end card)
and once in the narration. Access requests go to the site's contact section.

**Sound.** Music and SFX are synthesised by `src/audio.py` (royalty-free): key clicks land on each real
keystroke in the terminal recording, chimes on real results (compile done, score 100/100), mouse clicks on
the real clicks in the browser recording, whooshes on scene cuts, music ducked under the voice, mastered
to −14 LUFS.

## Suggested post copy

**Title:** I built an open-source engine that turns textbooks into full university lectures
**Caption:** Textbooks in → slides, notes, assignments, GATE practice and a live course website out. Every claim cited to the page. Built for my students at GCET Kashmir, now free for every teacher. Fork it: https://github.com/AUQIB92/Dr-Lones-Academic-Engine
**Tags:** #HigherEd #AcademicTwitter #EdTech #OpenSource #ClaudeCode #Teaching #GATE #LaTeX
