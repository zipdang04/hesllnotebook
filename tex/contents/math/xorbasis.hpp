#pragma once
#include "../contest/template.hpp"

// Subset XOR basis, bits 0..59. Each operation is O(60).
struct XorBasis{
	vector<ll> basis; // decreasing highest set bit
	ll reduce(ll x){
		for(ll b: basis) x = min(x, x ^ b);
		return x;
	}
	bool insert(ll x){
		x = reduce(x); if(!x) return false;
		basis.insert(lower_bound(all(basis), x, greater<ll>()),
			x);
		return true;
	}
	bool representable(ll x){ return reduce(x) == 0; }
	ll kth(ll k){
		ll x = 0; int i = sz(basis);
		for(ll b: basis){
			ll y = x ^ b;
			x = (k >> --i & 1) ? max(x, y) : min(x, y);
		}
		return x;
	}
	ll maxXor(){ return kth((1LL << sz(basis)) - 1); }
};
