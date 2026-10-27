#include "../../tex/contents/math/bigint.hpp"

typedef __int128 i128;
string str(i128 x){
	if (x == 0) return "0";
	unsigned __int128 u = x < 0 ? -(unsigned __int128)x : x; string s;
	while (u) s += char('0' + u % 10), u /= 10;
	if (x < 0) s += '-';
	reverse(s.begin(), s.end()); return s;
}
string str(const bigint &b){ ostringstream o; o << b; return o.str(); }
void fail(const string &what, const bigint &a, const bigint &d){
	cout << "FAIL " << what << "\na = " << a << "\nd = " << d << endl; exit(1);
}
mt19937_64 rng(7);
bigint randBig(int len){ // len digits, random sign
	string s = rng() % 2 ? "-" : "";
	s += char('1' + rng() % 9);
	EFOR(int, i, 1, len) s += char('0' + rng() % 10);
	return bigint(s);
}
// a vs __int128 (|a|, |d| < 2^63)
void small(ll a, ll d){
	bigint A(a), D(d);
	if (str(A / D) != str((i128)a / d) || str(A % D) != str((i128)a % d)) fail("bigint/bigint vs i128", A, D);
	if (-(ll)1e17 <= d && d <= (ll)1e17 && (str(A / d) != str((i128)a / d) || to_string(A % d) != str((i128)a % d)))
		fail("bigint/ll vs i128", A, D);
}
// identity check for big values
void big(const bigint &a, const bigint &d){
	auto [q, r] = divMod(a, d);
	if (q * d + r != a) fail("a != q*d + r", a, d);
	bigint absR = r < 0 ? -r : r, absD = d < 0 ? -d : d;
	if (!(absR < absD)) fail("|r| >= |d|", a, d);
	if (r != 0 && (r < 0) != (a < 0)) fail("sign of r", a, d);
	if (q != 0 && (q < 0) != ((a < 0) != (d < 0))) fail("sign of q", a, d);
	if (a / d != q || a % d != r) fail("operator/ % vs divMod", a, d);
	if (absD <= bigint((ll)1e17)){
		ll dl = stoll(str(d));
		if (a / dl != q || bigint(a % dl) != r) fail("bigint/ll vs bigint/bigint", a, d);
	}
}

int main(){
	vector<ll> corner = {0, 1, -1, 2, -2, 3, -3, 9, -9, 10, -10, 11, -11, 999999999, -999999999,
		1000000000, -1000000000, (ll)1e17, -(ll)1e17, (ll)1e17 - 1, -(ll)1e17 + 1, (ll)1e17 + 1,
		(ll)1e18 - 1, -(ll)1e18 + 1, (ll)1e18, -(ll)1e18, LLONG_MAX, LLONG_MIN + 1};
	for (ll a: corner) for (ll d: corner) if (d) small(a, d);
	for (ll d: corner) if (d) small(LLONG_MIN, d);
	uniform_int_distribution<ll> e18(-(ll)1e18 + 1, (ll)1e18 - 1), e3(-1000, 1000);
	EFOR(int, it, 0, 200000){
		ll a = e18(rng), d = it % 3 == 0 ? e3(rng) : e18(rng) >> (rng() % 60);
		if (it % 5 == 0) d = it % 2 ? 1 : -1;
		if (!d) d = 7;
		small(a, d);
	}
	// big: zero dividend, divisor +-1, a = +-d, |d| > |a|, random lengths up to 3000 digits
	EFOR(int, it, 0, 300){
		bigint a = randBig(1 + rng() % 3000), d = randBig(1 + rng() % 60);
		big(bigint(0), d); big(a, bigint(1)); big(a, bigint(-1));
		big(a, a); big(a, -a); big(d, a * a + 1); big(a, d);
		big(a, randBig(1 + rng() % 18)); // fits the ll overload
		big(a * d + randBig(1 + rng() % 5), d);
	}
	EFOR(int, it, 0, 10) big(randBig(2000 + rng() % 1000), randBig(1 + rng() % 2000));
	puts("OK");
}
