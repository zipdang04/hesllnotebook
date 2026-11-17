#include "../../tex/contents/ds/rollbackdsu.hpp"
#include <cassert>

RollbackDsu dsu(1);
vector<vector<int>> kids, ask;
vector<array<int, 2>> edge, qs;
vector<int> ans;

// recursive over the version tree, depth <= Q
void dfs(int x){
	int t = dsu.time();
	if(x) dsu.unite(edge[x][0], edge[x][1]);
	for(int i: ask[x]){
		auto [u, v] = qs[i];
		ans[i] = dsu.find(u) == dsu.find(v);
	}
	for(int y: kids[x]) dfs(y);
	dsu.rollback(t);
}

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	RollbackDsu d(6);
	int t0 = d.time();
	assert(t0 == 0);
	assert(d.unite(1, 2) && d.unite(3, 4));
	int t1 = d.time();
	assert(d.unite(2, 4));
	assert(d.size(1) == 4 && d.size(5) == 1);
	assert(d.stk.back() == make_pair(3, -2));
	assert(d.par[1] == -4);
	assert(5 - d.time() == 2);
	assert(!d.unite(1, 3) && d.time() == 3);
	assert(!d.unite(5, 5));
	d.rollback(d.time() + 3);
	assert(d.time() == 3 && d.size(1) == 4);
	RollbackDsu e(3);
	assert(e.time() == 0 && e.size(1) == 1);
	assert(e.find(1) != e.find(2));
	d.rollback(t1);
	assert(d.size(1) == 2 && d.size(3) == 2);
	assert(d.find(1) != d.find(3));
	assert(d.size(0) == 1);
	d.rollback(t0);
	FOR(i, 0, 5) assert(d.size(i) == 1);
	d = RollbackDsu(2);
	assert(d.time() == 0 && d.size(1) == 1);
	d = RollbackDsu(1);
	assert(d.size(0) == 1);
	int n, q;
	cin >> n >> q;
	dsu = RollbackDsu(n + 1);
	kids.resize(q + 1), ask.resize(q + 1), edge.resize(q + 1);
	qs.resize(q + 1), ans.assign(q + 1, -1);
	FOR(i, 1, q){
		int t, k, u, v;
		cin >> t >> k >> u >> v;
		if(t == 0){
			kids[k + 1].push_back(i);
			edge[i] = {u + 1, v + 1};
		}else{
			ask[k + 1].push_back(i);
			qs[i] = {u + 1, v + 1};
		}
	}
	dfs(0);
	FOR(i, 1, q) if(ans[i] >= 0) cout << ans[i] << '\n';
}
