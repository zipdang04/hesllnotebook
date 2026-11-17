#include "../../tex/contents/math/ntt.hpp"
#include <cassert>

using ull = unsigned long long;
vector<MintN> randVec(int n, ull &x){
	vector<MintN> a(n);
	for(MintN &v: a){
		x = x * 1103515245ULL + 12345ULL;
		v = x >> 16;
	}
	return a;
}
vector<MintN> naive(const vector<MintN> &a,
		const vector<MintN> &b){
	vector<MintN> c(sz(a) + sz(b) - 1);
	FOR(i, 0, sz(a) - 1)
		FOR(j, 0, sz(b) - 1)
			c[i + j] += a[i] * b[j];
	return c;
}

int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	ull x = 7;
	for(int n: {1, 2, 8, 1024}){
		vector<MintN> a = randVec(n, x), b = a;
		ntt(a);
		ntt(a, true);
		assert(a == b);
	}
	vector<MintN> one{1, 0, 0, 0};
	ntt(one);
	assert(one == vector<MintN>(4, 1));
	assert(convNtt({}, {1, 2}).empty());
	assert(convNtt({1}, {}).empty());
	assert(convNtt({3}, {4}) == vector<MintN>{12});
	FOR(n, 1, 80)
		for(int m: {1, 32, 33, 80}){
			vector<MintN> a = randVec(n, x), b = randVec(m, x);
			assert(convNtt(a, b) == naive(a, b));
		}
	// the root table grows, then is reused at a small size
	for(int n: {64, 4096, 1 << 23, 64}){
		vector<MintN> a = randVec(n, x), b = a;
		if(n == 64) assert(convNtt(a, b) == naive(a, b));
		ntt(a);
		ntt(a, true);
		assert(a == b);
	}
	int n, m; cin >> n >> m;
	vector<MintN> a(n), b(m);
	for(MintN &v: a) cin >> v;
	for(MintN &v: b) cin >> v;
	vector<MintN> c = convNtt(a, b);
	FOR(i, 0, sz(c) - 1)
		cout << c[i] << " \n"[i == sz(c) - 1];
}
