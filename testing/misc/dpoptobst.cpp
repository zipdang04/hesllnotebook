#include "../../tex/contents/misc/dpopt.hpp"

// AOJ ALDS1_10_D: probabilities have 4 decimals -> integers * 1e4, exact in ll
int main(){
	int n; scanf("%d", &n);
	vector<ll> p(n + 1), q(n + 1), s(2 * n + 2);
	auto rd = [](){ double x; scanf("%lf", &x); return (ll)llround(x * 10000); };
	FOR(int, i, 1, n) p[i] = rd();
	FOR(int, i, 0, n) q[i] = rd();
	// dummies 0..n; range [a, b] of dummies holds keys a+1..b; split k: root key k+1
	vector<ll> pre(n + 2);
	FOR(int, i, 0, n) pre[i + 1] = pre[i] + q[i] + (i ? p[i] : 0);
	auto dp = knuth(n + 1, [&](int a, int b){ return pre[b + 1] - pre[a] - (a ? p[a] : 0); }, [&](int i){ return q[i]; });
	printf("%.6f\n", dp[0][n] / 10000.0);
}
