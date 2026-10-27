#include "../../tex/contents/math/gauss.hpp"
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n; cin >> n; Mat a(n, vector<modint>(n));
	for (auto &r: a) for (auto &v: r) cin >> v;
	if (!inverse(a)){ cout << -1 << '\n'; return 0; }
	for (auto &r: a){ for (modint v: r) cout << v << ' '; cout << '\n'; } }
