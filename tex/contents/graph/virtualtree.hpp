#pragma once
#include "hld.hpp"

// Virtual tree of vs plus pairwise LCAs, vertices 1..n,
// O(k log k + k log n) for k = |vs|. The edge p -> v
// stands for depth[v] - depth[p] tree edges.
vector<pair<int, int>> virtualTree(HLD &hld,
		vector<int> vs){
	auto cmp = [&](int a, int b){
		return hld.pos[a] < hld.pos[b];
	};
	sort(all(vs), cmp);
	int m = sz(vs) - 1;
	FOR(i, 0, m - 1)
		vs.push_back(hld.lca(vs[i], vs[i+1]));
	sort(all(vs), cmp);
	vs.erase(unique(all(vs)), vs.end());
	vector<pii> res;
	FOR(i, 0, sz(vs) - 1)
		res.push_back({vs[i], i ? hld.lca(vs[i-1], vs[i]) : 0});
	return res;
}
