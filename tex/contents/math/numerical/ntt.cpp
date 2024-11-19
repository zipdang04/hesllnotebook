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

#define MAX 1000001
#define MOD 998244353

typedef unsigned long long ull;
struct modint{
	int v;
	static inline ll mod(ll num) {
		ll val=num-(ull)((__uint128_t(-1ULL/MOD)*num)>>64)*MOD;
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

// MOD=998244353=c*2^k+1 => ROOT=g^c, any g: gcd(g,MOD)=1
namespace FFT{
	constexpr int K_MOD = 23, BIT = 20, MAX_LEN = 1 << BIT;
	constexpr int ROOT = 15311432;

	modint _root[MAX_LEN + 1];
	vector<int> _rev[BIT + 1];
	void buildRoot() {
		_root[0] = 1; modint mul=ROOT; 
		FOR(int, i, 1, K_MOD-BIT) mul*=mul;
		FOR(int, i, 1, MAX_LEN) _root[i] = _root[i-1] * mul;
		for (int bit=0, len=1; len<=MAX_LEN; bit++, len*=2) {
			_rev[bit].resize((1 << bit), 0);
			for (int i = 1; i < (1 << bit); i++)
				_rev[bit][i]=((i&1)<<(bit-1))|_rev[bit-1][i/2];
		}
	}

	void transform(vector<modint> &v, bool invert){
		const int len = v.size(), coef = MAX_LEN / len; 
		const int bit = log2(len); 
		FOR(int, i, 0, len-1) if (i < _rev[bit][i])
			swap(v[i], v[_rev[bit][i]]);
		
		for (int jmp=1, span=2; span<=len; jmp*=2, span*=2)
		for (int beg = 0; beg < len; beg += span)
		for (int i = 0; i < jmp; i++) {
			int k=coef*len/jmp*i/2; if (invert) k=MAX_LEN-k;
			modint a = v[beg+i], b = _root[k] * v[beg+i+jmp];
			v[beg + i] = a + b; v[beg + i + jmp] = a - b;
		}
		if (invert) FOR(int, i, 0, len-1) v[i] /= len;
	}
	template<class T>
	vector<modint> multiply(vector<T> &_a, vector<T> &_b) {
		int len = int(_a.size() + _b.size()) - 1;
		len = 1 << (log2(len) + (cntBit(len) > 1));
		vector<modint>a(_a.begin(),_a.end()); a.resize(len,0);
		vector<modint>b(_b.begin(),_b.end()); b.resize(len,0);
		transform(a, false); transform(b, false);
		FOR(int, i, 0, len - 1) a[i] *= b[i];
		transform(a, true); return a;
	}
}

main()
{
	ios_base::sync_with_stdio(0); cin.tie(0);
	FFT::buildRoot();
	int n, m; cin >> n >> m;
	vector<int> a(n); FOR(int, i, 0, n-1) cin >> a[i];
	vector<int> b(m); FOR(int, i, 0, m-1) cin >> b[i];
	vector<modint> ans = FFT::multiply(a, b);
	FOR(int, i, 0, n-1+m-1) cout << ans[i] << ' ';
}