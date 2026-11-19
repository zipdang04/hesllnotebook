#include "../../tex/contents/strings/suffixarray.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	string s, t; cin >> s >> t;
	int n = sz(s), m = sz(t);
	string u = s + '$' + t + '#';
	SuffixArray sa(u);
	int best = 0, a = 0, c = 0;
	FOR(i, 1, sz(u) - 1){
		int p = sa.sa[i-1], q = sa.sa[i];
		if(p > n && p <= n + m && q < n)
			swap(p, q);
		if(p < n && q > n && q <= n + m
				&& maximize(best, sa.lcp[i])){
			a = p; c = q - n - 1;
		}
	}
	cout << a << ' ' << a + best << ' '
		<< c << ' ' << c + best << '\n';
}
