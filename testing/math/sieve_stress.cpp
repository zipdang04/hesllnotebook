#include "../../tex/contents/math/sieve.hpp"
#include <cassert>
#include <random>

int leastDiv(int x){
	if(x < 2) return 0;
	for(int d = 2; d * d <= x; d++)
		if(x % d == 0) return d;
	return x;
}

void checkFactors(Sieve &sv, int x){
	vector<int> factors = sv.factorize(x);
	assert(is_sorted(all(factors)));
	ll product = 1;
	for(int p: factors){
		assert(p >= 2 && p <= x);
		assert(leastDiv(p) == p);
		assert(product <= x / p);
		product *= p;
	}
	assert(product == x);
}

int main(){
	Sieve sv(1000000);
	vector<int> primes;
	assert(sz(sv.minDiv) == 1000001);
	FOR(x, 0, 1000000){
		int p = leastDiv(x);
		assert(sv.minDiv[x] == p);
		if(x >= 2 && p == x) primes.push_back(x);
		if(x > 0) checkFactors(sv, x);
	}
	assert(sv.primes == primes);
	Sieve small(0);
	mt19937 rng(712367);
	FOR(trial, 0, 300){
		int n = trial;
		if(trial > 200) n = rng() % 1000001;
		small = Sieve(n);
		assert(sz(small.minDiv) == n + 1);
		assert(small.minDiv[0] == 0);
		auto end = upper_bound(all(primes), n);
		vector<int> expected(primes.begin(), end);
		assert(small.primes == expected);
		FOR(j, 0, 20){
			int x = j == 0 ? n : rng() % (n + 1);
			assert(small.minDiv[x] == leastDiv(x));
			if(x > 0) checkFactors(small, x);
		}
	}
	assert(sv.primes == primes);
	return 0;
}
