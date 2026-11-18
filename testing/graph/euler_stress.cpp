#include "../../tex/contents/graph/euler.hpp"
#include <cassert>

bool brute(int u, int mask, bool directed,
		const vector<pair<int, int>> &edges){
	int m = sz(edges);
	if(mask == (1 << m) - 1) return true;
	FOR(i, 0, m - 1){
		if(mask & (1 << i)) continue;
		auto [a, b] = edges[i];
		if(!directed && b == u) swap(a, b);
		if(a != u) continue;
		bool ok = brute(b, mask | (1 << i), directed, edges);
		if(ok) return true;
	}
	return false;
}

int main(){
	mt19937 rng(12345);
	FOR(test, 1, 5000){
		bool directed = rng() % 2;
		int n = rng() % 5 + 1, m = rng() % 8;
		int base = rng() % 2, s = rng() % n + 1;
		vector<pii> edges(m);
		vector<vector<pii>> g(n + 1);
		FOR(i, 0, m - 1){
			int u = rng() % n + 1, v = rng() % n + 1;
			edges[i] = {u, v};
			g[u].push_back({v, i + base});
			if(!directed && (u != v || rng() % 2))
				g[v].push_back({u, i + base});
		}
		g[0].push_back({1, 0});
		for(auto &adj: g) shuffle(all(adj), rng);
		bool exists = brute(s, 0, directed, edges);
		auto w = eulerWalk(g, m, s);
		assert(w.empty() == !exists);
		if(w.empty()) continue;
		assert(sz(w) == m + 1);
		assert(w[0] == make_pair(s, -1));
		vector<char> used(m);
		FOR(i, 1, m){
			auto [v, id] = w[i];
			int u = w[i-1].fi;
			assert(1 <= v && v <= n);
			assert(base <= id && id < base + m);
			id -= base;
			assert(!used[id]);
			used[id] = 1;
			auto [a, b] = edges[id];
			if(!directed && b == u) swap(a, b);
			assert(a == u && b == v);
		}
	}
}
