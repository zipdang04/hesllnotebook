#!/bin/bash
set -u
CXX=${CXX:-g++}
TPL="$(pwd)/../tex/contents/contest/template.hpp"
W=$(mktemp -d)
trap 'rm -rf "$W"' EXIT

fail(){ echo "FAIL: $*"; exit 1; }
ok(){ echo "ok: $*"; }

# Check 1: compilation warning on narrowing
cat <<EOF > "$W/warn_test.cpp"
#include "$TPL"
int main() {
    int x = 0;
    ll y = 5;
    maximize(x, y);
    maximize(x, 1);
    return 0;
}
EOF

# Compile and capture stderr
OUT=$($CXX -std=c++17 -c "$W/warn_test.cpp" -o "$W/warn_test.o" 2>&1 || fail "compilation failed")

# Check that "may not fit" appears exactly once
CNT=$(echo "$OUT" | grep -c "may not fit" || true)
if [ "$CNT" -ne 1 ]; then
    fail "expected 1 warning containing 'may not fit', got $CNT:\n$OUT"
fi

# Line of `maximize(x, y);` is line 5 in warn_test.cpp
if ! echo "$OUT" | grep -E "warn_test\.cpp:5:" >/dev/null; then
    fail "warning did not point to line 5 (maximize(x, y)):\n$OUT"
fi

ok "narrowing warning check passed"

# Check 2: runtime assert with -DLOCAL on narrowing overflow
cat <<EOF > "$W/assert_test.cpp"
#include "$TPL"
int main() {
    int x = 0;
    ll y = 1000000000000LL;
    maximize(x, y);
    return 0;
}
EOF

# Build with -DLOCAL: must fail at runtime (assert)
$CXX -std=c++17 -DLOCAL -Wno-deprecated-declarations "$W/assert_test.cpp" -o "$W/run_local" || fail "compilation with -DLOCAL failed"
if "$W/run_local" 2>/dev/null; then
    fail "expected abort under -DLOCAL, but exited 0"
fi

# Build without -DLOCAL: must exit 0
$CXX -std=c++17 -Wno-deprecated-declarations "$W/assert_test.cpp" -o "$W/run_nolocal" || fail "compilation without -DLOCAL failed"
if ! "$W/run_nolocal"; then
    fail "expected exit 0 without -DLOCAL, but failed"
fi

ok "assert -DLOCAL vs non-LOCAL passed"
