#include "../../tex/contents/math/gauss.hpp"
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, m; cin >> n >> m;
	Mat a(n, vector<modint>(m)); vector<modint> b(n);
	for (auto &r: a) for (auto &v: r) cin >> v;
	for (auto &v: b) cin >> v;
	vector<modint> x; Mat ker;
	if (!solve(a, b, x, ker)){ cout << -1 << '\n'; return 0; }
	cout << ker.size() << '\n';
	for (modint v: x) cout << v << ' '; cout << '\n';
	for (auto &r: ker){ for (modint v: r) cout << v << ' '; cout << '\n'; } }
