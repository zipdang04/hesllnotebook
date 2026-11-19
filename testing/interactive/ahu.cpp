#include "../../tex/contents/interactive/ahu.hpp"
#include <cassert>
int main(){
	{
		AHU t;
		vector<vector<int>> g1(2);
		assert((t.rooted(g1, 1) == vector<int>{0, 1}));
		assert(t.unrooted(g1) == 1);
		g1[0] = {INT_MAX};
		assert((t.rooted(g1, 1) == vector<int>{0, 1}));
		assert(t.unrooted(g1) == 1);
		vector<vector<int>> g2 = {{}, {2}, {1}};
		assert(t.unrooted(g2) == t.rooted(g2, 1)[1]);
	}
	AHU t;
	vector<vector<int>> g4 = {{}, {2}, {1, 3}, {2, 4}, {3}};
	vector<vector<int>> s4 = {{}, {2, 3, 4}, {1}, {1}, {1}};
	vector<vector<int>> h4 = {{}, {3, 4}, {4}, {1}, {1, 2}};
	auto a = t.rooted(g4, 1), b = t.rooted(g4, 4);
	assert(a[1] == b[4] && a[2] == b[3] && a[4] == 1);
	assert(a[0] == 0 && b[0] == 0);
	assert(t.unrooted(g4) != t.unrooted(s4));
	assert(t.unrooted(g4) == t.unrooted(h4));
	t = AHU();
	assert(t.ids.empty());
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	int n; cin >> n;
	vector<vector<int>> g(n + 1);
	FOR(i, 1, n - 1){
		int p; cin >> p;
		g[p+1].push_back(i+1); g[i+1].push_back(p+1);
	}
	auto id = t.rooted(g, 1);
	assert(sz(id) == n + 1 && id[0] == 0);
	cout << sz(t.ids) << '\n';
	FOR(i, 1, n){
		assert(1 <= id[i] && id[i] <= sz(t.ids));
		cout << id[i] - 1 << (i == n ? '\n' : ' ');
	}
}
