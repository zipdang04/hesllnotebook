#pragma once
#pragma GCC optimize("O3,unroll-loops")
//#pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt") // x86 only, some judges reject

#include <bits/stdc++.h>
#include <cassert>
using namespace std;
typedef long long ll;
typedef long double ld;
template<class T> using PQMax = priority_queue<T>;
template<class T> using PQMin = priority_queue<T, vector<T>, greater<T>>;
// maximize/minimize(a, b): a = max/min(a, b), true if a changed. Compile warning if b may not fit in a's type
template<class T, class U, class = void> struct _narrows : true_type {};
template<class T, class U> struct _narrows<T, U, void_t<decltype(T{declval<U>()})>> : false_type {};
template<class T, class U> using _fits = enable_if_t<!is_integral_v<T> || !_narrows<T, U>::value, int>;
template<class T, class U> using _narrow = enable_if_t<is_integral_v<T> && _narrows<T, U>::value, int>;
#define _NARROW [[deprecated("the value may not fit in the variable's type")]]
template<class T1, class T2, _fits<T1, T2> = 0> bool maximize(T1 &a, T2 b){ return b > a ? (a = b, 1) : 0; }
template<class T1, class T2, _fits<T1, T2> = 0> bool minimize(T1 &a, T2 b){ return b < a ? (a = b, 1) : 0; }
#ifdef LOCAL
#define _FITS(T1, T2, b) assert(T2(T1(b)) == b) // debug build: stop if b really does not fit
#else
#define _FITS(T1, T2, b)
#endif
template<class T1, class T2, _narrow<T1, T2> = 0> _NARROW bool maximize(T1 &a, T2 b){
	_FITS(T1, T2, b); return b > a ? (a = b, 1) : 0; }
template<class T1, class T2, _narrow<T1, T2> = 0> _NARROW bool minimize(T1 &a, T2 b){
	_FITS(T1, T2, b); return b < a ? (a = b, 1) : 0; }
// read(): buffered, stdin only, integers only; never mix with cin/scanf
char _gc(){ // 0 at EOF
	static char b[1<<16]; static size_t n = 0, p = 0;
	if (p == n){ n = fread(b, 1, sizeof b, stdin); p = 0; if (!n) return 0; }
	return b[p++];
}
template<class T> bool read(T &x){
	char c = _gc(); bool neg = 0; x = 0;
	while (c && c != '-' && (c < '0' || c > '9')) c = _gc();
	if (!c) return 0;
	if (c == '-') neg = 1, c = _gc();
	for (; c >= '0' && c <= '9'; c = _gc()) x = x * 10 + (c - '0');
	if (neg) x = -x;
	return 1;
}
template<class T, class... Ts> bool read(T &x, Ts&... r){ return read(x) && read(r...); }

#define fi first
#define se second
#define FOR(type,i,a,b) for(type i=(a); i<=(type)(b); i++)
#define REV(type,i,b,a) for(type i=(b); i>=(type)(a); i--)

#define testBit(n, bit) (((n) >> (bit)) & 1)
#define flipBit(n, bit) ((n) ^ (1ll << (bit)))
#define cntBit(n) __builtin_popcountll(n)
// log2(n) = floor(log2 n) for integers; replaces std::log2 (do not call it on doubles)
#define log2(n) (63 - __builtin_clzll(n))

#define CURRENT_TIMESTAMP chrono::steady_clock::now().time_since_epoch().count()

#ifndef STRESS
#define STRESS 0
#endif
#ifdef LOCAL
#define dbg(...) cerr << "[" << #__VA_ARGS__ << "] = ", _dbg(__VA_ARGS__)
template<class T, class... Ts> void _dbg(const T &a, const Ts&... b){
	cerr << a; ((cerr << ' ' << b), ...); cerr << endl;
}
#else
#define dbg(...)
#endif
