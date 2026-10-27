#!/bin/bash

compile() {
	# stack size: linker flag on macOS, ulimit elsewhere
	if [ "$(uname)" = Darwin ]; then STACK="-Wl,-stack_size,0x10000000"; else STACK=""; ulimit -s unlimited 2> /dev/null; fi
	${CXX:-g++-16} $1.cpp -O2 -lm -fmax-errors=5 -march=native $STACK -s -o $1 2> /dev/null
	if [ $? -ne 0 ]
	then
		echo "Error during compilation"
		exit 1
	fi
}
createTest() {
	python3 $CREATION_SCRIPT -p $1
	if [ $? -ne 0 ]
	then
		echo "Error during code"
		exit 1
	fi
}
run() {
	# bash's time keyword measures the solution (ms) into .build/time; the solution's stderr is left alone
	( TIMEFORMAT=%R; { time ./$1 < $2 > $3 2>&4; } 4>&2 2> .build/time ) &
	pid=$!
	ticks=$(awk "BEGIN{printf \"%d\", $4 * 20}")
	i=0
	while [ $i -lt $ticks ]
	do
		kill -0 $pid 2>/dev/null || { wait $pid; return 0; }
		sleep 0.05
		i=$((i + 1))
	done
	kill -0 $pid 2>/dev/null && return 1
	wait $pid
	return 0
}
############################
MAIN_DIR=$(dirname $0)/..
echo $MAIN_DIR
TEST_DIR=library-checker-problems
cd $MAIN_DIR
if [ $(basename `pwd`) != "hesllnotebook" ]
then
	echo "Wrong directory!"
	exit 127
fi

CREATION_SCRIPT=$TEST_DIR/generate.py
CODING_BASEDIR=testing
############################
if [ -z $1 ] || [ -z $2 ]
then
	echo "No source code and/or testing folder provided!"
	exit 127
fi
code_file=$CODING_BASEDIR/$1
test_dir=$TEST_DIR/$2
inp_dir=$test_dir/in
out_dir=$test_dir/out

compile $code_file
createTest $2
############################
if [ -z $3 ] 
then
	TIME_LIMIT=1
else
	TIME_LIMIT=$3
fi
echo "Time limit: $TIME_LIMIT"

FAILED=0
MAX_TIME=0  # slowest case, for all.py's report (MAX_TIME_FILE)
mkdir .build
for file in `ls $test_dir/in`
do
	test_name=${file%%.*}
	echo "testing $test_name"
	inp_file=$test_dir/in/$test_name.in
	out_file=.build/$test_name.txt
	ans_file=$test_dir/out/$test_name.out

	run $code_file $inp_file $out_file $TIME_LIMIT
	if [ $? -eq 1 ]
	then
		echo "Time limit exceeded!"
		pkill -P $pid; kill $pid; FAILED=1  # the solution is a child of the timing subshell
		MAX_TIME=$TIME_LIMIT+
		break
	fi
	MAX_TIME=$(awk -v a="$MAX_TIME" -v b="$(tail -n 1 .build/time)" 'BEGIN{print (b + 0 > a + 0) ? b : a}')

	./$test_dir/checker $inp_file $out_file $ans_file
	if [ $? -ne 0 ]
	then
		FAILED=1
		break
	fi
done
rm -r .build
echo "Slowest case: ${MAX_TIME}s (limit ${TIME_LIMIT}s)"
[ -n "${MAX_TIME_FILE:-}" ] && echo "$MAX_TIME" > "$MAX_TIME_FILE"
exit $FAILED
