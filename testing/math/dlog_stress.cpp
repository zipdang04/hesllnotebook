#include "../../tex/contents/math/dlog.hpp"
#include <cassert>

vector<ll> bruteLog(ll a, ll m){
	a %= m;
	if(a < 0) a += m;
	vector<ll> first(m, -1);
	ll v = 1 % m;
	FOR(k, 0LL, 2 * m){
		if(first[v] == -1) first[v] = k;
		v = v * a % m;
	}
	return first;
}

void checkSqrt(ll a, ll p){
	ll b = a % p;
	if(b < 0) b += p;
	bool exists = false;
	FOR(x, 0LL, p - 1)
		if(x * x % p == b) exists = true;
	ll r = discreteSqrt(a, p);
	if(!exists) assert(r == -1);
	else{
		assert(0 <= r && r < p);
		assert(r * r % p == b);
	}
}

int main(){
	FOR(m, 1LL, 300)
		FOR(a, 0LL, m - 1){
			vector<ll> first = bruteLog(a, m);
			FOR(b, 0LL, m - 1){
				assert(discreteLog(a, b, m) == first[b]);
				assert(discreteLog(a - m, b - m, m)
					== first[b]);
			}
		}
	vector<ll> primes;
	FOR(p, 2LL, 1999){
		bool prime = true;
		for(ll d = 2; d * d <= p; d++)
			if(p % d == 0) prime = false;
		if(!prime) continue;
		primes.push_back(p);
		FOR(a, 0LL, p - 1) checkSqrt(a, p);
	}
	mt19937_64 rng(0);
	uniform_int_distribution<ll> value(
		numeric_limits<ll>::min(), numeric_limits<ll>::max());
	FOR(it, 1, 2000){
		ll m = rng() % 300 + 1;
		ll a = value(rng), b = value(rng);
		vector<ll> first = bruteLog(a, m);
		ll target = b % m;
		if(target < 0) target += m;
		assert(discreteLog(a, b, m) == first[target]);
		ll p = primes[rng() % sz(primes)];
		checkSqrt(a, p);
	}
}
