#include "../../tex/contents/graph/hld.hpp"
#include "../../tex/contents/ds/lazyseg.hpp"
#include <cassert>

void check(const vector<vector<int>> &g, int root,
		const vector<ll> &weight){
	int n = sz(g) - 1;
	vector<int> par(n + 1), depth(n + 1), q{root};
	FOR(i, 0, n - 1) for(int v: g[q[i]]){
		int u = q[i];
		if(v == par[u]) continue;
		par[v] = u; depth[v] = depth[u] + 1;
		q.push_back(v);
	}
	HLD h(g, root);
	assert(h.timer == n);
	vector<int> used(n + 1), sub(n + 1);
	vector<ll> base(n + 1);
	vector<LazySeg::Node> init(n + 1);
	FOR(u, 1, n) init[h.pos[u]] = {weight[u], 1};
	LazySeg seg(init);
	FOR(u, 1, n){
		assert(h.par[u] == par[u]);
		assert(h.depth[u] == depth[u]);
		assert(1 <= h.pos[u] && h.pos[u] <= n);
		assert(++used[h.pos[u]] == 1);
		base[h.pos[u]] = weight[u];
	}
	FOR(u, 1, n){
		vector<char> ancestor(n + 1);
		for(int x = u; x; x = par[x]) ancestor[x] = true;
		int l = h.pos[u], r = l + h.sub[u] - 1;
		assert(l <= r && r <= n);
		FOR(v, 1, n){
			int x = v;
			while(x && x != u) x = par[x];
			bool inside = l <= h.pos[v] && h.pos[v] <= r;
			assert(inside == (x == u));
			sub[u] += x == u;
		}
		assert(h.sub[u] == sub[u]);
		FOR(v, 1, n){
			int w = v;
			while(!ancestor[w]) w = par[w];
			assert(h.lca(u, v) == w);
			FOR(edge, 0, 1){
				vector<int> want(n + 1), got(n + 1);
				ll expected = 0, actual = 0;
				for(int start: {u, v})
					for(int x = start; x != w; x = par[x]){
						want[h.pos[x]] = 1;
						expected += weight[x];
					}
				if(!edge){
					want[h.pos[w]] = 1;
					expected += weight[w];
				}
				auto ranges = h.path(u, v, edge);
				assert(sz(ranges) <= 2 * log2(n) + 2);
				for(auto [a, b]: ranges){
					assert(1 <= a && a <= b && b <= n);
					FOR(p, a, b){
						assert(++got[p] == 1);
						actual += base[p];
					}
				}
				assert(got == want);
				MintN viaSeg = 0;
				for(auto [l, r]: ranges)
					viaSeg += seg.get(l, r).sum;
				assert(viaSeg.v == MintN(expected).v);
				assert(actual == expected);
			}
		}
	}
	FOR(u, 1, n){
		int heavy = h.heavy[u], largest = 0;
		for(int v: g[u]) if(par[v] == u)
			largest = max(largest, sub[v]);
		if(!largest) assert(heavy == 0);
		else{
			assert(1 <= heavy && heavy <= n);
			assert(par[heavy] == u && sub[heavy] == largest);
			assert(h.pos[heavy] == h.pos[u] + 1);
		}
		int head = u;
		if(par[u] && h.heavy[par[u]] == u)
			head = h.head[par[u]];
		assert(h.head[u] == head);
	}
}

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
		FOR(u, 1, n) shuffle(all(g[u]), rng);
		g[0] = {0, n + 1};
		vector<ll> weight(n + 1);
		FOR(u, 1, n)
			weight[u] = (ll)(rng() % 2000000001) - 1000000000;
		int root = rng() % n + 1;
		check(g, root, weight);
	}
	return 0;
}
