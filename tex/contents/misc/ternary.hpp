#pragma once
#include "../contest/template.hpp"
template<class F> ll ternaryInt(ll l, ll r, F f){
	while (l < r){
		ll m = l + (r - l) / 2;
		(f(m) > f(m + 1) ? l = m + 1 : r = m);
	}
	return l;
}
template<class F> double golden(double a, double b, F f){
	const double g = (sqrt(5.0) - 1) / 2;
	double x1 = b - g * (b - a), f1 = f(x1);
	double x2 = a + g * (b - a), f2 = f(x2);
	FOR(int, it, 1, 100){
		if (f1 < f2){
			b = x2; x2 = x1; f2 = f1;
			x1 = b - g * (b - a); f1 = f(x1);
		}else{
			a = x1; x1 = x2; f1 = f2;
			x2 = a + g * (b - a); f2 = f(x2);
		}
	}
	return (a + b) / 2;
}
template<class F> void anneal(double sec, double T0,
		double T1, F step){
	mt19937_64 rng(CURRENT_TIMESTAMP);
	auto start = chrono::steady_clock::now();
	double T = T0;
	auto ok = [&](double d){
		uniform_real_distribution<double> u(0, 1);
		return d <= 0 || u(rng) < exp(-d / T);
	};
	for (int it = 0; ; it++){
		if (it % 256 == 0){
			chrono::duration<double> el =
				chrono::steady_clock::now() - start;
			if (el.count() >= sec) return;
			T = T0 * pow(T1 / T0, el.count() / sec);
		}
		step(ok, rng);
	}
}
