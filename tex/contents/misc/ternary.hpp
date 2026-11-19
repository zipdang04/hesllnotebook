#pragma once
#include "../contest/template.hpp"

// ternSearch: integer argmin, O(log(hi - lo)) calls.
// goldenSearch: real argmin, 102 calls. Maximise: negate f.
// f: ll(ll). Smallest argmin on [lo, hi].
// Call: ternSearch(1, n, [&](ll x){ return cost(x); });
template<class Func> ll ternSearch(ll lo, ll hi, Func f){
	while(lo < hi){
		ll mid = lo + (hi - lo) / 2;
		if(f(mid) <= f(mid + 1))
			hi = mid;
		else
			lo = mid + 1;
	}
	return lo;
}
// f: double(double), unimodal on [lo, hi]. Argmin.
// Call: goldenSearch(lo, hi, f);
template<class Func>
double goldenSearch(double lo, double hi, Func f){
	// weighted sums of the ends: hi - lo may overflow
	double r = (sqrt(5) - 1) / 2, q = 1 - r;
	double x1 = q * hi + r * lo, x2 = q * lo + r * hi;
	double f1 = f(x1), f2 = f(x2);
	// 100 steps shrink [lo, hi] by 0.618^100 < 1e-20
	FOR(it, 1, 100){
		if(f1 < f2){
			hi = x2; x2 = x1; f2 = f1;
			x1 = q * hi + r * lo; f1 = f(x1);
		}else{
			lo = x1; x1 = x2; f1 = f2;
			x2 = q * lo + r * hi; f2 = f(x2);
		}
	}
	return lo;
}
