set -u
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
ROOT=$(cd .. && pwd)
CMD=$ROOT/tex/contents/contest/command.sh
CPP=$(echo "$CXX" | sed s/g++/cpp/)
command -v "$CPP" > /dev/null || CPP=cpp
mkdir $tmp/bin
printf '#!/bin/sh\nexec "%s" "$@"\n' "$CXX" > $tmp/bin/g++
printf '#!/bin/sh\nexec "%s" "$@"\n' "$CPP" > $tmp/bin/cpp
chmod +x $tmp/bin/g++ $tmp/bin/cpp
PATH="$tmp/bin:$PATH"
eval "$(awk '/^(c|o|h)\(\)\{/,/; \}$/' $CMD)"
type c o h > /dev/null || { echo "no functions"; exit 1; }
hh(){ h 2> /dev/null; }
fail(){ echo "FAIL: $1"; exit 1; }
cd "$tmp"
cat > a.cpp <<'F'
#include <bits/stdc++.h>
int main(){
	long long x, y; std::cin >> x >> y;
	std::cout << x + y << "\n";
#ifdef LOCAL
	std::cerr << "L";
#endif
}
F
c a || fail "c a"
[ "$(echo 2 3 | ./a 2> e)" = 5 ] || fail "c a output"
[ "$(cat e)" = L ] || fail "c a stderr"
o a || fail "o a"
[ "$(echo 2 3 | ./a 2> e)" = 5 ] || fail "o a output"
[ ! -s e ] || fail "o a stderr"
cat > b.cpp <<'F'
#include <bits/stdc++.h>
int main(){
	int x = INT_MAX; volatile int y = 1;
	std::cout << x + y << "\n";
}
F
c b || fail "c b"
./b > /dev/null 2>&1 && fail "ubsan exit"
o b || fail "o b"
./b > /dev/null 2>&1 || fail "b judge exit"
cat > d.cpp <<'F'
#include <bits/stdc++.h>
int main(){
	std::vector<int> v(3); v.reserve(10);
	return v[5];
}
F
c d || fail "c d"
./d > /dev/null 2>&1 && fail "assertions exit"
f=$ROOT/tex/contents/contest/template.hpp
H=$(cd $ROOT/tex && sh entryhash.sh \
	contents/contest/template.hpp 1 2> /dev/null)
[ "$(hh < $f)" = "$H" ] || fail "hash vs entryhash"
[ ${#H} = 6 ] || fail "hash length"
{ echo; printf '\t// c\n'; cat $f; printf '\n\t\n'; } \
	> t1.hpp
[ "$(hh < t1.hpp)" = "$H" ] || fail "hash not blank-proof"
sed 's/oo = 1e18/oo = 2e18/' $f > t2.hpp
[ "$(hh < t2.hpp)" != "$H" ] || fail "hash ignores a token"
H2=$(cd $ROOT/tex && sh entryhash.sh \
	contents/contest/template.hpp 4 20 2> /dev/null)
[ "$(sed -n 4,20p $f | hh)" = "$H2" ] || fail "hash range"
grep -q 'diff -b out ans' $CMD || fail "diff -b"
n=$(grep -c -- -std=gnu++20 $CMD)
[ $n -ge 2 ] || fail "std"
head -1 $CMD | grep -q '^ulimit -s 1048576' || fail ulimit
