#include "../../tex/contents/math/ntt.hpp"

int main()
{
	ios_base::sync_with_stdio(0); cin.tie(0);
	int n, m; cin >> n >> m;
	vector<modint> a(n), b(m);
	for (modint &x: a) cin >> x;
	for (modint &x: b) cin >> x;
	for (modint x: conv(a, b)) cout << x << ' ';
}
