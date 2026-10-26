#pragma once
#include "../contest/template.hpp"

struct HashString{ // two bases mod 2^61-1; hash = pair<ull, ull>
	typedef unsigned long long ull; typedef pair<ull, ull> H;
	static constexpr ull M = (1ull << 61) - 1;
	static ull mul(ull a, ull b){ __uint128_t c = (__uint128_t)a * b; ull r = (c >> 61) + (c & M); return r >= M ? r - M : r; }
	static ull sub(ull a, ull b){ return a >= b ? a - b : a + M - b; }
	static H cut(H a, H c, H q){ return {sub(a.fi, mul(c.fi, q.fi)), sub(a.se, mul(c.se, q.se))}; }
	vector<H> p, f, b; // powers; prefix hashes; suffix hashes of the reversed string
	HashString(const string &s){
		mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
		H B(rng() % (M - 256) + 256, rng() % (M - 256) + 256);
		int n = s.size(); p.assign(n + 2, H(1, 1)); f.assign(n + 2, H(0, 0)); b = f;
		FOR(int, i, 1, n) p[i] = {mul(p[i-1].fi, B.fi), mul(p[i-1].se, B.se)};
		FOR(int, i, 1, n) f[i] = {(mul(f[i-1].fi, B.fi) + (uint8_t)s[i-1]) % M, (mul(f[i-1].se, B.se) + (uint8_t)s[i-1]) % M};
		REV(int, i, n, 1) b[i] = {(mul(b[i+1].fi, B.fi) + (uint8_t)s[i-1]) % M, (mul(b[i+1].se, B.se) + (uint8_t)s[i-1]) % M};
	}
	H getF(int l, int r){ return cut(f[r], f[l-1], p[r-l+1]); } // s[l..r]
	H getB(int l, int r){ return cut(b[l], b[r+1], p[r-l+1]); } // reverse of s[l..r]
};
