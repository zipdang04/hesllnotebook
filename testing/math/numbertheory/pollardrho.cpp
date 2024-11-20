#include "../../../tex/contents/math/numbertheory/pollardrho.hpp"
main()
{
	ios_base::sync_with_stdio(0); cin.tie(0);
	int q; read(q);
	FOR(int, _, 1, q) {
		ll n; read(n);
		vector<ll> ans = Pollards::rho(n);
		sort(ans.begin(), ans.end());
		cout << ans.size() << ' ';
		for (ll i: ans) cout << i << ' ';
		cout << '\n';
	}
}