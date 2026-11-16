#include "../../tex/contents/math/pollardrho.hpp"
#include <cassert>
void check(ll n){
	ll prod = 1;
	for(ll p: factorize(n)){
		assert(isPrime(p));
		prod *= p;
	}
	assert(prod == n);
}
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	FOR(n, 1, 100000){
		check(n);
		if(n >= 4 && !isPrime(n)){
			ll d = findDivisor(n);
			assert(1 < d && d < n && n % d == 0);
		}
	}
	check(4611686018427387903LL);
	check(4611686014132420609LL);
	check(2147483647LL * 2147483629LL);
	check(1000000007LL * 998244353LL);
	int q; cin >> q;
	while(q--){
		ll n; cin >> n;
		vector<ll> v = factorize(n);
		sort(v.begin(), v.end());
		cout << v.size();
		for(ll p: v) cout << ' ' << p;
		cout << '\n';
	}
}
