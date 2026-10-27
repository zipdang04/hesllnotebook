#include "../../tex/contents/strings/suffixarray.hpp"

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	string s; cin >> s;
	SA r(s);
	ll n = s.size(), ans = n * (n + 1) / 2;
	for (int x: r.lcp) ans -= x;
	cout << ans << '\n';
}
