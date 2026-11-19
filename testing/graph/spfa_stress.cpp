#include "../../tex/contents/graph/spfa.hpp"
#include <cassert>

vector<ll> brute(const vector<vector<pair<int, ll>>> &g,
		int s){
	int n = sz(g) - 1;
	vector<ll> dist(n + 1, oo);
	vector<char> bad(n + 1);
	dist[s] = 0;
	FOR(i, 1, n - 1) FOR(u, 1, n){
		if(dist[u] == oo) continue;
		for(auto [v, w]: g[u])
			dist[v] = min(dist[v], dist[u] + w);
	}
	FOR(i, 1, n) FOR(u, 1, n){
		if(dist[u] == oo) continue;
		for(auto [v, w]: g[u]){
			if(dist[v] > dist[u] + w){
				dist[v] = dist[u] + w;
				bad[v] = true;
			}
			if(bad[u]) bad[v] = true;
		}
	}
	FOR(v, 1, n) if(bad[v]) dist[v] = -oo;
	return dist;
}

void check(const vector<vector<pair<int, ll>>> &g, int s){
	int n = sz(g) - 1;
	auto dist = brute(g, s);
	SPFA sp(g, s);
	assert(sp.dist == dist);
	assert(sz(sp.par) == n + 1);
	bool negCycle = find(all(dist), -oo) != dist.end();
	assert(sp.negCycle == negCycle);
	if(dist[s] == 0) assert(sp.par[s] == 0);
	FOR(v, 1, n){
		if(dist[v] == oo) assert(sp.par[v] == 0);
		if(abs(dist[v]) == oo || v == s) continue;
		int u = sp.par[v];
		assert(1 <= u && u <= n);
		assert(abs(dist[u]) < oo);
		bool found = false;
		for(auto [to, w]: g[u])
			if(to == v && dist[u] + w == dist[v])
				found = true;
		assert(found);
	}
}

struct BruteMean{
	ll num = oo;
	int den = 1;
	BruteMean(const vector<vector<pair<int, ll>>> &g){
		FOR(s, 1, sz(g) - 1) dfs(s, s, 0, 0, 0, g);
	}
	void dfs(int s, int u, int mask, ll sum, int len,
			const vector<vector<pair<int, ll>>> &g){
		mask |= 1 << u;
		for(auto [v, w]: g[u]){
			if(v == s){
				ll total = sum + w;
				int count = len + 1;
				if(num == oo || total * den < num * count){
					num = total;
					den = count;
				}
			}else if(!(mask & (1 << v)))
				dfs(s, v, mask, sum + w, len + 1, g);
		}
	}
};

void checkMean(const vector<vector<pair<int, ll>>> &g,
		int K){
	BruteMean best(g);
	if(best.num == oo) return;
	int n = sz(g) - 1;
	ll W = 0;
	FOR(u, 1, n) for(auto [v, w]: g[u])
		W = max(W, abs(w));
	ll lo = -K * W, hi = K * W;
	while(lo < hi){
		ll mid = lo + (hi - lo + 1) / 2;
		auto h = g;
		h.resize(n + 2);
		FOR(u, 1, n){
			for(auto &e: h[u]) e.se = K * e.se - mid;
			h[n+1].push_back({u, 0});
		}
		SPFA sp(h, n + 1);
		if(sp.negCycle) hi = mid - 1;
		else lo = mid;
	}
	assert(best.num * K >= lo * best.den);
	assert(best.num * K < (lo + 1) * best.den);
}

int main(){
	// repeated self-loops must not overflow
	vector<vector<pair<int, ll>>> loops(2);
	FOR(i, 1, 10)
		loops[1].push_back({1, -999999999999999999LL});
	SPFA sl(loops, 1);
	assert(sl.negCycle && sl.dist[1] == -oo);
	mt19937 rng(12345);
	FOR(trial, 1, 20000){
		int n = rng() % 8 + 1, m = rng() % 21;
		int s = rng() % n + 1;
		vector<vector<pair<int, ll>>> g(n + 1);
		g[0].push_back({n + 1, -100});
		FOR(i, 1, m){
			int u = rng() % n + 1, v = rng() % n + 1;
			ll w = (int)(rng() % 16) - 5;
			g[u].push_back({v, w});
		}
		FOR(u, 1, n) shuffle(all(g[u]), rng);
		check(g, s);
		if(n <= 6)
			checkMean(g, 1 + (int)(rng() % (2 * n)));
	}
	return 0;
}
