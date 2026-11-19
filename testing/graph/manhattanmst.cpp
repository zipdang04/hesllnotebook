#include "../../tex/contents/graph/manhattanmst.hpp"
#include <cassert>
ll prim(const vector<pll> &p){
	int n = sz(p) - 1; ll tot = 0;
	vector<ll> d(n + 1, 4 * oo); vector<int> in(n + 1);
	d[1] = 0;
	FOR(it, 1, n){
		int u = 0;
		FOR(v, 1, n) if(!in[v] && (!u || d[v] < d[u])) u = v;
		in[u] = 1; tot += d[u];
		FOR(v, 1, n) minimize(d[v], llabs(p[u].fi - p[v].fi)
			+ llabs(p[u].se - p[v].se));
	}
	return tot;
}
ll total(const vector<array<ll, 3>> &e){
	ll s = 0;
	for(auto [w, u, v]: e) s += w;
	return s;
}
int main(){
	assert(manhattanMST(vector<pll>(1)).empty());
	assert(manhattanMST(vector<pll>(2)).empty());
	vector<pll> r(2); r[0] = {LLONG_MIN, LLONG_MIN};
	assert(manhattanMST(r).empty());
	vector<pll> q(4, {2, 2});
	q[0] = {-5, 7};
	auto z = manhattanMST(q);
	assert(sz(z) == 2 && total(z) == 0);
	mt19937_64 rng(12345);
	FOR(t, 1, 600){
		int n = rng() % 30 + 1; ll R = t % 2 ? 3 : 1000;
		vector<pll> p(n + 1);
		FOR(i, 1, n) p[i] = {(ll)(rng() % (2 * R + 1)) - R,
			(ll)(rng() % (2 * R + 1)) - R};
		auto e = manhattanMST(p);
		assert(sz(e) == n - 1 && total(e) == prim(p));
	}
	FOR(t, 1, 200){
		int n = rng() % 5 + 1;
		vector<pll> p(n + 1);
		FOR(i, 1, n) p[i] = {((ll)(rng() % 3) - 1) * (ll)1e17,
			((ll)(rng() % 3) - 1) * (ll)1e17};
		auto e = manhattanMST(p);
		assert(sz(e) == n - 1 && total(e) == prim(p));
	}
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	int n; cin >> n;
	vector<pll> p(n + 1);
	FOR(i, 1, n) cin >> p[i].fi >> p[i].se;
	auto e = manhattanMST(p);
	assert(sz(e) == n - 1);
	ll sum = 0, last = 0;
	for(auto [w, u, v]: e){
		assert(w == llabs(p[u].fi - p[v].fi)
			+ llabs(p[u].se - p[v].se));
		assert(w >= last);
		last = w; sum += w;
	}
	cout << sum << '\n';
	for(auto [w, u, v]: e)
		cout << u - 1 << ' ' << v - 1 << '\n';
}
