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
#define log2_64(n) (64 - __builtin_clzll(n))
#define CURRENT_TIMESTAMP chrono::steady_clock::now().time_since_epoch().count()
#define randomize mt19937_64 mt(CURRENT_TIMESTAMP)

#define MAX 1000001
#define MOD 1000000007

typedef complex<ld> cd;
namespace FFT{
	constexpr ld TAU = 
		3.141592653589792384826433832795028841971693993751058 * 2;
	
	constexpr int BIT = 20, MAX_LEN = 1 << BIT;
	vector<int> _rev[BIT + 1];
	cd _root[MAX_LEN + 1];
	void buildRoot() {
		_root[0] = _root[MAX_LEN] = 1;
		for (int i=BIT-1, dist=1<<(BIT-1); i>=0; i--, dist>>=1) {
			cd w = polar(ld(1.0), TAU * dist / MAX_LEN);
			for (int pos = 0; pos < MAX_LEN; pos += 2 * dist)
				_root[pos + dist] = _root[pos] * w;
		}
		_rev[0] = {0};
		for (int bit=1, len=2; len<=MAX_LEN; bit++, len*=2) {
			_rev[bit].resize(len, 0);
			for (int i = 0; i < len; i++)
				_rev[bit][i]=((i&1)<<(bit-1))|_rev[bit-1][i/2];
		}
	}
	void dft(vector<cd> &poly, bool invert = false) { 
		assert((cntBit((int)poly.size()) == 1));
		const int n = poly.size(), coef = MAX_LEN / n;
		for (int dist=n/2, span=n; dist>0; dist/=2, span/=2) 
		for (int pos = 0; pos < dist; pos++) 
		for (int i=pos, k=0; i<n; i+=span, k+=dist) {
			int len = log2(n / span);  
			int newK = _rev[len][k / dist] * dist;
			int tmp = (invert ? (n - newK) : newK) * coef;
			cd a = poly[i], b = _root[tmp] * poly[i + dist];
			poly[i] = a + b, poly[i + dist] = a - b;
		}
		if (invert) for (cd &x: poly) x /= n;
	}
	template<class T>vector<T>conv(vector<T>_a,vector<T>_b){
		int len = int(_a.size() + _b.size()) - 1; 
		int bit = log2(len)+(cntBit(len)>1); len=1<<bit;
		vector<cd> a(len);
		for (int i = 0; i < len; i++) 
			a[i] = cd((i < _a.size())?_a[i]:0, (i<_b.size())?_b[i]:0);
		dft(a, false);
		for (int i = 0; i < len; i++)
			if (i < _rev[bit][i]) swap(a[i], a[_rev[bit][i]]);
		for (int i = 0; i < len; i++) a[i] *= a[i];
		vector<cd> b(a.begin(), a.end());
		for (int i = 0; i < len; i++)
			b[i] = a[i] - conj(a[-i & (len-1)]); //(n-i)%n
		dft(b, true);
		for (int i = 0; i < len; i++)
			if (i < _rev[bit][i]) swap(b[i], b[_rev[bit][i]]);

		while (len > 0 && b[len - 1].imag() < 1e-9) len--;
		vector<T> ans(len); 
		FOR(int, i, 0, len-1) ans[i]=round(b[i].imag()/4);
		return ans;
	}
}

int n, M, F;
vector<ll> a, b;

main()
{
	ios_base::sync_with_stdio(0); cin.tie(0);
	FFT::buildRoot();
	cin >> n >> M >> F;
	a = vector<ll>(n + 1, 0);
	b = vector<ll>(n + 1, 0);
	FOR(int, i, 1, M) {
		int num; cin >> num;
		a[num]++;
	}
	FOR(int, i, 1, F) {
		int num; cin >> num;
		b[num]++;
	}
	vector<ll> ans = FFT::conv(a, b);
	while (int(ans.size()) < n * 2 + 1) ans.push_back(0);
	FOR(int, i, 2, n * 2) cout << ans[i] << " \n"[i == n * 2];
}