#include "../../tex/contents/math/fps.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	assert(polyDiv({1, 2}, {1, 2, 3}).empty());
	assert(polyDiv({}, {1}).empty());
	assert((polyDiv({2, 4, 0}, {2})
		== vector<MintN>{1, 2, 0}));
	assert((polyDiv({-1, 0, 1}, {-1, 1})
		== vector<MintN>{1, 1}));
	int n, m; cin >> n >> m;
	vector<MintN> f(n), g(m);
	for(MintN &x: f)
		cin >> x;
	for(MintN &x: g)
		cin >> x;
	vector<MintN> q = polyDiv(f, g), r = f;
	vector<MintN> c = convNtt(g, q);
	FOR(i, 0, sz(c) - 1)
		r[i] -= c[i];
	r.resize(m - 1);
	while(!r.empty() && r.back() == 0)
		r.pop_back();
	cout << sz(q) << ' ' << sz(r) << '\n';
	FOR(i, 0, sz(q) - 1)
		cout << (i ? " " : "") << q[i];
	cout << '\n';
	FOR(i, 0, sz(r) - 1)
		cout << (i ? " " : "") << r[i];
	cout << '\n';
}
