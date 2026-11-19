#include "../../tex/contents/misc/ternary.hpp"
#include <cassert>

int main(){
	mt19937_64 rng(812734);
	auto rnd = [&](int lo, int hi){
		return uniform_int_distribution<int>(lo, hi)(rng);
	};
	FOR(it, 1, 20000){
		ll lo = rnd(-1000, 1000);
		int n = rnd(0, 300);
		ll hi = lo + n, c = rnd(-1200, 1200);
		int shape = rnd(0, 3);
		ll a = rnd(1, 20), plateau = rnd(0, 300);
		vector<ll> val(n + 1);
		FOR(i, 0, n){
			ll d = lo + i - c;
			if(shape == 0){
				val[i] = d * d;
			}else if(shape == 1){
				val[i] = abs(d);
			}else if(shape == 2){
				val[i] = d < 0 ? -a * d
					: max(0LL, d - plateau);
			}else if(i){
				// Jitter < 10 keeps slopes increasing.
				ll slope = 10 * (d - 1) + rnd(0, 9);
				val[i] = val[i-1] + slope;
			}
		}
		ll best = lo;
		FOR(x, lo + 1, hi)
			if(val[x-lo] < val[best-lo]) best = x;
		int calls = 0;
		auto f = [&](ll x){
			assert(lo <= x && x <= hi);
			calls++;
			return val[x-lo];
		};
		ll got = ternSearch(lo, hi, f);
		assert(got == best);
		int bound = 0;
		for(int size = 1; size < n + 1; size *= 2)
			bound += 2;
		assert(calls <= bound);
	}
	// hi - lo overflows
	auto id = [](double x){ return x; };
	assert(goldenSearch(-1e308, 1e308, id) < -9.9e307);
}
