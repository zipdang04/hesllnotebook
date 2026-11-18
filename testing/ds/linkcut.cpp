#include "../../tex/contents/ds/linkcut.hpp"
#include <cassert>

int main(){
	LinkCut roots(4);
	roots.link(1, 2); roots.link(3, 2);
	assert(roots.lca(1, 3) == 2);
	roots.cut(1, 2); assert(roots.lca(2, 3) == 2);
	roots.update(3, 2, {1}); assert(roots.lca(2, 3) == 3);
	roots.get(2, 3); assert(roots.lca(2, 3) == 2);
	LinkCut empty(1), s(2);
	assert(sz(empty.t) == 1);
	s.update(1, 7);
	assert(s.get(1, 1).sum == 7);
	assert(s.get(1, 1).size == 1);
	assert(s.connected(1, 1) && s.lca(1, 1) == 1);
	LinkCut a(5);
	FOR(u, 1, 4)
		a.update(u, 10 * u);
	FOR(u, 1, 3)
		a.link(u, u + 1);
	assert(a.get(1, 4).sum == 100);
	assert(a.get(4, 1).sum == 100);
	assert(a.get(2, 3).size == 2);
	a.update(1, 3, {5});
	assert(a.get(1, 4).sum == 115);
	a.update(2, 2, LinkCut::Tag());
	assert(a.get(1, 4).sum == 115);
	a.makeRoot(1); assert(a.lca(3, 4) == 3);
	a.makeRoot(4); assert(a.lca(1, 2) == 2);
	a.makeRoot(1);
	assert(a.connected(1, 4));
	a.update(4, 0);
	assert(a.lca(3, 4) == 3);
	ll sum = a.get(1, 4).sum;
	a.cut(2, 3);
	assert(!a.connected(1, 4) && a.connected(3, 4));
	a.link(2, 3);
	assert(a.get(1, 4).sum == sum);
	a.makeRoot(1); a.access(3);
	assert(a.access(4) == 3);
	assert(a.isRoot(4) && a.t[4].size == 4);
	assert(a.get(1, 1).val == 15);
	const auto &z = a.t[0];
	assert(z.size == 0 && z.sum == 0 && z.val == 0);
	assert(z.par == 0 && z.ch[0] == 0 && z.ch[1] == 0);
	assert(!z.rev && z.tag.add == 0);
	s = LinkCut(5);
	assert(sz(s.t) == 5);
	FOR(u, 1, 4){
		assert(s.get(u, u).sum == 0);
		assert(s.get(u, u).size == 1);
		FOR(v, 1, 4)
			assert(s.connected(u, v) == (u == v));
	}
	s.update(1, 99); s.link(1, 2);
	assert(a.get(1, 4).sum == sum);
	a.update(1, -10);
	assert(s.get(1, 2).sum == 99);
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, q; cin >> n >> q;
	LinkCut lct(n + 1);
	FOR(i, 1, n){
		ll x; cin >> x;
		lct.update(i, x);
	}
	FOR(i, 1, n - 1){
		int u, v; cin >> u >> v;
		lct.link(u + 1, v + 1);
	}
	FOR(i, 1, q){
		int type; cin >> type;
		if(type == 0){
			int u, v, w, x; cin >> u >> v >> w >> x;
			lct.cut(u + 1, v + 1); lct.link(w + 1, x + 1);
		}else if(type == 1){
			int p; ll x; cin >> p >> x;
			lct.update(p + 1, p + 1, {x});
		}else{
			int u, v; cin >> u >> v;
			cout << lct.get(u + 1, v + 1).sum << '\n';
		}
	}
}
