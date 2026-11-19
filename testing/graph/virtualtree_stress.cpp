#include "../../tex/contents/graph/virtualtree.hpp"
#include <cassert>

int main(){
	mt19937 rng(12345);
	FOR(trial, 1, 20000){
		int n = rng() % 12 + 1;
		vector<int> label(n);
		iota(all(label), 1);
		shuffle(all(label), rng);
		vector<vector<int>> g(n + 1);
		FOR(i, 1, n - 1){
			int u = label[i], v = label[rng() % i];
			g[u].push_back(v); g[v].push_back(u);
		}
		int root = rng() % n + 1;
		vector<int> par(n + 1), depth(n + 1), q{root};
		FOR(i, 0, n - 1) for(int v: g[q[i]]){
			int u = q[i];
			if(v == par[u]) continue;
			par[v] = u; depth[v] = depth[u] + 1;
			q.push_back(v);
		}
		auto lca = [&](int u, int v){
			while(depth[u] > depth[v]) u = par[u];
			while(depth[v] > depth[u]) v = par[v];
			while(u != v) u = par[u], v = par[v];
			return u;
		};
		vector<int> vs(rng() % (n + 1));
		for(int &v: vs) v = rng() % n + 1;
		vector<int> want(n + 1), edges(n + 1);
		for(int u: vs) want[u] = 1;
		int k = accumulate(all(want), 0);
		for(int u: vs){
			for(int v: vs){
				int w = lca(u, v);
				want[w] = 1;
				for(int x = u; x != w; x = par[x])
					edges[x] = 1;
				for(int x = v; x != w; x = par[x])
					edges[x] = 1;
			}
		}
		HLD hld(g, root);
		auto t = virtualTree(hld, vs);
		if(k) assert(sz(t) <= 2 * k - 1);
		vector<int> got(n + 1);
		int length = 0, last = 0;
		for(auto [v, p]: t){
			assert(1 <= v && v <= n);
			assert(++got[v] == 1);
			assert(hld.pos[v] > last);
			assert((p == 0) == (last == 0));
			last = hld.pos[v];
			int expected = par[v];
			while(expected && !want[expected])
				expected = par[expected];
			assert(p == expected);
			if(p) length += hld.depth[v] - hld.depth[p];
		}
		assert(got == want);
		assert(length == accumulate(all(edges), 0));
	}
	return 0;
}
