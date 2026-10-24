# hesllnotebook

ICPC team notebook for **HUS.HAD** (Hanoi University of Science): a 25-page landscape A4 PDF
(3 columns, `\footnotesize`). Every code entry is a header in `tex/contents/` that the PDF prints and
`testing/` checks against [Library Checker](https://judge.yosupo.jp) problems.

The planned table of contents (sections, entries, status, line budgets) is in
[`docs/TOC-draft.md`](docs/TOC-draft.md).

## Layout

```
tex/
  main.tex               inputs each section in order
  hesllnotebook.cls      page layout, ToC style, \cpp / \othercodes / \entry
  customlexers/lexers.py Pygments lexer for the template's macros
  contents/<section>/    one flat folder per section: section.tex + code
testing/
  <section>/<name>.cpp   test program for tex/contents/<section>/<header>
  all.py                 registry: test → Library Checker problem + time limit
  test.sh                compile, generate tests, run, check
library-checker-problems/  submodule (zipdang04's fork of yosupo06/library-checker-problems)
docs/TOC-draft.md        planned ToC
Makefile                 make test / make pdf / make yukitoken (CI runs the same targets)
.github/workflows/       verify.yml: tests, then the PDF as a downloadable artifact
```

Sections: `contest` (0), `flow` (1), `graph` (2), `ds` (3), `strings` (4), `geometry` (5), `math` (6),
`interactive` (7), `misc` (8), `reference` (9). Folders stay flat; groups inside a section exist only in
its `section.tex`.

## Code conventions (new code; older files are being brought in line)

- `#pragma once`, then `#include "../contest/template.hpp"` (plus any other entry it uses).
- One struct per entry, built with its size (`init(n, …)` or a constructor); no globals.
- Indexing depends on the use: sequence ranges are 1-indexed inclusive, math and bit arrays are
  0-indexed, graphs take `n` vertices.

## Building the PDF

Needs TeX Live with `titlesec`, `minted` and `vntex` (BasicTeX users:
`sudo tlmgr install collection-latexextra`).

```
make pdf          # tex/main.pdf
```

minted v3 (TeX Live 2024+) only runs a custom lexer that is allowed by its SHA-256 in
`~/.latexminted_config`:

```json
{ "custom_lexers": { "lexers.py": "<output of: shasum -a 256 tex/customlexers/lexers.py>" } }
```

Update the hash whenever `lexers.py` changes. minted v2 (e.g. MiKTeX) needs no config; the class picks
the right lexer name for either version.

### Adding an entry

In `tex/contents/<section>/section.tex`:

```latex
\cpp[4]{Dinic with scaling + min cut}{dinitzwscaling.hpp}   % print from line 4
\cpp[3]{SPFA}{spfa.hpp}[A note printed under the title.]
\entry{Hopcroft--Karp}                                      % heading only, no code yet
```

A section declares `\twolevels` (section → entry) or `\threelevels` (section → group → entry, with each
group a plain `\subsection`).

## Testing

```
git submodule update --init
pip3 install online-judge-tools    # for AOJ / yukicoder tests
make yukitoken <TOKEN>             # once: yukicoder API token (account settings → API) into .env

make test                          # every test
make test ARGS="math"              # one section
make test ARGS="flow dinitz"       # one test
```

`testing/all.py` registers two kinds of test:

- `TestCase(source, problem, tl)` — a Library Checker problem: `test.sh` generates its tests from the
  submodule, runs the program under the time limit and checks each output with the problem's checker.
- `OjTest(source, url, tl, error=None)` — AOJ, yukicoder, or any judge online-judge-tools supports: the
  judge's test data is downloaded once into `testing/.oj-tests/` and checked with `oj test` (`error` for
  floating-point answers). yukicoder tests need `YUKICODER_TOKEN` and are skipped without it.

The compiler is `$CXX` if set, otherwise the first of `g++-16`, `g++-15`, `g++-14`, `g++` (Apple clang
lacks `bits/stdc++.h` and pb_ds). To add a test, write `testing/<section>/<name>.cpp` that includes the
header and solves the problem, then register it in `all.py`.

### CI

`.github/workflows/verify.yml` runs `make test` on every push and pull request, then `make pdf` if the
tests pass; the PDF is the run's `notebook` artifact. yukicoder tests need the repository secret
`YUKICODER_TOKEN` (`gh secret set YUKICODER_TOKEN`).
