#include "../../tex/contents/strings/strhash.hpp"
#include <cassert>

void compare(const string &s, StrHash &h,
		const string &t, StrHash &ht){
	int n = sz(s), m = sz(t);
	FOR(i, 0, n) FOR(len, 0, n - i){
		string part = s.substr(i, len);
		ll x = h.get(i + 1, i + len);
		assert(0 <= x && x < StrHash::M);
		FOR(j, 0, m) FOR(k, 0, m - j){
			ll y = ht.get(j + 1, j + k);
			assert((x == y) == (part == t.substr(j, k)));
		}
	}
}

int radius(StrHash &h, int l, int r, int limit){
	int lo = 0, hi = limit;
	while(lo < hi){
		int mid = (lo + hi + 1) / 2;
		if(h.get(l - mid, r + mid)
				== h.getRev(l - mid, r + mid)) lo = mid;
		else hi = mid - 1;
	}
	return lo;
}

void check(const string &s, const string &t){
	int n = sz(s);
	StrHash h(s), ht(t);
	compare(s, h, s, h);
	compare(s, h, t, ht);
	ht = StrHash(s);
	assert(ht.get(1, n) == h.get(1, n));
	assert(sz(h.pw) == n + 1 && sz(h.f) == n + 1);
	assert(sz(h.b) == n + 2);
	assert(h.pw[0] == 1 && h.f[0] == 0);
	assert(h.b[n+1] == 0);
	FOR(i, 1, n){
		assert(h.get(i, i) == s[i-1]);
		assert(h.f[i] == h.get(1, i));
		assert(h.b[i] == h.getRev(i, n));
		assert(h.pw[i]
			== StrHash::mul(h.pw[i-1], h.pw[1]));
	}
	FOR(l, 1, n + 1){
		assert(h.get(l, l - 1) == 0);
		assert(h.getRev(l, l - 1) == 0);
	}
	string rev = s;
	reverse(all(rev));
	StrHash hr(rev);
	int bruteCount = 0, count = 0;
	FOR(l, 1, n) FOR(r, l, n){
		ll x = h.getRev(l, r);
		assert(0 <= x && x < StrHash::M);
		assert(x == hr.get(n - r + 1, n - l + 1));
		string part = s.substr(l - 1, r - l + 1);
		string back = part;
		reverse(all(back));
		bruteCount += part == back;
		assert((h.get(l, r) == x) == (part == back));
		FOR(d, 1, r - l + 1){
			bool period = true;
			FOR(k, l + d, r)
				if(s[k-1] != s[k-d-1]) period = false;
			assert((h.get(l, r - d) == h.get(l + d, r))
				== period);
		}
	}
	FOR(c, 1, n){
		count += radius(h, c, c, min(c - 1, n - c)) + 1;
		count += radius(h, c + 1, c, min(c, n - c));
	}
	assert(count == bruteCount);
	FOR(i, 0, n) FOR(j, 0, n){
		int lo = 0, hi = n - max(i, j);
		while(lo < hi){
			int mid = (lo + hi + 1) / 2;
			if(h.get(i + 1, i + mid)
					== h.get(j + 1, j + mid)) lo = mid;
			else hi = mid - 1;
		}
		int brute = 0;
		while(i + brute < n && j + brute < n
				&& s[i+brute] == s[j+brute]) brute++;
		assert(lo == brute);
		bool less;
		if(i + lo < n && j + lo < n)
			less = s[i+lo] < s[j+lo];
		else less = n - i < n - j;
		assert(less == (s.substr(i) < s.substr(j)));
	}
}

int main(){
	mt19937_64 rng(12345);
	ll mod = StrHash::M;
	assert(mod == (1LL << 61) - 1);
	for(ll a: {0LL, 1LL, mod / 2, mod - 2, mod - 1})
		for(ll c: {0LL, 1LL, mod / 2, mod - 2, mod - 1}){
			// Wide arithmetic is the independent product oracle.
			ll brute = (__int128)a * c % mod;
			assert(StrHash::mul(a, c) == brute);
		}
	FOR(trial, 1, 100000){
		ll a = rng() % mod, c = rng() % mod;
		ll brute = (__int128)a * c % mod;
		assert(StrHash::mul(a, c) == brute);
	}
	check("", "");
	check("a", "b");
	check(string(30, 'a'), string(30, 'a'));
	check("ababababababababababababababab",
		"bababababababababababababababa");
	check(string(1, char(1)), string(1, char(127)));
	FOR(trial, 1, 500){
		int n = rng() % 31, m = rng() % 31;
		string s(n, 'a'), t(m, 'a');
		for(char &c: s) c += rng() % 2;
		for(char &c: t) c += rng() % 2;
		check(s, t);
	}
	return 0;
}
