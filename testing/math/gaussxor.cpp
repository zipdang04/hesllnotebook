#include "../../tex/contents/math/gauss.hpp"
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, m; cin >> n >> m; vector<bitset<MAXB>> a(n);
	string s;
	FOR(int, i, 0, n - 1){ cin >> s; FOR(int, j, 0, m - 1) a[i][j] = s[j] == '1'; }
	cin >> s; FOR(int, i, 0, n - 1) a[i][m] = s[i] == '1';
	bitset<MAXB> x; vector<bitset<MAXB>> ker;
	if (solveXor(a, m, x, &ker) < 0){ cout << -1 << '\n'; return 0; }
	cout << ker.size() << '\n';
	auto pr = [&](bitset<MAXB> &v){ FOR(int, j, 0, m - 1) cout << v[j]; cout << '\n'; };
	pr(x); for (auto &v: ker) pr(v); }
