#!/usr/bin/env python3
"""
Build the two JKSSB Computer Awareness books (Detailed Notes + MCQ Bank) from
their single sources of truth:

  Notes  : Notes/JKSSB/NN-*-notes.tex
  MCQs   : CompetitiveExam/JKSSB/NN-*-questions.tex / NN-*-answers.tex
           + the shared CompetitiveExam/JKSSB/_supplemental.tex

Books are DERIVED artifacts (never edited in place). The script extracts each
source's body, orders lessons by the 12-unit syllabus (syllabi/JKSSB.md),
merges the supplemental MCQ items into each lesson's enumerate (continuous
numbering), writes include files under Books/JKSSB/_generated/, compiles each
master three times with XeLaTeX, and copies the PDFs to docs/books/jkssb/.

Four PDFs are produced: for each volume, a cover edition (front cover page) and
a NoCover edition (front matter starts at the title page). Every page carries a
subtle "Concepts That Click" (YouTube) watermark drawn on the background layer.

Lessons 48, 58 and 66 are excluded (revision/audit capstones).

Run:    python scripts/build-jkssb-books.py
"""

import json
import os
import re
import shutil
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
NOTES = os.path.join(ROOT, "Notes", "JKSSB")
CE = os.path.join(ROOT, "CompetitiveExam", "JKSSB")
BOOK = os.path.join(ROOT, "Books", "JKSSB")
GEN = os.path.join(BOOK, "_generated")
DOCS = os.path.join(ROOT, "docs", "books", "jkssb")

EXCLUDE = {48, 58, 66}

# Unit -> lessons, in syllabus order (syllabi/JKSSB.md). Unit 12 (48/58/66)
# is empty once the capstones are excluded and is therefore omitted.
UNITS = [
    ("Fundamentals & Data Representation", [1, 2, 3, 49, 50]),
    ("CPU, Hardware & Architecture", [4, 5, 6, 59]),
    ("Memory, Storage & Backup", [7, 8, 9, 10, 11]),
    ("I/O, Peripherals & Multimedia", [12, 13, 14, 15, 16, 65]),
    ("Software, Operating Systems, Windows & Utilities", [17, 18, 19, 20, 21, 45, 46, 52, 60]),
    ("Programming & Data Structures", [51]),
    ("Office Applications & File Formats", [22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 54, 55, 61]),
    ("Internet, Web & Email", [34, 35, 36, 37, 38, 39, 53, 63]),
    ("Networking", [40, 41, 62]),
    ("Cybersecurity & IT Law", [42, 43, 44, 56, 64]),
    ("Governance & Integration", [47, 57]),
]


def read(path):
    with open(path, "r", encoding="utf-8", errors="replace") as fh:
        return fh.read()


def find_lesson_file(number, folder, suffix):
    pre = "%02d-" % number
    for name in os.listdir(folder):
        if name.startswith(pre) and name.endswith(suffix):
            return os.path.join(folder, name)
    raise FileNotFoundError("no %s*%s in %s" % (pre, suffix, folder))


def lesson_title(number):
    path = find_lesson_file(number, NOTES, "-notes.tex")
    txt = read(path)
    m = re.search(r"\\title\{(.*?)\}", txt, re.S)
    if not m:
        raise ValueError("no \\title in %s" % path)
    title = re.sub(r"\s*---\s*Detailed Lecture Notes\s*$", "", m.group(1)).strip()
    return title


def notes_body(number):
    path = find_lesson_file(number, NOTES, "-notes.tex")
    txt = read(path)
    body = txt.split(r"\begin{document}", 1)[1].rsplit(r"\end{document}", 1)[0]
    body = body.replace(r"\maketitle", "", 1)
    m = re.search(
        r"\\noindent\\textbf\{Video lesson:\}\s*\\href\{([^}]+)\}\{[^}]*\}",
        body,
        re.S,
    )
    url = m.group(1).strip() if m else ""
    body = re.sub(
        r"\\noindent\\textbf\{Video lesson:\}\s*\\href\{[^}]+\}\{[^}]*\}\s*",
        "",
        body,
        count=1,
    )

    # The book is the COMPLETE, unit-ordered edition of the lesson notes: keep
    # the full body verbatim -- prose, section/subsection headings, definition
    # boxes, worked examples, tables, exercises and solutions. The lesson notes
    # remain the single source of truth; nothing is dropped.
    return body.strip(), url


BASE_MARKER = r"\begin{enumerate}[label=\textbf{Q\arabic*.}, leftmargin=*]"


