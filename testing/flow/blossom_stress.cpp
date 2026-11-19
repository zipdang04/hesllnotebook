#include "../../tex/contents/flow/blossom.hpp"

int main(){
	mt19937 rng(1);
	FOR(tc, 1, 200000){
		bool dense = tc % 4 == 0;
		int n = dense ? 10 : rng() % 11;
		vector<vector<int>> g(n + 1);
		vector<int> adj(n);
		if(n) g[0] = {1};
		auto add = [&](int u, int v){
			g[u].push_back(v);
			if(u != v || rng() % 2)
				g[v].push_back(u);
			if(u != v){
				adj[u-1] |= 1 << (v - 1);
				adj[v-1] |= 1 << (u - 1);
			}
		};
		if(dense){
			FOR(u, 1, n) FOR(v, u + 1, n)
				if(rng() % 10 < 7) add(u, v);
		}else if(n){
			int m = rng() % 25;
			FOR(i, 1, m){
				int u = rng() % n + 1;
				int v = rng() % n + 1;
				add(u, v);
			}
		}
		vector<int> dp(1 << n);
		FOR(mask, 1, (1 << n) - 1){
			int i = __builtin_ctz(mask);
			int rest = mask ^ (1 << i);
			dp[mask] = dp[rest];
			FOR(j, 0, n - 1)
				if((rest & adj[i]) & (1 << j))
					dp[mask] = max(dp[mask],
						1 + dp[rest ^ (1 << j)]);
		}
		GeneralMatching gm(g);
		bool ok = gm.pairs == dp[(1 << n) - 1];
		ok &= (int)gm.match.size() == n + 1;
		int matched = 0;
		if((int)gm.match.size() == n + 1){
			FOR(u, 1, n){
				int v = gm.match[u];
				if(!v) continue;
				matched++;
				if(v < 1 || v > n || v == u){
					ok = false;
					continue;
				}
				ok &= gm.match[v] == u;
				ok &= (adj[u-1] & (1 << (v - 1))) != 0;
			}
		}
		ok &= matched == 2 * gm.pairs;
		if(!ok){
			cerr << "case " << tc << '\n';
			return 1;
		}
	}
	return 0;
}
