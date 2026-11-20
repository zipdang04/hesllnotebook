#pragma once
#include "../contest/template.hpp"

// Arithmetic mod a prime MOD < 2^30 (template argument).
// + - * in O(1); / pow inv in O(log MOD) by Fermat.
template<int MOD> struct Mint{
	int v;
	Mint(ll a = 0): v((int)(a % MOD)){ if(v < 0) v += MOD; }
	Mint &operator+=(Mint b){
		v += b.v; if(v >= MOD) v -= MOD;
		return *this;
	}
	Mint &operator-=(Mint b){
		v -= b.v; if(v < 0) v += MOD;
		return *this;
	}
	Mint &operator*=(Mint b){
		v = (ll)v * b.v % MOD;
		return *this;
	}
	Mint &operator/=(Mint b){ return *this *= b.inv(); }
	Mint pow(ll n) const{
		Mint res = 1, a = *this;
		for(; n; n >>= 1, a *= a) if(n & 1) res *= a;
		return res;
	}
	Mint inv() const{ return pow(MOD - 2); }
	Mint operator-() const{ return Mint(0) - *this; }
	friend Mint operator+(Mint a, Mint b){ return a += b; }
	friend Mint operator-(Mint a, Mint b){ return a -= b; }
	friend Mint operator*(Mint a, Mint b){ return a *= b; }
	friend Mint operator/(Mint a, Mint b){ return a /= b; }
	friend bool operator==(Mint a, Mint b){
		return a.v == b.v;
	}
	friend bool operator!=(Mint a, Mint b){
		return a.v != b.v;
	}
	friend istream &operator>>(istream &in, Mint &a){
		ll x;
		if(in >> x) a = Mint(x);
		return in;
	}
	friend ostream &operator<<(ostream &out, Mint a){
		return out << a.v;
	}
};
typedef Mint<998'244'353> MintN;
