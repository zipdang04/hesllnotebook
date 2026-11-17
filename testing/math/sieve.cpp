#include "../../tex/contents/math/sieve.hpp"
#include <cassert>
int main(){
	Sieve zero(0), one(1), two(2);
	assert(zero.primes.empty() && sz(zero.minDiv) == 1);
	assert(zero.minDiv[0] == 0);
	assert(one.primes.empty() && sz(one.minDiv) == 2);
	assert(one.minDiv[0] == 0 && one.minDiv[1] == 0);
	assert(one.factorize(1).empty());
	assert(two.primes == vector<int>{2});
	assert(two.minDiv[2] == 2);
	assert(two.factorize(2) == vector<int>{2});
	Sieve s(100);
	assert(sz(s.primes) == 25 && s.primes.back() == 97);
	assert(sz(s.minDiv) == 101);
	assert(s.minDiv[0] == 0 && s.minDiv[1] == 0);
	assert(s.minDiv[91] == 7 && s.minDiv[97] == 97);
	assert(s.minDiv[100] == 2);
	assert(s.factorize(1).empty());
	assert(s.factorize(97) == vector<int>{97});
	s = Sieve(360);
	assert((s.factorize(360)
		== vector<int>{2, 2, 2, 3, 3, 5}));
	Sieve t(10);
	s = Sieve(5);
	assert((s.primes == vector<int>{2, 3, 5}));
	assert(sz(s.minDiv) == 6 && s.minDiv[5] == 5);
	assert((t.primes == vector<int>{2, 3, 5, 7}));
	assert(sz(t.minDiv) == 11 && t.minDiv[10] == 2);
	Sieve big(100000000);
	assert(sz(big.primes) == 5761455);
	assert(big.primes.back() == 99999989);
	assert(sz(big.minDiv) == 100000001);
	assert(big.minDiv[99999989] == 99999989);
	assert(big.minDiv[100000000] == 2);
	vector<int> factors = big.factorize(99999999);
	assert((factors == vector<int>{3, 3, 11, 73, 101, 137}));
	assert(is_sorted(all(factors)));
	ll product = 1;
	for(int p: factors)
		product *= p;
	assert(product == 99999999);
	return 0;
}
