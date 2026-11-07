#pragma once
#include "../contest/template.hpp"
typedef unsigned long long ull;
struct modint{
	using M = modint;
	inline static ll MOD = 998244353;
	int v;
	// Barrett: x % MOD for 0 <= x < 2^63, no division
	static ll mod(ll x){
		ll q = (__uint128_t(-1ULL / MOD) * x) >> 64;
		ll r = x - q * MOD;
		return r - (r >= MOD) * MOD;
	}
	modint(ll a = 0): v(mod(a < 0 ? a % MOD + MOD : a)){}
	M &operator+=(M b){
		v += b.v - MOD; v += (v < 0) * MOD;
		return *this;
	}
	M &operator-=(M b){
		v -= b.v; v += (v < 0) * MOD;
		return *this;
	}
	M &operator*=(M b){
		v = mod((ll)v * b.v); return *this;
	}
	M &operator/=(M b){ return *this *= b.inv(); }
	friend M operator+(M a, M b){ return a += b; }
	friend M operator-(M a, M b){ return a -= b; }
	friend M operator*(M a, M b){ return a *= b; }
	friend M operator/(M a, M b){ return a /= b; }
	friend M operator^(M a, ll e){
		M r = 1;
		for (; e; e /= 2){
			if (e & 1) r *= a;
			a *= a;
		}
		return r;
	}
	M inv() const{ return *this ^ (MOD - 2); }
	M operator-() const{ return M() - *this; }
	bool operator==(M b) const{ return v == b.v; }
	friend ostream &operator<<(ostream &s, M a){
		return s << a.v;
	}
	friend istream &operator>>(istream &s, M &a){
		ll x; s >> x; a = x; return s;
	}
};
