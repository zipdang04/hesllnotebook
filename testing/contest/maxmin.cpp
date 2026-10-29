#include "../../tex/contents/contest/template.hpp"
#include <cassert>
#include <random>

int main() {
	// maximize tests
	int a = 3;
	bool r1 = maximize(a, 5);
	assert(r1 == true && a == 5);
	bool r2 = maximize(a, 2);
	assert(r2 == false && a == 5);

	// minimize tests
	int b = 5;
	bool r3 = minimize(b, 2);
	assert(r3 == true && b == 2);
	bool r4 = minimize(b, 3);
	assert(r4 == false && b == 2);

	// non-narrowing type pairs
	ll la = 10;
	int ia = 20;
	assert(maximize(la, ia) == true && la == 20);

	double da = 1.5;
	int ib = 2;
	assert(maximize(da, ib) == true && da == 2.0);

	// Random loop against std::max / std::min
	mt19937_64 rng(1337);
	uniform_int_distribution<int> dist(-1000000, 1000000);
	FOR(int, iter, 1, 100000) {
		int cur = dist(rng);
		int val = dist(rng);

		int expected_max = max(cur, val);
		int test_cur = cur;
		bool changed = maximize(test_cur, val);
		assert(test_cur == expected_max);
		assert(changed == (val > cur));

		int expected_min = min(cur, val);
		test_cur = cur;
		changed = minimize(test_cur, val);
		assert(test_cur == expected_min);
		assert(changed == (val < cur));
	}

	return 0;
}
