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
#define log2(n) (31 - __builtin_clz(n))
#define log2ll(n) (63 - __builtin_clzll(n))
#define CURRENT_TIMESTAMP chrono::steady_clock::now().time_since_epoch().count()
#define randomize mt19937_64 mt(CURRENT_TIMESTAMP)

// remember to call sieve() before any factorize()
namespace Eratos_Factorable {
	constexpr ll MAX = 100000000;
	constexpr ll EST = 53000000; // 1.1*MAX/ln(MAX)
	int smallDiv[MAX + 1] = {};
	int primes[EST], cntPrime = 0;
	inline void sieve(int size = MAX) {
		memset(smallDiv, false, sizeof(smallDiv));
		primes[++cntPrime] = 2; primes[++cntPrime] = 3;
		for (int i=2; i<=size; i += 2) smallDiv[i] = 2;
		for (int i=3; i<=size; i += 6) smallDiv[i] = 3;
		for (int mul = 1; 6 * mul - 1 <= size; mul++) {
			bool pass = false;
			for(int i:{6*mul-1,6*mul+1})if(!smallDiv[i]){
				primes[++cntPrime] = i; smallDiv[i] = i;
				for (ll j = 1ll*i*i; j <= size; j += i*2)
					if (not smallDiv[j]) smallDiv[j] = i;
				if (1ll * i * i > size) pass = true;
			}
			if (pass) break;
		}
	}
	inline vector<int> factorize(ll number) {
		vector<int> ans;
		while (number > 1) {
			int d = smallDiv[number];
			while (number % d == 0)
				ans.push_back(d), number /= d;
		}
		return ans;
	}
}
