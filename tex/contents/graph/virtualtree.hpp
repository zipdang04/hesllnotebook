#pragma once
#include "../contest/template.hpp"
#include "hld.hpp"
// build(S): vs = S + pairwise LCAs sorted by pos (vs[0] = root), pa[i] = index of parent of vs[i]
// (pa[0] = -1), id[v] = index of v in vs; edge length = dep[vs[i]] - dep[vs[pa[i]]]
struct VirtualTree{
	HLD &h; vector<int> vs, pa, id;
	VirtualTree(HLD &h): h(h), id(h.n){}
	void build(vector<int> S){
		auto cmp = [&](int a, int b){ return h.pos[a] < h.pos[b]; };
		sort(S.begin(), S.end(), cmp); vs = S;
		EFOR(int, i, 1, (int)S.size()) vs.push_back(h.lca(S[i - 1], S[i]));
		sort(vs.begin(), vs.end(), cmp); vs.erase(unique(vs.begin(), vs.end()), vs.end());
		pa.assign(vs.size(), -1);
		EFOR(int, i, 0, (int)vs.size()) id[vs[i]] = i;
		EFOR(int, i, 1, (int)vs.size()) pa[i] = id[h.lca(vs[i - 1], vs[i])];
	}
};
