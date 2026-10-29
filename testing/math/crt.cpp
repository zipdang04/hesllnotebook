#include "../../tex/contents/math/crt.hpp"

int main(){
	pair<ll, ll> r{0, 1};
	FOR(int, i, 0, 2){
		ll x, y; cin >> x >> y;
		if (r.se != -1) r = crt(r.fi, r.se, x, y);
	}
	if (r.se == -1) cout << -1 << "\n";
	else cout << (r.fi ? r.fi : r.se) << "\n";
}