def base_inner(number, suffix):
    path = find_lesson_file(number, CE, suffix)
    txt = read(path)
    body = txt.split(r"\begin{document}", 1)[1]
    i = body.index(r"\begin{enumerate}") + len(r"\begin{enumerate}")
    j = i
    while j < len(body) and body[j] in " \t\r\n":
        j += 1
    if j < len(body) and body[j] == "[":
        depth = 0
        while j < len(body):
            if body[j] == "[":
                depth += 1
            elif body[j] == "]":
                depth -= 1
                if depth == 0:
                    break
            j += 1
        j += 1
    i = j
    end = body.index(r"\def\JKSSBLessonID")
    seg = body[i:end]
    k = seg.rindex(r"\end{enumerate}")
    return seg[:k].strip()


def read_braces(s, i, count):
    args = []
    n = len(s)
    for _ in range(count):
        while i < n and s[i] in " \t\r\n":
            i += 1
        if i >= n or s[i] != "{":
            raise ValueError("expected '{' at offset %d" % i)
        depth = 0
        start = i
        i += 1
        while i < n:
            c = s[i]
            if c == "\\":
                i += 2
                continue
            if c == "{":
                depth += 1
            elif c == "}":
                if depth == 0:
                    break
                depth -= 1
            i += 1
        args.append(s[start + 1:i])
        i += 1
    return args, i


def parse_supplemental():
    txt = read(os.path.join(CE, "_supplemental.tex"))
    out = {}
    for m in re.finditer(r"\\ifnum\\JKSSBLessonID=(\d+)(.*?)\\fi", txt, re.S):
        n = int(m.group(1))
        block = m.group(2)
        if r"\JKSSBItem" not in block:
            continue
        items = []
        pos = 0
        needle = r"\JKSSBItem"
        while True:
            idx = block.find(needle, pos)
            if idx < 0:
                break
            args, endp = read_braces(block, idx + len(needle), 5)
            items.append(args)
            pos = endp
        out[n] = items
    return out


def q_item(args):
    label, question, options, _key, _rat = args
    return (
        "\\item \\textit{%s} %s\n"
        "  \\begin{enumerate}[label=\\Alph*.]\n%s\n  \\end{enumerate}\n"
        % (label, question.strip(), options.strip())
    )


def a_item(args):
    _label, _q, _opts, key, rat = args
    return "\\item \\textbf{%s.} %s\n" % (key.strip(), rat.strip())


def tex_escape(s):
    return s.replace("&", r"\&")


def build_notes():
    lines = []
    for ui, (utitle, lessons) in enumerate(UNITS, start=1):
        utitle = tex_escape(utitle)
        lines.append("\\unitopener{%d}{%s}" % (ui, utitle))
        for n in lessons:
            title = lesson_title(n)
            body, url = notes_body(n)
            lines.append("\\lessonheading{%02d}{%s}" % (n, title))
            if url:
                lines.append("\\videoline{%s}" % url)
            lines.append(body)
            lines.append("")
    return "\n".join(lines)


def build_mcq(supp):
    lines = []
    missing = []
    for ui, (utitle, lessons) in enumerate(UNITS, start=1):
        utitle = tex_escape(utitle)
        lines.append("\\unitopener{%d}{%s}" % (ui, utitle))
        for n in lessons:
            title = lesson_title(n)
            lines.append("\\lessonheading{%02d}{%s}" % (n, title))
            lines.append(BASE_MARKER)
            lines.append(base_inner(n, "-questions.tex"))
            if n in supp:
                lines.append("\n".join(q_item(a) for a in supp[n]))
            else:
                missing.append(n)
            lines.append(r"\end{enumerate}")
        lines.append("\\answersection{%d}" % ui)
        for n in lessons:
            title = lesson_title(n)
            lines.append("\\answersubheading{%02d}{%s}" % (n, title))
            lines.append(BASE_MARKER)
            lines.append(base_inner(n, "-answers.tex"))
            if n in supp:
                lines.append("\n".join(a_item(a) for a in supp[n]))
            lines.append(r"\end{enumerate}")
    return "\n".join(lines), missing


def compile_book(master):
    env = dict(os.environ)
    env["TEXINPUTS"] = os.path.join(ROOT, "Preambles") + os.pathsep + env.get("TEXINPUTS", "")
    cmd = ["xelatex", "-interaction=nonstopmode", "-halt-on-error", master]
    log = os.path.join(BOOK, master[:-4] + ".log")
    if os.path.exists(log):
        os.remove(log)
    for run in range(1, 4):
        res = subprocess.run(cmd, cwd=BOOK, env=env, capture_output=True,
                             text=True, encoding="utf-8", errors="replace")
        if res.returncode not in (0, 1):
            print("  xelatex run %d failed (rc=%d)" % (run, res.returncode))
            print("\n".join(res.stdout.splitlines()[-40:]))
            return False
    if os.path.exists(log):
        logtxt = read(log)
        errs = [ln for ln in logtxt.splitlines() if ln.startswith("!")]
        if errs or "Emergency stop" in logtxt:
            print("  LaTeX errors in %s:" % master)
            for e in errs[:12]:
                print("    " + e)
            return False
    return True


