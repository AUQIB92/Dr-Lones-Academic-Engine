---
name: revenue-engine
description: Turn the whole repo's teaching output (slides, notes, GATE/competitive-exam banks, labs, instructor handouts, accreditation kits, explainer videos, the workflow template itself) into a ranked catalogue of sellable products with packaging, pricing tiers, channel plan, an exam-season launch calendar, and a measured feedback loop. Inventories what exists, classifies every asset as SELLABLE / LEAD-MAGNET / INTERNAL-ONLY under hard licensing and integrity guardrails, finds the gaps worth filling, and emits ready-to-launch kits by delegating builds to the existing skills. Use when user says "monetize the repo", "make money from my courses", "revenue plan", "productize my lectures", "what can I sell", "launch a GATE course", "build a product catalogue", "revenue engine", or `--review` after a launch. NOT for producing a video itself (that is an optional private add-on; this skill only plans which lectures get videos) and NEVER a channel for selling graded solutions, live exam papers, or copyrighted textbook content.
argument-hint: "[--audit|--plan|--launch <product-id>|--review] [--market gate|ugc-net|jkssb|university|colleges|all] [--budget-hours N]"
allowed-tools: ["Read", "Write", "Edit", "Grep", "Glob", "Bash", "Task", "Skill", "WebSearch"]
effort: high
disable-model-invocation: true
---

# Revenue Engine — the whole repo as a product company

Every skill in this repo produces a *teaching artifact*. This skill treats the library of artifacts as **inventory**, and turns it into products, channels and a launch calendar. It does not re-implement content creation: it finds what exists, decides what each thing is worth and to whom, and delegates the missing builds to the skills that own them.

**Honesty contract.** The skill maximizes *inputs* to revenue: product–market fit, packaging, reach, conversion and retention. It never states a revenue or sales figure as fact. Market sizes and prices come from cited searches or are marked `ASSUMED`. Analytics come only from exports the instructor provides.

## Hard guardrails (checked before anything is listed for sale)

| Never sell or publish | Why | Source of truth |
|---|---|---|
| `Assignments/**/*-solutions.*`, grading keys, rubrics with answers | Academic integrity, and it breaks your own course | `/create-assignment` |
| Minor / sessional papers and their solutions | A live institutional exam, gitignored end-to-end | `/create-minor-paper` |
| `Labs/**/*-reference/` | Instructor-only | `/lab-manual` |
| `master_supporting_docs/**/book.pdf`, scanned textbook pages, figures copied from books | Copyright | `.gitignore`, `index-textbook` |
| Verbatim third-party slides or papers in `supporting_slides/`, `supporting_papers/` | Copyright | — |
| Student submissions, grades, attainment data with names | Privacy | `/grade`, `/accreditation` |
| Anything conflicting with your employment contract or institution IP policy | Your call, not the skill's | Ask once in Phase 0 |

**PYQs:** official past papers (e.g. GATE, published by the organizing IIT) may be *referenced and solved*. Sell **your solutions, explanations and original questions**, not re-typeset official papers presented as your own. Every "real PYQ" label must already be verified by `/competitive-exam-questions`.

**Quality gate:** no product ships with any constituent below **90/100** on `scripts/quality_score.py` where it applies (stricter than the 80 commit gate: paying customers find errors, and refunds and reviews cost more than polish).

## Phase 0: Pre-flight (RUN_CONFIG)

Resolve everything now, then echo a Pre-Flight Report. Forked reviewers can't ask questions mid-run.

| Field | Default |
|---|---|
| Mode | `--audit` (inventory + valuation only), `--plan` (default: audit + catalogue + calendar), `--launch <id>`, `--review` |
| Markets | `--market`, else `all`. Built-in segments: **GATE CS aspirants**, **UGC-NET CS**, **state exams (JKSSB etc., see `Books/JKSSB`, `Notes/JKSSB`)**, **university students** (PCC course codes), **colleges/faculty** (B2B: instructor kits, labs, accreditation), **educator-developers** (the workflow template itself) |
| Business profile | `revenue/profile.md`. If missing, ask 5 questions **now**: brand/channel name, existing audience (YouTube subs, Telegram, email list), payment platforms available (e.g. Gumroad, Razorpay, Graphy/Teachable, YouTube memberships), weekly hours available (`--budget-hours`), and the institutional-IP constraint. |
| Exam calendar | Search the official dates for the target exams' next cycle (`WebSearch`) and record them with source links. Unknown → `ASSUMED`, flagged. |

## Phase 1: Inventory (mechanical, Haiku tier)

Glob every course and every output family, and build `revenue/inventory.csv` with one row per asset: `course, lecture, family, path, status(built|planned), quality_score, compiled_pdf(y/n), video(y/n), classification`.

