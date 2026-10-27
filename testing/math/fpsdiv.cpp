#include "../../tex/contents/math/fps.hpp"

int main(){
	ios_base::sync_with_stdio(0); cin.tie(0);
	int n, m; cin >> n >> m;
	FPS::poly f(n), g(m);
	for (auto &x: f) cin >> x;
	for (auto &x: g) cin >> x;
	auto [q, r] = FPS::divMod(f, g);
	while (!q.empty() && q.back() == 0) q.pop_back();
	while (!r.empty() && r.back() == 0) r.pop_back();
	cout << q.size() << ' ' << r.size() << '\n';
	for (auto x: q) cout << x << ' ';
	cout << '\n';
	for (auto x: r) cout << x << ' ';
	cout << '\n';
}
