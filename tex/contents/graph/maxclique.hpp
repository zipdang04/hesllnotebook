#pragma once
#include "../contest/template.hpp"

// branch and bound; greedy coloring of the candidates bounds the clique size. N >= n
template<int N> struct MaxClique{
	using B = bitset<N>;
	int n; vector<B> g; vector<int> cur, best;
	MaxClique(int n): n(n), g(n){}
	void addEdge(int u, int v){ if (u != v) g[u][v] = g[v][u] = 1; }
	void expand(B P){
		if (P.none()){ if (cur.size() > best.size()) best = cur; return; }
		vector<int> ord, col; B U = P;
		for (int k = 1; U.any(); k++) for (B Q = U; Q.any(); ){ // color class k: greedy independent set
			int v = Q._Find_first(); Q &= ~g[v]; Q[v] = U[v] = 0;
			ord.push_back(v); col.push_back(k);
		}
		REV(int, i, (int)ord.size() - 1, 0){
			if (cur.size() + col[i] <= best.size()) return;
			int v = ord[i]; cur.push_back(v); expand(P & g[v]); cur.pop_back(); P[v] = 0;
		}
	}
	vector<int> solve(){ // vertices of a maximum clique
		vector<int> id(n); iota(id.begin(), id.end(), 0); auto h = g;
		sort(id.begin(), id.end(), [&](int a, int b){ return g[a].count() > g[b].count(); });
		FOR(int, i, 0, n - 1) FOR(int, j, 0, n - 1) g[i][j] = h[id[i]][id[j]];
		cur.clear(); best.clear(); B P; FOR(int, i, 0, n - 1) P[i] = 1;
		expand(P); g = h; for (int &v: best) v = id[v];
		return best;
	}
};
