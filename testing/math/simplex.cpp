#include "../../tex/contents/math/simplex.hpp"
typedef __int128 L;
typedef vector<vector<ll>> M;

// exact brute: rows R (coef), rhs h; region {x : R x <= h}. best vertex of max c.x as fraction
// returns: false if no vertex; else (num, den>0)
ll det(M a){ // Bareiss, exact
	int n = a.size(); ll sg = 1, prev = 1;
	FOR(int, k, 0, n - 1){
		if (!a[k][k]){ int p = k + 1; while (p < n && !a[p][k]) p++; if (p == n) return 0; swap(a[p], a[k]); sg = -sg; }
		FOR(int, i, k + 1, n - 1){ FOR(int, j, k + 1, n - 1) a[i][j] = (ll)(((L)a[i][j] * a[k][k] - (L)a[i][k] * a[k][j]) / prev); }
		prev = a[k][k];
	}
	return n ? sg * a[n - 1][n - 1] : 1;
}
bool bestVertex(const M& R, const vector<ll>& h, const vector<ll>& c, L& bn, L& bd){
	int k = R.size(), n = c.size(); bool any = false;
	if (n == 0){ // only point is empty vector
		FOR(int, i, 0, k - 1) if (h[i] < 0) return false;
		bn = 0; bd = 1; return true;
	}
	vector<int> idx(n);
	function<void(int, int)> rec = [&](int p, int t){
		if (t == n){
			M a(n, vector<ll>(n));
			FOR(int, i, 0, n - 1) FOR(int, j, 0, n - 1) a[i][j] = R[idx[i]][j];
			ll D = det(a); if (!D) return;
			vector<ll> X(n);
			FOR(int, j, 0, n - 1){ M b = a; FOR(int, i, 0, n - 1) b[i][j] = h[idx[i]]; X[j] = det(b); }
			if (D < 0){ D = -D; for (ll& v: X) v = -v; }
			FOR(int, i, 0, k - 1){ L s = 0; FOR(int, j, 0, n - 1) s += (L)R[i][j] * X[j]; if (s > (L)h[i] * D) return; }
			L num = 0; FOR(int, j, 0, n - 1) num += (L)c[j] * X[j];
			if (!any || num * bd > bn * D){ bn = num; bd = D; any = true; }
			return;
		}
		FOR(int, i, p, k - 1){ idx[t] = i; rec(i + 1, t + 1); }
	};
	rec(0, 0);
	return any;
}
// 0: infeasible, 1: optimal (val), 2: unbounded
int brute(const M& A, const vector<ll>& b, const vector<ll>& c, long double& val){
	int m = A.size(), n = c.size();
	M R = A; vector<ll> h = b;
	FOR(int, j, 0, n - 1){ vector<ll> r(n); r[j] = -1; R.push_back(r); h.push_back(0); }
	L bn, bd;
	if (!bestVertex(R, h, c, bn, bd)) return 0;
	M Q = A; vector<ll> g(m, 0); // ray cone, sum d <= 1
	FOR(int, j, 0, n - 1){ vector<ll> r(n); r[j] = -1; Q.push_back(r); g.push_back(0); }
	Q.push_back(vector<ll>(n, 1)); g.push_back(1);
	L rn, rd; bestVertex(Q, g, c, rn, rd);
	if (rn > 0) return 2;
	val = (long double)bn / (long double)bd; return 1;
}

mt19937 rng(12345);
ll rnd(ll a, ll b){ return uniform_int_distribution<ll>(a, b)(rng); }

