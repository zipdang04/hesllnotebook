#include "../../tex/contents/misc/dpopt.hpp"

// AOJ ALDS1_10_D: probabilities have 4 decimals -> integers * 1e4, exact in ll.
// Position i = dummy q_{i-1} (i = 1..n+1); range [a, b] holds keys a..b-1, split k = root key k.
// dp[i][i] = q_{i-1} is folded out: every tree on [a, b] has each dummy as a leaf once, so answer = dp[1][n+1] + sum q.
int main(){
	int n; scanf("%d", &n);
	vector<ll> P(n + 1), Q(n + 2);
	auto rd = [](){ double x; scanf("%lf", &x); return (ll)llround(x * 10000); };
	FOR(int, i, 1, n) P[i] = P[i - 1] + rd();
	FOR(int, i, 1, n + 1) Q[i] = Q[i - 1] + rd();
	auto dp = knuth(n + 1, [&](int a, int b){ return P[b - 1] - P[a - 1] + Q[b] - Q[a - 1]; });
	printf("%.6f\n", (dp[1][n + 1] + Q[n + 1]) / 10000.0);
}
