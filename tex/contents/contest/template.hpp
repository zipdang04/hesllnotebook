#pragma once

#include <bits/stdc++.h>
using namespace std;
/*
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef tree<int, null_type, less<int>, rb_tree_tag,
	tree_order_statistics_node_update> ordered_set;
*/
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
void maximize(T1 &a, T2 b){ if (b > a) a = b; }
template<class T1, class T2>
void minimize(T1 &a, T2 b){ if (b < a) a = b; }

#define FOR(i, a, b) for (auto i = (a); i <= (b); i++)
#define REV(i, b, a) for (auto i = (b); i >= (a); i--)

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
	class = decltype(begin(declval<T&>()))>
ostream& operator<<(ostream& o, const T& v);
template<class A, class B>
ostream& operator<<(ostream& o, const pair<A, B>& p){
	return o << '(' << p.fi << ", " << p.se << ')';
}
template<class T, class, class>
ostream& operator<<(ostream& o, const T& v){
	o << '{'; string s;
	for (auto&& x: v) o << s << x, s = ", ";
	return o << '}';
}
#define dbg(...) cerr << "[" #__VA_ARGS__ "]:", \
	[](const auto&... x){ \
		((cerr << ' ' << x), ...); cerr << endl; \
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

/*
 * int main() {
 * 	ios_base::sync_with_stdio(0); cin.tie(nullptr);
 * }
 */
