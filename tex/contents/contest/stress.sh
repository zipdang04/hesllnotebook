#!/bin/bash
# ./stress.sh p [-m stress|tests|inter|multi] [-c chk]
#   [-n 500] [-s 1] [-t 2]
set -o pipefail
p=$1; shift; m=stress c= n=500 s=1 t=2
while getopts m:c:n:s:t: o; do eval $o=\$OPTARG; done
F="-std=gnu++20 -O2 -g -DSTRESS"
F+=" -fsanitize=address,undefined -fno-sanitize-recover=all"
for f in $p gen brute $c interactor noise; do
	[ -f $f.cpp ] && { g++ $F $f.cpp -o $f || exit 1; }
done
run(){ timeout $t ./$p < $1 > $2; r=$?; [ $r = 0 ] && return
	[ $r = 124 ] && echo TLE || echo "RE $r"; return 1; }
ok(){ if [ $c ]; then ./$c in out ans
	else diff -b out ans > /dev/null; fi; }
if [ $m = tests ]; then
	for f in tests/*.in; do
		cp $f in; cp ${f%.in}.ans ans; echo -n "$f: "
		run in out && { ok && echo PASS || echo FAIL; }
	done; exit
fi
for((i = s; i < s + n; i++)); do
	case $m in
		stress) ./gen $i > in && ./brute < in > ans &&
			run in out && ok ;;
		inter) rm -f fifo; mkfifo fifo
			timeout $t ./$p < fifo | ./interactor $i > fifo ;;
		multi) ./gen $i > in && run in ans &&
			./noise $i < ans > in2 && mv in2 in &&
			run in out && ok ;;
	esac || { echo "FAIL seed $i"; mkdir -p tests
		for x in in out ans; do
			[ -f $x ] && cp $x tests/fail_$i.$x; done; exit 1; }
done; echo "OK $n tests"
