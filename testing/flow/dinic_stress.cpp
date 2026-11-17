#include "../../tex/contents/flow/dinic.hpp"
#include <cassert>

ll brute(vector<vector<ll>> cap, int s, int t){
	int n = sz(cap);
	ll flow = 0;
	while(true){
		vector<int> par(n, -1), q = {s};
		par[s] = s;
		for(int i = 0; i < sz(q); i++){
			int u = q[i];
			FOR(v, 0, n - 1) if(par[v] < 0 && cap[u][v]){
				par[v] = u;
				q.push_back(v);
			}
		}
		if(par[t] < 0)
			return flow;
		ll pushed = LLONG_MAX;
		for(int v = t; v != s; v = par[v])
			pushed = min(pushed, cap[par[v]][v]);
		for(int v = t; v != s; v = par[v]){
			cap[par[v]][v] -= pushed;
			cap[v][par[v]] += pushed;
		}
		flow += pushed;
	}
}

struct Arc{ int u, v, id; ll cap; };

int main(){
	mt19937_64 rng(712367);
	FOR(it, 0, 1799){
		int n = 2 + (int)(rng() % 7);
		int m = (int)(rng() % 21);
		int s = (int)(rng() % n);
		int t = (s + 1 + (int)(rng() % (n - 1))) % n;
		int regime = it / 3 % 3;
		bool undirected = it % 3 == 0;
		if(regime == 2)
			m = max(m, 1);
		int special = m ? (int)(rng() % m) : -1;
		Dinic d(n);
		vector<Arc> arcs;
		vector<vector<ll>> cap(n, vector<ll>(n));
		FOR(i, 0, m - 1){
			int u = (int)(rng() % n);
			int v = (int)(rng() % n);
			ll c = rng() % 4;
			if(regime == 1)
				c = rng() % 100000000000000001LL;
			if(regime == 2 && i == special)
				c = 1LL << 60;
			ll rcap = undirected ? c : 0;
			int id = d.addEdge(u, v, c, rcap);
			assert(id == 2 * i);
			arcs.push_back({u, v, id, c});
			cap[u][v] += c;
			cap[v][u] += rcap;
		}
		Dinic cur = d;
		ll flow = d.maxFlow(s, t);
		assert(flow == brute(cap, s, t));
		vector<int> side(n);
		FOR(v, 0, n - 1)
			side[v] = d.sideS(v);
		assert(side[s] && !side[t]);
		ll cut = 0;
		vector<ll> balance(n);
		for(auto e: arcs){
			if(side[e.u] && !side[e.v])
				cut += e.cap;
			if(undirected && side[e.v] && !side[e.u])
				cut += e.cap;
			ll f = undirected
				? (d.edges[e.id^1].cap - d.edges[e.id].cap) / 2
				: d.flowOf(e.id);
			assert(f >= (undirected ? -e.cap : 0));
			assert(f <= e.cap);
			balance[e.u] += f;
			balance[e.v] -= f;
		}
		assert(cut == flow);
		assert(balance[s] == flow);
		assert(balance[t] == -flow);
		FOR(v, 0, n - 1) if(v != s && v != t)
			assert(balance[v] == 0);
		assert(cur.maxFlow(s, t) == flow);
	}
	return 0;
}