int fails = 0; int cnt[3];
void check(const M& A, const vector<ll>& b, const vector<ll>& c){
	int m = A.size(), n = c.size();
	long double ex = 0; int st = brute(A, b, c, ex); cnt[st]++;
	vector<vector<double>> Ad(m, vector<double>(n)); vector<double> bd(b.begin(), b.end()), cd(c.begin(), c.end());
	FOR(int, i, 0, m - 1) FOR(int, j, 0, n - 1) Ad[i][j] = A[i][j];
	vector<double> x;
	double v = Simplex<double>(Ad, bd, cd).solve(x);
	bool bad = false;
	if (st == 0) bad = !(v == -numeric_limits<double>::infinity());
	else {
		if (st == 2) bad = !(v == numeric_limits<double>::infinity());
		else bad = !(fabsl(v - ex) <= 1e-6 * max(1.0L, fabsl(ex)));
		// x feasible always, c.x == v when optimal
		if ((int)x.size() != n) bad = true;
		else {
			FOR(int, j, 0, n - 1) if (x[j] < -1e-6) bad = true;
			FOR(int, i, 0, m - 1){ double s = 0; FOR(int, j, 0, n - 1) s += Ad[i][j] * x[j]; if (s > bd[i] + 1e-6 * max(1.0, fabs(bd[i]))) bad = true; }
			if (st == 1){ double s = 0; FOR(int, j, 0, n - 1) s += cd[j] * x[j]; if (fabs(s - v) > 1e-6 * max(1.0, fabs(v))) bad = true; }
		}
	}
	if (bad){
		fails++;
		if (fails <= 5){
			printf("FAIL m=%d n=%d brute st=%d val=%.9Lf got %.9f\n", m, n, st, ex, v);
			FOR(int, i, 0, m - 1){ FOR(int, j, 0, n - 1) printf("%lld ", A[i][j]); printf("<= %lld\n", b[i]); }
			printf("c:"); for (ll t: c) printf(" %lld", t); printf("\nx:"); for (double t: x) printf(" %.6f", t); puts("");
		}
	}
}

// big cases: strong duality. primal max c.x, Ax<=b, x>=0 ; dual max -b.y, -A^T y <= -c, y>=0 (= -min b.y)
template<class T> void dualCase(int m, int n, int sh){
	vector<vector<T>> A(m, vector<T>(n)), At(n, vector<T>(m)); vector<T> b(m), c(n), nb(m), nc(n), x, y;
	FOR(int, i, 0, m - 1){ FOR(int, j, 0, n - 1) A[i][j] = rnd(-100, 100), At[j][i] = -A[i][j]; b[i] = rnd(-1000 * sh, 1000); nb[i] = -b[i]; }
	FOR(int, j, 0, n - 1){ c[j] = rnd(-100, 100 * sh); nc[j] = -c[j]; }
	T p = Simplex<T>(A, b, c).solve(x), d = -Simplex<T>(At, nc, nb).solve(y);
	const T inf = numeric_limits<T>::infinity();
	bool bad = false;
	if (p == -inf) bad = !(d == inf || d == -inf);      // dual unbounded (d=-(+inf)... ) or infeasible
	else if (p == inf) bad = !(d == inf);               // dual infeasible: -(-inf) = inf
	else {
		bad = !(fabsl(p - d) <= 1e-6 * max(1.0L, fabsl(p)));
		T s = 0; FOR(int, j, 0, n - 1){ s += c[j] * x[j]; if (x[j] < -1e-6) bad = true; }
		if (fabsl(s - p) > 1e-6 * max(1.0L, fabsl(p))) bad = true;
		FOR(int, i, 0, m - 1){ T t = 0; FOR(int, j, 0, n - 1) t += A[i][j] * x[j]; if (t > b[i] + 1e-6 * max(1.0L, fabsl(b[i]))) bad = true; }
	}
	static int st[3]; st[p == -inf ? 0 : p == inf ? 2 : 1]++;
	if (bad){ fails++; printf("DUAL FAIL m=%d n=%d p=%.9Lf d=%.9Lf\n", m, n, (long double)p, (long double)d); }
	if (sh == 2) printf("dual stats inf %d opt %d unb %d\n", st[0], st[1], st[2]);
}

