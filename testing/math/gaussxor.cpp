#include "../../tex/contents/math/gauss.hpp"
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, m; cin >> n >> m; vector<bitset<MAXB>> a(n);
	string s;
	EFOR(int, i, 0, n){ cin >> s; EFOR(int, j, 0, m) a[i][j] = s[j] == '1'; }
	cin >> s; EFOR(int, i, 0, n) a[i][m] = s[i] == '1';
	bitset<MAXB> x; vector<bitset<MAXB>> ker;
	if (solveXor(a, m, x, &ker) < 0){ cout << -1 << '\n'; return 0; }
	cout << ker.size() << '\n';
	auto pr = [&](bitset<MAXB> &v){ EFOR(int, j, 0, m) cout << v[j]; cout << '\n'; };
	pr(x); for (auto &v: ker) pr(v); }
