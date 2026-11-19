#include "../../tex/contents/flow/stoerwagner.hpp"
#include <cassert>

ll across(const vector<vector<pair<int, ll>>> &g,
		const vector<int> &side){
	assert(!side.empty() && sz(side) < sz(g) - 1);
	vector<char> in(sz(g));
	for(int u: side){
		assert(1 <= u && u < sz(g) && !in[u]);
		in[u] = 1;
	}
	ll sum = 0;
	for(int u: side)
		for(auto [v, w]: g[u]) if(!in[v])
			sum += w;
	return sum;
}

int main(){
	for(ll w: {oo - 1, oo, oo + 1, LLONG_MAX}){
		vector<vector<pair<int, ll>>> h = {
			{}, {{2, w}}, {{1, w}}
		};
		auto [cut, side] = globalMinCut(h);
		assert(cut == w && across(h, side) == w);
	}
	{
		// A later phase improves a first-phase cut above oo.
		ll B = 1000000000000000000LL;
		vector<vector<pair<int, ll>>> t = {{}, {{2, B}, {3, B}},
			{{1, B}, {3, 2 * B}}, {{1, B}, {2, 2 * B}}};
		auto [c, sd] = globalMinCut(t);
		assert(c == 2 * B && across(t, sd) == 2 * B);
	}
	vector<vector<pair<int, ll>>> h(8);
	FOR(u, 1, 6)
		FOR(v, 1, 6) if(u != v)
			h[u].push_back({v, 900000000000000000LL});
	auto [largeCut, largeSide] = globalMinCut(h);
	assert(largeCut == 0 && across(h, largeSide) == 0);
	FOR(n, 0, 1){
		auto [cut, side] = globalMinCut(
			vector<vector<pair<int, ll>>>(n + 1));
		assert(cut == oo && side.empty());
	}
	vector<vector<pair<int, ll>>> g = {
		{}, {{2, 3}, {2, 4}, {1, 9}}, {{1, 3}, {1, 4}}
	};
	auto [cut, side] = globalMinCut(g);
	assert(cut == 7 && sz(side) == 1);
	assert(across(g, side) == cut);
	g = {{}, {{2, 5}}, {{1, 5}}, {}};
	tie(cut, side) = globalMinCut(g);
	assert(cut == 0 && across(g, side) == 0);
	g = {{}, {{2, 1}, {4, 10}}, {{1, 1}, {3, 10}},
		{{2, 10}, {4, 1}}, {{3, 1}, {1, 10}}};
	tie(cut, side) = globalMinCut(g);
	sort(all(side));
	assert(cut == 2 && across(g, side) == 2);
	assert(side == vector<int>({2, 3})
		|| side == vector<int>({1, 4}));
	g[0] = {{1, 100}};
	auto [cut0, side0] = globalMinCut(g);
	sort(all(side0));
	assert(cut0 == cut && side0 == side);
	assert(across(g, side0) == cut0);
	return 0;
}
