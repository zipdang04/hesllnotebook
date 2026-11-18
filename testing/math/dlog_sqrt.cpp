#include "../../tex/contents/math/dlog.hpp"
#include <cassert>
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	assert(discreteSqrt(0, 2) == 0);
	assert(discreteSqrt(1, 2) == 1);
	assert(discreteSqrt(0, 7) == 0);
	assert(discreteSqrt(3, 7) == -1);
	ll x = discreteSqrt(2, 7);
	assert(x == 3 || x == 4);
	x = discreteSqrt(-5, 7);
	assert(0 <= x && x < 7 && mulMod(x, x, 7) == 2);
	ll p = 1000000000000000001LL;
	while(!isPrime(p))
		p += 8;
	ll x0 = 123456789123LL, a = mulMod(x0, x0, p);
	ll r = discreteSqrt(a, p);
	assert(0 <= r && r < p && mulMod(r, r, p) == a);
	a = 2;
	while(powMod(a, (p - 1) / 2, p) != p - 1)
		++a;
	assert(discreteSqrt(a, p) == -1);
	p = 9223372036854775783LL;
	r = discreteSqrt(4, p);
	assert(r == 2 || r == p - 2);
	int t; cin >> t;
	while(t--){
		ll y; cin >> y >> p;
		cout << discreteSqrt(y, p) << '\n';
	}
}
