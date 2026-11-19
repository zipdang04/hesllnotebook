#include "../../tex/contents/strings/zfunc.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	assert(zFunc("").empty());
	assert((zFunc("a") == vector<int>{1}));
	assert((zFunc("aab") == vector<int>{3, 1, 0}));
	vector<int> want{7, 0, 1, 0, 3, 0, 1};
	assert(zFunc("abacaba") == want);
	// note: pattern p in s, and the period test
	string p = "aba", t = "xabababa";
	vector<int> z = zFunc(p + '#' + t), at;
	int m = sz(p);
	FOR(i, m + 1, sz(z) - 1)
		if(z[i] == m) at.push_back(i - m - 1);
	assert((at == vector<int>{1, 3, 5}));
	string u = "abcabcab";
	vector<int> zu = zFunc(u);
	FOR(d, 1, sz(u)){
		bool period = true;
		FOR(i, 0, sz(u) - d - 1)
			if(u[i] != u[i + d]) period = false;
		assert(period == (d == sz(u) || d + zu[d] == sz(u)));
	}
	string s;
	cin >> s;
	vector<int> res = zFunc(s);
	FOR(i, 0, sz(res) - 1)
		cout << res[i] << " \n"[i == sz(res) - 1];
}
