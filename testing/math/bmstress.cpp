#include "../../tex/contents/math/bm.hpp"
using FPS::poly;

mt19937_64 rng(1);
ll rnd(ll lo, ll hi){ return uniform_int_distribution<ll>(lo, hi)(rng); }
modint rndMod(bool nonzero = false){ modint x; do x = rnd(0, 998244352); while (nonzero && x == 0); return x; }
void rndFill(poly &a){ for (auto &x: a) x = rndMod(); }
void fail(const string &what){ cout << "FAIL " << what << endl; exit(1); }

bool satisfies(const poly &s, const poly &c){ // s[i] = sum c[j] s[i-1-j] for |c| <= i < |s|
	EFOR(int, i, (int)c.size(), (int)s.size()){
		modint t = 0; EFOR(int, j, 0, (int)c.size()) t += c[j] * s[i-1-j];
		if (t != s[i]) return false;
	}
	return true;
}
bool consistent(const poly &s, int d){ // does an order-d recurrence exist? Gauss on [A|b]
	vector<poly> a;
	EFOR(int, i, d, (int)s.size()){ poly r(d+1); EFOR(int, j, 0, d) r[j] = s[i-1-j]; r[d] = s[i]; a.push_back(r); }
	int row = 0;
	EFOR(int, col, 0, d){
		int p = row; while (p < (int)a.size() && a[p][col] == 0) p++;
		if (p == (int)a.size()) continue;
		swap(a[p], a[row]); modint iv = a[row][col].inv();
		EFOR(int, r, 0, (int)a.size()) if (r != row && a[r][col] != 0){
			modint f = a[r][col] * iv; FOR(int, j, col, d) a[r][j] -= f * a[row][j];
		}
		row++;
	}
	EFOR(int, r, row, (int)a.size()) if (a[r][d] != 0) return false;
	return true;
}
modint naiveKth(poly a, const poly &c, ll k){ // extend the recurrence term by term
	int d = c.size(); if (d == 0) return 0;
	while ((ll)a.size() <= k){
		modint t = 0; EFOR(int, j, 0, d) t += c[j] * a[a.size()-1-j]; a.push_back(t);
	}
	return a[k];
}
typedef vector<poly> mat;
mat matMul(const mat &x, const mat &y){
	int n = x.size(); mat z(n, poly(n));
	EFOR(int, i, 0, n) EFOR(int, l, 0, n) EFOR(int, j, 0, n) z[i][j] += x[i][l] * y[l][j];
	return z;
}
modint matKth(const poly &a, const poly &c, ll k){ // companion matrix power, O(d^3 log k)
	int d = c.size(); if (d == 0) return 0;
	mat M(d, poly(d)), R(d, poly(d));
	EFOR(int, j, 0, d) M[0][j] = c[j];
	EFOR(int, i, 1, d) M[i][i-1] = 1;
	EFOR(int, i, 0, d) R[i][i] = 1;
	for (ll e = k; e; e /= 2, M = matMul(M, M)) if (e & 1) R = matMul(R, M);
	modint t = 0; // state (a_{d-1}, ..., a_0) -> R[d-1] row gives a_k
	EFOR(int, j, 0, d) t += R[d-1][j] * a[d-1-j];
	return t;
}
modint naiveDiv(const poly &P, const poly &Q, ll k){ // [x^k] P/Q by series division
	poly r(k+1); modint iq = Q[0].inv();
	FOR(int, i, 0, (int)k){
		modint t = i < (int)P.size() ? P[i] : 0;
		FOR(int, j, 1, min(i, (int)Q.size()-1)) t -= Q[j] * r[i-j];
		r[i] = t * iq;
	}
	return r[k];
}

int main(){
	// 1. BM minimality + validity on short / degenerate sequences (many zeros), N = 0..9
	EFOR(int, it, 0, 20000){
		int n = rnd(0, 9); poly s(n);
		for (auto &x: s) x = it % 3 == 0 ? rndMod() : modint(rnd(0, 2) * (rnd(0, 2) > 0));
		poly c = berlekampMassey(s);
		if (!satisfies(s, c)) fail("BM invalid n=" + to_string(n));
		int d = 0; while (!consistent(s, d)) d++;
		if ((int)c.size() != d) fail("BM not minimal n=" + to_string(n));
	}
	if (!berlekampMassey({}).empty() || !berlekampMassey(poly(50)).empty()) fail("BM empty / all zeros");
	// 2. BM recovers an order-d recurrence from 2d terms; kthTerm(s, BM(s), k) continues it
	EFOR(int, it, 0, 300){
		int d = rnd(1, 60); poly c(d), a(d);
		rndFill(c); rndFill(a);
		poly s(2*d); EFOR(int, i, 0, 2*d) s[i] = naiveKth(a, c, i);
		poly g = berlekampMassey(s);
		if ((int)g.size() > d) fail("BM too long d=" + to_string(d));
		ll k = rnd(0, 10*d);
		if (kthTerm(s, g, k) != naiveKth(a, c, k)) fail("BM+kth d=" + to_string(d));
	}
	// 3. kthTerm vs naive, d = 0..12 (d = 0: all zeros), k < d included
	EFOR(int, it, 0, 3000){
		int d = rnd(0, 12); poly c(d), a(d);
		rndFill(c); rndFill(a);
		ll k = rnd(0, 50);
		if (kthTerm(a, c, k) != naiveKth(a, c, k)) fail("kth d=" + to_string(d) + " k=" + to_string(k));
	}
	EFOR(int, it, 0, 20){ // larger d, |a| > d (extra terms ignored)
		int d = rnd(100, 400); poly c(d), a(d + rnd(0, 5));
		rndFill(c); rndFill(a);
		ll k = rnd(0, 3000);
		if (kthTerm(a, c, k) != naiveKth(poly(a.begin(), a.begin() + d), c, k)) fail("kth large d=" + to_string(d));
	}
	// 4. k up to 1e18 vs companion-matrix power, d = 1..6
	EFOR(int, it, 0, 2000){
		int d = rnd(1, 6); poly c(d), a(d);
		rndFill(c); rndFill(a);
		ll k = it < 50 ? (ll)1e18 - it : rnd(0, (ll)1e18);
		if (kthTerm(a, c, k) != matKth(a, c, k)) fail("kth big k d=" + to_string(d) + " k=" + to_string(k));
	}
	// 5. bostanMori with any |P| (empty, >= |Q|), Q[0] != 0
	EFOR(int, it, 0, 5000){
		int q = rnd(1, 8), p = rnd(0, 14); poly P(p), Q(q);
		rndFill(P); rndFill(Q);
		Q[0] = rndMod(true);
		ll k = rnd(0, 60);
		if (bostanMori(P, Q, k) != naiveDiv(P, Q, k)) fail("bostanMori |P|=" + to_string(p) + " |Q|=" + to_string(q) + " k=" + to_string(k));
	}
	cout << "ok" << endl;
}
