#include "../../../tex/contents/math/numerical/ntt.hpp"

main()
{
	ios_base::sync_with_stdio(0); cin.tie(0);
	FFT::buildRoot();
	int n, m; cin >> n >> m;
	vector<int> a(n); FOR(int, i, 0, n-1) cin >> a[i];
	vector<int> b(m); FOR(int, i, 0, m-1) cin >> b[i];
	vector<modint> ans = FFT::multiply(a, b);
	FOR(int, i, 0, n-1+m-1) cout << ans[i] << ' ';
}