#include "../../tex/contents/strings/manacher.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	assert(manacher("").empty());
	assert((manacher("a") == vector<int>{1}));
	assert((manacher("aa") == vector<int>{1, 2, 1}));
	// Exhaust all short binary strings, including empty gaps.
	FOR(n, 1, 10) FOR(mask, 0, (1 << n) - 1){
		string s(n, 'a');
		FOR(i, 0, n - 1) s[i] += mask >> i & 1;
		vector<int> want(2 * n - 1);
		FOR(l, 0, n - 1) FOR(r, l, n - 1){
			bool pal = true;
			FOR(j, 0, (r - l) / 2)
				if(s[l+j] != s[r-j]) pal = false;
			if(pal) want[l+r] = max(want[l+r], r - l + 1);
		}
		assert(manacher(s) == want);
	}
	string s; cin >> s;
	vector<int> p = manacher(s);
	FOR(i, 0, sz(p) - 1)
		cout << p[i] << " \n"[i == sz(p) - 1];
}
