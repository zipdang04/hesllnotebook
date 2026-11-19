#pragma once
#include "../contest/template.hpp"

// MST of points p[1..n] (p[0] unused) under |dx| + |dy|.
// O(n log n), no recursion.
vector<array<ll, 3>> manhattanMST(vector<pair<ll, ll>> p){
	int n = sz(p) - 1; vector<int> id(n); iota(all(id), 1);
	vector<array<ll, 3>> cand, res;
	FOR(k, 0, 3){
		sort(all(id), [&](int i, int j){
			return p[i].fi - p[j].fi < p[j].se - p[i].se;
		});
		map<ll, int> sweep;
		for(int i: id){
			for(auto it = sweep.lower_bound(-p[i].se);
					it != sweep.end(); sweep.erase(it++)){
				int j = it->se;
				ll dx = p[i].fi - p[j].fi, dy = p[i].se - p[j].se;
				if(dy > dx) break;
				cand.push_back({dx + dy, i, j});
			}
			sweep[-p[i].se] = i;
		}
		FOR(i, 1, n)
			if(k & 1) p[i].fi = -p[i].fi;
			else swap(p[i].fi, p[i].se);
	}
	sort(all(cand));
	vector<int> dsu(n + 1); iota(all(dsu), 0);
	auto find = [&](int x){
		while(dsu[x] != x) x = dsu[x] = dsu[dsu[x]];
		return x;
	};
	for(auto [w, u, v]: cand){
		int a = find(u), b = find(v);
		if(a == b) continue;
		dsu[a] = b; res.push_back({w, u, v});
	}
	return res;
}
