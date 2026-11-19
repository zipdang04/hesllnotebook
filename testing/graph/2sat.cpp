#include "../../tex/contents/graph/2sat.hpp"
#include <cassert>
int main(){
	assert(TwoSat(0).solve());
	TwoSat a(1); a.either(1, 1); a.either(-1, -1);
	assert(!a.solve());
	TwoSat b(2); b.either(-1, -1); b.either(1, 2);
	assert(b.n == 2 && sz(b.g) == 6);
	assert(b.node(2) == 4 && b.node(-2) == 5);
	assert(b.g[0].empty() && b.g[1].empty());
	assert(b.solve() && !b.val[1] && b.val[2]);
	assert(sz(b.val) == 3 && !b.val[0]);
	TwoSat c(3); c.either(1, 1);
	c.either(-1, 2); c.either(-2, 3);
	assert(c.solve() && c.val[1] && c.val[2] && c.val[3]);
	b = TwoSat(1); b.either(-1, -1);
	assert(b.solve() && !b.val[1]);
	assert(c.val[1] && c.val[2] && c.val[3]);
	c.either(-3, -3);
	assert(!c.solve());
	// both implication edges of each clause, once per copy
	TwoSat d(2); d.either(1, 1); d.either(1, -2);
	assert(d.g[3] == vector<int>({2, 2, 5}));
	assert(d.g[4] == vector<int>({2}));
	TwoSat t(1);
	assert(t.solve());
	t.either(-1, -1);
	assert(t.solve() && !t.val[1]);
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	string p, cnf; int n, m; cin >> p >> cnf >> n >> m;
	TwoSat ts(n);
	vector<pii> clauses;
	FOR(i, 1, m){
		int x, y, zero; cin >> x >> y >> zero;
		ts.either(x, y); clauses.push_back({x, y});
	}
	if(!ts.solve()){
		cout << "s UNSATISFIABLE" << '\n'; return 0;
	}
	for(auto [x, y]: clauses){
		bool vx = x > 0 ? ts.val[x] : !ts.val[-x];
		bool vy = y > 0 ? ts.val[y] : !ts.val[-y];
		assert(vx || vy);
	}
	cout << "s SATISFIABLE" << '\n' << 'v';
	FOR(x, 1, n)
		cout << ' ' << (ts.val[x] ? x : -x);
	cout << " 0" << '\n';
}
