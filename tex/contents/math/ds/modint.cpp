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

typedef unsigned long long ull;
ll MOD = 1000000007;
struct modint{
	int v;
	static inline ll mod(ll num) {
		ll val=num-ull((__uint128_t(-1ULL/MOD)*num)>>64)*MOD;
		return val-(val>=MOD)*MOD;
	}
	modint inv() const{
		ll answer = 1, a = v, n = MOD - 2;
		while (n) { 
			if (n & 1) answer = mod(answer * a); 
			a = mod(a * a); n >>= 1; 
		}
		return answer;
	}

	modint(ll a = 0)
		{ v=(a<0)?(MOD-mod(-a)):mod(a);v-=(v>=MOD)*MOD; }
	inline modint& operator += (modint b) 
		{ v+=b.v; v-=(v>=MOD)*MOD; return *this; }
	inline modint& operator -= (modint b) 
		{ v+=MOD-b.v; v-=(v>=MOD)*MOD; return*this; }
	inline modint& operator *= (modint b) 
		{ v = mod(1ll * v * b.v); return *this; }
	inline modint& operator /= (modint b) 
		{ return (*this)*=b.inv(); }
	inline modint& operator ^= (ll n) {
		modint a = v; v = 1;
		while (n) {if (n & 1) *this *= a; a *= a, n >>= 1;} 
		return *this;
	}
};
inline modint operator+(modint a, modint b){return a+=b;}
inline modint operator-(modint a, modint b){return a-=b;}
inline modint operator*(modint a, modint b){return a*=b;}
inline modint operator/(modint a, modint b){return a/=b;}
inline modint operator^(modint a, ll n){return a^=n;}
inline bool operator == (modint a, modint b)
	{ return a.v==b.v; }
inline bool operator != (modint a, modint b)
	{ return a.v!=b.v; }
inline bool operator < (modint a, modint b)
	{ return a.v<b.v; }
inline bool operator > (modint a, modint b)
	{ return a.v>b.v; }
inline bool operator <= (modint a, modint b)
	{ return a.v<=b.v; }
inline bool operator >= (modint a, modint b)
	{ return a.v>=b.v; }
inline istream& operator >> (istream& s, modint &i) 
	{ ll tmp; s >> tmp; i = tmp; return s; }
inline ostream& operator << (ostream& s, modint i) 
	{return s << i.v;}

#define MAX 10000000
modint fact[MAX + 1], ifac[MAX + 1];

main()
{
	ios_base::sync_with_stdio(0); cin.tie(0);
	int t; read(t, MOD);
	const int LIMIT = min(1ll*MAX, MOD - 1);
	fact[0] = 1; FOR(int, i, 1, LIMIT) fact[i] = fact[i - 1] * i;
	ifac[LIMIT] = fact[LIMIT].inv();
	REV(int, i, LIMIT - 1, 0) ifac[i] = ifac[i + 1] * (i + 1);

	FOR(int, _, 1, t) {
		int n, k; read(n, k);
		if (n < k) {cout << "0\n"; continue;}
		modint ans = 1;
		while (n > 0 and k > 0) {
			int dN = modint::mod(n), dK = modint::mod(k);
			ans *= fact[dN] * ifac[dK] * ifac[dN - dK];
			n /= MOD, k /= MOD;
		}
		cout << ans << '\n';
	}
}