#include "../../tex/contents/math/crt.hpp"
#include <cassert>
int main(){
	ll x, y;
	assert(extGcd(0, 0, x, y) == 0);
	assert(x == 1 && y == 0);
	assert(extGcd(5, 0, x, y) == 5);
	assert(x == 1 && y == 0);
	assert(extGcd(0, 5, x, y) == 5);
	assert(5 * y == 5);
	assert(extGcd(240, 46, x, y) == 2);
	assert(240 * x + 46 * y == 2);
	assert(abs(x) <= 23 && abs(y) <= 120);
	ll f = 7540113804746346429, h = 4660046610375530309;
	assert(extGcd(f, h, x, y) == 1);
	assert(x == 1779979416004714189);
	assert(y == -2880067194370816120);
	assert(crt(0, 1, 0, 1) == pll(0, 1));
	assert(crt(5, 6, 2, 4) == pll(-1, -1));
	assert(crt(2, 3, 3, 5) == pll(8, 15));
	assert(crt(1, 4, 3, 6) == pll(9, 12));
	assert(crt(-1, 4, 0, 3) == pll(3, 12));
	assert(crt(-1, 4, -2, 3) == pll(7, 12));
	ll m = 2147483647, n = 1000000007;
	ll a = m - 1, b = 12345;
	auto [r, l] = crt(a, m, b, n);
	assert(r % m == a && r % n == b);
	assert(0 <= r && r < l && l == m * n);
	assert(crt(b, n, a, m) == pll(r, l));
	m = 2147483647, n = 2147483649;
	assert(crt(m - 1, m, n - 1, n)
		== pll(m * n - 1, m * n));
	pll res = {0, 1};
	for(auto [v, mod]: vector<pll>{{2, 3}, {3, 5}, {2, 7}}){
		res = crt(res.fi, res.se, v, mod);
		if(res.fi == -1)
			break;
	}
	assert(res == pll(23, 105));
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	cin >> a >> b;
	extGcd(a, b, x, y);
	cout << x << ' ' << y << '\n';
}
