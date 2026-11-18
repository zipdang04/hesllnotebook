#include "../../tex/contents/strings/suffixarray.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	string s; cin >> s;
	int n = sz(s);
	SuffixArray sa(s + '$');
	ll ans = (ll)n * (n + 1) / 2;
	FOR(i, 1, n)
		ans -= sa.lcp[i];
	cout << ans << '\n';
}
