#pragma once
#include "../contest/template.hpp"

typedef unsigned long long ull;
ull mulMod(ull a, ull b, ull m){ return (__uint128_t)a * b % m; }
ull powMod(ull a, ull e, ull m){
	ull r = 1;
	for (; e; e /= 2, a = mulMod(a, a, m)) if (e & 1) r = mulMod(r, a, m);
	return r;
}
bool isPrime(ull n){	// deterministic, all n < 2^64
	if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
	ull s = __builtin_ctzll(n - 1), d = n >> s;
	for (ull a: {2, 325, 9375, 28178, 450775, 9780504, 1795265022}){
		ull p = powMod(a % n, d, n), i = s;
		while (p != 1 && p != n - 1 && a % n && i--) p = mulMod(p, p, n);
		if (p != n - 1 && i != s) return 0;
	}
	return 1;
}
vector<ull> factor(ull n){	// prime factors with multiplicity, unsorted
	if (n == 1) return {};
	if (isPrime(n)) return {n};
	ull x = 0, y = 0, t = 30, prd = 2, i = 1, q;
	auto f = [&](ull v){ return mulMod(v, v, n) + i; };
	while (t++ % 40 || __gcd(prd, n) == 1){
		if (x == y) x = ++i, y = f(x);
		if ((q = mulMod(prd, max(x, y) - min(x, y), n))) prd = q;
		x = f(x), y = f(f(y));
	}
	ull g = __gcd(prd, n);
	auto l = factor(g), r = factor(n / g);
	l.insert(l.end(), r.begin(), r.end());
	return l;
}
