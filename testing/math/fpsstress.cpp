#include "../../tex/contents/math/fps.hpp"
using FPS::poly;

mt19937 rng(1);
modint rnd(bool nonzero = false){
	while (true){ modint x = rng() % 998244353; if (!nonzero || x != 0) return x; }
}
poly rndPoly(int len){ poly a(len); for (auto &x: a) x = rnd(); return a; }
modint at(const poly &a, int i){ return i < (int)a.size() ? a[i] : 0; }
// naive O(n^2) references; all take the first n terms of a (a may be shorter or longer than n)
poly naiveMul(const poly &a, const poly &b){
	if (a.empty() || b.empty()) return {};
	poly c(a.size() + b.size() - 1);
	EFOR(int, i, 0, (int)a.size()) EFOR(int, j, 0, (int)b.size()) c[i+j] += a[i] * b[j];
	return c;
}
poly naiveInv(const poly &a, int n){
	poly b(n); modint i0 = a[0].inv();
	EFOR(int, i, 0, n){
		modint s = i == 0 ? 1 : 0;
		FOR(int, j, 1, i) s -= at(a, j) * b[i-j];
		b[i] = s * i0;
	}
	return b;
}
poly naiveLog(const poly &a, int n){ // i b_i = i a_i - sum_{k<i} k b_k a_{i-k}
	poly b(n);
	EFOR(int, i, 1, n){
		modint s = at(a, i) * i;
		EFOR(int, k, 1, i) s -= b[k] * k * at(a, i-k);
		b[i] = s / i;
	}
	return b;
}
poly naiveExp(const poly &a, int n){ // i e_i = sum_{k=1..i} k a_k e_{i-k}
	poly e(n); e[0] = 1;
	EFOR(int, i, 1, n){
		modint s = 0;
		FOR(int, k, 1, i) s += at(a, k) * k * e[i-k];
		e[i] = s / i;
	}
	return e;
}
pair<poly, poly> naiveDiv(poly a, const poly &b){
	int n = a.size(), m = b.size();
	if (n < m) return {{}, a};
	poly q(n-m+1); modint ib = b.back().inv();
	REV(int, i, n-m, 0){
		q[i] = a[i+m-1] * ib;
		EFOR(int, j, 0, m) a[i+j] -= q[i] * b[j];
	}
	a.resize(m-1); return {q, a};
}
void fail(const char *what, int n, int len){
	printf("FAIL %s n=%d |a|=%d\n", what, n, len); exit(1);
}
int main(){
	FOR(int, n, 1, 300) EFOR(int, rep, 0, 3){
		int len = 1 + rng() % (n + 6); // shorter, equal or longer than n
		poly a = rndPoly(len), b = rndPoly(1 + rng() % (n + 6));
		if (FPS::mul(a, b) != naiveMul(a, b)) fail("mul", n, len);
		a[0] = rnd(true);
		if (FPS::inv(a, n) != naiveInv(a, n)) fail("inv", n, len);
		a[0] = 1;
		if (FPS::log(a, n) != naiveLog(a, n)) fail("log", n, len);
		a[0] = 0;
		if (FPS::exp(a, n) != naiveExp(a, n)) fail("exp", n, len);
		// divMod: random sizes, b.back() != 0, a may have trailing zeros
		poly f = rndPoly(rng() % (n + 1)), g = rndPoly(1 + rng() % n);
		g.back() = rnd(true);
		if (!f.empty() && rng() % 3 == 0) f.back() = 0;
		auto qr = FPS::divMod(f, g);
		if (qr != naiveDiv(f, g)) fail("divMod", (int)f.size(), (int)g.size());
	}
	if (!FPS::mul({}, {1}).empty()) fail("mul empty", 0, 0);
	// large: identities at n = 2^17 + 3 (non power of two)
	int n = (1 << 17) + 3;
	poly a = rndPoly(n); a[0] = 1;
	poly one = FPS::mul(a, FPS::inv(a, n)); one.resize(n);
	poly e(n); e[0] = 1;
	if (one != e) fail("inv large", n, n);
	if (FPS::exp(FPS::log(a, n), n) != a) fail("exp(log) large", n, n);
	puts("ok");
}
