#include "../../tex/contents/math/fwht.hpp"
int main(){
	mt19937_64 rng(12345);
	FOR(int, seed, 1, 3000){
		int N = rng() % 7, n = 1 << N; ll M = seed % 2 ? 5 : 998244353;
		vector<ll> A(n), B(n);
		for (ll &x: A) x = rng() % M;
		for (ll &x: B) x = rng() % M;
		auto fail = [&](const char *what){
			printf("FAIL %s seed=%d N=%d\n", what, seed, N); exit(1);
		};
		// convolve + fwht round trip for all ops
		for (char op: {'^', '|', '&'}){
			vector<modint> a(A.begin(), A.end()), b(B.begin(), B.end()), c(n);
			FOR(int, i, 0, n - 1) FOR(int, j, 0, n - 1){
				int k = op == '^' ? i ^ j : op == '|' ? i | j : i & j;
				c[k] += a[i] * b[j];
			}
			if (convolve(a, b, op) != c) fail("convolve");
			vector<modint> t = a; fwht(t, op, 0); fwht(t, op, 1);
			if (t != a) fail("inverse");
		}
		// or/and transform = subset/superset sums
		{
			vector<modint> a(A.begin(), A.end()), o = a, d = a;
			fwht(o, '|', 0); fwht(d, '&', 0);
			FOR(int, s, 0, n - 1){
				modint so = 0, sd = 0;
				FOR(int, t, 0, n - 1){
					if ((t & s) == t) so += a[t];
					if ((t & s) == s) sd += a[t];
				}
				if (o[s] != so || d[s] != sd) fail("subset/superset sums");
			}
		}
		// max/min over subsets/supersets (ll)
		for (int mx = 0; mx < 2; mx++) for (int sup = 0; sup < 2; sup++){
			vector<ll> a = A;
			sos(a, [&](ll p, ll q){ return mx ? max(p, q) : min(p, q); }, sup);
			FOR(int, s, 0, n - 1){
				ll r = mx ? LLONG_MIN : LLONG_MAX;
				FOR(int, t, 0, n - 1) if (sup ? (t & s) == s : (t & s) == t)
					r = mx ? max(r, A[t]) : min(r, A[t]);
				if (a[s] != r) fail("sos max/min");
			}
		}
	}
	puts("OK");
}
