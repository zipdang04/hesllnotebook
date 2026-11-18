ulimit -s 1048576 # 1 GB stack (in KB), once per terminal
# debug build (-DLOCAL: dbg on): c a && ./a < in
c(){ g++ -std=gnu++20 -Wall -Wextra -Wshadow -O2 -g \
	-DLOCAL -D_GLIBCXX_ASSERTIONS \
	-fsanitize=address,undefined -fno-sanitize-recover=all \
	$1.cpp -o $1; }
# judge build: timing, and the last run before a submit
o(){ g++ -std=gnu++20 -O2 -g $1.cpp -o $1; }
# -std=gnu++20 in c and o: set it to the judge's value
./a < in > out && diff -b out ans
/usr/bin/time -v ./a < big.in > /dev/null # max RSS = memory
chmod +x *.sh # before running shell files
# gdb at -O2: vars <optimized out>; compile -O0 to step
# hash as printed by each title, no blanks or comments:
# h < a.cpp, or sed -n 4,40p a.cpp | h
h(){ cpp -dD -P -fpreprocessed | tr -d '[:space:]' \
	| md5sum | cut -c-6; }
