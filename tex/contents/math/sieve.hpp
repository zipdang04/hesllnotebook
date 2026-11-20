#pragma once
#include "../contest/template.hpp"

// Linear sieve, O(n), 4 bytes/number; primes ascending.
// minDiv[x]: least prime factor, x in 0..n, 0 if x < 2.
// factorize(x): 1 <= x <= n, ascending, O(log x).
struct Sieve{
	vector<int> primes, minDiv;
	Sieve(int n): minDiv(n + 1){
		FOR(i, 2, n){
			if(!minDiv[i]) minDiv[i] = i, primes.push_back(i);
			for(int p: primes){
				if(p > minDiv[i] || (ll)i * p > n) break;
				minDiv[i * p] = p;
			}
		}
	}
	vector<int> factorize(int x){
		vector<int> factors;
		while(x > 1){
			factors.push_back(minDiv[x]); x /= minDiv[x];
		}
		return factors;
	}
};
