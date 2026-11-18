# test of tex/contents/contest/stress.sh
set -u
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
ROOT=$(cd .. && pwd)
script=$ROOT/tex/contents/contest/stress.sh
fail(){ echo "FAIL: $1"; exit 1; }
mkdir "$tmp/bin"
cat > "$tmp/bin/g++" <<'F'
#!/bin/sh
exec "$CXX" "$@"
F
if ! command -v timeout > /dev/null; then
	cat > "$tmp/bin/timeout" <<'F'
#!/usr/bin/perl
my $t = shift; my $pid = fork;
if(!$pid){ exec @ARGV or exit 127; }
$SIG{ALRM} = sub { kill 'KILL', $pid; exit 124; };
alarm $t;
waitpid $pid, 0;
exit($? & 127 ? 128 + ($? & 127) : $? >> 8);
F
fi
chmod +x "$tmp/bin/"*
PATH="$tmp/bin:$PATH"
mkdir "$tmp/base"
cat > "$tmp/base/gen.cpp" <<'F'
#include <cstdio>
#include <cstdlib>
#include <random>
int main(int, char **argv){
	std::mt19937_64 rng(atoll(argv[1]));
	printf("%d %d\n", int(rng() % 9 + 1),
		int(rng() % 9 + 1));
}
F
cat > "$tmp/base/brute.cpp" <<'F'
#include <cstdio>
int main(){
	int a, b;
	if(scanf("%d%d", &a, &b) != 2)
		return 1;
	printf("%d\n", a + b);
}
F
newCase(){
	mkdir "$tmp/$1"
	cd "$tmp/$1" || fail "cd $1"
	cp "$tmp/base/"*.cpp .
	cp brute.cpp p.cpp
}
checkOK(){
	bash "$script" p "$@" > log 2>&1 || fail "$PWD"
	tail -1 log | grep -qx "OK $2 tests" || fail "OK"
	tail -1 log
}
checkFail(){
	bash "$script" p "$@" > log 2>&1
	[ $? = 1 ] || fail "failure exit"
	grep -q '^FAIL seed ' log || fail "failure seed"
	grep '^FAIL seed ' log
}
newCase correct
checkOK -n 50
newCase wrong
cat > p.cpp <<'F'
#include <cstdio>
int main(){
	int a, b; scanf("%d%d", &a, &b);
	printf("%d\n", a == 7 ? 0 : a + b);
}
F
checkFail -n 100
seed=$(sed -n 's/^FAIL seed //p' log)
saved=tests/fail_$seed
[ -f "$saved.in" ] || fail "saved input"
read -r a b < "$saved.in"
[ "$a" = 7 ] || fail "wrong seed input"
[ "$(cat "$saved.ans")" = "$((a + b))" ] ||
	fail "saved answer belongs to seed"
[ "$(cat "$saved.out")" = 0 ] || fail "saved output"
newCase missing
rm gen.cpp
checkFail -n 3
grep -q '^OK' log && fail "missing gen accepted"
newCase badgen
cat > gen.cpp <<'F'
#include <cstdio>
int main(){ puts("1 2"); return 3; }
F
checkFail -n 1
grep -q '^OK' log && fail "nonzero gen accepted"
newCase rejected
cat > chk.cpp <<'F'
int main(){ return 1; }
F
checkFail -n 1 -c chk
cmp out ans || fail "checker rejection fixture"
newCase tle
cat > p.cpp <<'F'
#include <cstdio>
int main(){
	int a, b; scanf("%d%d", &a, &b);
	if(a == 3)
		while(true){}
	printf("%d\n", a + b);
}
F
checkFail -n 100 -t 1
grep -qx TLE log || fail "stress TLE"
newCase tests
cat > p.cpp <<'F'
#include <cstdio>
int main(){
	int a; scanf("%d", &a);
	if(a == 2)
		while(true){}
	if(a == 3)
		return 3;
	printf("%d\n", a);
}
F
mkdir tests
for name in 0a 1 2 3; do
	value=${name%a}
	echo "$value" > "tests/$name.in"
	echo 0 > "tests/$name.ans"