Families: `Slides`, `Notes`, `CompetitiveExam` (questions/answers), `Assignments` (student sets only), `Labs` (student manuals only), `InstructorHandouts`, `Accreditation`, `Videos` (briefs and published links from `syllabi/*.videos.yaml`), `Books/*`, `Figures`, and the repo template (`.claude/`, `guide/`).

Classify each row:
- **SELLABLE**: original, quality ≥ 90, not guardrailed.
- **LEAD-MAGNET**: original and good, but worth more as a free hook (lecture 01 of each course, one GATE set per subject, the notes for the most-searched topic).
- **INTERNAL-ONLY**: anything in the guardrail table.
- **NEEDS-WORK**: sellable in principle, but quality < 90 or uncompiled. Route it to the owning skill (`/slide-excellence`, `/qa-notes`, `/proofread`, `/compile-latex`).

Run `./scripts/check-surface-sync.sh` and the quality scorer. Report failures as `NEEDS-WORK`; don't hide them.

## Phase 2: Demand and competition (fan-out, parallel, forked)

Spawn in **one message** one research fork per target market (`general-purpose`, WebSearch on). Each returns, with **cited URLs**:
- Top queries and pain points (for example "COA GATE numericals", "OS scheduling PYQ solutions"). Use `/yt-viral` and `/last30days` on the subject + exam.
- 3–5 competitor products: format, price, review complaints. Their gaps are your angle.
- Price bands people actually pay, in local currency.
- Seasonality: when buying intent peaks relative to the exam date.

Unsourced numbers are tagged `ASSUMED` and can't drive a top-3 recommendation alone.

## Phase 3: Product catalogue (judgment, Opus tier)

Combine inventory and demand into `revenue/catalogue.md`. The **product ladder** (free → low → core → premium → B2B) is the default shape:

| Tier | Product archetypes built from this repo | Built from |
|---|---|---|
| Free (reach) | YouTube explainers + Shorts, 1 GATE set per subject, lecture-01 notes, Telegram/WhatsApp daily-PYQ drip | your video pipeline (optional), `CompetitiveExam/`, `Notes/` |
| Low ticket | Subject **GATE PYQ + solutions workbook** (PDF/print), topic "numericals packs", formula/cheat-sheet books | `CompetitiveExam/<CODE>/*-answers`, `/competitive-exam-questions` |
| Core | **Full subject course**: video playlist + notes + graded practice + mock tests, sold per subject or as a GATE-CS bundle | Slides, Notes, Videos, Assignments (student sets), `/coding-assignment` |
| Premium | Live cohort / doubt-clearing, mentorship, test series with rankings | Your time; cap it at `--budget-hours` |
| B2B (colleges) | **Faculty course kits** (slides + notes + labs + handouts + assignment banks, editable), **NBA/AICTE accreditation pack/service**, lab manuals licensed per institution | `InstructorHandouts/`, `Labs/`, `/accreditation-autopilot` |
| B2B (educators) | This **Claude Code academic workflow** as a paid setup, workshop or template support | `.claude/`, `guide/`, README |
| Print / publishing | Workbooks via print-on-demand (Amazon KDP, Notion Press, Pothi) | Notes + CompetitiveExam |

For every candidate product, score these and show the arithmetic:

```
Score = Demand(1-5) × Willingness-to-pay(1-5) × Asset-readiness(1-5) × Differentiation(1-5)
        ÷ Build-effort-hours-bucket(1-5)
```

