#pragma once
#include "template.hpp"
char _gc(){ // 0 at EOF
	static char b[1<<16]; static size_t n = 0, p = 0;
	if (p == n){ n = fread(b, 1, sizeof b, stdin); p = 0; if (!n) return 0; }
	return b[p++];
}
template<class T> bool readInt(T &x){
	char c = _gc(); bool neg = 0; x = 0;
	while (c && c != '-' && (c < '0' || c > '9')) c = _gc();
	if (!c) return 0;
	if (c == '-') neg = 1, c = _gc();
	for (; c >= '0' && c <= '9'; c = _gc()) x = x * 10 + (c - '0');
	if (neg) x = -x;
	return 1;
}
template<class T, class... Ts> bool readInt(T &x, Ts&... r){ return readInt(x) && readInt(r...); }
