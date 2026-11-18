set -u
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
for std in c++17 gnu++20; do
	for d in "" -DLOCAL; do
		$CXX -std=$std -Wall -Wextra -Wshadow -O2 $d \
			contest/dbg.cpp -o $tmp/dbg 2> $tmp/err
		if [ $? != 0 ] || [ -s $tmp/err ]; then
			echo "compile $std $d:"; cat $tmp/err; exit 1
		fi
		$tmp/dbg || { echo "run $std $d failed"; exit 1; }
	done
done
