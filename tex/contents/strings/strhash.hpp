#pragma once
#include "../contest/template.hpp"

// Polynomial hash mod 2^61 - 1, random base shared by all
// objects in a run. Ranges 1-based closed; build O(n),
// get / getRev O(1).
struct StrHash{
	static constexpr ll M = (1LL << 61) - 1;
	vector<ll> pw, f, b;
	StrHash(const string &s): pw(sz(s) + 1), f(sz(s) + 1),
			b(sz(s) + 2){
		int n = sz(s);
		static ll B = 256 + mt19937_64(CURRENT_TIMESTAMP())()
			% (M / 2);
		pw[0] = 1;
		FOR(i, 1, n){
			pw[i] = mul(pw[i - 1], B);
			f[i] = (mul(f[i - 1], B) + s[i - 1]) % M;
		}
		REV(i, n, 1) b[i] = (mul(b[i + 1], B) + s[i - 1]) % M;
	}
	static ll mul(ll a, ll c){
		// __int128: a * c reaches 2^122; fold it mod 2^61 - 1
		__int128 x = (__int128)a * c;
		ll r = (ll)(x & M) + (ll)(x >> 61);
		return r >= M ? r - M : r;
	}
	ll get(int l, int r){
		ll x = f[r] - mul(f[l - 1], pw[r - l + 1]);
		return x < 0 ? x + M : x;
	}
	ll getRev(int l, int r){
		ll x = b[l] - mul(b[r + 1], pw[r - l + 1]);
		return x < 0 ? x + M : x;
	}
};
