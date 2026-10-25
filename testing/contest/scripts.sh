#!/bin/bash
# Self-test for contest/{stress.sh,hash.sh,gen.cpp,checker.cpp} and tex/entryhash.sh.
# Run from testing/: bash contest/scripts.sh   (CXX = compiler, set by all.py)
set -u
TEX=$(cd "$(dirname "$0")/../../tex" && pwd); C=$TEX/contents/contest
CXX=${CXX:-g++}; export CXX
W=$(mktemp -d); trap 'rm -rf "$W"' EXIT
fail(){ echo "FAIL: $*"; exit 1; }
ok(){ echo "ok: $*"; }
build(){ $CXX -std=c++17 -O2 -x c++ - -o "$1" || fail "compile $1"; }   # source on stdin
# sanitizers: keep stress.sh's default if this compiler supports them, else disable
echo 'int main(){}' | $CXX -fsanitize=address,undefined -x c++ - -o "$W/san" 2>/dev/null && "$W/san" || export SAN=
# gcc's cpp for hash.sh (Apple's cpp is clang and rejects -fpreprocessed)
mkdir "$W/bin"; for c in cpp-16 cpp-15 cpp-14 cpp; do command -v $c > /dev/null && break; done
ln -s "$(command -v $c)" "$W/bin/cpp"; export PATH="$W/bin:$PATH"

# ---------- hash.sh ----------
H(){ sh "$C/hash.sh"; }
printf 'int f(int a){\n\treturn a + 1; // inc\n}\n' > "$W/a.cpp"
printf '/* c */ int f(int a)\n{ return a+1;\n\n}   \n' > "$W/b.cpp"
printf 'int f(int a){ return a + 2; }\n' > "$W/c.cpp"
ha=$(H < "$W/a.cpp"); [ "$ha" = "$(H < "$W/a.cpp")" ] || fail "hash not stable"
[[ $ha =~ ^[0-9a-f]{6}$ ]] || fail "hash format '$ha'"
[ "$ha" = "$(H < "$W/b.cpp")" ] || fail "hash depends on whitespace/comments"
[ "$ha" != "$(H < "$W/c.cpp")" ] || fail "hash ignores a token change"
F=$TEX/contents/contest/gen.cpp
[ "$(cd "$TEX" && sh entryhash.sh "$F" 2)" = "$(tail -n +2 "$F" | H)" ] || fail "entryhash.sh != hash.sh"
[ "$(cd "$TEX" && sh entryhash.sh "$F" 2 4)" = "$(sed -n 2,4p "$F" | H)" ] || fail "entryhash.sh range"
ok "hash.sh / entryhash.sh"

# ---------- gen.cpp + checker.cpp (built from the notebook files) ----------
$CXX -std=c++17 -O2 "$C/gen.cpp" -o "$W/gen" || fail "compile gen"
$CXX -std=c++17 -O2 "$C/checker.cpp" -o "$W/checker" || fail "compile checker"
build "$W/valid" <<'CPP'
#include <bits/stdc++.h>
using namespace std;
vector<int> p; int f(int x){ return p[x] == x ? x : p[x] = f(p[x]); }
int main(){ int n, u, v, c = 0; cin >> n; if (n < 1 || n > 8) return 1; p.resize(n + 1); iota(p.begin(), p.end(), 0);
	while (cin >> u >> v){ if (u < 1 || u > n || v < 1 || v > n || f(u) == f(v)) return 1; p[f(u)] = f(v); c++; }
	return c == n - 1 ? 0 : 1; }
CPP
diff <("$W/gen" 5) <("$W/gen" 5) > /dev/null || fail "gen not reproducible"
d=0; for i in 6 7 8 9 10; do cmp -s <("$W/gen" 5) <("$W/gen" $i) || d=1; done; [ $d = 1 ] || fail "gen ignores seed"
for i in $(seq 1 300); do "$W/gen" $i | "$W/valid" || fail "gen seed $i: not a tree on 1..n, n in [1,8]"; done
ok "gen reproducible, valid trees"

