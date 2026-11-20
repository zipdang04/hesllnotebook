#pragma once
#include "millerrabin.hpp"

// findDivisor: composite n, 4 <= n < 2^62; 1 < d < n.
//   About n^(1/4) mulMods (heuristic); deterministic.
// factorize: 1 <= n < 2^62; prime factors, unsorted.
ll findDivisor(ll n){
	// prd = 2 covers n = 4; t = 30 makes the first gcd early
	ll x = 0, y = 0, t = 30, prd = 2, c = 1;
	auto f = [&](ll v){ return (mulMod(v, v, n) + c) % n; };
	while(t++ % 40 || __gcd(prd, n) == 1){
		if(x == y) x = ++c, y = f(x); // new f on restart
		ll q = mulMod(prd, abs(x - y), n); if(q) prd = q;
		x = f(x), y = f(f(y));
	}
	return __gcd(prd, n);
}
vector<ll> factorize(ll n){
	if(n == 1) return {}; if(isPrime(n)) return {n};
	ll d = findDivisor(n);
	vector<ll> l = factorize(d), r = factorize(n / d);
	l.insert(l.end(), r.begin(), r.end());
	return l;
}
