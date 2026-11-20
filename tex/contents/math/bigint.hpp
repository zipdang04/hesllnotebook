#pragma once
#include "ntt.hpp"

// Signed big integer. + - compare O(n); * O(n log n) by NTT
// on decimal digits (schoolbook when one side has <= 32
// limbs); / % schoolbook O(nm).
struct Bigint{
	static const int BASE = 1'000'000'000;
	static constexpr int P10[9] = {1, 10, 100, 1'000, 10'000,
		100'000, 1'000'000, 10'000'000, 100'000'000};
	bool neg = false;
	vector<int> d;
	Bigint(ll x = 0): neg(x < 0){
		for(; x; x /= BASE) d.push_back(abs((int)(x % BASE)));
	}
	Bigint(const string &s): neg(s[0] == '-'){
		for(int i = sz(s); i > neg; i -= 9){
			int j = max((int)neg, i - 9);
			d.push_back(stoi(s.substr(j, i - j)));
		}
		trim();
	}
	string str() const{
		if(d.empty()) return "0";
		string s = (neg ? "-" : "") + to_string(d.back());
		REV(i, sz(d) - 2, 0) // keep 9 digits of BASE + d[i]
			s += to_string(BASE + d[i]).substr(1);
		return s;
	}
	void trim(){
		while(!d.empty() && !d.back()) d.pop_back();
		if(d.empty()) neg = false;
	}
	int cmpAbs(const Bigint &b) const{
		if(sz(d) != sz(b.d)) return sz(d) < sz(b.d) ? -1 : 1;
		REV(i, sz(d) - 1, 0) if(d[i] != b.d[i])
			return d[i] < b.d[i] ? -1 : 1;
		return 0;
	}
	int cmp(const Bigint &b) const{
		if(neg != b.neg) return neg ? -1 : 1;
		return neg ? -cmpAbs(b) : cmpAbs(b);
	}
	void addAbs(const Bigint &b){
		int carry = 0; d.resize(max(sz(d), sz(b.d)) + 1);
		FOR(i, 0, sz(d) - 1){
			d[i] += carry + (i < sz(b.d) ? b.d[i] : 0);
			carry = d[i] >= BASE;
			d[i] -= carry * BASE;
		}
		trim();
	}
	void subAbs(const Bigint &b){
		int borrow = 0;
		FOR(i, 0, sz(d) - 1){
			d[i] -= borrow + (i < sz(b.d) ? b.d[i] : 0);
			borrow = d[i] < 0;
			d[i] += borrow * BASE;
		}
		trim();
	}
	Bigint operator-() const{
		Bigint a = *this; a.neg = !neg && !d.empty();
		return a;
	}
	Bigint &operator+=(const Bigint &b){
		if(neg == b.neg) addAbs(b);
		else if(cmpAbs(b) >= 0) subAbs(b);
		else{
			Bigint a = b;
			a.subAbs(*this); *this = a;
		}
		return *this;
	}
	Bigint &operator-=(const Bigint &b){ return *this += -b; }
	Bigint operator*(const Bigint &b) const{
		int n = sz(d), m = sz(b.d);
		if(!n || !m) return 0;
		Bigint a;
		const int NAIVE = 32;
		if(min(n, m) <= NAIVE){
			vector<ll> c(n + m);
			FOR(i, 0, n - 1){
				ll carry = 0;
				FOR(j, 0, m - 1){
					carry += c[i+j] + (ll)d[i] * b.d[j];
					c[i+j] = carry % BASE, carry /= BASE;
				}
				c[i+m] += carry;
			}
			a.d.assign(all(c));
		}else{
			vector<MintN> x(9 * n), y(9 * m);
			FOR(i, 0, sz(x) - 1) x[i] = d[i/9] / P10[i%9] % 10;
			FOR(i, 0, sz(y) - 1) y[i] = b.d[i/9] / P10[i%9] % 10;
			vector<MintN> z = convNtt(x, y);
			ll carry = 0; a.d.resize(n + m);
			FOR(i, 0, 9 * (n + m) - 1){
				if(i < sz(z)) carry += z[i].v;
				a.d[i/9] += carry % 10 * P10[i%9];
				carry /= 10;
			}
		}
		a.neg = neg != b.neg; a.trim();
		return a;
	}
	pair<Bigint, Bigint> divMod(const Bigint &b) const{
		int m = sz(b.d), norm = BASE / (b.d.back() + 1);
		if(m == 1){
			Bigint q = *this; ll r = 0, c = b.d[0]; // O(n)
			REV(i, sz(d) - 1, 0)
				r = r * BASE + d[i], q.d[i] = r / c, r %= c;
			q.neg = neg != b.neg; q.trim();
			return {q, neg ? -r : r};
		}
		Bigint x = *this * norm, y = b * norm, q, r;
		x.neg = y.neg = false;
		q.d.resize(sz(x.d));
		REV(i, sz(x.d) - 1, 0){
			r.d.insert(r.d.begin(), x.d[i]); r.trim();
			ll hi = sz(r.d) > m ? r.d[m] : 0;
			ll lo = sz(r.d) >= m ? r.d[m-1] : 0;
			ll e = (hi * BASE + lo) / y.d.back();
			minimize(e, BASE - 1);
			r -= y * e;
			while(r.neg) r += y, e--;
			q.d[i] = e;
		}
		q.neg = neg != b.neg; q.trim();
		return {q, *this - q * b};
	}
	friend Bigint operator+(Bigint a, const Bigint &b){
		return a += b;
	}
	friend Bigint operator-(Bigint a, const Bigint &b){
		return a -= b;
	}
	friend Bigint operator/(const Bigint &a, const Bigint &b){
		return a.divMod(b).fi;
	}
	friend Bigint operator%(const Bigint &a, const Bigint &b){
		return a.divMod(b).se;
	}
	friend bool operator<(const Bigint &a, const Bigint &b){
		return a.cmp(b) < 0;
	}
	friend bool operator>(const Bigint &a, const Bigint &b){
		return a.cmp(b) > 0;
	}
	friend bool operator<=(const Bigint &a, const Bigint &b){
		return a.cmp(b) <= 0;
	}
	friend bool operator>=(const Bigint &a, const Bigint &b){
		return a.cmp(b) >= 0;
	}
	friend bool operator==(const Bigint &a, const Bigint &b){
		return a.cmp(b) == 0;
	}
	friend bool operator!=(const Bigint &a, const Bigint &b){
		return a.cmp(b) != 0;
	}
	friend istream &operator>>(istream &in, Bigint &a){
		string s;
		if(in >> s) a = Bigint(s);
		return in;
	}
	friend ostream &operator<<(ostream &out, const Bigint &a){
		return out << a.str();
	}
};
