#pragma once
#include "../contest/template.hpp"

// fread reader for integers; read(a, b, ..) false at EOF.
// About 5x faster than cin. Not for interactive problems.
int readChar(){
	static unsigned char buf[1 << 16];
	static int len = 0, pos = 0;
	if(pos == len)
		len = (int)fread(buf, 1, sizeof buf, stdin), pos = 0;
	return pos < len ? buf[pos++] : -1;
}
template<class T> bool read(T &x){
	int c = readChar(), neg = 0;
	x = 0;
	while(c != '-' && (c < '0' || c > '9')){
		if(c < 0) return false;
		c = readChar();
	}
	if(c == '-') neg = 1, c = readChar();
	for(; c >= '0' && c <= '9'; c = readChar())
		x = x * 10 - (c - '0');
	if(!neg) x = -x;
	return true;
}
// several values, left to right; stops at the first failure
template<class... Ts> bool read(Ts &...x){
	return (read(x) && ...);
}
