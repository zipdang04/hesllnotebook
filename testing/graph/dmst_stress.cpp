#include "../../tex/contents/graph/dmst.hpp"
#include <cassert>

bool reachesRoot(const vector<int> &par, int r){
	int n = sz(par) - 1;
	FOR(v, 1, n){
		int u = v;
		FOR(step, 1, n) if(u != r) u = par[u];
		if(u != r) return false;
	}
	return true;
}

struct Brute{
	int n, root;
	ll cost = 0; bool found = false;
	vector<int> par;
	vector<vector<pair<int, ll>>> in;
	Brute(const vector<vector<pair<int, ll>>> &g, int r)
		: n(sz(g) - 1), root(r), par(n + 1), in(n + 1){
		FOR(u, 1, n)
			for(auto [v, w]: g[u])
				in[v].push_back({u, w});
		dfs(1, 0);
	}
	void dfs(int v, ll sum){
		if(v > n){
			if(reachesRoot(par, root)){
				cost = found ? min(cost, sum) : sum;
				found = true;
			}
			return;
		}
		if(v == root){
			dfs(v + 1, sum);
			return;
		}
		for(auto [u, w]: in[v]){
			par[v] = u;
			dfs(v + 1, sum + w);
		}
	}
};

int main(){
	mt19937 rng(712367);
	DirectedMST dm(vector<vector<pair<int, ll>>>(2), 1);
	// costs above oo are legal
	vector<vector<pair<int, ll>>> big(4);
	big[1].push_back({2, 600000000000000000LL});
	big[2].push_back({3, 600000000000000000LL});
	Brute bb(big, 1); dm = DirectedMST(big, 1);
	assert(bb.found && bb.cost == dm.cost && dm.cost > oo);
	FOR(trial, 1, 50000){
		int n = 1 + (int)(rng() % 6);
		int m = (int)(rng() % 13);
		int r = 1 + (int)(rng() % n);
		int banned = 0;
		if(n > 1 && trial % 5 == 0) banned = r % n + 1;
		vector<vector<pair<int, ll>>> g(n + 1);
		FOR(i, 1, m){
			int u = 1 + (int)(rng() % n);
			int v = 1 + (int)(rng() % n);
			ll w = (int)(rng() % 16) - 5;
			if(v != banned) g[u].push_back({v, w});
		}
		g[0].push_back({1, -100});
		Brute brute(g, r);
		dm = DirectedMST(g, r);
		assert((dm.cost != LLONG_MAX) == brute.found);
		if(!brute.found) continue;
		assert(dm.cost == brute.cost);
		assert(sz(dm.par) == n + 1);
		assert(dm.par[0] == 0 && dm.par[r] == 0);
		ll sum = 0;
		FOR(v, 1, n) if(v != r){
			int u = dm.par[v];
			assert(1 <= u && u <= n);
			ll best = oo;
			for(auto [to, w]: g[u]) if(to == v)
				best = min(best, w);
			assert(best != oo);
			sum += best;
		}
		assert(reachesRoot(dm.par, r));
		assert(sum == dm.cost);
	}
	return 0;
}
