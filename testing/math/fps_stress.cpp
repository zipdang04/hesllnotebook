#include "../../tex/contents/math/fps.hpp"
#include <cassert>

vector<MintN> bruteInv(const vector<MintN> &a, int n){
	vector<MintN> g(n);
	MintN inv = a[0].inv();
	g[0] = inv;
	FOR(k, 1, n - 1){
		FOR(j, 1, min(k, sz(a) - 1))
			g[k] -= a[j] * g[k-j];
		g[k] *= inv;
	}
	return g;
}

vector<MintN> bruteLog(const vector<MintN> &a, int n){
	vector<MintN> inv = bruteInv(a, n), g(n);
	FOR(k, 1, n - 1){
		FOR(j, 1, min(k, sz(a) - 1))
			g[k] += MintN(j) * a[j] * inv[k-j];
		g[k] /= MintN(k);
	}
	return g;
}

vector<MintN> bruteExp(const vector<MintN> &a, int n){
	vector<MintN> g(n);
	g[0] = 1;
	FOR(k, 1, n - 1){
		FOR(j, 1, min(k, sz(a) - 1))
			g[k] += MintN(j) * a[j] * g[k-j];
		g[k] /= MintN(k);
	}
	return g;
}

vector<MintN> bruteDiv(vector<MintN> a,
		const vector<MintN> &b){
	int k = max(0, sz(a) - sz(b) + 1);
	vector<MintN> q(k);
	MintN inv = b.back().inv();
	REV(i, k - 1, 0){
		q[i] = a[i + sz(b) - 1] * inv;
		FOR(j, 0, sz(b) - 1)
			a[i+j] -= q[i] * b[j];
	}
	return q;
}

int main(){
	mt19937 rng(634);
	assert(fpsExp({}, 1) == vector<MintN>{1});
	FOR(it, 1, 500){
		int n = rng() % 70 + 1;
		vector<MintN> a(rng() % 90 + 1);
		vector<MintN> b(rng() % 40 + 1);
		for(auto &x: a) x = rng() % 998244353;
		for(auto &x: b) x = rng() % 998244353;
		b.back() = rng() % 998244352 + 1;
		a[0] = rng() % 998244352 + 1;
		assert(fpsInv(a, n) == bruteInv(a, n));
		a[0] = 1;
		assert(fpsLog(a, n) == bruteLog(a, n));
		vector<MintN> want = a;
		want.resize(n);
		assert(fpsExp(fpsLog(a, n), n) == want);
		a[0] = 0;
		assert(fpsExp(a, n) == bruteExp(a, n));
		want = a;
		want.resize(n);
		assert(fpsLog(fpsExp(a, n), n) == want);
		a[0] = rng() % 998244353;
		vector<MintN> q = polyDiv(a, b);
		assert(q == bruteDiv(a, b));
		FOR(i, 0, sz(q) - 1)
			FOR(j, 0, sz(b) - 1)
				a[i+j] -= q[i] * b[j];
		FOR(i, sz(b) - 1, sz(a) - 1)
			assert(a[i] == MintN(0));
	}
}
