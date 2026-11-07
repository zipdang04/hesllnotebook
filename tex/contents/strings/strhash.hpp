#pragma once
#include "../contest/template.hpp"
struct HashString{
	typedef unsigned long long ull;
	static constexpr ull M = (1ull << 61) - 1;
	static ull mul(ull a, ull b){
		__uint128_t c = (__uint128_t)a * b;
		ull r = (c >> 61) + (c & M);
		return r >= M ? r - M : r;
	}
	vector<ull> p, f, b;
	HashString(const string &s){
		static mt19937_64 rng(CURRENT_TIMESTAMP);
		static ull B = rng() % (M - 256) + 256;
		int n = s.size();
		p.assign(n + 1, 1); f.assign(n + 1, 0); b = f;
		FOR(int, i, 0, n - 1){
			p[i + 1] = mul(p[i], B);
			f[i + 1] = (mul(f[i], B) + (uint8_t)s[i]) % M;
		}
		REV(int, i, n - 1, 0)
			b[i] = (mul(b[i + 1], B) + (uint8_t)s[i]) % M;
	}
	ull cut(ull x, ull y, int len){
		return (x + M - mul(y, p[len])) % M;
	}
	ull getF(int l, int r){ return cut(f[r], f[l], r - l); }
	ull getB(int l, int r){ return cut(b[l], b[r], r - l); }
};
