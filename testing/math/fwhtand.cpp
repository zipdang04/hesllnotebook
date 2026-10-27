#include "../../tex/contents/math/fwht.hpp"
int main(){
	ios_base::sync_with_stdio(0); cin.tie(0);
	int N; cin >> N; int n = 1 << N;
	vector<modint> a(n), b(n);
	for (auto &x: a) cin >> x;
	for (auto &x: b) cin >> x;
	for (modint x: convolve(a, b, '&')) cout << x << ' ';
	cout << '\n';
}
