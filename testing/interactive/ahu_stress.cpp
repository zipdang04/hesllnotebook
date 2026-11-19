#include "../../tex/contents/interactive/ahu.hpp"
#include <cassert>

mt19937 rng(712367);

void addEdge(vector<vector<int>> &g, int u, int v){
	g[u].push_back(v);
	g[v].push_back(u);
}

vector<int> permutation(int n){
	vector<int> p(n + 1);
	iota(all(p), 0);
	shuffle(p.begin() + 1, p.end(), rng);
	return p;
}

vector<vector<int>> relabel(const vector<vector<int>> &g,
		const vector<int> &p){
	int n = sz(g) - 1;
	vector<vector<int>> h(n + 1);
	FOR(u, 1, n){
		for(int v: g[u])
			h[p[u]].push_back(p[v]);
		shuffle(all(h[p[u]]), rng);
	}
	h[0] = {0, n + 1, -1};
	return h;
}

vector<vector<int>> makeTree(int n, int shape){
	vector<vector<int>> g(n + 1);
	int spine = (n + 1) / 2;
	FOR(v, 2, n){
		int par = 1;
		if(shape == 0)
			par = 1 + rng() % (v - 1);
		if(shape == 1 || (shape == 3 && v <= spine))
			par = v - 1;
		if(shape == 3 && v > spine)
			par = 1 + rng() % spine;
		addEdge(g, par, v);
	}
	return relabel(g, permutation(n));
}

bool isomorphic(const vector<vector<int>> &a,
		const vector<vector<int>> &b, int ra = 0, int rb = 0){
	int n = sz(a) - 1;
	vector<vector<int>> x(n + 1, vector<int>(n + 1));
	vector<vector<int>> y = x;
	FOR(u, 1, n){
		for(int v: a[u])
			x[u][v] = 1;
		for(int v: b[u])
			y[u][v] = 1;
	}
	vector<int> p(n + 1);
	iota(all(p), 0);
	do{
		if(ra && p[ra] != rb)
			continue;
		bool same = [&]{
			FOR(u, 1, n)
				FOR(v, 1, n)
					if(x[u][v] != y[p[u]][p[v]])
						return false;
			return true;
		}();
		if(same)
			return true;
	}while(next_permutation(p.begin() + 1, p.end()));
	return false;
}

int main(){
	AHU t;
	vector<vector<int>> one(2);
	vector<int> leaf = {0, 1};
	assert(t.rooted(one, 1) == leaf);
	assert(t.unrooted(one) == 1);
	t = AHU();
	assert(t.ids.empty());
	FOR(it, 0, 2999){
		// First cover every size and shape, then random sizes.
		int n = it < 28 ? it / 4 + 1 : 1 + rng() % 7;
		auto a = makeTree(n, it % 4);
		auto p = permutation(n);
		auto h = relabel(a, p), b = h;
		if(rng() % 2)
			b = makeTree(n, rng() % 4);
		int ua = t.unrooted(a), ub = t.unrooted(b);
		assert((ua == ub) == isomorphic(a, b));
		int ra = 1 + rng() % n, rb = 1 + rng() % n;
		auto x = t.rooted(a, ra), y = t.rooted(b, rb);
		bool same = isomorphic(a, b, ra, rb);
		assert((x[ra] == y[rb]) == same);
		auto z = t.rooted(h, p[ra]);
		for(const auto &id: {x, y, z}){
			assert(sz(id) == n + 1 && id[0] == 0);
			FOR(v, 1, n)
				assert(1 <= id[v] && id[v] <= sz(t.ids));
		}
		FOR(v, 1, n)
			assert(x[v] == z[p[v]]);
	}
	t = AHU();
	int n = 500000;
	vector<vector<int>> a(n + 1), b(n + 1);
	FOR(v, 2, n){
		addEdge(a, v - 1, v);
		addEdge(b, n - v + 2, n - v + 1);
	}
	int ua = t.unrooted(a), ub = t.unrooted(b);
	assert(ua == ub);
}