def build_variant(master, key, outname, with_cover):
    """Compile `master` with or without its cover page and publish `outname`."""
    if with_cover:
        workmaster = master
    else:
        txt = read(os.path.join(BOOK, master))
        txt = txt.replace(r"\input{book-style}",
                          r"\def\BOOKNOCOVER{}\input{book-style}", 1)
        workmaster = "%s-nocover.tex" % key
        with open(os.path.join(BOOK, workmaster), "w", encoding="utf-8") as fh:
            fh.write(txt)
    tag = "with cover" if with_cover else "no cover"
    print("compiling %s (%s) ..." % (workmaster, tag))
    produced = os.path.join(BOOK, workmaster[:-4] + ".pdf")
    if os.path.exists(produced):
        os.remove(produced)
    if not compile_book(workmaster):
        sys.exit("FAILED compiling %s" % workmaster)
    if not os.path.exists(produced):
        sys.exit("no PDF produced for %s" % workmaster)
    shutil.move(produced, os.path.join(BOOK, outname))
    shutil.copy2(os.path.join(BOOK, outname), os.path.join(DOCS, outname))
    print("built + copied %s" % outname)
    if not with_cover:
        for ext in (".tex", ".aux", ".log", ".out", ".toc"):
            p = os.path.join(BOOK, workmaster[:-4] + ext)
            if os.path.exists(p):
                os.remove(p)


def publish_covers():
    """Copy web-optimized cover images to docs/books/jkssb/covers/ for the hub
    download menu. Uses Pillow (JPEG, ~150 KB) when available; otherwise copies
    the PNG bytes under a .jpg name (browsers sniff the actual image type)."""
    dst_dir = os.path.join(DOCS, "covers")
    os.makedirs(dst_dir, exist_ok=True)
    try:
        from PIL import Image
    except ImportError:
        Image = None
    for src in ("notes-cover.png", "mcq-cover.png"):
        srcpath = os.path.join(BOOK, "covers", src)
        if not os.path.exists(srcpath):
            print("WARNING: missing cover %s" % srcpath)
            continue
        out = src[:-4] + ".jpg"
        dstpath = os.path.join(dst_dir, out)
        if Image is not None:
            im = Image.open(srcpath).convert("RGB")
            width = 760
            height = round(im.height * width / im.width)
            im.resize((width, height), Image.LANCZOS).save(
                dstpath, "JPEG", quality=82, optimize=True, progressive=True)
        else:
            shutil.copy2(srcpath, dstpath)
        print("published cover %s (%.0f KB)" % (
            out, os.path.getsize(dstpath) / 1024.0))


def main():
    os.makedirs(GEN, exist_ok=True)
    os.makedirs(DOCS, exist_ok=True)

    supp = parse_supplemental()
    print("supplemental lessons parsed: %d" % len(supp))

    notes = build_notes()
    with open(os.path.join(GEN, "notes-content.tex"), "w", encoding="utf-8") as fh:
        fh.write(notes)
    mcq, missing = build_mcq(supp)
    with open(os.path.join(GEN, "mcq-content.tex"), "w", encoding="utf-8") as fh:
        fh.write(mcq)
    if missing:
        print("WARNING: no supplemental found for lessons %s" % missing)

    manifest = {
        "units": len(UNITS),
        "lessons": sum(len(l) for _, l in UNITS),
        "excluded": sorted(EXCLUDE),
        "supplemental_lessons": sorted(supp.keys()),
        "missing_supplemental": missing,
    }
    with open(os.path.join(GEN, "manifest.json"), "w", encoding="utf-8") as fh:
        json.dump(manifest, fh, indent=2)
    print("manifest: %s" % manifest)

    books = [("notes-book.tex", "notes", "JKSSB-Computer-Awareness-Detailed-Notes"),
             ("mcq-book.tex", "mcq", "JKSSB-Computer-Awareness-MCQ-Bank")]
    for master, key, stem in books:
        build_variant(master, key, stem + ".pdf", with_cover=True)
        build_variant(master, key, stem + "-NoCover.pdf", with_cover=False)

    publish_covers()


if __name__ == "__main__":
    main()