done
bash "$script" p -m tests -t 1 > log 2>&1
cat > expected <<'F'
tests/0a.in: PASS
tests/1.in: FAIL
tests/2.in: TLE
tests/3.in: RE 3
F
diff -u expected log || fail "tests verdicts"
cat log
newCase inter
cat > interactor.cpp <<'F'
#include <cstdlib>
#include <iostream>
int main(int, char **argv){
	int hidden = atoi(argv[1]) % 100 + 1;
	for(int i = 0; i < 10; i++){
		int x;
		if(!(std::cin >> x))
			return 1;
		char c = x < hidden ? '>' : x > hidden ? '<' : '=';
		std::cout << c << '\n' << std::flush;
		if(c == '=')
			return 0;
	}
	return 1;
}
F
cat > p.cpp <<'F'
#include <iostream>
int main(){
	int lo = 1, hi = 100;
	while(lo <= hi){
		int mid = (lo + hi) / 2;
		std::cout << mid << '\n' << std::flush;
		char c;
		if(!(std::cin >> c))
			return 1;
		if(c == '=')
			return 0;
		if(c == '<')
			hi = mid - 1;
		else
			lo = mid + 1;
	}
	return 1;
}
F
cp p.cpp search.cpp
checkOK -n 20 -m inter
cat > p.cpp <<'F'
#include <iostream>
int main(){
	for(int i = 1; i <= 100; i++){
		std::cout << i << '\n' << std::flush;
		char c;
		if(!(std::cin >> c))
			return 1;
		if(c == '=')
			return 0;
	}
}
F
checkFail -m inter -s 50 -n 3
grep -qx 'FAIL seed 50' log || fail "inter seed"
sed 's/return 0;/return 1;/' search.cpp > p.cpp
checkFail -m inter -n 1
newCase multi
cat > p.cpp <<'F'
#include <cstdio>
int main(){
	int a, b; scanf("%d%d", &a, &b);
	printf("%d %d\n", a, b);
}
F
cat > noise.cpp <<'F'
#include <cstdio>
int main(){
	int c = getchar();
	while(c != EOF){
		putchar(c);
		c = getchar();
	}
}
F
cat > chk.cpp <<'F'
#include <fstream>
int main(int, char **argv){
	std::ifstream out(argv[2]), ans(argv[3]);
	int a, b, x, y;
	if(!(out >> a >> b) || !(ans >> x >> y))
		return 1;
	return a == x && b == y ? 0 : 1;
}
F
checkOK -n 20 -m multi -c chk
newCase multiFail
cat > gen.cpp <<'F'
#include <cstdio>
int main(){ puts("1"); }
F
cat > p.cpp <<'F'
#include <cstdio>
int main(){
	int x, extra; scanf("%d", &x);
	if(scanf("%d", &extra) == 1) x++;
	printf("%d\n", x);
}
F
cat > noise.cpp <<'F'
#include <cstdio>
int main(){
	int x; scanf("%d", &x); printf("%d 0\n", x);
}
F
cp "$ROOT/tex/contents/contest/chk.cpp" chk.cpp
checkFail -n 1 -m multi -c chk
[ "$(cat tests/fail_1.in)" = '1 0' ] || fail "multi input"
[ "$(cat tests/fail_1.ans)" = 1 ] || fail "multi answer"
[ "$(cat tests/fail_1.out)" = 2 ] || fail "multi output"
bash "$script" p -m tests -c chk > log 2>&1
grep -qx 'tests/fail_1.in: FAIL' log || fail "multi replay"
cat log
gen=$ROOT/tex/contents/contest/gen.cpp
chk=$ROOT/tex/contents/contest/chk.cpp
if [ -f "$gen" ] && [ -f "$chk" ]; then
	newCase notebook
	cp "$gen" gen.cpp; cp "$chk" chk.cpp
	cat > brute.cpp <<'F'
#include <cstdio>
int main(){
	int n; scanf("%d", &n); printf("%d\n", n);
}
F
	cp brute.cpp p.cpp
	checkOK -n 30 -c chk
else
	echo 'SKIP notebook gen/chk: not implemented yet'
fi
