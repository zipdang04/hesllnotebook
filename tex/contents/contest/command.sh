ulimit -s 1048576   # 1 GB stack (in KB), set once per terminal
# compile (-DLOCAL: dbg on), run, compare
g++ -std=c++17 -Wall -Wextra -O2 -g -DLOCAL \
	-fsanitize=address,undefined $1.cpp -o $1
# gdb at -O2: vars <optimized out>, steps jump; -O0 to step
./$1 < in > out && diff -w out ans
# before running shell files
chmod +x *.sh
