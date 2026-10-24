# Notebook ToC (v2, draft)

Draft replacing `.scrap/TOC-deprecated.md`, from a five-school debate plus an arbitrator verdict (2026-10-23).
Revised 2026-10-24: three levels where a section is large enough, Hopcroft–Karp added, §7 regrouped,
file paths updated to the flat per-section layout (`tex/contents/<section>/`, `testing/<section>/`).

Legend:
- **Kind:** `CODE`, `TEXT`, or `CODE+TEXT` (code with a short note under it or in its comments).
- **~N:** estimated printed lines (exact for files that exist). One page holds about 130 lines
  (landscape A4, 3 columns, `\footnotesize`); measured once from the 7-page PDF, so re-measure later.
- **uses:** other notebook entries the code depends on (the template is implied everywhere).
- **Status:** ✓ written and in the PDF · ◐ written in the repo but not wired, or failing its test ·
  no mark = still to write. `[n/7]` = how many of the 7 surveyed notebooks include the topic.

```
NOTEBOOK (25 pp)  —  section → entry, or section → group → entry where a section has ~8+ entries
│                    that split naturally (each group ≥ 2 entries). Folders stay flat per section.
│
├── 0. Contest  (2 levels) ........................................ ~163 lines · 1.25 pp
│   ├── 0.1 Template + pragmas + dbg under #ifdef LOCAL    CODE       ~55              ✓ contest/template.hpp (add pragmas/dbg)
│   ├── 0.2 Shell: compile/run + hash.sh                   CODE       ~10              ✓ contest/command.sh (hash.sh to write)
│   ├── 0.3 Fast IO (fread)                                CODE       ~15
│   ├── 0.4 stress.sh + usage note (the "--help" text):    CODE+TEXT  ~28
│   │   modes: stress (default) gen $seed → sol → brute →
│   │   check, on failure print seed + save tests/fail_<seed>;
│   │   tests: run sol on every tests/*.in vs .ans (samples
│   │   first), PASS/FAIL/RE/TLE per file; interactive: mkfifo
│   │   loop with ./interactor <seed>; multipass: sol → noise
│   │   <seed> → sol → check. Options -c checker (else diff -w),
│   │   -n iterations, -s first seed, -t time limit. Builds with
│   │   -DSTRESS + sanitizers; timeout + exit code on sol
│   ├── 0.5 Gen helper: rnd(l, r) seeded from argv[1];     CODE       ~10
│   │   random tree (random parent + relabel)
│   ├── 0.6 Checker skeleton: reads in/out/ans,            CODE       ~8
│   │   exit 0 = OK
│   └── 0.7 Troubleshoot + edge cases + when to stress:    TEXT       ~37
│       hand-made cases first (teammate writes them on paper,
│       one tests/ folder per problem); stress after WA, or when
│       random input is always valid + greedy/DP unproven + brute
│       ≤ 5 min; small n first; under -DSTRESS shrink block sizes /
│       thresholds to 1–3 (const int, not #define).
│       Edge cases — size: min (n=1, empty), max (TL/ML/overflow,
│       generated, not hand-written); values: 0, negative, ±1e9,
│       2³¹, 1e18, all equal, all distinct, sorted, reversed, many
│       duplicates; graphs: 1 vertex, disconnected, self-loops,
│       multi-edges, path, star, complete, Y-shape; answers:
│       impossible, ties, several valid, at a boundary; multitest:
│       big case then small (state not cleared); format: whitespace,
│       YES/Yes, decimals; geometry: parallel/coincident lines, far
│       intersections, circles at 0/1/2 points, collinear, on axes
│
├── 1. Flow & Matching  (3 levels) ................................ ~272 lines · 2 pp (over by ~12)
│   ├── 1.1 Flow
│   │   ├── 1.1.1 Dinic with scaling + min cut             CODE       ~45              ✓ flow/dinitzwscaling.hpp (passes test, TL 1 s)
│   │   │   (reverse BFS from t, multi-push DFS)
│   │   ├── 1.1.2 MCMF (SPFA/Dijkstra potentials)          CODE       ~50              ◐ test only: testing/flow/mcmf.cpp (header missing)
│   │   ├── 1.1.3 Flow models: lower bounds / circulation  TEXT       ~12  uses: Dinic, MCMF   [4/7]
│   │   │   via super source-sink; min-cost circulation by
│   │   │   saturating negative edges; vertex capacities by node
│   │   │   splitting; project selection / max closure;
│   │   │   König (min VC = max M), Dilworth (min chain cover = n − M), Hall
│   │   ├── 1.1.4 O(V³) Global min cut, Stoer–Wagner       CODE       ~25              [7/7]
│   │   └── 1.1.5 Gomory–Hu tree                           CODE       ~15  uses: Dinic [3/7+]
│   └── 1.2 Matching
│       ├── 1.2.1 O(E√V) Hopcroft–Karp                     CODE       ~25              (5–10× faster than Dinic on bipartitematching)
│       ├── 1.2.2 O(V³) Hungarian                          CODE       ~35              [6/7]
│       └── 1.2.3 O(V³) General matching (blossom)         CODE       ~65              [6/7]
│
├── 2. Graph  (3 levels; trees live here) ......................... ~422 lines · 3.25 pp
│   ├── 2.1 Connectivity
│   │   ├── 2.1.1 SCC + 2-SAT, iterative Tarjan            CODE       ~50              ◐ graph/scc.hpp (WA) + graph/2sat.hpp (✓ in PDF; segfault at 1e6): merge
│   │   ├── 2.1.2 Lowlink: bridges, articulation points,   CODE       ~40              [6/7]
│   │   │   BCC
│   │   └── 2.1.3 Euler path / circuit                     CODE       ~25
│   ├── 2.2 Paths & spanning trees
│   │   ├── 2.2.1 SPFA + negative cycle; min mean cycle =  CODE+TEXT  ~53              ✓ graph/spfa.hpp  [5/7]
│   │   │   binary search λ, negative cycle on w − λ
│   │   ├── 2.2.2 O(E log V) Directed MST                  CODE       ~50  uses: Rollback DSU (§3.3.1)   [5/7]
│   │   ├── 2.2.3 Manhattan MST                            CODE       ~30              [3/7]
│   │   └── 2.2.4 Matrix-tree theorem                      TEXT       ~4   uses: Gauss det (§6.2.1)
│   ├── 2.3 Trees
│   │   ├── 2.3.1 HLD (gives LCA)                          CODE       ~45  pairs with: Lazy segtree (§3.1.1)
│   │   ├── 2.3.2 Centroid decomposition                   CODE       ~30
│   │   └── 2.3.3 Virtual / compressed tree                CODE       ~20  uses: HLD
│   │       (LCT: see §3.2.2)
│   └── 2.4 Hard
│       ├── 2.4.1 Dominator tree                           CODE       ~40              [5/7]
│       └── 2.4.2 Max clique                               CODE       ~35              [5/7]
│
├── 3. Data Structures  (3 levels; only code you can't retype) .... ~366 lines · 3 pp
│   ├── 3.1 Segment trees
│   │   ├── 3.1.1 Lazy segment tree, generic               CODE       ~50
│   │   ├── 3.1.2 Persistent segment tree; range add via   CODE+TEXT  ~76              ✓ ds/ps_seg.hpp (verified 14/14)
│   │   │   never-pushed tags
│   │   └── 3.1.3 Li Chao tree (lines + segments;          CODE       ~35              [CHT 6/7]
│   │       compress x)
│   ├── 3.2 Balanced trees
│   │   ├── 3.2.1 Implicit treap                           CODE       ~55              [5/7]
│   │   └── 3.2.2 Link-cut tree                            CODE       ~80              [6/7]
│   └── 3.3 Other
│       ├── 3.3.1 Rollback DSU                             CODE       ~20
│       ├── 3.3.2 pb_ds order-statistic tree +             CODE       ~15              ◐ ds/hash.hpp (works, not wired)
│       │   gp_hash_table with custom hash
│       └── 3.3.3 Mo's algorithm (+ Hilbert order)         CODE       ~35
│
├── 4. Strings  (2 levels) ........................................ ~291 lines · 2.25 pp
│   ├── 4.1 Z-function (KMP omitted by the user's rule)    CODE       ~10
│   ├── 4.2 Rolling hash; palindrome = forward ==          CODE+TEXT  ~100             ✓ strings/strhash.hpp
│   │   backward hash, longest palindrome by binary search per centre
│   ├── 4.3 O(N log N) Suffix array + LCP; cyclic SA       CODE+TEXT  ~61              ◐ strings/suffixarray.hpp (remove undeclared MAX)  [6/7]
│   │   without sentinel: sa[0] = minimum rotation
│   ├── 4.4 Aho–Corasick                                   CODE       ~40              ◐ strings/ahocorasick.hpp (trie only)  [5/7]
│   ├── 4.5 Suffix automaton                               CODE       ~35
│   ├── 4.6 Eertree (palindromic tree)                     CODE       ~30
│   └── 4.7 Lyndon factorisation (Duval)                   CODE       ~15
│
├── 5. Geometry  (2 levels; a 3D group would hold 1 entry) ........ ~394 lines · 3 pp
│   ├── 5.1 Point/line primitives, orientation,            CODE       ~60              (new code: no point2D exists in the repo)
│   │   segment intersect/dist
│   ├── 5.2 Polygon: area, point-in-polygon, cut;          CODE+TEXT  ~44  uses: Point
│   │   Pick + triangle formulas
│   ├── 5.3 Convex hull + hull diameter + point-in-hull    CODE       ~45  uses: Point
│   ├── 5.4 Circles: intersections, tangents,              CODE       ~70  uses: Point [6/7]
│   │   circumcircle; MEC
│   ├── 5.5 Half-plane intersection                        CODE       ~45  uses: Point [5/7]
│   ├── 5.6 Closest pair / kd-tree                         CODE       ~50  uses: Point
│   └── 5.7 3D: point3, 3D hull, volume                    CODE       ~80              [6/7]
│
├── 6. Math (code)  (3 levels) .................................... ~774 lines · 6 pp
│   ├── 6.1 Number theory
│   │   ├── 6.1.1 Modint                                   CODE       ~49              ✓ math/modint.hpp
│   │   ├── 6.1.2 Bigint                                   CODE       ~145 uses: NTT (§6.3.2)   ✓ math/bigint.hpp
│   │   ├── 6.1.3 Ext-gcd, CRT                             CODE       ~20
│   │   ├── 6.1.4 Sieve (factorable)                       CODE       ~32              ✓ math/eratos_factorable.cpp (fails at N=1: fix)
│   │   ├── 6.1.5 Miller–Rabin                             CODE       ~38              ✓ math/millerrabin.hpp
│   │   ├── 6.1.6 Pollard rho                              CODE       ~27  uses: Miller–Rabin   ✓ math/pollardrho.hpp
│   │   ├── 6.1.7 Discrete log / discrete sqrt             CODE       ~45  uses: Ext-gcd [5/7]
│   │   └── 6.1.8 Floor sum                                CODE       ~12
│   ├── 6.2 Linear algebra
│   │   ├── 6.2.1 Gauss–Jordan: solve, det, inverse,       CODE+TEXT  ~73  uses: modint   ✓ math/gauss.hpp (solve only)
│   │   │   rank, mod 2 (solveXor)
│   │   └── 6.2.2 Simplex                                  CODE       ~60              [5/7]
│   ├── 6.3 Polynomials & transforms
│   │   ├── 6.3.1 FFT                                      CODE       ~64              ✓ math/fft.hpp
│   │   ├── 6.3.2 NTT                                      CODE       ~47  uses: modint   ✓ math/ntt.hpp
│   │   ├── 6.3.3 FWHT (xor/and/or); or/and transform =    CODE+TEXT  ~22  uses: modint
│   │   │   SOS, swap + for max/min
│   │   ├── 6.3.4 FPS: inv, log, exp, div                  CODE       ~70  uses: NTT   [5/7]
│   │   ├── 6.3.5 Berlekamp–Massey + Bostan–Mori           CODE       ~40  uses: NTT   [5/7]
│   │   └── 6.3.6 Lagrange interpolation                   CODE+TEXT  ~15  uses: modint
│   └── 6.4 Numerical  (1 entry: merge into 6.3 or take ternary search from §8 if it stays alone)
│       └── 6.4.1 Simpson integration                      CODE       ~15
│
├── 7. Interactive & Multi-pass  (3 levels) ....................... ~122 lines · 1 pp
│   ├── 7.1 Checklists
│   │   ├── 7.1.1 Interactive: on -1 / final answer exit   TEXT       ~10
│   │   │   at once; randomness: clock seed vs adaptive judge;
│   │   │   query lower bound = log(#answers)/log(#responses);
│   │   │   one lie: n(q+1) ≤ 2^q; local run: mkfifo f;
│   │   │   (./sol < f) | (./interactor > f)
│   │   ├── 7.1.2 Multi-pass: detect pass from input;      TEXT       ~10
│   │   │   fixed RNG seed; canonical order (never iterate
│   │   │   unordered_map); TL per pass; nothing carries over
│   │   └── 7.1.3 Noise taxonomy: what the judge does →    TEXT       ~10
│   │       which entry to use
│   ├── 7.2 Codes
│   │   ├── 7.2.1 Linear codes: Reed–Solomon, random       TEXT       ~12  uses: Lagrange (§6.3.6), Gauss solveXor (§6.2.1)
│   │   │   GF(2) code, stuck bits (syndrome)
│   │   ├── 7.2.2 Bit-level codes: self-delimiting         TEXT       ~10  uses: Rolling hash (§4.2)
│   │   │   (unary, Elias-γ, padding), VT, pairs, checksum
│   │   └── 7.2.3 Hamming SECDED                           CODE       ~15
│   └── 7.3 Encoding
│       ├── 7.3.1 Rank / unrank: subsets, multisets,       CODE       ~35  uses: bigint (§6.1.2)
│       │   permutations
│       └── 7.3.2 Tree canonical form (AHU) — optional     CODE       ~20
│
├── 8. Misc  (2 levels; code only, only if no section fits) ....... ~76 lines · 0.75 pp
│   ├── 8.1 DP optimisation: D&C, Knuth, Aliens            CODE+TEXT  ~46
│   │   (with conditions)
│   └── 8.2 Ternary / golden-section search + simulated    CODE       ~30
│       annealing
│
├── 9. Math Reference  (2 levels; text, last pages) ............... ~185 lines · 1.5 pp
│   ├── 9.1 Counting: binomial identities, Lucas,          TEXT       ~50
│   │   Stirling 1/2, Catalan, Bell, Burnside
│   ├── 9.2 Number theory: φ/μ, Möbius inversion,          TEXT       ~30
│   │   divisor bounds, HCN, large primes
│   ├── 9.3 Sums, Faulhaber, generating functions          TEXT       ~30
│   ├── 9.4 Games: Sprague–Grundy, Nim variants            TEXT       ~15
│   ├── 9.5 Probability, trig/geometry identities          TEXT       ~30
│   └── 9.6 Techniques prompt list                         TEXT       ~30
│
└── Reserve (blank; only code that passes testing/ may fill it) ............... 1 pp
                                                                    TOTAL = 25 pp
```

