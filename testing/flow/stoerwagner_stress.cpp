#include "../../tex/contents/flow/stoerwagner.hpp"
#include <cassert>

ll across(const vector<vector<pair<int, ll>>> &g,
		int mask){
	ll res = 0;
	FOR(u, 1, sz(g) - 1){
		if(!(mask & (1 << (u - 1)))) continue;
		for(auto [v, w]: g[u]) if(!(mask & (1 << (v - 1))))
			res += w;
	}
	return res;
}
int main(){
	mt19937 rng(123456789);
	FOR(tc, 1, 2000){
		int n = 1 + (int)(rng() % 10);
		int m = (int)(rng() % 16);
		vector<vector<pair<int, ll>>> g(n + 1);
		FOR(i, 1, m){
			int u = 1 + (int)(rng() % n);
			int v = 1 + (int)(rng() % n);
			ll w = rng() % 11;
			g[u].push_back({v, w});
			g[v].push_back({u, w});
		}
		ll best = oo;
		FOR(mask, 1, (1 << n) - 2)
			best = min(best, across(g, mask));
		auto [cut, side] = globalMinCut(g);
		assert(cut == best);
		if(n == 1){
			assert(side.empty());
			continue;
		}
		assert(!side.empty() && sz(side) < n);
		int mask = 0;
		for(int u: side){
			assert(1 <= u && u <= n);
			assert(!(mask & (1 << (u - 1))));
			mask |= 1 << (u - 1);
		}
		assert(across(g, mask) == cut);
	}
	return 0;
}
