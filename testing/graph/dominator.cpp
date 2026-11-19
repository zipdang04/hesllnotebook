#include "../../tex/contents/graph/dominator.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	Dominator single(vector<vector<int>>(2), 1);
	assert(single.idom == vector<int>({0, 1}));
	vector<vector<int>> diamond{{}, {2, 3}, {4}, {4}, {}, {}};
	vector<int> want{0, 1, 1, 1, 1, 0};
	Dominator check(diamond, 1);
	assert(check.idom == want);
	vector<vector<int>> cycle{{}, {2, 3}, {3}, {4}, {2}};
	Dominator cut(cycle, 1);
	assert(cut.idom == vector<int>({0, 1, 1, 1, 3}));
	cut = Dominator(cycle, 2);
	assert(cut.idom == vector<int>({0, 0, 2, 2, 3}));
	diamond[4].push_back(4); diamond[2].push_back(4);
	check = Dominator(diamond, 1);
	assert(check.idom == want);
	diamond[0] = {1, 4};
	check = Dominator(diamond, 1);
	assert(check.idom == want);
	check = Dominator(cycle, 1);
	assert(check.idom == vector<int>({0, 1, 1, 1, 3}));
	assert(cut.idom == vector<int>({0, 0, 2, 2, 3}));
	assert(single.idom == vector<int>({0, 1}));
	int n, m, s; cin >> n >> m >> s;
	vector<vector<int>> g(n + 1);
	FOR(i, 1, m){
		int a, b; cin >> a >> b;
		g[a + 1].push_back(b + 1);
	}
	Dominator dt(g, s + 1);
	assert(dt.idom[s + 1] == s + 1);
	assert(dt.idom[0] == 0);
	FOR(v, 1, n)
		cout << dt.idom[v] - 1 << (v == n ? '\n' : ' ');
}
