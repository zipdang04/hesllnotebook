#include "../../tex/contents/contest/template.hpp"
#include <cassert>

// Notebook recipe "Print a negative cycle", followed literally.
// start = 0: dist all 0 (cycle anywhere); else dist from start.
vector<int> recipe(int n, const vector<array<ll, 3>> &e, int start){
	vector<ll> d(n + 1, start ? oo : 0); vector<int> par(n + 1);
	if(start) d[start] = 0;
	int x = 0;
	FOR(pass, 1, n){
		x = 0;
		for(auto [u, v, w]: e) if(d[u] < oo && d[u] + w < d[v])
			d[v] = d[u] + w, par[v] = u, x = v;
	}
	if(!x) return {};
	FOR(i, 1, n) x = par[x];
	vector<int> cyc{x};
	for(int y = par[x]; y != x; y = par[y]) cyc.push_back(y);
	reverse(all(cyc));
	return cyc;
}

int main(){
	mt19937 rng(11);
	FOR(it, 1, 20000){
		int n = rng() % 7 + 1, m = rng() % 15;
		vector<array<ll, 3>> e(m);
		for(auto &[u, v, w]: e) u = rng() % n + 1, v = rng() % n + 1, w = (ll)(rng() % 21) - 6;
		// brute: Floyd on cycle weights; reach[s][u]
		vector<vector<ll>> D(n + 1, vector<ll>(n + 1, oo));
		vector<vector<char>> R(n + 1, vector<char>(n + 1));
		FOR(u, 1, n) R[u][u] = 1;
		for(auto [u, v, w]: e) D[u][v] = min(D[u][v], w), R[u][v] = 1;
		FOR(k, 1, n) FOR(i, 1, n) FOR(j, 1, n){
			if(R[i][k] && R[k][j]) R[i][j] = 1;
			if(D[i][k] < oo && D[k][j] < oo) D[i][j] = min(D[i][j], max(-oo / 2, D[i][k] + D[k][j]));
		}
		FOR(start, 0, n){
			bool want = false;
			FOR(u, 1, n) if(D[u][u] < 0 && (!start || R[start][u])) want = true;
			auto c = recipe(n, e, start);
			assert(!c.empty() == want);
			if(c.empty()) continue;
			// edges c[i] -> c[i+1] exist; pick lightest each; total < 0; vertices distinct
			ll tot = 0; set<int> seen(all(c)); assert(sz(seen) == sz(c));
			FOR(i, 0, sz(c) - 1){
				int u = c[i], v = c[(i + 1) % sz(c)]; ll b = oo;
				for(auto [a, bb, w]: e) if(a == u && bb == v) b = min(b, w);
				assert(b < oo); tot += b;
			}
			assert(tot < 0);
		}
	}
	puts("OK");
}
