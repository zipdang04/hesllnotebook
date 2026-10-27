#include "../../tex/contents/math/fft.hpp"
int main(){
	ios_base::sync_with_stdio(0); cin.tie(0);
	int n, m; cin >> n >> m;
	vector<ll> a(n), b(m);
	for (ll &x: a) cin >> x;
	for (ll &x: b) cin >> x;
	for (ll x: convMod<1000000007>(a, b)) cout << x << ' ';
	cout << '\n';
}
