#include "../../tex/contents/flow/gomoryhu.hpp"
mt19937_64 mt;
ll rnd(ll a, ll b) { return uniform_int_distribution<ll>(a, b)(mt); }
int main(){
	for (int seed = 0; seed < 10000; seed++){
		mt.seed(seed);
		int n = seed % 10 == 0 ? rnd(1, 2) : rnd(2, 12);
		ll W = vector<ll>{0, 1, 10, (ll)1e9, (ll)2.7e16}[rnd(0, 4)];
		int m = rnd(0, 3 * n), k = 0;
		if (n > 1 && rnd(0, 3) == 0) k = rnd(1, n - 1);
		vector<array<ll,3>> e;
		FOR(int, i, 0, m - 1){
			int u = rnd(0, n-1), v = rnd(0, n-1);
			if (k && (u < k) != (v < k)) continue;
			e.push_back({u, v, rnd(0, W)});
		}
		vector<ll> cw(1 << n);
		FOR(int, mask, 0, (1 << n) - 1) for (auto &[u, v, w]: e) if ((mask >> u & 1) != (mask >> v & 1)) cw[mask] += w;
		GomoryHu g(n); for (auto &[u, v, w]: e) g.addEdge(u, v, w);
		g.run();
		// tree: build adjacency, check connected with n-1 edges
		vector<vector<pair<int,ll>>> adj(n);
		bool ok = true;
		FOR(int, i, 1, n - 1){ int p = g.par[i]; if (p < 0 || p >= n || p == i) ok = false; else adj[i].push_back({p, g.cut[i]}), adj[p].push_back({i, g.cut[i]}); }
		FOR(int, s, 0, n - 1){
			vector<ll> mn(n, -1); mn[s] = LLONG_MAX; vector<int> st{s};
			while (!st.empty()){ int u = st.back(); st.pop_back(); for (auto [v, w]: adj[u]) if (mn[v] < 0) mn[v] = min(mn[u], w), st.push_back(v); }
			FOR(int, t, 0, n - 1) if (t != s){
				ll ex = LLONG_MAX;
				FOR(int, mask, 0, (1 << n) - 1) if ((mask >> s & 1) && !(mask >> t & 1)) ex = min(ex, cw[mask]);
				if (mn[t] != ex){ ok = false; printf("seed %d pair %d %d got %lld exp %lld\n", seed, s, t, mn[t], ex); break; }
			}
			if (!ok) break;
		}
		// min-cut side check for a random pair
		if (n >= 2){
			int s = rnd(0, n-1), t = rnd(0, n-2); if (t >= s) t++;
			Flow f = g.base; f.s = s; f.t = t; ll fl = f.calculate(); int mask = 0;
			FOR(int, v, 0, n - 1) if (f.level[v] < 0) mask |= 1 << v;
			if (!(mask >> s & 1) || (mask >> t & 1) || cw[mask] != fl){ ok = false; printf("seed %d side bad\n", seed); }
		}
		if (!ok){ printf("n=%d\n", n); for (auto &[u,v,w]: e) printf("%lld %lld %lld\n", u, v, w); return 1; }
	}
	puts("ok");
}
