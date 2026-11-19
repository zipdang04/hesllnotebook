#pragma once
#include "../contest/template.hpp"

// fread reader for integers; read(a, b, ..) reads in order,
// false at EOF or at a failure. Not for interactive.
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
	for(; c != '-' && (c < '0' || c > '9'); c = readChar())
		if(c < 0) return false;
	if(c == '-') neg = 1, c = readChar();
	if(c < '0' || c > '9') return false;
	for(; c >= '0' && c <= '9'; c = readChar())
		x = x * 10 - (c - '0');
	if(!neg) x = -x;
	return true;
}
template<class... Ts>
enable_if_t<(is_integral_v<Ts> && ...), bool>
read(Ts &...x){
	return (read(x) && ...);
}
