# Notebook house style

Every printed entry in `tex/contents/` follows these rules.

The goal is code a contestant can retype from paper under pressure. Readability beats shortness; shortness beats
cleverness. Exemplar: tex/contents/flow/dinitzwscaling.hpp + its note in tex/contents/flow/section.tex.

Layout
- Line 1 `#pragma once`, then the `#include` lines, then the code on the NEXT line (no blank line).
  `\cpp[k]` starts at that first code line (k = number of pragma/include lines + 1). Never print pragma/includes.
- Every line ≤ 58 characters with tabs counted as 2 (measured: 58 fits on one printed row, 59 wraps).
  Check: `expand -t2 FILE | awk 'length > 58'` must print nothing.
- One idea per line. Two short statements may share a line when they belong together and the line fits
  (`int v = q.front(); q.pop();`, `e.cap -= got; g[e.to][e.rev].cap += got;`). Never three statements on a line,
  never comma-chained statements (`a = 1, b = 2;` — write `a = 1; b = 2;`), never a statement hidden inside another
  (`int p = dsu[w] = par[w]`). Also fine on one line: a short guard `if (x) return y;` / `if (x) continue;`, and a loop
  or if whose body is ONE short statement (`FOR(int, i, 0, n - 1) p[i] = i;`). Never nested ternaries; at most one
  `?:` per line.
- No blank lines inside the code.
- Braces tight: `struct X{`, `void f(int n){`, `for (...){`, `if (...){` (no space before `{`). A one-line function
  body may stay on its line: `int size(){ return n; }`.

Comments
- No trailing comments after code. A comment gets its own line, above the code it explains.
- Comments only for a non-obvious step (an invariant, why a trick works). The interface contract (what it computes,
  how to call it, limits, complexity) lives in the NOTE, never in a code comment. No header comment repeating the note.

Interface
- One struct per entry, built by a constructor taking its size: `Dinic d(n);`. No `init()`, no globals.
- Method names:
  - The main method is always `run(...)` and takes what the query needs: `d.run(s, t)`, `mf.run(s, t, limit)`,
    `hk.run()`. The note says what it returns.
  - Building: `addEdge(...)`, or `add`/`addX(...)` for what is added.
  - Questions asked after `run` are named after their answer, and the same answer has the same name in every entry:
    `cap(id)` (residual capacity), `flow(id)` (flow on an edge), `cover()`.
- camelCase names; short but meaningful (`g`, `level`, `cap`, `dist`, `par` are fine; no `oppo`, `capa`, `pa`/`par` pairs).
  Fields keep their names unless the name says nothing (`co`, `chk`, single letters for main results).
  Helper/demo code that is not interface: `_helper` / `_example`.
- Template and callback parameters:
  - Every one is named in the note's Usage line with its signature or allowed types, e.g. "`add(i, e)`, `del(i, e)`
    put/take `a[i]` at the left (e = 0) or right (e = 1) end; `calc()` returns the answer", "T = ll or modint".
  - Only one type is ever used: write that type, no template (`convMod(a, b, M)` takes M at runtime).
  - Two or more real types (`Point<ll>`/`Point<ld>`, `Simplex<double>`/`<long double>`, `MCMF<C>`): a template with a
    default, so the common case is `<>`. Never a `typedef` to edit by hand.
- Indexing: array range structures (lazy/persistent segment tree, Mo, treap positions, HLD `pos`) are 1-indexed with
  inclusive ranges; strings, graphs, math, geometry and the Li Chao tree are 0-indexed. Graphs take n vertices.
- Loops: template macros, both INCLUSIVE: `FOR(int, i, a, b)` = a..b · `REV(int, i, b, a)` = b down to a.
  Sizes as `FOR(int, i, 0, (int)v.size() - 1)` (the cast matters when v is empty).
  Range-for (`for (int v: g[u])`) when iterating a container; plain `for`/`while` only for non-counting loops
  (pointer walks, `for (int &i = ptr[v]; ...)`).
- template.hpp gives bits/stdc++, `using namespace std`, `ll`, `ld`, fi/se, FOR/REV, maximize/minimize.

## Notes (the [note] of \cpp): KACTL-style, full sentences
Order, each on its own line:
1. One plain sentence (no label) saying what it computes / solves. Name the algorithm only if useful.
2. `\fld{Usage}` how to call it: constructor, methods with their arguments and return values, indexing,
   preconditions (s ≠ t, no negative cycles, call once), what the fields mean afterwards.
3. `\fld{Time}` complexity (and memory if it matters), as a formula.
4. Optional `\fld{Note}` for one gotcha or numeric limit ("exact in ll for |c| ≤ 1e9").
Rules: full sentences in plain words. No arrow chains (`a → b → c`), no test logs ("tested n = 500 in 0.04 s" →
say the limit instead), no problem names as explanations, no `\fld` labels beyond the four above. Never claim more
than the tests show. Never repeat the code's comments. LaTeX: accents as \"o (ö); `]` inside a note must be `{]}`
(a bare `]` ends the note argument); check the rendered PDF.

