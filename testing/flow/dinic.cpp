#include "../../tex/contents/flow/dinic.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	Dinic e(2);
	assert(e.maxFlow(0, 1) == 0 && e.sideS(0) && !e.sideS(1));
	assert(e.maxFlow(1, 1) == 0);
	Dinic b(2);
	b.addEdge(0, 1, 1LL << 60);
	assert(b.maxFlow(0, 1) == 1LL << 60);
	assert(b.flowOf(0) == 1LL << 60);
	Dinic c(3);
	assert(c.addEdge(0, 1, 5) == 0);
	assert(c.addEdge(1, 2, 3) == 2);
	assert(c.addEdge(0, 2, 0) == 4);
	assert(c.maxFlow(0, 2) == 3);
	assert(c.flowOf(0) == 3 && c.flowOf(2) == 3);
	assert(c.maxFlow(0, 2) == 0);
	assert(c.sideS(0) && c.sideS(1) && !c.sideS(2));
	Dinic base(3);
	base.addEdge(0, 1, 5), base.addEdge(1, 2, 3);
	Dinic cur = base;
	assert(cur.maxFlow(0, 2) == 3 && base.edges[0].cap == 5);
	assert(base.maxFlow(0, 2) == 3);
	cur = Dinic(3);
	assert(sz(cur.edges) == 0);
	Dinic und(2);
	und.addEdge(0, 1, 4, 4);
	assert(und.maxFlow(1, 0) == 4);
	Dinic big(2);
	big.addEdge(0, 1, 1LL << 61, 1LL << 61);
	assert(big.maxFlow(0, 1) == 1LL << 61);
	Dinic sp(3);
	sp.addEdge(1, 1, 7), sp.addEdge(0, 1, 2);
	sp.addEdge(0, 1, 3);
	sp.addEdge(1, 2, 4);
	assert(sp.maxFlow(0, 2) == 4 && sp.maxFlow(1, 1) == 0);
	Dinic asy(2);
	asy.addEdge(0, 1, 2, 3);
	Dinic asy2 = asy;
	assert(asy.maxFlow(0, 1) == 2 && asy2.maxFlow(1, 0) == 3);
	int L, R, m; cin >> L >> R >> m;
	int s = L + R, t = L + R + 1;
	Dinic d(L + R + 2);
	FOR(a, 0, L - 1) d.addEdge(s, a, 1);
	FOR(j, 0, R - 1) d.addEdge(L + j, t, 1);
	vector<int> ea(m), eb(m), id(m);
	FOR(i, 0, m - 1){
		cin >> ea[i] >> eb[i];
		id[i] = d.addEdge(ea[i], L + eb[i], 1);
	}
	ll f = d.maxFlow(s, t);
	cout << f << '\n';
	FOR(i, 0, m - 1) if(d.flowOf(id[i]) == 1)
		cout << ea[i] << ' ' << eb[i] << '\n';
	assert(d.sideS(s) && !d.sideS(t));
	int cover = 0;
	FOR(a, 0, L - 1) cover += !d.sideS(a);
	FOR(j, 0, R - 1) cover += d.sideS(L + j);
	assert(cover == f);
	FOR(i, 0, m - 1)
		assert(!d.sideS(ea[i]) || d.sideS(L + eb[i]));
}
