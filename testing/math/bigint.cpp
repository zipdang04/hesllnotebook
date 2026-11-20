#include "../../tex/contents/math/bigint.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	for(string s: {"-0", "000"}){
		Bigint a(s);
		assert(a.d.empty() && !a.neg && a.str() == "0");
	}
	assert(Bigint(LLONG_MIN).str() == "-9223372036854775808");
	assert(Bigint(-1).str() == "-1");
	assert((Bigint(-7) / 2).str() == "-3");
	assert((Bigint(-7) % 2).str() == "-1");
	assert((Bigint(7) / -2).str() == "-3");
	assert((Bigint(7) % -2).str() == "1");
	Bigint carry = Bigint(999999999) + 1;
	assert(carry.str() == "1000000000" && sz(carry.d) == 2);
	assert(!(Bigint(-5) * 0).neg && !(-Bigint(0)).neg);
	for(auto [a, b]: vector<pll>{{-5, 3}, {3, 3}, {-2, -7}}){
		Bigint x(a), y(b);
		assert((x < y) == (a < b));
		assert((x > y) == (a > b));
		assert((x <= y) == (a <= b));
		assert((x >= y) == (a >= b));
		assert((x == y) == (a == b));
		assert((x != y) == (a != b));
		assert(x.cmp(y) == (a > b) - (a < b));
		assert(x.cmpAbs(y)
			== (abs(a) > abs(b)) - (abs(a) < abs(b)));
	}
	Bigint x("1" + string(360, '0')), y = x - 1;
	assert((x * x).str() == "1" + string(720, '0'));
	assert(y * y == x * x - x * 2 + 1);
	Bigint a = 5; a = Bigint("-12"); a += 20;
	assert(a.str() == "8");
	a.addAbs(-2); assert(a == 10);
	a.subAbs(-10); assert(a == 0 && !a.neg);
	a.d = {0, 0}; a.neg = true; a.trim();
	assert(a.d.empty() && !a.neg);
	a = Bigint("999999999999999999"); a += a;
	assert(a.str() == "1999999999999999998");
	a -= a; assert(a == 0 && !a.neg);
	a = Bigint("1000000000000000000000000000");
	Bigint b("1000000000000000001");
	auto [q, r] = a.div(b);
	assert(q == 999999999 && r == 999999999000000001LL);
	assert(q * b + r == a && r.cmpAbs(b) < 0);
	assert(5 < a && a > 5 && 0 == Bigint(0));
	assert(1 + Bigint(-1) == 0);
	Mint<1000000007> mod = 3;
	assert((mod * mod).v == 9);
	int t; cin >> t;
	FOR(i, 1, t){
		Bigint u, v; cin >> u >> v;
		cout << u * v << '\n';
	}
}
