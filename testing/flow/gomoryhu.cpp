#include "../../tex/contents/flow/gomoryhu.hpp"
#include <cassert>

void edge(vector<vector<pair<int, ll>>> &g,
		int u, int v, ll w){
	g[u].push_back({v, w}), g[v].push_back({u, w});
}

vector<ll> costs(const vector<array<ll, 3>> &t, int n){
	vector<ll> c;
	for(auto [u, v, w]: t){
		assert(1 <= u && u <= n && 1 <= v && v <= n && u != v);
		c.push_back(w);
	}
	sort(all(c));
	return c;
}

int main(){
	assert(gomoryHu(
		vector<vector<pair<int, ll>>>(2)).empty());
	assert(gomoryHu(
		vector<vector<pair<int, ll>>>(1)).empty());
	vector<vector<pair<int, ll>>> two(3);
	edge(two, 1, 2, 2), edge(two, 1, 2, 3);
	vector<array<ll, 3>> t = gomoryHu(two);
	assert(sz(t) == 1 && t[0] == (array<ll, 3>{2, 1, 5}));
	vector<vector<pair<int, ll>>> big(3);
	edge(big, 1, 2, 1LL << 40);
	assert(gomoryHu(big)
		== (vector<array<ll, 3>>{{2, 1, 1LL << 40}}));
	vector<vector<pair<int, ll>>> huge(3);
	edge(huge, 1, 2, 2000000000000000000LL);
	assert(gomoryHu(huge) == (vector<array<ll, 3>>{
		{2, 1, 2000000000000000000LL}}));
	// Values only: dropping the weight-2 edge cuts 3 off (3).
	vector<vector<pair<int, ll>>> v(4);
	edge(v, 1, 3, 1), edge(v, 2, 3, 2);
	assert(gomoryHu(v) == (vector<array<ll, 3>>{
		{2, 1, 1}, {3, 2, 2}}));
	vector<vector<pair<int, ll>>> p(4);
	edge(p, 1, 2, 4), edge(p, 2, 3, 1);
	assert(costs(gomoryHu(p), 3) == (vector<ll>{1, 4}));
	p[0].push_back({1, 100});
	assert(costs(gomoryHu(p), 3) == (vector<ll>{1, 4}));
	vector<vector<pair<int, ll>>> d(4);
	edge(d, 1, 2, 7);
	assert(costs(gomoryHu(d), 3) == (vector<ll>{0, 7}));
	return 0;
}
