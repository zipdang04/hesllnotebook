# House style

The style of every entry under `tex/contents/**` and every test under `testing/**` on the
`second-main` rewrite.

It has two kinds of reader:

- **the team**, who type an entry from paper, read it over a shoulder, and change it in the
  middle of a contest;
- **implementer agents**, who write an entry from a spec. A spec wins over this file where the
  two differ, and it must say that it differs.

Each rule comes with a short reason and a small example. When a case is not covered, choose
what is easiest to retype from paper without a mistake.

Contents:

1. [Layout](#1-layout)
2. [Naming](#2-naming)
3. [File shape](#3-file-shape)
4. [Structure and interface](#4-structure-and-interface)
5. [Types](#5-types)
6. [Indexing](#6-indexing)
7. [Loops](#7-loops)
8. [Comments and notes](#8-comments-and-notes)
9. [Performance](#9-performance)
10. [Tests](#10-tests)
11. [Porting from reference notebooks](#11-porting-from-reference-notebooks)
12. [Checklist before reporting](#12-checklist-before-reporting)

Appendices: [A. Template](#appendix-a-what-the-template-provides) ·
[B. Build](#appendix-b-build) · [C. Labels and the class file](#appendix-c-labels-and-the-class-file) ·
[D. Trial findings](#appendix-d-where-the-trial-findings-went) · [Open](#open)

---

## 1. Layout

### 1.1 Width and indentation

| Rule | Why |
|---|---|
| No line is wider than **60 columns**, a tab counting as 2. This holds for `.hpp` files and for tests. | The printed column holds 60 code characters. A longer line wraps and loses its indentation. |
| Indent with tabs, one per level. | One key per level. |
| No trailing blanks. The file ends with a newline. | Clean diffs. |
| No blank line after line 3 of an entry. | The top comment and the braces already separate the parts. |

Page count does not matter now. Never pack code or text to save lines.

Check a file. It prints the lines that are too wide and must print nothing:

```sh
python3 -c 'import sys
for f in sys.argv[1:]:
  for i, l in enumerate(open(f, encoding="utf-8"), 1):
    n = len(l.rstrip("\n").expandtabs(2))
    if n > 60: print(f, i, n)' FILE...
```

Do not use `awk 'length > 60'` for this on macOS. Its `length` counts bytes, so a Vietnamese
comment of 23 characters is reported as 35.

### 1.2 Spacing

| Where | Write | Not |
|---|---|---|
| Keywords | `if(x)`, `for(..)`, `while(..)`, `switch(..)` | `if (x)` |
| Opening brace | `){`, `struct SCC{`, `do{` | `) {` |
| `else` | `}else{`, `}else if(x){` | `} else {` |
| `do` loop | `do{ .. }while(x);` | |
| One-line body in braces | `[&](ll v){ return v * v; }`, empty: `{}` | `{return v;}` |
| Assignment, comparison, logic, `?:` | `a = b`, `a += b`, `a <= b`, `a && b`, `c ? a : b` | `a=b` |
| Arithmetic, shifts, bit operators | `a * x + b`, `n >> 1`, `id ^ 1` | `a*x+b` (but see 1.3) |
| Unary operators and casts | `!x`, `-x`, `i++`, `(int)v.size()`, `(__int128)a * b` | `! x`, `(int) v.size()` |
| Comma, and `;` in a `for` head | `f(a, b)`, `for(; n; n >>= 1)` | `f(a,b)` |
| Range `for` | `for(int v: g[u])`, `for(auto [v, w]: g[u])` | `for(int v : g[u])` |
| Inside `()` and `[]` | `f(a)`, `a[i]` | `f( a )` |
| References and pointers | `const string &s`, `int &x` | `const string& s` |
| Templates | `template<class T>`, `vector<vector<int>>` | `template <class T>` |
| Initialiser list | `Dsu(int n): par(n + 1){}` | `Dsu(int n) : par(n + 1) {}` |
| Line break in output | `'\n'` | `"\n"`, `endl` |

Why so exact: in the trial, `if (` and `if(`, `}else{` and `} else {`, `v: g` and `v : g`,
`'\n'` and `"\n"` all varied between two entries by the same implementer.

Spacing never changes an entry's printed hash. The hash ignores whitespace and comments.

### 1.3 Tight arithmetic

Arithmetic may be written without spaces in exactly two places:

1. inside `[]`: `a[i+1]`, `lcp[a+1]`, `tree[2*id+1]`;
2. in a call argument that is **one** operator between two names or literals:
   `dfs(v, d+1)`, `get(l, r-1)`, `build(2*id, lo, mid)`.

Everywhere else it is spaced: `(lo + hi) / 2`, `4 * n`, `(mulMod(v, v, n) + c) % n`.
Spaced is always allowed. If a line is too long, break it. Do not squeeze it.

Why: "fine in short arguments" was read broadly in the trial (`(lo + hi)/2`, `4*n`,
`-xs.begin()` to make a line fit).

### 1.4 Lines and braces

| Rule | Why | Example |
|---|---|---|
| One idea per line. Two or three short statements may share a line when they are one step. | A line number names one thing, and a debug print fits between two lines. | `stk.push_back(u); onStk[u] = 1;` |
| Statements joined by a comma are fine for one step. | Same. | `x = f(x), y = f(f(y));` |
| A short body may stay on the line of its `if`, `for` or `while`. | Less to type. | `if(lo == hi) return res;` |
| One `for` or `while` may be followed by one `if` on the same line. | Common "for each, if" shape. | `for(int v: g[u]) if(!comp[v]){` |
| A body of one statement goes on the next line, one tab deeper, without braces. A body of two or more statements is braced. | One statement needs no `}` to type back; a braced body gets a guide (1.5). | `for(auto &e: g[u]) if(e.rem > 0)` then `eps = max(eps, -rc(e, u));` |
| Prefer an early `return`, `continue` or a loop condition to an `if` around the rest of a block. | One guide fewer, shorter lines. | `if(low[u] < num[u]) return;` |
| Past 4 guide levels inside a function, look for a natural helper (one step of the algorithm). If there is none, keep the nesting. Advice, not checked. | Several ticks on one line are hard to count back. | cost scaling: `while(refine());` |
| No comment after an opening `{`: put it on its own line above. | A `{` followed by a comment keeps its braces and gets no guide. | |
| Inside functions, `//` comments or a one-line `/* .. */` only. | The guide marker would land inside a multi-line comment and print as text. | |
| In bodies use the template's `sz(x)` and `all(x)`. Public signatures spell types out. | Shorter lines. | `FOR(i, 0, sz(in) - 1)`, `sort(all(xs))` |
| No blank lines inside a function or struct. | See 1.1. | |

### 1.5 Brace guides (printed form)

The source keeps every brace; it compiles and is tested as written. The PDF prints a
display copy (`tex/braceguides.py`, called by `\cpp`):

| Block | Printed | Typed back |
|---|---|---|
| Function body, `for`, `while`, `if`, `else`, `do`..`while` | no `{` at the end of the header and no `}` line; a thin gray guide down the body and a tick under its last line; `}while(c);` prints as `while(c);` after the tick | `{` after the header, one `}` per tick |
| `struct` / `class`, lambda, one-line block, brace initialiser | as written, braces included | nothing |
| `contest/template.hpp` | as written, no guides | nothing |

A `}` is dropped only when it is alone on its line (or followed by `else` / a do-while's
`while`), its `{` ends the header line, and the body is indented deeper. Line numbers count
printed lines. The printed hash is computed from the source, braces included, so code typed
back with its braces matches it.

### 1.6 Breaking a long line

Break after a comma or before a binary operator. The rest goes **one tab deeper**. If the
statement opens a block, it goes **two tabs deeper**, so that it does not line up with the body.

```cpp
// one tab: no block follows
return combine(get(2 * id, lo, mid, l, r),
	get(2 * id + 1, mid + 1, hi, l, r));

// two tabs: a block follows
void dfs(int u, int pe,
		const vector<vector<pair<int, int>>> &g){
	num[u] = low[u] = ++timer;
```

A trailing comment that does not fit goes on the line above the code.

### 1.7 Plain over short

When the two conflict, the form a contestant recognises wins over the shorter one.

| Do not | Do |
|---|---|
| a recursive call inside an argument: `minimize(low[u], num[v] ? num[v] : dfs(g, v));` | `if(!num[v]) dfs(v, g);` then `low[u] = min(low[u], low[v]);` |
| an assignment inside a condition: `if((q = mulMod(prd, d, n))) prd = q;` | `ll q = mulMod(prd, d, n);` then `if(q) prd = q;` |
| a flag tested at the loop head: `if(ok) break;` | `continue` / `break` / `return` at the point where the fact is known |
| deep nesting | an early `continue` or `return`; aim for at most 3 levels inside a function |

The line range in a spec is a **ceiling, not a target**. Never golf to reach it.

Exception: speed. A faster form that is longer or less plain is allowed, with a comment. See
section 9.

---

## 2. Naming

| Rule | Why | Example |
|---|---|---|
| Functions and variables in camelCase. | One look tells a call from a type. | `addEdge`, `isBridge`, `bccCnt` |
| Types in PascalCase. An acronym stays in capitals. | Same. | `LowLink`, `LiChao`, `SCC`, `Mint` |
| A constant that is edited per problem: one capital letter or `UPPER_CASE`. | It stands out in the edit block. | `static const int K = 26;`, `NO_TAG` |
| Short names are fine if they are intelligible. Single letters for roles everybody knows. | Fast to type, still clear. | `u v w`, `s t`, `n m`, `i j`, `l r`, `x y` |
| State that lives across functions gets a word. | A letter gives no hint there. | `dist`, `par`, `excess`; not `p`, `ex` |
| No leading underscore. | `_X` and global `_x` are reserved in C++. | `build`, not `_build` |
| Do not reuse a name the template defines. | Some are macros and replace your name silently. | list in [Appendix A](#appendix-a-what-the-template-provides) |
| Infinity is `oo`. | One word after every entry. | `if(sp.dist[t] == oo)` |

### Shared names

The same job has the same name in every entry, so an entry can be called without rereading it.

| Job | Name |
|---|---|
| Add an edge; the run of a flow | `addEdge(u, v, ..)`; `maxFlow(s, t)` |
| Point or range query; update | `get(..)`; `update(..)` |
| Decide and fill the answer (2-SAT and the like) | `solve()` |
| Distance, parent, component of a vertex | `dist`, `par`, `comp` |
| Number of components | `cnt` (`bccCnt` when two counts exist) |
| DFS order number, low link, DFS clock | `num`, `low`, `timer` |
| Explicit stack | `stk` |
| Modular product and power | `mulMod(a, b, m)`, `powMod(a, n, m)`: the only ones in the notebook |

Inside a port, local names stay as in the source. See section 11.

---

## 3. File shape

### 3.1 The three header lines

Every `.hpp` starts with exactly these three lines. The third is blank.

```cpp
#pragma once
#include "../contest/template.hpp"

```

An entry that builds on another entry includes that entry instead, and nothing else:

```cpp
#pragma once
#include "millerrabin.hpp"

```

Why: every entry is printed with `\cpp[4]{Title}{file.hpp}`, which starts at line 4. A fixed
header means one print offset for all files, and no test needs anything before its `#include`.

`template.hpp` itself is the one file with its own header.

### 3.2 The top comment

Line 4 starts a comment of **1 to 3 lines**. It says what a typist must know before calling
the entry:

1. what it computes;
2. the index base (`vertices 1..n`, `ranges 1-based inclusive`);
3. the complexity;
4. **the recursion depth**, if the code recurses.

```cpp
// SCC by Tarjan, O(V + E), vertices 1..n. Edge u->v gives
// comp[u] >= comp[v] (ids in 1..cnt, reverse topo order).
// Recursive, depth <= V.
```

Recursion is allowed. There is no iterative Tarjan. But the depth line is not optional: the
stack counts toward the memory limit, and a depth of 10^6 costs roughly 100 MB. A spec repeats
"recursive, depth <= .." in its "the top comment must say" list, because the line was dropped
once in the trial.

Anything longer (why it works, modelling tricks, formulas) goes in the note. See section 8.

### 3.3 The rest of the file

The code follows the comment directly: one struct, or a few free functions. Nothing else is at
file scope. Section 4 says which.

File names are lower case without separators (`lowlink.hpp`, `pollardrho.hpp`) and are unique
across the whole notebook, because the file name is also the entry's label (8.5).

---

## 4. Structure and interface

### 4.1 Which shape

> **A free function when there is one result and no state. A struct otherwise.**

A struct is one of three kinds.

| Shape | Use it when | The call | Examples |
|---|---|---|---|
| **Free function** | one result, nothing kept between calls | `vector<int> z = zFunc(s);` | Z, `isPrime`, `factorize`, ext-gcd, floor sum, `conv` |
| **Result struct**: the constructor computes | run once, but several outputs or a recursive helper with shared arrays | `SCC scc(g);` then `scc.cnt`, `scc.comp[u]` | SCC, lowlink, suffix array + LCP, shortest paths, HLD, sieve |
| **Builder**: add, then run, then ask | things are added before the run, or updates and queries alternate | `Dinic d(n + 1); d.addEdge(u, v, c); d.maxFlow(s, t);` | flows, matching, 2-SAT, Aho–Corasick, every data structure |
| **Value type** | it is a number or a point | `Mint a = 3; a *= b;` | modint, bigint, `Point<T>` |

Why a struct as soon as there is state: follow-up questions (the path, the cut, the members of
a component) need the arrays to still be there after the run. A free function has thrown them
away, or it needs globals.

### 4.2 Never

| Never | Why | Instead |
|---|---|---|
| a namespace | It reads globals from outside, and two entries cannot coexist. | a struct or free functions |
| a global variable, `MAX`, `MAXN`, `MOD` | A guess that is too small is a run-time error; a full clear for each test is a TLE. | `vector` members sized in the constructor |
| `init()` | A second, hand-written constructor that must list every member. Forgetting one is a multitest bug. | the constructor; a new test case is `d = Dinic(n + 1);` |
| `class`, `private:` | At a contest you need to reach every member. | `struct`, everything public |
| getters | Same. | public members: `scc.comp[u]`, `ts.val[x]` |
| `#define` in an entry | A leaked macro breaks the next entry. | a function, or a constant in the struct |
| a file-scope `typedef` or constant | Two entries will pick the same name. | inside the struct; exception in 4.6 |
| `inline` | It does nothing in a single file. | |
| `std::function` for recursion | Bigger frames, slower calls. | a member function |
| a reference member to the input | `scc = SCC(g2);` stops compiling, and a temporary argument dangles. | pass the input to the helper as a parameter |

### 4.3 Inside a struct

Order of the members, top to bottom:

1. the edit block, if any (4.5);
2. data members, results first;
3. the constructor;
4. helpers (`dfs`, `push`, `insert`);
5. the public methods.

```cpp
struct SCC{
	int cnt = 0, timer = 0;
	vector<int> comp, num, low, stk;
	SCC(const vector<vector<int>> &g):
			comp(g.size()), num(g.size()), low(g.size()){
		FOR(u, 1, (int)g.size() - 1) if(!num[u]) dfs(u, g);
	}
	void dfs(int u, const vector<vector<int>> &g){
		..
	}
};
```

| Rule | Why |
|---|---|
| Work arrays and helpers are ordinary public members. | No `private:`; they are often what a follow-up question needs. |
| Input comes in by `const &` through the constructor or the arguments. Output goes out as the return value or as public members. | Nothing to call "first", nothing to forget. |
| The graph is the **last** parameter of a recursive helper: `dfs(u, g)`, `dfs(u, pe, g)`. | One order everywhere; the trial had both. |
| The struct can be assigned: `x = X(..)` is the reset between test cases. | Nothing can be left over from the previous test. |
| Keep the size as a member (`int n`) when it is used more than once. | It removes repeated `(int)xs.size() - 1` casts. |
| A constructor parameter never has the name of a member. | `Seg(int n): n(n)` warns under `-Wshadow`, which the debug build uses. Size from the argument without storing it, or name the parameter `n_`. |
| A constructor that takes a container by value moves it: `xs(move(pts))`. | No second copy. |
| Edges and nodes are small named structs with aggregate initialisation. | Adding a field is one word, and every `push_back({v, cap})` still compiles. |

### 4.4 Templates and function parameters

**Write the concrete type.** A template parameter is allowed only when two different types
really occur in one program.

| Thing | Both in one program? | Write |
|---|---|---|
| cost, capacity, distance | no, `ll` covers `int` | `ll` |
| graph representation | no | the types in section 6.3 |
| alphabet size | no | `static const int K = 26;` inside the struct |
| segment tree value and tag | rarely | a `struct Node` and a `typedef` in the edit block |
| point coordinate | **yes**: integer hull, then real distances | `template<class T> struct Point` |

The only single-letter parameter is `T`, for the element or coordinate type of a value type.

**A function parameter (lambda)** is allowed only when the argument really is a function in
the problem. It is never used to describe data.

| Allowed | Not allowed |
|---|---|
| the predicate of a binary search | the adjacency of a graph |
| the function to integrate or minimise | the edge cost |
| add and remove in Mo's algorithm | the combine of a segment tree (edit a member function) |
| a comparator | "what to do with each component" (return `comp` instead) |

The template parameter is named by its role, and one comment line shows the call:

```cpp
// pred: bool(ll), false..false true..true on [lo, hi].
// Call: firstTrue(0, n, [&](ll x){ return can(x); });
template<class Pred> ll firstTrue(ll lo, ll hi, Pred pred){
```

The caller never has to write a lambda with an `auto` parameter. A local lambda inside an
entry, with concrete parameter types and next to its only use, is fine.

### 4.5 What gets edited per problem

A `typedef` or a constant inside the struct is for what a problem changes. Put it at the top of
the struct, between two marker comments, with concrete types.

```cpp
struct LazySeg{
	// ---- EDIT: node value, pending tag, two functions ----
	struct Node{ ll sum = 0; int len = 0; };
	typedef ll Tag;
	const Tag NO_TAG = 0;
	Node combine(Node a, Node b){ .. }
	void apply(int id, Tag t){ .. }
	// ---- end of EDIT ----
```

Why: the contestant edits ten lines with visible types instead of instantiating a template.
The printed default is chosen to match the judge problem that tests it. Two trees with
different operations in one solution: copy the struct and rename it.

### 4.6 Free-function entries

| Rule | Why | Example |
|---|---|---|
| Names are full words that no other entry would pick. | Any two entries can be typed into one file. | `isPrime`, not `check` |
| A table that must persist is `static` inside the function and grows on demand. | No global, no `build()` to remember. | `static vector<Cd> root;` |
| A file-scope `typedef` is allowed only for a type the entry's own signatures need, with a name no other entry would pick. | The signature cannot be written without it. | `typedef complex<double> Cd;`, `typedef Point<ll> PL;` |

### 4.7 One entry using another

An entry uses another entry only through its public interface: it calls a function or
constructs a struct. It includes the other file on header line 2.

```cpp
bool solve(){
	SCC scc(g);
	..
}
```

It never passes a lambda into another entry's loop. If it needs that loop over its own data,
it has its own copy of the loop.

---

## 5. Types

| Rule | Why | Example |
|---|---|---|
| Values are `ll`: weights, costs, capacities, distances, sums, coordinates. | `int` input always fits; no overflow hunt, no template. | `ll cap;` |
| Indices, counts and ids are `int`. | They index vectors. | `int u`, `int id` |
| `__int128` is written directly, only where a product or a total really exceeds `ll`, with a comment. | It is rare and should be visible. | `return (__int128)a * b % m;` |
| No `unsigned`, except for hash values and bit sets. | Mixed signs are a silent bug. | |
| No `size_t` variable. Cast a size where it is used. | See 7.2. | `(int)v.size()` |
| `vector<char>` for flags written in an inner loop; `vector<bool>` only for results. | `vector<bool>` is slow to write. | |
| Pairs: `pii`/`pll` in bodies, members as `.fi`/`.se`. In a loop over pairs, unpack with names instead. | `.fi` is short; `[v, w]` says what the fields mean. | `p.fi + p.se`; `for(auto [v, w]: g[u])` |

### `oo`

The template has one shared infinity:

```cpp
const ll oo = 1e18;
```

| Rule | Why |
|---|---|
| Use the shared `oo` for "no value" and "unreachable". | The caller compares with the same word after every entry. |
| `oo` is `ll`. Never store it in an `int`. An `int` sentinel is `-1`, or `INT_MAX` written in place. | `int x = oo` is garbage. |
| An entry may assume that `oo` plus **one** weight does not overflow. Nothing more. | `2 * oo` fits in `ll`; sums of many values near `oo` do not. |
| `-oo` may mean "below everything" (after a negative cycle). | Same word, same test. |

**Exception.** An entry whose legal answers exceed 10^18 (Li Chao and the like, where values
reach 2·10^18) keeps its own:

```cpp
struct LiChao{
	static constexpr ll oo = LLONG_MAX;  // "no line"
```

The caller then compares with `LiChao::oo`. The top comment or the note says so. The member
hides the template's `oo` inside the struct without a warning (checked with `-Wshadow`).

---

## 6. Indexing

### 6.1 The rule in one table

| Thing | Base | Example |
|---|---|---|
| A range argument `(l, r)` | inclusive on both ends, numbered as in the statement (1-based) | `h.get(l, r)`, `seg.update(l, r, x)` |
| An array with one cell per position of a **string** | 0-based, like `std::string` | `z[i]`, `sa[i]`, `lcp[i]` |
| An array with one cell per position of a **sequence** (input `a`, prefix sums, sparse-table starts) | 1-based, as in the statement; prefix `p[0] = 0` | `a[1..n]`, `st[k][i]` with `i` in `1..n` |
| Bits of a mask or a number, and levels of a doubling table | from 0 (bit 0 = lowest) | `testBit(mask, 0)`, `st[0][i]`, `up[k][u]` with `k` from 0 |
| Graph vertices | 1-based; vertex 0 exists and is never used | `g(n + 1)` |
| Component ids, BCC ids | 1-based; 0 = none | `comp[u]` in `1..cnt` |
| Tree nodes inside a structure (trie, treap, automaton) | the entry says: usually node 0 = root or null | `aho.t[0]` |

The entry never converts a base. A test or a solution converts once, where it reads the input.

### 6.2 Ranges

- **Public ranges are closed**: `[l, r]`. This matches the inclusive `FOR`.
- **Exception:** a half-open range is allowed where the standard library or the algorithm is
  half-open by nature, and the top comment says so. Examples: `sort(all(a))`, `lower_bound`,
  `a.begin() + k`; cyclic indices (`(i + 1) % n` over polygon vertices `0..n-1`, cyclic shifts);
  a compressed coordinate `lower_bound(all(xs), x) - xs.begin()`; the inside of a port (an
  iterative segment tree over `[l, r)`) as long as its public `get(l, r)` is closed.
- A data structure over positions takes the indices as given. The caller sizes it:
  `LazySeg seg(n + 1);` serves positions `1..n` with no `- 1` anywhere. Position 0 exists and
  is unused. The same object serves 0-based ranks: `Rmq rmq(sa.lcp); rmq.get(a + 1, b);`.
- The string hash is the one entry whose base is fixed in its code: `get(l, r)` is 1-based,
  and `get(l, l - 1)` is the empty string.

### 6.3 Graphs

| Graph | Type | `g[u]` holds |
|---|---|---|
| unweighted | `vector<vector<int>>` | `v` |
| weighted | `vector<vector<pair<int, ll>>>` | `{v, w}` |
| edge identity matters (bridges, Euler) | `vector<vector<pair<int, int>>>` | `{v, edge id}` |
| flow network | owned by the flow struct, built with `addEdge` | |

There is no alias for these types. The entry reads the number of vertices from the graph.

| Rule | Why | Example |
|---|---|---|
| The entry uses `g.size()`. It takes no `n`. | One argument fewer to get wrong. | `int n = (int)g.size() - 1;` |
| The caller builds `g(n + 1)`. Vertices are `1..n`. | Statements are 1-based; nobody subtracts 1. | `g[u].push_back(v);` |
| Vertex 0 is never touched and never counted. `g[0]` is not read. | A component count must not include it. | loops are `FOR(u, 1, n)` |
| Ids the entry gives out start at 1; 0 means "none" or "not yet". | The zero-filled vector is the initial state. | `comp[0] == 0` always |
| A builder that owns its graph is sized like a vector. | Same habit as `g(n + 1)`. | `Dinic d(n + 1);` |
| Exception: in a flow struct every vertex `0..N-1` is real, so a super source or sink can be any free index. `maxFlow(s, t)` takes any `s != t`. | Flow models add extra vertices; reserving 0 buys nothing there. | `Dinic d(n + 3); d.maxFlow(n + 1, n + 2);` |

### 6.4 Strings

> **Arrays follow the string, ranges follow the statement.**

- Anything indexed by position is **0-based**: `s`, `z`, `sa`, `rnk`, `lcp`, positions stored
  in nodes. A 0-based index is "the number of characters before it", so it is also a length:
  `i + z[i] == n`, `s.substr(sa[i], lcp[i])` need no conversion.
- A function that takes a range takes `(l, r)` **1-based inclusive**. In the string chapter
  that is the hash: `get(l, r)` and `getRev(l, r)`.

The two halves meet in two places. Each is a fixed formula.

| Crossing | Formula |
|---|---|
| 0-based start `p`, length `L` → hash range | `h.get(p + 1, p + L)` |
| 1-based input position `l` → array index | `rnk[l - 1]`, `z[l - 1]`, `s[l - 1]` |

In the first formula the `+ 1` is on the left only. `(p + 1, p + L + 1)` and `(p, p + L - 1)`
are the typical mistakes. This line is printed on the hash entry.

Three more facts are the same under any indexing rule. They get one comment line on the entry
they belong to:

- end index `e`, length `L` → start `e - L + 1` (Aho–Corasick, automaton, eertree);
- `lcp[i]` pairs ranks `i - 1` and `i`, so the LCP of ranks `a < b` is `min lcp[a+1..b]`;
- the suffix array is **cyclic**. `SuffixArray(s)` sorts rotations. For suffixes the caller
  writes `SuffixArray(s + '$')`; then `sa[0] == n` and the real suffixes are ranks `1..n`.
  The LCS of two strings uses `s + '#' + t + '$'`: the final `'$'` is required.

### 6.5 Library Checker is 0-based

The test adds 1 when it reads and subtracts 1 when it prints. See section 10.

---

## 7. Loops

### 7.1 `FOR` and `REV`

```cpp
#define FOR(i,a,b) for(auto i=(a); i<=(b); i++)
#define REV(i,b,a) for(auto i=(b); i>=(a); i--)
```

Both are **inclusive**. The type of `i` is the type of the **start value**.

| Rule | Example |
|---|---|
| A loop with unit steps over a fixed range uses `FOR` or `REV`. | `FOR(u, 1, n)`, `REV(c, scc.cnt, 1)` |
| An exclusive upper bound is written as `- 1`. There is no `EFOR`. | `FOR(i, 0, n - 1)` |
| A `ll` loop needs a `ll` start. | `FOR(x, 1LL, n)`, `FOR(i, 0LL, k)` |
| Use a range `for` when the index is not needed. | `for(int v: g[u])` |
| Anything else is a plain `for` or `while`: other steps, two variables, a bound that moves. | `for(; n; n >>= 1)` |

### 7.2 Traps

| Trap | What happens | Write |
|---|---|---|
| `FOR(i, 0, v.size() - 1)` | `v.size() - 1` is unsigned. On an empty vector it is 2^64 - 1 and the loop runs. | `FOR(i, 0, (int)v.size() - 1)` |
| `REV(i, v.size() - 1, 0)` | `i` is `size_t`, and `i >= 0` is always true: an endless loop. | `REV(i, (int)v.size() - 1, 0)` |
| `FOR(i, 1, n)` with `ll n` | `i` is `int` and overflows when `n` > 2^31. The compiler says nothing. | `FOR(i, 1LL, n)` |

The first two give a warning with `-Wall -Wextra`. The third does not.

So: **every `.size()` in a loop bound is cast to `int`**, and a loop variable never has an
unsigned type.

### 7.3 A container that grows in the loop

A loop over a container that changes size in the body is a plain `for` with the bound in view.
It is not a `FOR`.

```cpp
// BFS over a vector used as a queue
for(int i = 0; i < (int)q.size(); i++){
	int u = q[i];
	for(int v: g[u]) if(!seen[v]) seen[v] = 1, q.push_back(v);
}
```

Why: `FOR(i, 0, (int)q.size() - 1)` works today only because the macro happens to evaluate
its bound again on every iteration. A reader takes `FOR` for a fixed range. In the trial the
implementer who wrote the plain `for` produced the clearer test.

For the same reason the body of a `FOR` never changes its loop variable or its bound.

---

## 8. Comments and notes

### 8.1 Comments in code

| Rule | Why | Example |
|---|---|---|
| English or Vietnamese, both are fine. | The team reads both. | `// v đã xong` |
| The top comment is 1 to 3 lines (3.2). | It is what the typist needs. | |
| A trailing comment gives a reason or an invariant, after one space. No padding to a column. | Padding is what makes lines wrap. | `if(a % n == 0) continue; // says nothing` |
| If code and comment do not fit in 60 columns, the comment goes on the line above. | Same. | |
| A constant with a reason gets the reason. | Otherwise somebody "fixes" it. | `// prd = 2 covers n = 4` |
| No comment that repeats the code. | Noise. | |

Comments do not change the printed hash.

### 8.2 What goes where

| Content | Place |
|---|---|
| What it computes, index base, complexity, recursion depth | The top comment (3.2). **Not repeated in the note.** |
| How to call it, what comes back, preconditions | The note. |
| A trick that turns the *problem* into this entry's input (bounds, vertex capacity, closure, "max: negate") | A text entry next to it. The note has one line: `\fld{Models} \S\ref{flowmodels}`. |
| A formula sheet, a checklist, a taxonomy | A text entry. |

### 8.3 The note

The note is the fifth argument of `\cpp`, typeset under the title with real formulas.

```tex
\cpp[4]{SCC}{scc.hpp}[
	\fld{Use} \texttt{vector<vector<int>> g(n + 1); g[u].push\_back(v); SCC scc(g);}
	\fld{Result} \texttt{scc.cnt} components; \texttt{scc.comp[u]} is in $1..\texttt{cnt}$.
	An edge $u \to v$ has $\texttt{comp[u]} \ge \texttt{comp[v]}$: ids $\texttt{cnt}, \dots, 1$ are a topological order.
	\fld{Needs} nothing: self-loops, multi-edges and isolated vertices are fine.
]\label{scc}

\cpp[4]{Pollard rho}{pollardrho.hpp}[
	\fld{Use} \texttt{vector<ll> f = factorize(n);}
	\fld{Result} prime factors with multiplicity, unsorted.
	\fld{Needs} $1 \le n < 2^{62}$. Uses \texttt{isPrime} (\S\ref{millerrabin}).
	\fld{findDivisor} a divisor $d$ of a composite $n$, $1 < d < n$.
]\label{pollardrho}
```

| Rule | Value |
|---|---|
| Labels, in this order | `Use`, `Result`, `Needs`, then one line per other public function, labelled with its name. Each one only when it has content. |
| Other labels | None, except `Time` when the top comment has no room, and `Models` for the cross-reference of 8.2. |
| `Use` | **One line of code** in `\texttt`: the shortest full call, with the sizing (`g(n + 1)`, `Dinic d(n + 1)`). |
| Each line | One fact; at most **2 printed rows**. |
| The whole note | At most **5 labels and 8 printed rows**. Longer means part of it belongs in a text entry. |
| Sentences | A verb in every line; at most one semicolon; no arrow as a verb; no problem name as an explanation; an abbreviation is expanded where it first appears in the entry. |
| Limits | Numbers, not words: "sums below $9 \cdot 10^{18}$", not "may overflow". |
| A formula or recurrence | Displayed with `\[ .. \]`; every symbol in it is named in the line above. |
| Code words | `\texttt{..}`; an underscore is `\_`. |
| Space | Never pack the note to save space. If it does not fit the limits, ask what to move out. |

The macro `\fld` prints the label in bold and adds the colon.

### 8.4 Text entries

Text sections (contest checklists, formula sheets, flow models) follow these rules.

| Rule | Value |
|---|---|
| Structure | `\topic{Heading}` per topic. Under it: **one plain sentence** that says when to use it. Then the steps or the formula. |
| Steps of a construction | A `facts` list, one step per item, in the order you would type them. |
| Formulas | Displayed, one per line. Inline math only for single symbols and short terms; at most **2 inline formulas per sentence**. |
| After a formula | One line that names the symbols, if any is new. |
| Value lists (Catalan, Bell, primes) | A ruled table with a header row when there are more than about 8 values, not a run of numbers in a sentence. |
| Checklists | `checks`, grouped under `\topic` headings; an item is a short imperative. |
| Taxonomies ("noise → remedy") | A two-column table with headings, not arrows in a list. |
| A worked example per model | One line with numbers: "edge $u \to v$ with bounds $[2, 5]$: capacity 3, $d_v$ += 2, $d_u$ −= 2". |
| Density | No paragraph longer than **3 printed rows**; no sentence with more than one semicolon; never two topics under one heading. |
| Font | `\small` with `\tightdisplay`. |
| Page count | Never a reason to merge paragraphs. If it is too long, ask what to cut. |

### 8.5 Cross-references

Section numbers are never typed by hand.

| Rule | Example |
|---|---|
| Every entry has a label equal to its file name without the extension. It is written directly after the `\cpp` call, after the note if there is one. | `\cpp[4]{SCC}{scc.hpp}[ .. ]\label{scc}` |
| An entry without code yet takes one too. | `\entry{Floor sum}\label{floorsum}` |
| A note or a text refers to an entry with `\S\ref`. | `uses SCC (\S\ref{scc})` |
| A code comment names the entry, not a number. | `// needs isPrime (Miller–Rabin)` |

This needs no change to the class file. See [Appendix C](#appendix-c-labels-and-the-class-file).

---

## 9. Performance

Entries are **as fast as possible**. When speed and brevity conflict, speed wins. The code
still has to be readable: a trick comes with a comment that says what it does.

| Rule | Example |
|---|---|
| Take the fastest known variant that fits an entry. | Miller–Rabin with the 7-base set `{2, 325, 9375, 28178, 450775, 9780504, 1795265022}`, not 12 prime bases |
| Port the tuned original, not a textbook outline. | Dinic with scaling, BFS from the sink and multi-push DFS; Pollard with batched gcds |
| Flat storage in hot paths. | flows: `vector<Edge> edges` with `edges[id ^ 1]` as the reverse, plus `vector<vector<int>> adj` of ids |
| Fixed inner arrays inside nodes. | `int nxt[26]` in an Aho–Corasick node |
| No allocation in an inner loop. Size vectors once. | build `vector<int> buf(n)` outside the loop |
| No `std::function`, no `map` or `set` where an array or a sorted vector works. | |
| Pass containers by `const &`. Never by value into a recursion. | `void dfs(int u, const vector<vector<int>> &g)` |
| Recursive helpers have few parameters and few locals. | A small frame makes depth 10^6 affordable. |
| `vector<char>` for flags in inner loops. | |
| Tables that must persist: `static` inside the function, grown on demand. | roots of unity in FFT and NTT |
| `'\n'`, never `endl`. | |

Limits on this rule:

- The graph types of 6.3 are fixed, even though a flat edge array would be faster. Passing the
  graph the statement gives matters more.
- No `#pragma` in an entry. Pragmas belong to the template.
- **Measure, do not guess.** A change that makes an entry longer or less plain needs a measured
  gain on the entry's judge test. The report gives both times.

---

## 10. Tests

### 10.1 Where and how

A test is `testing/<section>/<name>.cpp`, registered in `testing/all.py`.

```sh
cd testing && python3 all.py <section> <name>
```

| Rule | Why |
|---|---|
| **Library Checker first.** Use another judge (AOJ and others) only when Library Checker has no problem for the entry. Use a self-made stress test against a brute force only when no judge has one, or in addition. | Judge data is stronger than anything written by hand. |
| The first line of the test is `#include "../../tex/contents/<section>/<file>"`. Nothing comes before it except a comment. | It proves that the entry needs no setup: no macro, no `using`, no global. |
| The test includes the printed file itself, unchanged. | What is tested is what is typed. |
| A test that uses `assert` has `#include <cassert>` as line 2. | The template does not include it, and GCC 16's `bits/stdc++.h` does not either. |
| **Conversions live in the test.** Library Checker is 0-based: the test adds 1 when it reads and subtracts 1 when it prints. The entry never converts. | The entry stays as the contestant will use it. |
| The test follows this style too (60 columns, spacing, `FOR`). | Tests are read as usage examples. |
| Do not change a time limit or a registration line that the spec gives. | |

### 10.2 The whole interface

A test exercises **everything the interface promises**, not only what the judge asks for.

- Every public member and method is used by at least one test. If the judge problem needs only
  `cnt`, the test also asserts on `comp`.
- Every promise in the top comment and the note has a check. A promise the judge data never
  reaches gets a **self-check with `assert`, at the top of `main`, before the input is read**.

Examples from the trial, where the interface promised these and no outlined test reached them:

| Entry | Self-check |
|---|---|
| SCC | `SCC(vector<vector<int>>(1)).cnt == 0` (n = 0); `scc = SCC(g2);` compiles (reassignment) |
| Lowlink | a hand-made case with edge id 0 as a real edge, an unused id, a self-loop stored once, and garbage in `g[0]` |
| Li Chao | one `addSegment` with `l > r`, which must do nothing |

Standard cases to consider for every entry: the empty and the smallest input, vertex 0 left
untouched, self-loops and multi-edges, isolated vertices, two objects alive at once.

### 10.3 Checks that are not committed

A check that was run but is not in the repo proves nothing later. So:

- if it fits in about 15 lines, put it in the test file as a self-check;
- otherwise list it in the report **with its source**.

### 10.4 Brute forces

A brute force is the simplest correct program, written once and reused. For example one small
struct that labels components by BFS, used for every "are these connected without x"
question. No dead branches and no two branches that do the same thing: a test is also read.

---

## 11. Porting from reference notebooks

Most entries are ports from kactl, el-vasito or icpc-teamnote.

> **Restyle the outside, keep the inside.**

### 11.1 Order of work

1. Port the source faithfully and make the test pass.
2. Restyle, then run the test again.

A failure after step 2 is a restyling mistake, not a porting mistake. If an own design fails
its test three times, stop: port the spec's primary reference faithfully, keeping the spec's
public interface.

### 11.2 Change

| In the source | House style |
|---|---|
| globals next to a free function; `MAXN` arrays; `template<size_t N>` sizes | members of one struct, `vector`s sized in the constructor |
| a callback for the output | return it, or public members |
| `template<class C>` cost types, `ull` values | `ll` |
| public names in snake_case or `AddEdge` | camelCase, with the shared names of section 2 |
| a public base of 0, or half-open ranges | section 6: convert at the public boundary only |
| `rep(i, a, b)` (kactl), `fore(i, a, b)` (el-vasito): exclusive | `FOR(i, a, b - 1)` |
| `SZ(x)` | `sz(x)` (template) |
| `all(x)`, `vi`, `pii`, `sz(x)` | keep: the template defines them (Appendix A) |
| `pb`, `fst`, `snd` | `push_back`, `fi`, `se` |
| `) {`, 2-space indent, lines over 60 columns | section 1 |
| several statements packed on a line; a call or an assignment inside a condition | split them (1.4, 1.7) |
| the kactl header (`Description`, `Time`, `Usage`) | base, complexity and depth → top comment; the rest → the note |
| a random generator at file scope, leaked `#define`s | a deterministic walk, or a `static` generator inside the function |

### 11.3 Keep

- the names of local variables and work arrays;
- the index base the algorithm uses **inside**;
- the order of statements, and comma-joined statements that are one step;
- every constant and its reason (`t = 30`, `prd = 2`): add the comment if the source has none;
- the tuned parts. Section 9: the port must not be slower than its source.

Why: the port can then be compared with its tested source line by line. Renaming locals is
where porting bugs come from.

### 11.4 Check by hand

- el-vasito's `fore` evaluates its bound **once**. `FOR` evaluates it on every iteration.
  Look at every `fore` loop whose body changes the container or a variable in the bound, and
  write those as a plain `for` (7.3).
- A source that is 0-based inside and gets 1-based vertices from the caller: the simplest
  correct port sizes its arrays by `g.size()` and starts its vertex loops at 1. Do not add
  `- 1` at every access.

---

## 12. Checklist before reporting

Run this on every `.hpp` and every test you wrote. Each line is a yes or no.

**Mechanical**

- [ ] The width check of 1.1 prints nothing.
- [ ] `head -3 FILE` shows the three header lines; line 3 is blank.
- [ ] Tabs for indentation, no trailing blanks, no blank line after line 3.
- [ ] This prints nothing, or every hit has a reason from section 4:
      `grep -nwE 'EFOR|EREV|namespace|class|private|inline|init|memset|function|define|typedef' FILE`
      (`class` in `template<class T>`, and a `typedef` in an edit block, are the expected hits).
- [ ] The entry compiles alone, without a warning, under both standards:

      ```sh
      for s in c++17 gnu++20; do
        printf '#include "tex/contents/SEC/FILE"\nint main(){}\n' \
          | ${CXX:-g++-16} -std=$s -Wall -Wextra -Wshadow -fsyntax-only -x c++ -
      done
      ```

**Layout and names**

- [ ] `if(`, `for(`, `while(`, `}else{`, `v: g`, `'\n'`.
- [ ] Tight arithmetic only inside `[]` or as a one-operator call argument.
- [ ] No recursive call or assignment inside an argument or a condition.
- [ ] No name collides with the template ([Appendix A](#appendix-a-what-the-template-provides)).
- [ ] Shared names used: `stk`, `timer`, `num`, `low`, `comp`, `cnt`, `dist`, `par`, `get`,
      `update`, `addEdge`.

**Interface**

- [ ] The shape is the one of section 4.1, and the spec's signatures are exact.
- [ ] No reference member; `x = X(..)` compiles; the graph is the last parameter of `dfs`.
- [ ] Graph entries use `g.size()`, never touch vertex 0, and give ids from 1.
- [ ] `ll` for values; `__int128` only where the spec says; the shared `oo`, or an own
      `static constexpr` one with the reason.
- [ ] Every `.size()` in a loop bound is cast to `int`; no `FOR` over a growing container.

**Comment and note**

- [ ] The top comment has 1 to 3 lines: what, base, complexity, **recursion depth**.
- [ ] The note is the spec's text, in the format of 8.3: labels `Use`, `Result`, `Needs`,
      then function names; at most 5 labels and 8 printed rows; nothing from the top comment.
- [ ] The entry has `\label{<file>}`; no section number is typed by hand.

**Tests**

- [ ] The test starts with the `#include`; conversions are in the test.
- [ ] Every public member and every promise of the interface has a check.
- [ ] `python3 all.py <section> <name>` passes.
- [ ] The PDF builds. Never commit `tex/main.pdf`; the coordinator restores it.

**The report contains**

1. the last output lines of every passing test run, pasted;
2. every failed run and its cause;
3. every deviation from the spec or from this file, with the reason; "none" if none;
4. the printed line count of each `.hpp`: `tail -n +4 FILE | wc -l`;
5. the line `Output written on main.pdf (..)` from the build. Not latexmk's last line, which
   only says "up-to-date";
6. every check that was run and not committed, with its source (10.3);
7. for a speed change, the times before and after (section 9);
8. what was not checked.

---

## Appendix A. What the template provides

`tex/contents/contest/template.hpp`, after it is updated to match this file.

| Name | Kind | Note |
|---|---|---|
| `ll`, `ld` | typedef | `long long`, `long double` |
| `oo` | `const ll oo = 1e18;` | **new** |
| `PQMax<T>`, `PQMin<T>` | alias | |
| `maximize(a, b)`, `minimize(a, b)` | function | returns `bool`: whether `a` changed. Relax with `if(minimize(d[v], d[u] + w)) par[v] = u;`; prefer them to `a = min(a, b)` |
| `fi`, `se` | macro | `first`, `second` |
| `FOR(i, a, b)`, `REV(i, b, a)` | macro | **changed**: `auto`, no type argument (7.1) |
| `testBit`, `flipBit`, `cntBit`, `log2` | macro | `log2(n)` is the integer log of a positive integer |
| `CURRENT_TIMESTAMP()`, `randomize` | macro | `randomize;` declares `mt19937_64 mt` |
| `dbg(..)` | macro, only under `-DLOCAL` | **new** (ToC 0.1); never used in an entry |
| `pii`, `pll` | typedef | **new**, next to `ll`; used with `.fi`/`.se` (section 5) |
| `vi`, `vll`, `all(x)`, `sz(x)` | typedef, macro | **new**, under `/* in this template */` at the end; usable in bodies, public signatures still spell types out |

Changes to make in the template:

- **remove** `#include <cassert>` (printed code has no `assert`; GCC 16's `bits/stdc++.h` does not provide it, so a test that asserts includes `<cassert>` itself), both `read()` overloads (Fast IO,
  §0.3, replaces them), `EFOR`, `EREV`;
- **remove** the "remember to fill in `#define MAX` / `#define MOD`" lines of the closing
  comment. Following them breaks entries. The `main` reminder stays;
- **change** `FOR` and `REV` to the two lines of 7.1;
- **add** `const ll oo = 1e18;` after the typedefs.

Because of the macros, an entry must not:

- use any name in the table as its own, nor `mt`;
- call `log2` on a `double`, or write `std::log2`. The macro replaces both. The first truncates
  silently, the second does not compile.

## Appendix B. Build

The debug build stays at `-O2`, with sanitizers, `-Wshadow` and `-D_GLIBCXX_ASSERTIONS`:

```bash
g++ -std=gnu++20 -Wall -Wextra -Wshadow -O2 -g -DLOCAL \
	-D_GLIBCXX_ASSERTIONS -fsanitize=address,undefined \
	-fno-sanitize-recover=all $1.cpp -o $1
```

| Flag | Why |
|---|---|
| `-O2` | The "maybe uninitialised" warning needs optimisation, and the build runs faster. |
| `-Wshadow` | Catches an inner `n` that hides an outer one. Entries must be silent under it (4.3). |
| `-D_GLIBCXX_ASSERTIONS` | Catches `v[i]` with `size <= i < capacity`, which ASan cannot see. Not `-D_GLIBCXX_DEBUG`: that one makes `lower_bound` linear. |
| `-fno-sanitize-recover=all` | Without it a signed overflow only prints a line and the run goes on. |

Printed next to it:

> gdb at `-O2` shows variables as `<optimized out>` and steps jump around. To step through a
> program, compile by hand with `-O0 -g`.

Timing, and the last sample run before a submit, use a second build without sanitizers and
without `-DLOCAL`: `g++ -std=gnu++20 -O2 -g $1.cpp -o $1`.

Entries stay valid C++17 and are checked under both standards (section 12).

## Appendix C. Labels and the class file

**No change to `tex/hesllnotebook.cls` is needed.**

`\othercodes`, and so `\cpp`, calls the heading command at the outer level, so the current
label after the call is still the entry's own number. Checked with a small document that uses
the class as it is, under `\threelevels`. All four placements gave the right number
(`\S\ref` printed §1.1.1 to §1.1.4) with no warning:

| Placement | Works |
|---|---|
| `\cpp[4]{A}{a.hpp}\label{a}` (no note) | yes |
| `\cpp[4]{B}{b.hpp}[ .. ]\label{b}` (after the note) | yes |
| `\cpp[4]{C}{c.hpp}[\label{c} .. ]` (inside the note) | yes |
| `\entry{E}\label{e}` | yes |

The rule in 8.5 uses the first two forms, because one place then fits entries with and without
a note.

Optional, **not checked**: to make the label automatic, append `\label{#5}` to the heading
line of `\othercodes`:

```tex
\csname\entrylevel\endcsname[#3]{#3\entryhash}\gdef\entryhash{}\label{#5}
```

The key is then the file name **with** its extension (`\S\ref{scc.hpp}`), and a file printed
twice gives a "multiply defined label" warning. The hand-written label avoids both.

## Appendix D. Where the trial findings went

The ten items of "What to change in RULES.md and the specs" in the trial results:

| # | Finding | Now in |
|---|---|---|
| 1 | keyword spacing; `}else{`; `v: g`; `'\n'` | 1.2 |
| 2 | tight arithmetic was read broadly | 1.3 |
| 3 | plain over short; the line range is a ceiling | 1.6 |
| 4 | graph parameter last, `stk`, constructor placement | 2 (shared names), 4.3 |
| 5 | `FOR` with a moving bound | 7.3: a plain `for` |
| 6 | recursion depth dropped from a top comment | 3.2, and the checklist |
| 7 | checks that were run but not committed | 10.3 |
| 8 | report the `Output written on main.pdf` line; no `git checkout tex/main.pdf` by the implementer | 12 |
| 9 | lowlink: id 0, unused id, loop stored once, garbage in `g[0]` | 10.2 |
| 10 | SCC: n = 0 and reassignment; Li Chao: `l > r` | 10.2 |

Items 6, 9 and 10 also bind the spec writer: a spec repeats the depth line in its "must say"
list, and lists the self-checks of 10.2 in its test outline.

---

## Open

1. **`-std` in the printed build line.** `gnu++20` (World Finals) or `c++17` (today's
   `command.sh`). The regional judge's flags are unknown until the practice session.
2. **MCMF and SPFA.** Whether MCMF keeps its own queue loop or shares one with the SPFA entry
   is left for the interface design pass.
3. **Modint.** Runtime modulus as now, or `template<int MOD>`. It decides the signatures of
   NTT, Gauss and FPS.
4. **`assert` in printed code.** None so far: preconditions are in `\fld{Needs}` and the tests
   assert. Say so if a cheap `assert` should be printed where a wrong call fails silently.
5. **A test that includes every entry** (`testing/includeall.cpp`), to catch name collisions.
   Suggested, not built.
