#include "../../tex/contents/math/bigint.hpp"
#include <cassert>
#include <climits>
#include <random>

mt19937_64 rng(1234567);

void valid(const Bigint &a){
	assert(!a.d.empty() || !a.neg);
	assert(a.d.empty() || a.d.back() != 0);
	for(int x: a.d) assert(0 <= x && x < Bigint::BASE);
}

string decimal(__int128 x){
	if(!x) return "0";
	bool neg = x < 0;
	if(neg) x = -x;
	string s;
	while(x){
		s += char('0' + x % 10);
		x /= 10;
	}
	if(neg) s += '-';
	reverse(all(s));
	return s;
}

void check(const Bigint &a, __int128 x){
	valid(a);
	assert(a.str() == decimal(x));
}

__int128 small(){
	__int128 x = 0;
	int n = rng() % 37;
	FOR(i, 0, n - 1) x = x * 10 + rng() % 10;
	return rng() % 2 ? x : -x;
}

Bigint large(int n, int top = 0){
	Bigint a;
	FOR(i, 0, n - 1)
		a.d.push_back(rng() % Bigint::BASE);
	a.d.back() = top ? top : 1 + rng() % (Bigint::BASE - 1);
	a.neg = rng() % 2;
	valid(a);
	return a;
}

Bigint product(Bigint a, Bigint b){
	bool neg = a.neg != b.neg;
	a.neg = b.neg = false;
	Bigint sum = 0;
	FOR(j, 0, sz(b.d) - 1){
		Bigint part = a * Bigint(b.d[j]);
		valid(part);
		if(!part.d.empty())
			part.d.insert(part.d.begin(), j, 0);
		valid(part);
		sum += part;
		valid(sum);
	}
	if(neg) sum = -sum;
	valid(sum);
	return sum;
}

void division(const Bigint &a, const Bigint &b){
	auto qr = a.divMod(b);
	valid(qr.fi);
	valid(qr.se);
	Bigint p = qr.fi * b;
	valid(p);
	Bigint s = p + qr.se;
	valid(s);
	assert(s == a);
	assert(qr.se.cmpAbs(b) < 0);
	assert(qr.se.d.empty() || qr.se.neg == a.neg);
}

int main(){
	vector<ll> edges = {LLONG_MIN, LLONG_MAX, 0,
		-999999999, 999999999, -1000000000, 1000000000};
	for(ll x: edges) check(Bigint(x), x);
	uniform_int_distribution<ll> gen(LLONG_MIN, LLONG_MAX);
	FOR(i, 1, 10000){
		ll x = gen(rng);
		check(Bigint(x), x);
		__int128 u = small(), v = small();
		Bigint a(decimal(u)), b(decimal(v));
		check(a, u);
		check(b, v);
		check(a + b, u + v);
		check(a - b, u - v);
		check(-a, -u);
		assert((a < b) == (u < v));
		assert((a > b) == (u > v));
		assert((a <= b) == (u <= v));
		assert((a >= b) == (u >= v));
		assert((a == b) == (u == v));
		assert((a != b) == (u != v));
		assert(a.cmp(b) == (u > v) - (u < v));
		if(v){
			check(a / b, u / v);
			check(a % b, u % v);
			auto qr = a.divMod(b);
			check(qr.fi, u / v);
			check(qr.se, u % v);
		}
		__int128 limit = 1000000000000000000LL;
		if(-limit < u && u < limit &&
				-limit < v && v < limit)
			check(a * b, u * v);
	}
	FOR(i, 0, 299){
		Bigint a = large(1 + rng() % 120);
		int top = i % 3 == 0 ? 1 :
			(i % 3 == 1 ? Bigint::BASE - 1 : 0);
		Bigint b = large(1 + rng() % 60, top);
		division(a, b);
		Bigint multiple = b * large(1 + rng() % 60);
		valid(multiple);
		division(multiple, b);
	}
	FOR(n, 1, 80){
		for(int m: {n, 32, 33}){
			Bigint a = large(n), b = large(m);
			Bigint got = a * b;
			valid(got);
			assert(got == product(a, b));
		}
	}
}
