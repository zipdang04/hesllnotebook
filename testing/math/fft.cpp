#include "../../tex/contents/math/fft.hpp"
#include <cassert>
int main(){
	vector<Cd> a = {1, 0, 0, 0};
	fft(a);
	for(Cd x: a) assert(abs(x - Cd(1)) < 1e-9);
	a = {0, 1, 0, 0};
	fft(a);
	vector<Cd> direction = {1, Cd(0, 1), -1, Cd(0, -1)};
	FOR(i, 0, 3) assert(abs(a[i] - direction[i]) < 1e-9);
	a.assign(8, 1);
	fft(a);
	FOR(i, 0, sz(a) - 1)
		assert(abs(a[i] - Cd(i ? 0 : 8)) < 1e-9);
	a = {Cd(5)};
	fft(a);
	assert(a == vector<Cd>{Cd(5)});
	vector<Cd> v(16);
	FOR(i, 0, sz(v) - 1) v[i] = Cd(i - 7, i % 3 - 1);
	vector<Cd> w = v;
	fft(w);
	reverse(w.begin() + 1, w.end());
	fft(w);
	FOR(i, 0, sz(v) - 1)
		assert(abs(w[i] / 16.0 - v[i]) < 1e-9);
	assert(conv({}, {1.0}).empty());
	assert(conv({1.0}, {}).empty());
	assert(convMod({}, {1}, 7).empty());
	assert(convMod({1}, {}, 7).empty());
	assert(conv({3}, {4})[0] == 12);
	assert(convMod({5}, {6}, 7) == vector<ll>{2});
	vector<double> c = conv({1, 2}, {3, -4, 5});
	vector<ll> want = {3, 2, -3, 10};
	assert(sz(c) == sz(want));
	FOR(i, 0, sz(c) - 1) assert(llround(c[i]) == want[i]);
	c = conv({0.5, -1.25}, {2, 0.5});
	vector<double> fractional = {1, -2.25, -0.625};
	assert(sz(c) == sz(fractional));
	FOR(i, 0, sz(c) - 1)
		assert(abs(c[i] - fractional[i]) < 1e-9);
	const ll P = 1000000007;
	want = {1, P - 2};
	assert(convMod({P - 1, 2}, {P - 1}, P) == want);
	want = {0, 0, 0};
	assert(convMod({3, 4}, {5, 6}, 1) == want);
	want = {15, 38, 24};
	assert(convMod({3, 4}, {5, 6}, 998244353) == want);
	ios_base::sync_with_stdio(0); cin.tie(0);
	int n, m; cin >> n >> m;
	vector<ll> b(n), d(m);
	for(ll &x: b) cin >> x;
	for(ll &x: d) cin >> x;
	for(ll x: convMod(b, d, P)) cout << x << ' ';
	cout << '\n';
}
