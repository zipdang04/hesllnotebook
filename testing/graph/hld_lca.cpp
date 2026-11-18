#include "../../tex/contents/graph/hld.hpp"
#include <cassert>

int main(){
	HLD h(vector<vector<int>>(2));
	assert(h.lca(1, 1) == 1);
	assert((h.path(1, 1) == vector<pii>{{1, 1}}));
	assert(h.path(1, 1, true).empty());
	assert(h.pos[1] == 1 && h.sub[1] == 1);
	assert(h.par[1] == 0 && h.depth[1] == 0);
	assert(h.heavy[1] == 0 && h.head[1] == 1);
	assert(h.timer == 1 && h.pos[0] == 0);
	vector<vector<int>> chain = {{1}, {2}, {1, 3}, {2}};
	h = HLD(chain, 3);
	assert(h.par[3] == 0 && h.par[1] == 2);
	assert(h.lca(1, 2) == 2 && h.depth[1] == 2);
	assert(h.heavy[3] == 2 && h.heavy[2] == 1);
	assert(h.head[1] == 3 && h.sub[3] == 3);
	assert(h.pos[3] == 1 && h.pos[1] == 3);
	assert(h.depth[3] == 0 && h.pos[0] == 0);
	vector<vector<int>> star = {{1}, {2, 3}, {1}, {1}};
	h = HLD(star);
	vector<int> covered;
	for(auto [l, r]: h.path(2, 3, true))
		FOR(i, l, r)
			covered.push_back(i);
	sort(all(covered));
	vector<int> leaves = {h.pos[2], h.pos[3]};
	sort(all(leaves));
	assert(covered == leaves);
	covered.clear();
	for(auto [l, r]: h.path(2, 1, true))
		FOR(i, l, r)
			covered.push_back(i);
	assert(covered == vector<int>{h.pos[2]});
	star[0].clear(); HLD clean(star);
	assert(h.pos == clean.pos && h.par == clean.par);
	assert(h.depth == clean.depth && h.sub == clean.sub);
	assert(h.heavy == clean.heavy && h.head == clean.head);
	vector<vector<int>> forest = {{1}, {2}, {1}, {4}, {3}};
	h = HLD(forest, 3);
	assert(h.timer == 2 && h.pos[1] == 0);
	assert(h.pos[2] == 0 && h.sub[3] == 2);
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, q; cin >> n >> q;
	vector<vector<int>> g(n + 1);
	vector<int> par(n + 1);
	FOR(u, 2, n){
		cin >> par[u]; par[u]++;
		g[u].push_back(par[u]);
		g[par[u]].push_back(u);
	}
	HLD hld(g, 1);
	FOR(u, 2, n){
		assert(hld.par[u] == par[u]);
		assert(hld.depth[u] == hld.depth[par[u]] + 1);
	}
	while(q--){
		int u, v; cin >> u >> v;
		cout << hld.lca(u + 1, v + 1) - 1 << '\n';
	}
}
