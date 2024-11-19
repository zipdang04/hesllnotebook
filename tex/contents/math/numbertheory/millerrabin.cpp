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
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
template <class T> using PQMax = priority_queue<T>;
template <class T> using PQMin = priority_queue<T, vector<T>, greater<T>>;
template <class T1, class T2>
void maximize(T1 &a, T2 b){
	if (b > a) a = b;
}
template <class T1, class T2>
void minimize(T1 &a, T2 b){
	if (b < a) a = b;
}
template <class T>
void read(T &number)
{
	bool negative = false;
	register int c;
	number = 0;
	c = getchar();
	while (c != '-' && !isalnum(c)) c = getchar();
	if (c=='-'){
		negative = true;
		c = getchar();
	}
	for (; (c>47 && c<58); c=getchar())
		number = number *10 + c - 48;
	if (negative)
		number *= -1;
}
template <class T, class ...Ts>
void read(T &a, Ts& ... args){
	read(a);
	read(args...);
}

/*
struct Node
{
	int node, len;
	Node() {node = len = 0;}
	Node(int node, int len) {this -> node = node, this -> len = len;}
};
typedef vector<Node> vg;
*/

#define fi first
#define se second

#define FOR(type, i, a, b) for(type i = (a); i <= (b); i++)
#define REV(type, i, b, a) for(type i = (b); i >= (a); i--)

#define testBit(n, bit) (((n) >> (bit)) & 1)
#define flipBit(n, bit) ((n) ^ (1ll << (bit)))
#define cntBit(n) __builtin_popcount(n)
#define cntBitll(n) __builtin_popcountll(n)
#define CURRENT_TIMESTAMP chrono::steady_clock::now().time_since_epoch().count()
#define randomize mt19937_64 mt(CURRENT_TIMESTAMP)

#define MAX 1000001
#define MOD 1000000007

namespace MillerRabin{
	typedef __int128_t i128;
	constexpr int SMALL_PRIMES[12] = {2,3,5,7,11,13,17,19,23,29,31,37};

	inline ll _power(ll a, ll n, ll mod) {
		ll ans = 1;
		while (n) {
			if (n & 1) ans = (i128)ans * a % mod;
			a = (i128)a * a % mod; n >>= 1;
		}
		return ans;
	}
	inline bool _fermatCheck(ll n, ll a, ll pw, int p2) {
		ll num = _power(a, pw, n);
		if (num == 1 || num == n - 1) return true;	
		FOR(int, i, 1, p2-1) {
			num = (i128) num * num % n;
			if (num == n - 1) return true;
		}
		return false;
	}

	inline bool checkPrime(const ll n) {
		if (n == 2 || n == 3 || n == 5 || n == 7) return true;
		if (n < 11 || (n & 1) == 0) return false;
		ll d = n-1; int p2 = 0;
		while ((d & 1) == 0) d >>= 1, p2++;
		for (int a: SMALL_PRIMES)
			if (n == a) return true;
			else if (not _fermatCheck(n, a, d, p2)) 
				return false;
		return true;
	}
}

main()
{
	ios_base::sync_with_stdio(0); cin.tie(0);
	int q; read(q);
	FOR(int, _, 1, q) {
		ll num; read(num);
		cout << (MillerRabin::checkPrime(num) ? "Yes\n" : "No\n");
	}
}