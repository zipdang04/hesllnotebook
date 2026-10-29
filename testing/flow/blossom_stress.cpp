#include "../../tex/contents/flow/blossom.hpp"
int main(){
	mt19937 rng(1);
	for (int it = 0; it < 200000; it++){
		int n = rng() % 13 + 1; double p = (rng() % 100) / 100.0;
		vector<pair<int,int>> e; vector<int> adj(n);
		FOR(int, i, 0, n - 1) FOR(int, j, i+1, n - 1) if (rng() % 100 < p * 100) e.push_back({i, j});
		shuffle(e.begin(), e.end(), rng);
		Blossom b(n); for (auto [u, v]: e){ if (rng()%2) swap(u,v); b.addEdge(u, v); adj[u] |= 1<<v; adj[v] |= 1<<u; }
		int k = b.run();
		vector<int> dp(1<<n, 0);
		FOR(int, m, 1, (1<<n) - 1){ int i = __builtin_ctz(m); int r = m & ~(1<<i); dp[m] = dp[r];
			for (int x = r & adj[i]; x; x &= x-1){ int j = __builtin_ctz(x); dp[m] = max(dp[m], dp[r & ~(1<<j)] + 1);} }
		int c = 0;
		FOR(int, i, 0, n - 1){ int j = b.mate[i]; if (j != n){ if (b.mate[j] != i || !(adj[i]>>j&1)) {puts("bad mate"); return 1;} c++; } }
		if (k != dp[(1<<n)-1] || c != 2*k){ printf("FAIL it=%d n=%d k=%d dp=%d c=%d\n", it, n, k, dp[(1<<n)-1], c); return 1; }
	}
	puts("ok");
}
