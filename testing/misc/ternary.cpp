#include "../../tex/contents/misc/ternary.hpp"
#include <cassert>
int main(){
	assert(ternSearch(1, 100, [](ll x){
		return (x - 37) * (x - 37);
	}) == 37);
	assert(ternSearch(-100, -1, [](ll x){
		return abs(x + 5);
	}) == -5);
	assert(ternSearch(3, 10, [](ll x){ return x; }) == 3);
	assert(ternSearch(3, 10, [](ll x){ return -x; }) == 10);
	assert(ternSearch(-7, -7, [](ll x){ return x; }) == -7);
	assert(ternSearch(0, 20, [](ll x){
		return x < 4 ? 3 * (4 - x) : max(0LL, x - 9);
	}) == 4);
	assert(ternSearch(-4000000000000000000LL,
		4000000000000000000LL, [](ll x){
			return abs(x - 5);
		}) == 5);
	for(double c: {-3.7, 0.0, 2.5, 9.99}){
		int calls = 0;
		double x = goldenSearch(-10, 20, [&](double v){
			calls++;
			return (v - c) * (v - c);
		});
		assert(abs(x - c) <= 1e-9);
		assert(calls == 102);
		x = goldenSearch(-10, 20, [&](double v){
			return abs(v - c);
		});
		assert(abs(x - c) <= 1e-9);
	}
	double x = goldenSearch(-1000, 1000, [](double v){
		return (v - 2.5) * (v - 2.5) + 1;
	});
	assert(abs(x - 2.5) <= 1e-7);
	x = goldenSearch(0, 3, [](double v){
		return cosh(v - 1.3);
	});
	assert(abs(x - 1.3) <= 1e-7);
	x = goldenSearch(0, 1, [](double v){ return v; });
	assert(abs(x) <= 1e-9);
	x = goldenSearch(0, 1, [](double v){ return -v; });
	assert(abs(x - 1) <= 1e-9);
	assert(goldenSearch(2.5, 2.5, [](double v){
		return v * v;
	}) == 2.5);
}
