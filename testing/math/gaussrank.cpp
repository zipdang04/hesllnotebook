#include "../../tex/contents/math/gauss.hpp"
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, m; cin >> n >> m; Mat a(n, vector<modint>(m));
	for (auto &r: a) for (auto &v: r) cin >> v;
	cout << rref(a, m).size() << '\n'; }
