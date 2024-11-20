compile() {
	g++ $1.cpp -O2 -lm -fmax-errors=5 -march=native -s -o $1
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
############################
MAIN_DIR=$(dirname $0)/..
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
echo $code_file
mkdir .build
for file in `ls $test_dir/in`
do
	test_name=${file%%.*}
	echo "testing $test_name"
	inp_file=$test_dir/in/$test_name.in
	out_file=.build/output.txt
	ans_file=$test_dir/out/$test_name.out
	./$code_file < $inp_file > $out_file
	./$test_dir/checker $inp_file $out_file $ans_file
	if [ $? -ne 0 ]
	then
		break
	fi
done
rm -r .build