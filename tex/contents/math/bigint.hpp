#pragma once
#include "../contest/template.hpp"
#include "ntt.hpp"

// sign + base-10 digits, little-endian, no leading zeros; zero is never negative
struct bigint{
	using V = vector<int>; using R = const bigint &;
	bool neg = 0; V d;
	bigint(ll x = 0): neg(x < 0){
		ull u = neg ? 0 - ull(x) : x;
		do d.push_back(u % 10), u /= 10; while (u);
	}
	bigint(const string &s): neg(s[0] == '-'){ // [-]digits, leading zeros ok
		REV(int, i, (int)s.size() - 1, neg) d.push_back(s[i] - '0');
		trim();
	}
	bigint &trim(){
		while (d.size() > 1 && !d.back()) d.pop_back();
		if (d.size() == 1 && !d[0]) neg = 0;
		return *this;
	}
	static bool absLess(const V &a, const V &b){
		if (a.size() != b.size()) return a.size() < b.size();
		return lexicographical_compare(a.rbegin(), a.rend(), b.rbegin(), b.rend());
	}
	static V addAbs(V a, const V &b, int s){ // |a| + s|b|; s = -1 needs |a| >= |b|
		a.resize(max(a.size(), b.size()) + 1); int c = 0;
		FOR(int, i, 0, (int)a.size() - 1){
			a[i] += c + s * (i < (int)b.size() ? b[i] : 0);
			c = a[i] < 0 ? -1 : a[i] > 9; a[i] -= 10 * c;
		}
		return a;
	}
	bigint operator-() const{ bigint r = *this; r.neg ^= 1; return r.trim(); }
	bigint &operator+=(R o){
		if (neg == o.neg) d = addAbs(d, o.d, 1);
		else if (!absLess(d, o.d)) d = addAbs(d, o.d, -1);
		else d = addAbs(o.d, d, -1), neg = o.neg;
		return trim();
	}
	bigint &operator-=(R o){ return *this += -o; }
	friend bigint operator+(bigint a, R b){ return a += b; }
	friend bigint operator-(bigint a, R b){ return a -= b; }
	friend bigint operator*(bigint a, bigint b){ // NTT: |a| + |b| - 1 <= 2^22 digits
		int n = a.d.size(), m = b.d.size(); bigint r; r.d.assign(n + m, 0);
		if (min(n, m) < 64) FOR(int, i, 0, n - 1) FOR(int, j, 0, m - 1) r.d[i + j] += a.d[i] * b.d[j];
		else{
			vector<modint> p = FFT::multiply(a.d, b.d); // coefficients <= 81 min(n, m) < MOD
			FOR(int, i, 0, n + m - 2) r.d[i] = p[i].v;
		}
		int c = 0;
		for (int &x: r.d) x += c, c = x / 10, x %= 10;
		r.neg = a.neg ^ b.neg; return r.trim();
	}
	ll divBy(ll b){ // *this /= b, returns old *this % b; 0 < |b| <= 1e17
		ll r = 0, m = abs(b), s = neg ? -1 : 1;
		REV(int, i, (int)d.size() - 1, 0) r = r * 10 + d[i], d[i] = r / m, r %= m;
		neg ^= b < 0; trim(); return s * r;
	}
	friend bigint operator/(bigint a, ll b){ a.divBy(b); return a; }
	friend ll operator%(bigint a, ll b){ return a.divBy(b); }
	friend pair<bigint, bigint> divMod(R a, R b){ // b != 0; O(|a| |b|)
		bigint q, r; q.d.assign(a.d.size(), 0);
		REV(int, i, (int)a.d.size() - 1, 0){
			r.d.insert(r.d.begin(), a.d[i]); r.trim();
			while (!absLess(r.d, b.d)) r.d = addAbs(move(r.d), b.d, -1), r.trim(), q.d[i]++;
		}
		q.neg = a.neg ^ b.neg; r.neg = a.neg;
		return {q.trim(), r.trim()};
	}
	friend bigint operator/(R a, R b){ return divMod(a, b).fi; } // C++: truncates toward 0
	friend bigint operator%(R a, R b){ return divMod(a, b).se; } // sign of a
	friend bool operator<(R a, R b){
		return a.neg != b.neg ? a.neg : a.neg ? absLess(b.d, a.d) : absLess(a.d, b.d);
	}
	friend bool operator>(R a, R b){ return b < a; }
	friend bool operator<=(R a, R b){ return !(b < a); }
	friend bool operator>=(R a, R b){ return !(a < b); }
	friend bool operator==(R a, R b){ return a.neg == b.neg && a.d == b.d; }
	friend bool operator!=(R a, R b){ return !(a == b); }
	friend istream &operator>>(istream &is, bigint &a){ string s; is >> s; a = bigint(s); return is; }
	friend ostream &operator<<(ostream &os, R a){
		string s(a.neg, '-');
		REV(int, i, (int)a.d.size() - 1, 0) s += char('0' + a.d[i]);
		return os << s;
	}
};
