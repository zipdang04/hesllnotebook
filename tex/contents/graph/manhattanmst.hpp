#pragma once
#include "../contest/template.hpp"

struct ManhattanMST{
	ll cost = 0; vector<array<ll,3>> edges;  // MST edges {w, u, v}
	ManhattanMST(vector<ll> x, vector<ll> y){
		int n = x.size(); vector<int> id(n), p(n);
		vector<array<ll,3>> cand;  // <= 4n edges, contain an MST
		iota(id.begin(), id.end(), 0); iota(p.begin(), p.end(), 0);
		FOR(int, k, 0, 3){
			sort(id.begin(), id.end(), [&](int i, int j){ return x[i] + y[i] < x[j] + y[j]; });
			map<ll,int> sw;
			for (int i: id){
				for (auto it = sw.lower_bound(-y[i]); it != sw.end(); it = sw.erase(it)){
					int j = it->se; ll dx = x[i] - x[j], dy = y[i] - y[j];
					if (dy > dx) break;
					cand.push_back({dx + dy, i, j});
				}
				sw[-y[i]] = i;
			}
			FOR(int, i, 0, n - 1) if (k & 1) x[i] = -x[i]; else swap(x[i], y[i]);
		}
		sort(cand.begin(), cand.end());
		auto f = [&](int v){ while (p[v] != v) v = p[v] = p[p[v]]; return v; };
		for (auto [w, u, v]: cand) if (f(u) != f(v)) p[f(u)] = f(v), cost += w, edges.push_back({w, u, v});
	}
};
