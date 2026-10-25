#include "../../tex/contents/graph/centroid.hpp"

// checks par, lev, rem during the callback, call order, forests; level bound on big paths/stars
int n; vector<pair<int,int>> E;
bool check(Centroid &cd, vector<int> &order, vector<vector<bool>> &remAt){
	int lg = 0; while ((2 << lg) <= n) lg++; // floor(log2 n)
	vector<int> when(n, -1);
	EFOR(int, i, 0, (int)order.size()){ if (when[order[i]] >= 0) return 0; when[order[i]] = i; }
	if ((int)order.size() != n) return 0;
	// S(c) = vertices whose centroid-ancestor chain contains c
	vector<vector<int>> S(n);
	EFOR(int, u, 0, n){
		int steps = 0;
		for (int x = u; x >= 0; x = cd.par[x]){ S[x].push_back(u); if (++steps > n) return 0; }
	}
	EFOR(int, c, 0, n){
		int p = cd.par[c];
		if (cd.lev[c] != (p < 0 ? 0 : cd.lev[p] + 1) || cd.lev[c] > lg) return 0;
		if (p >= 0 && when[p] > when[c]) return 0;
		vector<int> in(n, 0); for (int u: S[c]) in[u] = 1;
		vector<int> inP(n, 0);
		if (p >= 0){ for (int u: S[p]) inP[u] = 1; inP[p] = 0; }
		// rem during f(c): c and ancestors removed, rest of S(c) not
		for (int x = c; x >= 0; x = cd.par[x]) if (!remAt[c][x]) return 0;
		for (int u: S[c]) if (u != c && remAt[c][u]) return 0;
		// S(c) connected; components of S(c) \ {c} have size <= |S(c)| / 2;
		// S(c) is a full component of S(p) \ {p} (or of the forest if p < 0)
		for (auto [a, b]: E){
			if (in[a] != in[b]){
				int o = in[a] ? b : a;
				if (p < 0 || inP[o]) return 0;
			}
		}
		vector<int> lab(n, -1); int k = 0;
		auto comp = [&](int s, int ban){
			vector<int> st{s}; lab[s] = k; int cnt = 0;
			while (!st.empty()){
				int u = st.back(); st.pop_back(); cnt++;
				for (auto [a, b]: E) for (int t = 0; t < 2; t++){
					int x = t ? b : a, y = t ? a : b;
					if (x == u && in[y] && y != ban && lab[y] < 0) lab[y] = k, st.push_back(y);
				}
			}
			k++; return cnt;
		};
		if (comp(c, -1) != (int)S[c].size()) return 0;
		fill(lab.begin(), lab.end(), -1); lab[c] = -2;
		for (int u: S[c]) if (lab[u] == -1 && comp(u, c) * 2 > (int)S[c].size()) return 0;
	}
	return 1;
}
int main(){
	mt19937 rng(1);
	EFOR(int, it, 0, 3000){
		n = rng() % 30 + 1; E.clear();
		Centroid cd(n);
		vector<int> perm(n); iota(perm.begin(), perm.end(), 0); shuffle(perm.begin(), perm.end(), rng);
		int shape = rng() % 3;
		EFOR(int, i, 1, n){
			if (rng() % 6 == 0) continue; // forest
			int j = shape == 0 ? rng() % i : shape == 1 ? i - 1 : max(0, i - 1 - (int)(rng() % 3));
			E.push_back({perm[i], perm[j]}); cd.addEdge(perm[i], perm[j]);
		}
		vector<int> order; vector<vector<bool>> remAt(n);
		cd.run([&](int c){ order.push_back(c); remAt[c] = cd.rem; });
		if (!check(cd, order, remAt)){
			printf("FAIL n=%d\n", n); for (auto [a, b]: E) printf("%d %d\n", a, b);
			return 1;
		}
	}
	// big path and star: level bound floor(log2 n), each vertex visited once
	for (int shape: {0, 1}){
		n = 200000; Centroid cd(n);
		EFOR(int, i, 1, n) cd.addEdge(shape ? 0 : i - 1, i);
		int calls = 0, mx = 0;
		cd.run([&](int c){ calls++; mx = max(mx, cd.lev[c]); });
		if (calls != n || mx > 17){ printf("FAIL big shape=%d calls=%d maxlev=%d\n", shape, calls, mx); return 1; }
	}
	puts("OK");
}
