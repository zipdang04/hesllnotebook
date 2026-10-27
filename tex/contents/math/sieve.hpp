#pragma once
#include "../contest/template.hpp"

// linear sieve on [0, n], n >= 1
struct Sieve{
	vector<int> spf, primes; // spf[x] = smallest prime factor of x (x >= 2); primes ascending
	Sieve(int n): spf(n + 1){
		FOR(int, i, 2, n){
			if (!spf[i]) spf[i] = i, primes.push_back(i);
			for (int p: primes){
				if (p > spf[i] || (ll)i * p > n) break;
				spf[i * p] = p;
			}
		}
	}
	vector<int> factorize(int x){ // 1 <= x <= n; prime factors ascending, with multiplicity
		vector<int> f;
		for (; x > 1; x /= spf[x]) f.push_back(spf[x]);
		return f;
	}
};