int main(){
	// Beale's cycling example (scaled to integers)
	check({{1, -32, -4, 36}, {1, -24, -1, 6}, {0, 0, 1, 0}}, {0, 0, 1}, {3, -80, 2, -24});
	int T = 60000;
	FOR(int, t, 0, T - 1){
		int mode = t % 7;
		int n = rnd(mode == 5 ? 0 : 1, 4), m = rnd(0, 6);
		ll C = mode == 4 ? 100 : 3, Bv = mode == 4 ? 1000 : 5;
		M A(m, vector<ll>(n)); vector<ll> b(m), c(n);
		FOR(int, i, 0, m - 1){ FOR(int, j, 0, n - 1) A[i][j] = rnd(-C, C); b[i] = rnd(-Bv, Bv); }
		FOR(int, j, 0, n - 1) c[j] = rnd(-C, C);
		if (mode == 1) FOR(int, i, 0, m - 1) if (rnd(0, 1)) b[i] = 0; // degenerate at origin
		if (mode == 2 && n){ // many constraints tight at one integer point p
			vector<ll> p(n); FOR(int, j, 0, n - 1) p[j] = rnd(0, 2);
			FOR(int, i, 0, m - 1) if (rnd(0, 2)){ b[i] = 0; FOR(int, j, 0, n - 1) b[i] += A[i][j] * p[j]; }
		}
		if (mode == 3) FOR(int, i, 0, m - 1) if (i && rnd(0, 1)){ // duplicates / equalities / sums
			int k = rnd(0, i - 1), w = rnd(0, 2);
			if (w == 0){ A[i] = A[k]; b[i] = b[k]; }
			else if (w == 1){ FOR(int, j, 0, n - 1) A[i][j] = -A[k][j]; b[i] = -b[k]; }
			else { int l = rnd(0, i - 1); FOR(int, j, 0, n - 1) A[i][j] = A[k][j] + A[l][j]; b[i] = b[k] + b[l]; }
		}
		if (mode == 6 && n){ // equalities a.x = a.p (pairs of rows), some redundant combos, p >= 0 feasible or shifted
			vector<ll> p(n); FOR(int, j, 0, n - 1) p[j] = rnd(0, 2);
			int k = rnd(1, 3); A.clear(); b.clear();
			FOR(int, q, 0, k - 1){
				vector<ll> a(n); FOR(int, j, 0, n - 1) a[j] = rnd(-3, 3);
				if (q >= 2 || (q && rnd(0, 1))) FOR(int, j, 0, n - 1) a[j] = A[0][j] * rnd(-1, 1) + A[2 * (q - 1)][j] * rnd(-1, 1);
				ll v = 0; FOR(int, j, 0, n - 1) v += a[j] * p[j];
				if (!rnd(0, 5)) v += rnd(-1, 1);
				A.push_back(a); b.push_back(v);
				vector<ll> na(n); FOR(int, j, 0, n - 1) na[j] = -a[j];
				A.push_back(na); b.push_back(-v);
			}
			FOR(int, q, 0, (rnd(0, 1)) - 1){ vector<ll> a(n); FOR(int, j, 0, n - 1) a[j] = rnd(-3, 3); A.push_back(a); b.push_back(rnd(-5, 5)); }
		}
		check(A, b, c);
	}
	FOR(int, t, 0, 299){ int m = rnd(1, 40), n = rnd(1, 40); if (t % 2) dualCase<double>(m, n, t % 4 == 1); else dualCase<long double>(m, n, t % 4 == 0); }
	dualCase<double>(1, 1, 2);
	{ // performance + duality at m = n = 200: feasible (b = A x* + slack, many b < 0) and bounded (row sum x <= 1e4)
		int m = 200, n = 200; vector<vector<double>> A(m, vector<double>(n)), At(n, vector<double>(m)); vector<double> b(m), c(n), nb(m), nc(n), x, y, xs(n);
		FOR(int, j, 0, n - 1) xs[j] = rnd(0, 10);
		FOR(int, i, 0, m - 1){
			b[i] = rnd(0, 100);
			FOR(int, j, 0, n - 1){ A[i][j] = i ? rnd(-100, 100) : 1; b[i] += A[i][j] * xs[j]; }
			if (!i) b[i] = 10000;
		}
		FOR(int, j, 0, n - 1) c[j] = rnd(-50, 100);
		FOR(int, i, 0, m - 1){ nb[i] = -b[i]; FOR(int, j, 0, n - 1) At[j][i] = -A[i][j]; }
		FOR(int, j, 0, n - 1) nc[j] = -c[j];
		int neg = 0; FOR(int, i, 0, m - 1) neg += b[i] < 0;
		auto t0 = clock(); double p = Simplex<double>(A, b, c).solve(x), d = -Simplex<double>(At, nc, nb).solve(y);
		printf("big 200x200 (%d rows b<0) primal %.6f dual %.6f time %.3f\n", neg, p, d, (double)(clock() - t0) / CLOCKS_PER_SEC);
		if (!(fabs(p) < 1e18 && fabs(p - d) <= 1e-6 * max(1.0, fabs(p)))) fails++;
	}
	printf("cases %d infeasible %d optimal %d unbounded %d fails %d\n", T + 1, cnt[0], cnt[1], cnt[2], fails);
	return fails ? 1 : 0;
}