Budget check: 1.25 + 2 + 3.25 + 3 + 2.25 + 3 + 6 + 1 + 0.75 + 1.5 + 1 = **25**.
Estimated content: ~3,060 lines ≈ 23.5 pp at 130 lines/page. §1 is ~12 lines over its 2 pp (Hopcroft–Karp).

Status: the tree has 82 entries (one per printed file). 18 have code in the repo (15 of them in the PDF, 3 ◐ not wired);
MCMF has a test but no header. The other 64 entries are still to write (MCMF among them), 14 of them prose.
Out of the printed book, replaced by notes on the entry that covers them: persistent lazy segtree, standalone
SOS DP, LR flow / circulation code (flow models), min mean cycle (SPFA), LineContainer (Li Chao),
Manacher (rolling hash), min rotation (suffix array).
Everything in the user's plan that is not shown above is out of the printed book, or is a candidate for the
reserve.

## Rules

- Section → entry by default. Add a group level (section → group → entry) only when a section has ~8+
  entries that split naturally and each group holds ≥ 2. Never more than three levels.
- In the TeX, a section declares `\twolevels` or `\threelevels`; groups are plain `\subsection`s.
- Folders stay flat per section (`tex/contents/<section>/`, `testing/<section>/`); groups exist only in
  `section.tex`.
- Theory on *how to use* an algorithm sits next to its code; *what to try* prose goes in Math Reference.
- Misc takes code only, and only when no topical section fits.
- Leave out code the team can retype from memory.
- The reserve page is filled only by code that passes `testing/`.
- Each entry prints a "uses: X (§n)" line under its title when it depends on another entry.
