#pragma once
#pragma GCC optimize("O3,unroll-loops")
//#pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt") // x86 only, some judges reject

#include <bits/stdc++.h>
#include <cassert>
using namespace std;
/*
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;
*/
typedef long long ll;
typedef long double ld;
template<class T> using PQMax = priority_queue<T>;
template<class T> using PQMin = priority_queue<T, vector<T>, greater<T>>;
template<class T1, class T2>
void maximize(T1 &a, T2 b){ if (b > a) a = b; }
template<class T1, class T2>
void minimize(T1 &a, T2 b){ if (b < a) a = b; }
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
#define FOR(type,i,a,b) for(type i=(a); i<=(b); i++)
#define REV(type,i,b,a) for(type i=(b); i>=(a); i--)
#define EFOR(type,i,a,b) for(type i=(a); i<(b); i++)
#define EREV(type,i,b,a) for(type i=(b); i>(a); i--)

#define testBit(n, bit) (((n) >> (bit)) & 1)
#define flipBit(n, bit) ((n) ^ (1ll << (bit)))
#define cntBit(n) __builtin_popcountll(n)
#define log2(n) (63 - __builtin_clzll(n))

#define CURRENT_TIMESTAMP chrono::steady_clock::now().time_since_epoch().count()
#define randomize mt19937_64 mt(CURRENT_TIMESTAMP)

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
