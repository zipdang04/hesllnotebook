#include "../../tex/contents/math/crt.hpp"
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	pll res = {0, 1};
	FOR(i, 1, 3){
		ll a, m; cin >> a >> m;
		res = crt(res.fi, res.se, a, m);
		if(res.fi == -1){
			cout << -1 << '\n';
			return 0;
		}
	}
	cout << (res.fi == 0 ? res.se : res.fi) << '\n';
}
