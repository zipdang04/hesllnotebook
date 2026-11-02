# compile (-DLOCAL: dbg on), run, compare
g++ -std=gnu++20 -Wall -Wextra -O2 -g -DLOCAL \
	-fsanitize=address,undefined $1.cpp -o $1
./$1 < in > out && diff -w out ans
chmod +x *.sh   # before running shell files
