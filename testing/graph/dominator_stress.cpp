#include "../../tex/contents/graph/dominator.hpp"
int N; vector<vector<int>> A;
vector<char> reach(int s, int ban){ // reachable from s without passing ban
	vector<char> r(N, 0); if (s == ban) return r;
	vector<int> st{s}; r[s] = 1;
	while (!st.empty()){ int v = st.back(); st.pop_back(); for (int u: A[v]) if (u != ban && !r[u]) r[u] = 1, st.push_back(u); }
	return r;
}
int main(){
	mt19937 rng(7);
	EFOR(int, it, 0, 20000){
		N = rng() % 9 + 1; int M = rng() % (N * 3 + 1), s = rng() % N; A.assign(N, {});
		Dominator d(N); vector<pair<int, int>> E;
		EFOR(int, i, 0, M){
			int a = rng() % N, b = rng() % N;
			if (rng() % 4 == 0 && !E.empty()) tie(a, b) = E[rng() % E.size()];
			if (rng() % 8 == 0) b = a;
			E.push_back({a, b}); A[a].push_back(b); d.addEdge(a, b);
		}
		auto got = d.run(s); auto r0 = reach(s, -1); vector<int> exp(N, -1);
		vector<vector<char>> dom(N, vector<char>(N, 0)); // dom[d][v]: d dominates v
		EFOR(int, x, 0, N) if (r0[x]){ auto r = reach(s, x); EFOR(int, v, 0, N) dom[x][v] = r0[v] && (v == x || !r[v]); }
		EFOR(int, v, 0, N) if (r0[v]){
			if (v == s){ exp[v] = s; continue; }
			EFOR(int, c, 0, N) if (c != v && dom[c][v]){ // idom: strict dominator dominated by all others
				bool ok = true; EFOR(int, o, 0, N) if (o != v && dom[o][v] && !dom[o][c]) ok = false;
				if (ok) exp[v] = c;
			}
		}
		if (got != exp){
			cerr << "MISMATCH n=" << N << " s=" << s << "\n";
			for (auto [a, b]: E) cerr << a << "->" << b << "\n";
			EFOR(int, v, 0, N) cerr << v << ": got " << got[v] << " exp " << exp[v] << "\n";
			return 1;
		}
	}
	{ // deep: 2e5 cycle, then path + back edges to the start
		int n = 200000; Dominator d(n);
		EFOR(int, i, 0, n) d.addEdge(i, (i + 1) % n);
		auto p = d.run(0);
		EFOR(int, i, 0, n) if (p[i] != max(i - 1, 0)){ cerr << "deep cycle bad at " << i << "\n"; return 1; }
		Dominator e(n);
		EFOR(int, i, 0, n - 1) e.addEdge(i, i + 1);
		EFOR(int, i, 0, n) if (i % 7 == 0) e.addEdge(i, 0);
		auto q = e.run(0);
		EFOR(int, i, 0, n) if (q[i] != max(i - 1, 0)){ cerr << "deep path bad at " << i << "\n"; return 1; }
	}
	cout << "OK\n";
}
