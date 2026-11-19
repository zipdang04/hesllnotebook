#include "../../tex/contents/flow/mcmf.hpp"
#include <cassert>

struct Arc{
	int u, v;
	ll cap, cost;
};

pll brute(int n, int s, int t, const vector<Arc> &in,
		ll limit = oo){
	vector<Arc> arcs;
	for(auto e: in){
		arcs.push_back(e);
		arcs.push_back({e.v, e.u, 0, -e.cost});
	}
	ll flow = 0, cost = 0;
	while(flow < limit){
		vector<ll> dist(n, oo);
		vector<int> par(n, -1);
		dist[s] = 0;
		FOR(pass, 1, n - 1){
			FOR(id, 0, sz(arcs) - 1){
				auto e = arcs[id];
				if(!e.cap || dist[e.u] == oo)
					continue;
				ll d = dist[e.u] + e.cost;
				if(d >= dist[e.v])
					continue;
				dist[e.v] = d;
				par[e.v] = id;
			}
		}
		if(dist[t] == oo)
			break;
		for(int v = t; v != s; v = arcs[par[v]].u){
			int id = par[v];
			arcs[id].cap--;
			arcs[id ^ 1].cap++;
		}
		flow++;
		cost += dist[t];
	}
	return {flow, cost};
}

int main(){
	{
		// Sum |cost| <= 1e18; a partial cost sum leaves ll.
		MCMF a(2), b(4);
		a.addEdge(0, 1, 20000000000LL, -500000000);
		a.addEdge(0, 1, 20000000000LL, 500000000);
		assert(a.maxFlow(0, 1) == pll(40000000000LL, 0));
		b.addEdge(0, 1, 1, 1);
		ll h = 490000000000000000LL;
		b.addEdge(2, 3, 1, -h);
		b.addEdge(3, 1, 1, -h);
		b.addEdge(0, 2, 1, 0);
		assert(b.maxFlow(0, 1) == pll(2, 1 - 2 * h));
	}
	mt19937 rng(712367);
	auto draw = [&](int l, int r){
		return uniform_int_distribution<int>(l, r)(rng);
	};
	FOR(tc, 1, 3000){
		int n = draw(2, 7), m = draw(0, 15);
		int s = draw(0, n - 1);
		int t = (s + draw(1, n - 1)) % n;
		vector<ll> h(n);
		for(ll &x: h)
			x = draw(-5, 5);
		MCMF mf(n);
		vector<Arc> in;
		FOR(i, 0, m - 1){
			int u = draw(0, n - 1), v = draw(0, n - 1);
			ll cap = draw(0, 3);
			ll cost = u == v ? draw(0, 3)
				: h[v] - h[u] + draw(0, 4);
			int id = mf.addEdge(u, v, cap, cost);
			assert(id == 2 * i);
			in.push_back({u, v, cap, cost});
		}
		MCMF copy = mf;
		pll want = brute(n, s, t, in);
		pll got = mf.maxFlow(s, t);
		assert(got == want);
		vector<ll> balance(n);
		ll cost = 0;
		FOR(i, 0, m - 1){
			auto e = in[i];
			ll f = mf.flowOf(2 * i);
			assert(0 <= f && f <= e.cap);
			balance[e.u] -= f;
			balance[e.v] += f;
			cost += f * e.cost;
		}
		assert(cost == got.se);
		FOR(v, 0, n - 1){
			ll b = v == s ? -got.fi : 0;
			if(v == t)
				b = got.fi;
			assert(balance[v] == b);
		}
		if(want.fi > 0){
			ll limit = want.fi - 1;
			pll limited = copy.maxFlow(s, t, limit);
			assert(limited.fi == limit);
			assert(limited == brute(n, s, t, in, limit));
		}
		pll again = mf.maxFlow(s, t);
		assert(again == pll(0, 0));
	}
}
