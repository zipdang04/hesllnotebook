#include "../../tex/contents/math/bigint.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include <cassert>
#include <algorithm>

using namespace std;
typedef __int128_t i128;

static string to_str(i128 x) {
	if (x == 0) return "0";
	string s = "";
	bool neg = false;
	unsigned __int128 u;
	if (x < 0) {
		neg = true;
		u = 0ULL - (unsigned __int128)x;
	} else {
		u = (unsigned __int128)x;
	}
	while (u > 0) {
		s += char('0' + (u % 10));
		u /= 10;
	}
	if (neg) s += '-';
	reverse(s.begin(), s.end());
	return s;
}

static string b_to_str(const bigint &b) {
	ostringstream oss;
	oss << b;
	return oss.str();
}

int main() {
	mt19937_64 rng(42);
	uniform_int_distribution<ll> dist_full(numeric_limits<ll>::min(), numeric_limits<ll>::max());
	uniform_int_distribution<ll> dist_1e18(-1000000000000000000LL, 1000000000000000000LL);
	uniform_int_distribution<ll> dist_small(-1000, 1000);
	uniform_int_distribution<ll> dist_tiny(-10, 10);

	vector<ll> corner = {
		0, 1, -1, 2, -2, 9, -9, 10, -10,
		99, -99, 100, -100,
		1000000000LL, -1000000000LL,
		1000000000000000000LL, -1000000000000000000LL,
		numeric_limits<ll>::max(), numeric_limits<ll>::min(),
		numeric_limits<ll>::max() - 1, numeric_limits<ll>::min() + 1
	};

	auto check_pair = [&](ll a, ll b) {
		bigint ba(a), bb(b);
		string stra = to_str(a), strb = to_str(b);
		bigint sa(stra), sb(strb);

		// printing and string ctor
		assert(b_to_str(ba) == stra);
		assert(b_to_str(bb) == strb);
		assert(b_to_str(sa) == stra);
		assert(b_to_str(sb) == strb);
		assert(ba == sa);
		assert(bb == sb);

		// comparisons
		assert((ba == bb) == (a == b));
		assert((ba != bb) == (a != b));
		assert((ba < bb) == (a < b));
		assert((ba <= bb) == (a <= b));
		assert((ba > bb) == (a > b));
		assert((ba >= bb) == (a >= b));

		// unary minus
		bigint neg_ba = -ba;
		assert(b_to_str(neg_ba) == to_str(-(i128)a));

		// addition
		bigint sum = ba + bb;
		assert(b_to_str(sum) == to_str((i128)a + b));
		bigint sum_eq = ba; sum_eq += bb;
		assert(b_to_str(sum_eq) == to_str((i128)a + b));

		// subtraction
		bigint diff = ba - bb;
		assert(b_to_str(diff) == to_str((i128)a - b));
		bigint diff_eq = ba; diff_eq -= bb;
		assert(b_to_str(diff_eq) == to_str((i128)a - b));
	};

	// Test all pairs of corner cases
	for (ll a : corner) {
		for (ll b : corner) {
			check_pair(a, b);
		}
	}

	// 20000 random cases
	const int N = 20000;
	for (int i = 0; i < N; i++) {
		ll a, b;
		int type = i % 4;
		if (type == 0) {
			a = dist_full(rng);
			b = dist_full(rng);
		} else if (type == 1) {
			a = dist_1e18(rng);
			b = dist_1e18(rng);
		} else if (type == 2) {
			a = dist_small(rng);
			b = dist_small(rng);
		} else {
			a = dist_tiny(rng);
			b = dist_tiny(rng);
		}
		if (i % 7 == 0) b = a;
		if (i % 11 == 0 && a != numeric_limits<ll>::min()) b = -a;
		check_pair(a, b);
	}

	return 0;
}
