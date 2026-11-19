set -u
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
fail(){ echo "FAIL: $1"; exit 1; }
source=../tex/contents/contest/gen.cpp
for std in c++17 gnu++20; do
	"$CXX" -std=$std -O2 -Wall -Wextra -Wshadow \
		"$source" -o "$tmp/gen" 2> "$tmp/err" ||
		fail "compile $std"
	[ ! -s "$tmp/err" ] || fail "warnings $std"
	"$tmp/gen" 5 > "$tmp/first" || fail "seed 5"
	"$tmp/gen" 5 > "$tmp/second" || fail "seed 5 again"
	cmp -s "$tmp/first" "$tmp/second" ||
		fail "determinism $std"
done
python3 - "$tmp/gen" > "$tmp/log" 2>&1 <<'PY'
import subprocess
import sys

outputs = set()
up = down = single = False
labels = newFirst = newSecond = False
for seed in range(1, 301):
	text = subprocess.check_output([sys.argv[1], str(seed)])
	if seed <= 20:
		outputs.add(text)
	lines = text.splitlines()
	assert len(lines[0].split()) == 1, (seed, 'n tokens')
	n = int(lines[0])
	assert 1 <= n <= 8, (seed, 'n range')
	assert len(lines) == n, (seed, 'line count')
	par = list(range(n + 1))
	def root(u):
		while par[u] != u:
			u = par[u]
		return u
	seen = set()
	for k, line in enumerate(lines[1:]):
		u, v = map(int, line.split())
		if k:
			if u in seen:
				newSecond = True
			else:
				newFirst = True
		seen |= {u, v}
		labels |= max(u, v) != k + 2
		assert 1 <= u <= n and 1 <= v <= n, seed
		assert u != v, (seed, 'self loop')
		assert root(u) != root(v), (seed, 'cycle')
		par[root(u)] = root(v)
		up |= u < v
		down |= u > v
	assert len({root(u) for u in range(1, n + 1)}) == 1
	if n == 1:
		assert text == b'1\n', seed
		single = True
assert len(outputs) >= 2, 'distinct seeds'
assert up and down, 'both edge directions'
assert single, 'n = 1'
assert labels, 'labels are not shuffled'
assert newFirst and newSecond, 'edge ends are not swapped'
print('300 seeds: valid, shuffled labels, swapped ends')
PY
[ $? = 0 ] || fail "generator properties"
cat "$tmp/log"
echo 'deterministic in c++17 and gnu++20; no warnings'
cat > "$tmp/width.cpp" <<'F'
#define main genMain
#include "gen.cpp"
#undef main
int main(){
	rng.seed(1);
	for(int i = 0; i < 1000; i++){
		long long x = rnd(0, LLONG_MAX - 1);
		long long y = rnd(LLONG_MIN + 2, 0);
		long long z = rnd(-3, 3);
		if(x < 0 || y > 0 || z < -3 || z > 3) return 1;
	}
}
F
"$CXX" -std=gnu++20 -I../tex/contents/contest \
	-Wno-return-type -fsanitize=undefined \
	-fno-sanitize-recover=all \
	"$tmp/width.cpp" -o "$tmp/width" || fail "compile width"
"$tmp/width" || fail "rnd width"
echo 'rnd: width up to 2^63 - 1 is fine'
