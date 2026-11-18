#include "../../tex/contents/math/gauss.hpp"

int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	int n, m;
	cin >> n >> m;
	vector<vector<MintN>> a(n, vector<MintN>(m));
	for(auto &row: a) for(auto &x: row) cin >> x;
	cout << gauss(a, m).fi << '\n';
}
