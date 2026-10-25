#include "../../tex/contents/strings/eertree.hpp"
int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	string s; cin >> s;
	Eertree<> e; vector<int> v;
	for (char c: s) v.push_back(e.add(c) - 1);
	int m = e.t.size(); vector<int> par(m);
	for (int u = 0; u < m; u++) for (int x: e.t[u].nxt) if (x) par[x] = u;
	cout << m - 2 << '\n';
	for (int u = 2; u < m; u++) cout << par[u] - 1 << ' ' << e.t[u].link - 1 << '\n';
	for (int x: v) cout << x << ' ';
	cout << '\n';
}
