#pragma once
#include "../contest/template.hpp"
// argmin of f on integers [l, r]
template<class F> ll ternaryInt(ll l, ll r, F f){
	while (l < r){
		ll m = l + (r - l) / 2;
		(f(m) > f(m + 1) ? l = m + 1 : r = m);
	}
	return l;
}
// argmin of unimodal f on reals [a, b], 1 call/iter
template<class F> double goldenSection(double a, double b, F f, int iters = 100){
	const double g = (sqrt(5.0) - 1) / 2;
	double x1 = b - g * (b - a), x2 = a + g * (b - a), f1 = f(x1), f2 = f(x2);
	FOR(int, it, 0, iters - 1){
		if (f1 < f2){ b = x2; x2 = x1; f2 = f1; x1 = b - g * (b - a); f1 = f(x1); }
		else{ a = x1; x1 = x2; f1 = f2; x2 = a + g * (b - a); f2 = f(x2); }
	}
	return (a + b) / 2;
}
// simulated annealing, returns best state
template<class S, class E, class N> S anneal(S s, E energy, N neighbour, double seconds, double T0 = 1, double T1 = 1e-9){
	mt19937_64 rng(CURRENT_TIMESTAMP);
	using dsec = chrono::duration<double>;
	auto t0 = chrono::steady_clock::now();
	double cur = energy(s), best = cur, T = T0;
	S bs = s;
	for (int it = 0; ; it++){
		if (!(it & 255)){  // geometric cooling T0 -> T1
			double fr = dsec(chrono::steady_clock::now() - t0).count() / seconds;
			if (!(fr < 1)) break;
			T = T0 * pow(T1 / T0, fr);
		}
		S t = neighbour(s, T, rng);
		double e = energy(t);
		if (e <= cur || uniform_real_distribution<double>(0, 1)(rng) < exp((cur - e) / T)){
			s = t; cur = e;
			if (cur < best) best = cur, bs = s;
		}
	}
	return bs;
}
