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
	for line in lines[1:]:
		u, v = map(int, line.split())
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
print('300 seeds: valid trees, both directions, n = 1')
PY
[ $? = 0 ] || fail "generator properties"
cat "$tmp/log"
echo 'deterministic in c++17 and gnu++20; no warnings'
