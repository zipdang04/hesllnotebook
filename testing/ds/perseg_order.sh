#!/bin/bash
set -eu
# Text sums (non-commutative) through an edited copy of PerSeg: ll is redefined.
dir=$(mktemp -d)
trap 'rm -rf "$dir"' EXIT
cat > "$dir/order.cpp" <<'EOF'
#include "ROOT/tex/contents/contest/template.hpp"
#include <cassert>

struct Text{
	string s;
	Text(int x = 0): s(x ? 1 : 0, char('0' + x)){}
	Text(string x): s(x){}
};
Text operator+(Text x, Text y){ return x.s + y.s; }
#define ll Text
#include "ROOT/tex/contents/ds/perseg.hpp"
#undef ll

int main(){
	PerSeg s({{1}, {2}, {3}, {4}});
	int v = s.update(0, 1, {5});
	assert(s.get(0, 1, 3).sum.s == "234");
	assert(s.get(v, 0, 2).sum.s == "153");
	cout << "OK: ordered combine" << '\n';
}
EOF
sed "s#ROOT#$(cd .. && pwd)#" "$dir/order.cpp" > "$dir/o.cpp"
"$CXX" -std=c++17 -O2 -Wall -Wextra -Wshadow "$dir/o.cpp" -o "$dir/order"
"$dir/order"
