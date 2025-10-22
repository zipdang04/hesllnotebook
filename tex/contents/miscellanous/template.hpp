#pragma once

#include <bits/stdc++.h>
using namespace std;
/*
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;
*/
typedef long long ll;
typedef long double ld;
template<class T>using PQMax=priority_queue<T>;
template<class T>using PQMin=priority_queue<T, vector<T>, greater<T>>;
template<class T1, class T2>
void maximize(T1 &a, T2 b){ if (b > a) a = b; }
template<class T1, class T2>
void minimize(T1 &a, T2 b){ if (b < a) a = b; }
template<class T> void read(T &number) {
	bool negative = false; register int c;
	number = 0; c = getchar();
	while (c!='-' && !isalnum(c)) c = getchar();
	if (c=='-') negative = true, c = getchar();
	for (; (c>47 && c<58); c=getchar()) 
		number = number*10 + c-48;
	if (negative) number *= -1;
}
template<class T, class ...Ts>
void read(T &a, Ts&... args){
	read(a); read(args...);
}

#define fi first
#define se second
#define FOR(type,i,a,b) for(type i=(a); i<=(b); i++)
#define REV(type,i,b,a) for(type i=(b); i>=(a); i--)
#define EFOR(type,i,a,b) for(type i=(a); i<(b); i++)
#define EREV(type,i,b,a) for(type i=(b); i>(a); i--)
#define testBit(n, bit) (((n) >> (bit)) & 1)
#define flipBit(n, bit) ((n) ^ (1ll << (bit)))
#define cntBit(n) __builtin_popcount(n)
#define cntBitll(n) __builtin_popcountll(n)
#define log2(n) (31 - __builtin_clz(n))
#define log2ll(n) (63 - __builtin_clzll(n))
#define CURRENT_TIMESTAMP chrono::steady_clock::now().time_since_epoch().count()
#define randomize mt19937_64 mt(CURRENT_TIMESTAMP)

// remember to fill in:
// #define MAX ???
// #define MOD ???

// int main()
// {ios_base::sync_with_stdio(0);cin.tie(0);}