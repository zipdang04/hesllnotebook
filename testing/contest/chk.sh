set -u
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
fail(){ echo "FAIL: $1"; exit 1; }
source=../tex/contents/contest/chk.cpp
check(){
	printf '%s' "$2" > "$tmp/out"
	"$tmp/chk" "$tmp/in" "$tmp/out" "$tmp/ans"
	[ $? = "$1" ] || fail "checker: $3"
}
: > "$tmp/in"
for std in c++17 gnu++20; do
	"$CXX" -std=$std -O2 -Wall -Wextra -Wshadow \
		"$source" -o "$tmp/chk" 2> "$tmp/err" ||
		fail "compile $std"
	[ ! -s "$tmp/err" ] || fail "warnings $std"
	printf '42\n' > "$tmp/ans"
	check 0 $'42\n' 'equal'
	check 0 '  42' 'leading blanks, no newline'
	check 0 $'42\n\n\n' 'trailing blank lines'
	check 1 '41' 'wrong number'
	check 1 '' 'missing output'
	check 1 '42 7' 'extra number'
	check 1 '42 abc' 'extra word'
	check 1 'abc' 'not a number'
	printf '%s' -5 > "$tmp/ans"
	check 0 '-5' 'negative'
	printf '%s' 1000000000000000000 > "$tmp/ans"
	check 0 '1000000000000000000' 'large integer'
	echo "$std: 10 checker cases passed; no warnings"
done
