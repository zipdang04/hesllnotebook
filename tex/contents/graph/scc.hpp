#pragma once
#include "../contest/template.hpp"

struct SCC{
	int n, k; vector<vector<int>> g; vector<int> comp;
	SCC(int n): n(n), g(n) {}
	void addEdge(int u, int v){ g[u].push_back(v); }
	int run(){ // iterative Tarjan; comp[v] in [0, k), topological order: edge u->v => comp[u] <= comp[v]
		vector<int> num(n), low(n), it(n), st, cs; comp.assign(n, -1); int t = k = 0;
		auto visit = [&](int v){ num[v] = low[v] = ++t; st.push_back(v); cs.push_back(v); };
		FOR(int, s, 0, n - 1) if (!num[s]) for (visit(s); !cs.empty();){
			int u = cs.back();
			if (it[u] < (int)g[u].size()){
				int v = g[u][it[u]++];
				if (!num[v]) visit(v);
				else if (comp[v] < 0) low[u] = min(low[u], num[v]);
				continue;
			}
			cs.pop_back();
			if (!cs.empty()) low[cs.back()] = min(low[cs.back()], low[u]);
			if (low[u] == num[u]){
				int x; do x = st.back(), st.pop_back(), comp[x] = k; while (x != u);
				k++;
			}
		}
		for (int &c: comp) c = k - 1 - c; // Tarjan finds sinks first
		return k;
	}
};
// vars 0..n-1; literal x or ~x (negation)
struct TwoSat: SCC{
	vector<bool> val;
	TwoSat(int n): SCC(2 * n), val(n) {}
	int lit(int x){ return x < 0 ? 2 * ~x + 1 : 2 * x; }
	void either(int a, int b){ a = lit(a), b = lit(b); addEdge(a ^ 1, b); addEdge(b ^ 1, a); }
	bool solve(){
		run();
		FOR(int, i, 0, n / 2 - 1){
			if (comp[2 * i] == comp[2 * i + 1]) return 0;
			val[i] = comp[2 * i] > comp[2 * i + 1];
		}
		return 1;
	}
};
