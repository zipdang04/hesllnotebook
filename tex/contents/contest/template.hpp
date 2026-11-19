#pragma once

// #pragma GCC optimize("O3,unroll-loops")
// #pragma GCC target("avx2,popcnt,lzcnt,bmi,bmi2")
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
#define fi first
#define se second

template<class T> using PQMax = priority_queue<T>;
template<class T> using PQMin =
	priority_queue<T, vector<T>, greater<T>>;
template<class T1, class T2>
bool maximize(T1 &a, T2 b){ return b > a ? (a = b, 1) : 0; }
template<class T1, class T2>
bool minimize(T1 &a, T2 b){ return b < a ? (a = b, 1) : 0; }

#define FOR(i, a, b) for(auto i = (a); i <= (b); i++)
#define REV(i, b, a) for(auto i = (b); i >= (a); i--)
#define testBit(n, bit) (((n) >> (bit)) & 1)
#define flipBit(n, bit) ((n) ^ (1ll << (bit)))
#define cntBit(n) __builtin_popcountll(n)
#define log2(n) (63 - __builtin_clzll(n))

#define CURRENT_TIMESTAMP() \
	chrono::steady_clock::now().time_since_epoch().count()
#define randomize mt19937_64 mt(CURRENT_TIMESTAMP())

#ifdef LOCAL
template<class T, class =
	enable_if_t<!is_convertible_v<T, string>>,
	class = decltype(begin(declval<T &>()))>
ostream &operator<<(ostream &o, const T &v);
template<class A, class B>
ostream &operator<<(ostream &o, const pair<A, B> &p){
	return o << '(' << p.fi << ", " << p.se << ')';
}
template<class T, class, class>
ostream &operator<<(ostream &o, const T &v){
	o << '{'; string s;
	for(auto &&x: v) o << s << x, s = ", ";
	return o << '}';
}
template<size_t N> // char array: stop at NUL or N
ostream &operator<<(ostream &o, char (&s)[N]){
	return o.write(s, strnlen(s, N));
}
#define dbg(...) cerr << "[" #__VA_ARGS__ "]:", \
	[](auto &&...dbg_){ \
		((cerr << ' ' << dbg_), ...); cerr << '\n'; \
	}(__VA_ARGS__)
#else
#define dbg(...)
#endif

/* in this template */
typedef vector<int> vi;
typedef vector<ll> vll;
#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
const ll oo = 1e18;
// int main(){
// 	ios_base::sync_with_stdio(0); cin.tie(nullptr);
// }
