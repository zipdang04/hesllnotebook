#!/bin/bash
# ./stress.sh [mode] [-c checker] [-n 1000] [-s 1] [-t 2]
# stress: gen $i>in; sol<in>out; brute<in>ans; check
#   fail: print seed, keep tests/fail_<i>.{in,out,ans}
# tests: sol on tests/*.in vs .ans: PASS/FAIL/RE/TLE
# interactive: sol <-> interactor $i by fifo; OK: exit 0
# multipass: sol<in>p1; noise $i in p1>in2; sol<in2>out
# check: checker in out ans (multipass: ans = p1), exit
#   0 = OK; default diff -w. -t: whole seconds
# builds X.cpp newer than X: -DSTRESS $SAN (asan+ubsan)
m=stress; [[ $1 && $1 != -* ]] && m=$1 && shift
dw(){ diff -w $2 $3 > /dev/null; }; c=dw n=1000 s=1 t=2
while getopts c:n:s:t: o; do eval $o=\$OPTARG; done
type timeout &> /dev/null ||
	timeout(){ perl -e 'alarm shift; exec @ARGV' "$@"; }
for f in *.cpp; do [ $f -nt ${f%.*} ] && { ${CXX:-g++} \
	-O2 -DSTRESS ${SAN--fsanitize=address,undefined} \
	$f -o ${f%.*} || exit 1; }; done
run(){ timeout $t ./sol <$1 >$2; r=$?; ((r)) && v="RE $r"
	((r == 124 || r == 142)) && v=TLE; return $r; }
if [ $m = tests ]; then for f in tests/*.in; do
	run $f out && { $c $f out ${f%.in}.ans && v=PASS ||
	v=FAIL; }; echo "$f $v"; done; exit; fi
[ $m = interactive ] && rm -f fifo && mkfifo fifo
for ((i = s; i < s + n; i++)); do v=WA
	if [ $m = interactive ]; then timeout $t ./sol < fifo |
		timeout $t ./interactor $i > fifo && continue
		echo "seed $i: interactor failed"; exit 1; fi
	./gen $i > in; if [ $m = multipass ]; then run in p1 &&
		./noise $i in p1 > in2 && run in2 out && cp p1 ans
	else run in out && ./brute < in > ans; fi &&
		$c in out ans && continue
	echo "seed $i: $v"; mkdir -p tests
	for e in in out ans; do cp $e tests/fail_$i.$e; done
	exit 1
done; echo "OK: seeds $s..$((s + n - 1))"
