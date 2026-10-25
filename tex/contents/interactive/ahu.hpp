#pragma once
#include "../contest/template.hpp"

// canonical ids of rooted trees; ids are shared by every call on the same object, so equal id <=> isomorphic
struct AHU{
	map<vector<int>, int> mp;
	vector<int> id; // id[v] = class of the subtree of v after the last call

	int rooted(const vector<vector<int>> &g, int r){ // returns id[r]
		int n = g.size(); vector<int> par(n, -1), ord{r};
		id.assign(n, 0);
		for (int i = 0; i < (int)ord.size(); i++) for (int w: g[ord[i]]) if (w != par[ord[i]]) par[w] = ord[i], ord.push_back(w);
		vector<vector<int>> ch(n);
		for (int i = n - 1; i >= 0; i--){
			int v = ord[i]; sort(ch[v].begin(), ch[v].end());
			id[v] = mp.emplace(ch[v], mp.size()).first->second;
			if (i) ch[par[v]].push_back(id[v]);
		}
		return id[r];
	}
	int unrooted(const vector<vector<int>> &g){ // min over the center(s)
		int n = g.size(); vector<int> d(n), q, c;
		EFOR(int, v, 0, n) if ((d[v] = g[v].size()) <= 1) q.push_back(v);
		for (int i = 0, left = n; left > 0;){
			c.assign(q.begin() + i, q.end()); left -= q.size() - i;
			for (int e = q.size(); i < e; i++) for (int w: g[q[i]]) if (--d[w] == 1) q.push_back(w);
		}
		int best = INT_MAX;
		for (int r: c) best = min(best, rooted(g, r));
		return best;
	}
};