- Asset-readiness = share of constituents already SELLABLE.
- Differentiation = what competitors lack (animated + verified + exam-mapped is the repo's default edge).
- Rank the products, then **cut to a top 3** that fit `--budget-hours`. A long list is how solo creators stall.

Each product card shows: target buyer, promise (one sentence, an outcome, not "comprehensive notes"), contents with source paths, missing pieces with the owning skill, format, price (anchor/standard/launch), channel, the lead magnet that feeds it, and its guardrail check.

### Pricing rules
- Three price points per ladder rung (decoy/anchor). Regional pricing for India, with purchasing-power parity for international buyers.
- Bundle > à-la-carte: a subject bundle priced at about 60–70% of the sum of its parts.
- Launch discount with a real deadline tied to the exam calendar. No fake scarcity.
- B2B is priced per institution per year, never per student.

## Phase 4: Funnel and channel plan

`revenue/funnel.md`: one funnel per top-3 product.

```
Search/Shorts/YouTube  →  free lead magnet (email/Telegram opt-in)  →  low-ticket workbook
      →  core course (upsell on thank-you page)  →  cohort / test series
Colleges: LinkedIn + faculty network + accreditation-season outreach → demo kit → annual licence
```

- Every free asset has exactly one CTA. Use UTM links `utm_source/medium/campaign=<product-id>` everywhere.
- Videos come from whatever pipeline you use (an optional private add-on). This skill only assigns which lectures get videos first: highest demand × readiness. Package them with `/yt-package`, `/yt-seo`, `/yt-chapters`, `/yt-shorts`.
- Owned audience over rented audience: every channel funnels to an email/Telegram list you control.
- Watermark/brand all PDFs (name + URL in the footer through `Preambles/header.tex`'s `\coursecode` footer pattern). Per-buyer watermarking is a platform setting. Recommend it, don't build DRM.

## Phase 5: Launch calendar

`revenue/calendar.md`: a week-by-week plan, counted backwards from each exam date found in Phase 0:
- **T-6 to T-4 months:** ship the core course and the free videos (the search ranking needs time).
- **T-3 months:** low-ticket workbook + test series launch, discount deadline.
- **T-6 weeks:** revision packs, mock tests, live crash cohort.
- **Post-exam:** answer-key/solution videos (a huge spike), then the next-cycle pre-sale.
- **B2B:** align with the semester start and NBA/AICTE submission windows.

Every week lists tasks with an owner skill and hours, and fits `--budget-hours`. When it doesn't fit, defer the lowest-scored product. Don't overcommit.

## Phase 6: `--launch <product-id>` (build the product)

1. Fill the gaps by **delegating**, in dependency order: `/create-lecture` → `/lecture-notes` → `/competitive-exam-questions` → `/create-assignment` (student set only) → (video add-on, if installed) → `/compile-latex`. Never author course content inside this skill.
2. Verify: the quality gate (≥ 90) on every constituent; `/verify-claims` / `/verify-algorithm` / `/verify-symbolic` on worked numbers; `/proofread`; guardrail scan (grep the bundle manifest for `solutions`, `minor`, `-reference`, `book.pdf` and hard-fail on any hit).
3. Assemble `revenue/products/<id>/`: `manifest.yaml` (source paths and checksums), the compiled bundle (gitignored if paid, see below), `sales-page.md` (headline, outcome promise, contents, sample pages, FAQ, refund policy, instructor bio, and only real testimonials or none), `listing.md` per platform, email/Telegram launch sequence (5 messages), and 10 social posts derived from the content.
4. Run `/humanize` on all sales copy. AI-sounding copy converts worse.
5. **Instructor approval gate:** present the product, the price and the guardrail report. Nothing is uploaded, published, priced or emailed by this skill. Publishing is the instructor's action.

**Repo hygiene:** paid bundles and `revenue/` business data (prices, buyer data, revenue) are **gitignored** by default. Add `revenue/products/*/dist/` and `revenue/data/` to `.gitignore`. Only `catalogue.md`/`calendar.md` templates are committable, and they are generic. This repo is also a public template (see `meta-governance.md`).

## Phase 7: `--review` (feedback loop, monthly)

Ask for platform exports (sales CSV, refunds, YouTube Studio, email open/click, UTM conversions). Compute per product: visitors → leads → buyers → refunds → revenue per hour invested.
- Low traffic → reach (videos, Shorts, SEO).
- Traffic without leads → lead magnet or CTA.
- Leads without sales → offer, price or sales page.
- Refunds/complaints → quality. Route the specific lecture to `/slide-excellence` / `/qa-notes`.
- Write `revenue/reviews/<YYYY-MM>.md`, update the scores in `catalogue.md`, and re-rank. Kill products below their hours cost after two cycles (two-strikes, see `summary-parity.md`).
- Append generic lessons to MEMORY.md as `[LEARN:revenue]`. Keep personal numbers in `.claude/state/personal-memory.md`.

## Outputs

```
revenue/
  profile.md          # business profile (Phase 0)
  inventory.csv       # every asset, classified
  catalogue.md        # scored, top-3 cut
  funnel.md           # per product
  calendar.md         # exam-anchored, hours-budgeted
  products/<id>/      # launch kits (dist/ gitignored)
  reviews/YYYY-MM.md  # monthly loop
```

End with a short hand-off: the top 3 products with their scores, the single next action for this week, the guardrail result, and the open `ASSUMED` items to confirm.

## Cross-references

- `/competitive-exam-questions`, `/lecture-notes`, `/create-assignment`, `/coding-assignment`, `/lab-manual`, `/instructor-handout`, `/accreditation-autopilot`: the product-builders.
- `/publish-course-hub`: the free storefront and lead-magnet hub.
- `/yt-viral`, `/yt-plan`, `/last30days`: demand research.
- `/humanize`, `/proofread`, `/verify-claims`: copy and content QA.
- `.claude/rules/orchestrator-protocol.md` (RUN_CONFIG, fan-out), `model-routing.md` (tiers), `meta-governance.md` (public-template hygiene).
