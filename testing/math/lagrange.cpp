#include "../../tex/contents/math/lagrange.hpp"
#include <cassert>

int main(){
	vector<MintN> y{0, 1, 5, 14};
	assert(interpolateAt(y, 10) == 385);
	assert(interpolateAt(y, 2) == 5);
	assert(interpolateAt(y, -3) == -5);
	MintN t = LLONG_MIN;
	assert(interpolateAt(y, LLONG_MIN)
		== t * (t + 1) * (2 * t + 1) / 6);
	assert(interpolateAt(vector<MintN>{7},
		123456789012LL) == 7);
	assert(interpolate(vector<MintN>{3}, vector<MintN>{7})
		== vector<MintN>{7});
	vector<MintN> empty;
	assert(interpolate(empty, empty).empty());
	vector<Mint<7>> small(7);
	FOR(i, 0, 6) small[i] = Mint<7>(i).pow(6) + 1;
	assert(interpolateAt(small, 10)
		== Mint<7>(10).pow(6) + 1);
	vector<MintN> x{1, 2, 4}, squares{1, 4, 16};
	vector<MintN> expected{0, 0, 1};
	assert(interpolate(x, squares) == expected);
}
