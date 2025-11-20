#include "../../../tex/contents/math/numbertheory/gauss.hpp"

int n, m;
Gaussian::Elimination gauss;
int a[505][505], b[505];
int main() {
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	cin >> n >> m;
	gauss = Gaussian::Elimination(m);
	FOR(int, i, 0, n-1) FOR(int, j, 0, m-1) cin >> a[i][j];
	FOR(int, i, 0, n-1) cin >> b[i];
	FOR(int, i, 0, n-1)
		gauss.add({vector<modint>(a[i], a[i] + m), b[i]});
	auto [ans, basis] = gauss.solve();
	if (ans.empty()) cout << -1 << '\n';
	else{

		cout << basis.size() << '\n';
		for (modint i: ans) cout << i <<  ' '; cout << '\n';
		for (auto v: basis){
			for (modint i: v) cout << i << ' ';
			cout << '\n';
		}
	}
}