# ---------- stress mode: max-degree vertex ----------
S=$W/stress; mkdir "$S"; cp "$C/stress.sh" "$S"; cp "$W/gen" "$W/checker" "$S"; cd "$S"
deg='#include <bits/stdc++.h>
using namespace std;
int main(){ int n, u, v; cin >> n; vector<int> d(n + 1); for (int i = 1; i < n; i++) cin >> u >> v, d[u]++, d[v]++;'
echo "$deg int b = 1; for (int i = 1; i <= n; i++) if (d[i] > d[b]) b = i; cout << b << endl; }" > brute.cpp
echo "$deg int b = 1; for (int i = 1; i <= n; i++) if (d[i] >= d[b]) b = i; cout << b << endl; }" > sol.cpp   # other valid answer
bash stress.sh -c ./checker -n 100 > log 2>&1 || { cat log; fail "correct sol rejected by checker"; }
grep -q "OK" log || fail "no OK line"; [ ! -d tests ] || fail "tests/ created on success"
bash stress.sh -n 100 > log 2>&1 && fail "diff -w accepted a different valid answer"
seed=$(sed -n 's/^seed \([0-9]*\): WA.*/\1/p' log); [ -n "$seed" ] || { cat log; fail "no 'seed N: WA' line"; }
for e in in out ans; do [ -s tests/fail_$seed.$e ] || fail "tests/fail_$seed.$e missing"; done
cmp -s tests/fail_$seed.in <(./gen $seed) || fail "saved input is not gen $seed"
rm -rf tests
echo "$deg int b = 1; for (int i = 1; i < n; i++) if (d[i] > d[b]) b = i; cout << b << endl; }" > sol.cpp   # misses vertex n
sleep 1; touch sol.cpp
bash stress.sh -c ./checker -n 100 -s 7 > log 2>&1 && fail "wrong sol passed"
seed=$(sed -n 's/^seed \([0-9]*\): WA.*/\1/p' log); [ -n "$seed" ] && [ "$seed" -ge 7 ] || { cat log; fail "bad seed line"; }
./checker tests/fail_$seed.in tests/fail_$seed.out tests/fail_$seed.ans 2> /dev/null && fail "saved case passes checker"
good='int b = 1; for (int i = 1; i <= n; i++) if (d[i] > d[b]) b = i; cout << b << endl; }'
echo "$deg if (n == 3) abort(); $good" > sol.cpp
sleep 1; touch sol.cpp; rm -rf tests
bash stress.sh -c ./checker -n 100 > log 2>&1 && fail "crashing sol passed"
grep -q "^seed [0-9]*: RE" log || { cat log; fail "crash not reported as RE"; }
echo "$deg if (n == 3) for (volatile int z = 0;; z++); $good" > sol.cpp
sleep 1; touch sol.cpp; rm -rf tests
bash stress.sh -c ./checker -n 100 -t 1 > log 2>&1 && fail "looping sol passed"
grep -q "^seed [0-9]*: TLE" log || { cat log; fail "loop not reported as TLE"; }
ok "stress mode (checker, diff -w, WA/RE/TLE, fail files, -s)"
export SAN=   # sanitizer startup is slow (macOS); the modes below are built without it

# ---------- tests mode ----------
T=$W/tests; mkdir -p "$T/tests"; cp "$C/stress.sh" "$T"; cd "$T"
echo '#include <bits/stdc++.h>
#ifndef STRESS
#error stress.sh must build with -DSTRESS
#endif
int main(){ long long x; std::cin >> x; if (x < 0) abort(); for (volatile int z = 0; x == 0; z++); std::cout << 2 * x << "\n"; }' > sol.cpp
echo 3 > tests/a.in; echo '6  ' > tests/a.ans
echo 4 > tests/b.in; echo 9 > tests/b.ans
echo -1 > tests/c.in; echo 0 > tests/c.ans
echo 0 > tests/d.in; echo 0 > tests/d.ans
bash stress.sh tests -t 1 > log 2>&1
for l in "tests/a.in PASS" "tests/b.in FAIL" "tests/c.in RE" "tests/d.in TLE"; do grep -q "^$l" log || { cat log; fail "tests mode: no '$l'"; }; done
ok "tests mode (PASS/FAIL/RE/TLE)"

# ---------- interactive mode: guess x in [1, 100], <= 7 queries ----------
I=$W/inter; mkdir "$I"; cp "$C/stress.sh" "$I"; cd "$I"
echo '#include <bits/stdc++.h>
int main(int, char **argv){ int x = atoi(argv[1]) % 100 + 1, g, q = 0;
	while (std::cin >> g){ if (++q > 7) return 1; if (g == x){ std::cout << "=" << std::endl; return 0; } std::cout << (g < x ? "<" : ">") << std::endl; }
	return 1; }' > interactor.cpp
guess(){ echo "#include <bits/stdc++.h>
int main(){ int l = 1, r = $1; std::string s; while (l <= r){ int m = (l + r) / 2; std::cout << m << std::endl; std::cin >> s; if (s == \"=\") return 0; if (s == \"<\") l = m + 1; else r = m - 1; } }" > sol.cpp; sleep 1; touch sol.cpp; }
guess 100; bash stress.sh interactive -n 100 > log 2>&1 || { cat log; fail "correct interactive sol rejected"; }
guess 99; bash stress.sh interactive -s 90 -n 20 > log 2>&1 && fail "wrong interactive sol passed"
grep -q "^seed 99: interactor failed" log || { cat log; fail "interactive: wrong seed line"; }
echo 'int main(){ for (;;); }' > sol.cpp; sleep 1; touch sol.cpp
timeout_start=$SECONDS; bash stress.sh interactive -n 1 -t 1 > log 2>&1 && fail "silent sol passed"
[ $((SECONDS - timeout_start)) -le 10 ] || fail "interactive mode hangs"
ok "interactive mode"

# ---------- multipass mode: send x in [0, 255] as 8 (index, bit) lines, noise shuffles them ----------
M=$W/multi; mkdir "$M"; cp "$C/stress.sh" "$M"; cd "$M"
echo '#include <bits/stdc++.h>
int main(int, char **argv){ std::cout << "1 " << atoi(argv[1]) * 37 % 256 << "\n"; }' > gen.cpp
echo '#include <bits/stdc++.h>
using namespace std;
int main(int, char **argv){ ifstream p(argv[3]); vector<string> l; string s; while (getline(p, s)) if (s.size()) l.push_back(s);
	shuffle(l.begin(), l.end(), mt19937(atoi(argv[1]))); cout << 2 << "\n"; for (auto &x: l) cout << x << "\n"; }' > noise.cpp
echo '#include <bits/stdc++.h>
using namespace std;
int main(int, char **argv){ ifstream in(argv[1]), out(argv[2]); int t, x, y; in >> t >> x; return out >> y && x == y ? 0 : 1; }' > chk.cpp
pass(){ echo "#include <bits/stdc++.h>
using namespace std;
int main(){ int t, x = 0, i, b; cin >> t; if (t == 1){ cin >> x; for (i = 0; i < 8; i++) cout << i << ' ' << (x >> i & 1) << '\n'; return 0; }
	for (int k = 0; cin >> i >> b; k++) x |= b << $1; cout << x << '\n'; }" > sol.cpp; sleep 1; touch sol.cpp; }
pass i; bash stress.sh multipass -c ./chk -n 200 > log 2>&1 || { cat log; fail "correct multipass sol rejected"; }
pass k; bash stress.sh multipass -c ./chk -n 200 > log 2>&1 && fail "wrong multipass sol passed"
grep -q "^seed [0-9]*: WA" log || { cat log; fail "multipass: no seed line"; }
ok "multipass mode"
echo "ALL OK"
