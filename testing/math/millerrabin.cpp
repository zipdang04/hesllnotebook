#include "../../tex/contents/math/millerrabin.hpp"
#include <cassert>
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	ll big = 9000000000000000000LL;
	assert(mulMod(big - 1, big - 1, big) == 1);
	assert(mulMod(0, 5, 7) == 0 && mulMod(3, 4, 1) == 0);
	assert(powMod(2, 10, 1000) == 24);
	assert(powMod(5, 0, 7) == 1 && powMod(5, 0, 1) == 0);
	assert(powMod(big + 2, 3, big) == 8);
	FOR(n, -3, 100000){
		bool p = n >= 2;
		for(ll d = 2; d * d <= n; d++)
			if(n % d == 0) p = false;
		assert(isPrime(n) == p);
	}
	assert(isPrime(9223372036854775783LL));
	assert(!isPrime(9223372036854775807LL));
	assert(!isPrime(3037000493LL * 3037000493LL));
	int q; cin >> q;
	while(q--){
		ll n; cin >> n;
		cout << (isPrime(n) ? "Yes" : "No") << '\n';
	}
}
