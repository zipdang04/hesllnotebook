#include "../../tex/contents/graph/dmst.hpp"
int main(){
	mt19937 rng(1);
	EFOR(int, it, 0, 200000){
		int n = rng() % 6 + 1, m = rng() % 10; ll W = it % 3 == 0 ? 1000000000 : it % 3 == 1 ? 5 : 20;
		bool neg = it % 2;
		vector<DMST::Edge> es; DMST d(n);
		EFOR(int, i, 0, m){
			int a = rng() % n, b = rng() % n; ll w = neg ? (ll)(rng() % (2 * W + 1)) - W : (ll)(rng() % (W + 1));
			es.push_back({a, b, w}); d.addEdge(a, b, w);
		}
		EFOR(int, rep, 0, 2){ // second call: other root, same object
			int r = rng() % n;
			vector<vector<int>> inc(n); EFOR(int, i, 0, m) if (es[i].b != r && es[i].a != es[i].b) inc[es[i].b].push_back(i);
			ll best = LLONG_MAX; vector<int> ch(n, 0); bool ok = true;
			EFOR(int, v, 0, n) if (v != r && inc[v].empty()) ok = false;
			if (ok) while (true){
				vector<int> p(n, -1); ll c = 0;
				EFOR(int, v, 0, n) if (v != r){ p[v] = es[inc[v][ch[v]]].a; c += es[inc[v][ch[v]]].w; }
				bool tree = true;
				EFOR(int, v, 0, n){ int x = v, k = 0; while (x != r && k <= n) x = p[x], k++; if (x != r) tree = false; }
				if (tree) best = min(best, c);
				int v = 0;
				while (v < n){ if (v == r){ v++; continue; } if (++ch[v] < (int)inc[v].size()) break; ch[v] = 0; v++; }
				if (v == n) break;
			}
			auto [c, par] = d.solve(r);
			bool bad = false;
			if (best == LLONG_MAX) bad = !par.empty();
			else if (par.empty() || c != best || par[r] != -1) bad = true;
			else{
				ll s = 0;
				EFOR(int, v, 0, n) if (v != r){
					ll mw = LLONG_MAX; for (auto &e: es) if (e.a == par[v] && e.b == v) mw = min(mw, e.w);
					if (mw == LLONG_MAX){ bad = true; break; }
					s += mw;
				}
				EFOR(int, v, 0, n){ int x = v, k = 0; while (x != r && k <= n && x >= 0) x = par[x], k++; if (x != r) bad = true; }
				if (s != best) bad = true;
			}
			if (bad){
				printf("FAIL it=%d rep=%d n=%d r=%d best=%lld got=%lld\n", it, (int)rep, n, r, best, c);
				for (auto &e: es) printf("%d %d %lld\n", e.a, e.b, e.w);
				return 1;
			}
		}
	}
	puts("OK");
}
