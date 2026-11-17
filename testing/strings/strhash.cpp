#include "../../tex/contents/strings/strhash.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	StrHash e("");
	assert(e.get(1, 0) == 0 && e.getRev(1, 0) == 0);
	StrHash abc("abc"), b("b");
	assert(abc.get(2, 2) == b.get(1, 1));
	e = StrHash("ab");
	assert(e.getRev(1, 2) == StrHash("ba").get(1, 2));
	FOR(l, 1, 3){
		FOR(r, l - 1, 2){
			ll x = e.get(l, r), y = e.getRev(l, r);
			assert(0 <= x && x < StrHash::M);
			assert(0 <= y && y < StrHash::M);
		}
	}
	FOR(l, 1, 3){
		assert(e.get(l, l - 1) == 0);
		assert(e.getRev(l, l - 1) == 0);
	}
	const ll M = StrHash::M;
	for(ll a: {0LL, 1LL, 12345678901234LL, M / 2, M - 1})
		for(ll c: {0LL, 1LL, 987654321987654LL, M - 1}){
			ll want = (ll)((__int128)a * c % M);
			assert(StrHash::mul(a, c) == want);
		}
	string s;
	cin >> s;
	int n = sz(s);
	StrHash h(s);
	auto pal = [&](int l, int r){
		return h.get(l, r) == h.getRev(l, r);
	};
	FOR(i, 0, 2 * n - 2){
		// even i: odd centre c; odd i: even centre c, c + 1
		bool odd = i % 2 == 0;
		int c = odd ? i / 2 + 1 : (i + 1) / 2;
		int lo = 0, hi = min(c - odd, n - c);
		while(lo < hi){
			int k = (lo + hi + 1) / 2;
			if(pal(c - k + !odd, c + k)) lo = k;
			else hi = k - 1;
		}
		cout << 2 * lo + odd << (i == 2 * n - 2 ? '\n' : ' ');
	}
}
