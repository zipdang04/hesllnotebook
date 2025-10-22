#include "../../../tex/contents/math/numerical/fft.hpp"

#define MOD 1'000'000'007
#define CUT 31622ll

int n, m;
vector<ll> a, b;

vector<ll> operator + (vector<ll> a, vector<ll> b) {
	int len = max(a.size(), b.size());
	a.resize(len, 0); b.resize(len, 0);
	FOR(int, i, 0, len-1) a[i] = (a[i] + b[i]) % MOD;
	return a;
}
vector<ll> operator * (vector<ll> a, ll b) {
	for (ll &num: a) num = num % MOD * b % MOD;
	return a;
}

main()
{
	ios_base::sync_with_stdio(0); cin.tie(0);
	FFT::buildRoot();
	cin >> n >> m;
	a = vector<ll>(n, 0);
	b = vector<ll>(m, 0);
	for (ll &i: a) cin >> i;
	for (ll &i: b) cin >> i;
	// cerr << "oke" << endl;
	vector<ll> a2 = a, b2 = b;
	FOR(int, i, 0, n-1) a2[i] = a[i] / CUT, a[i] %= CUT;
	FOR(int, i, 0, m-1) b2[i] = b[i] / CUT, b[i] %= CUT;
	// vector<ll> a3 = a2, b3 = b2;
	// FOR(int, i, 0, n-1) a3[i] = a2[i] / CUT, a2[i] %= CUT;
	// FOR(int, i, 0, m-1) b3[i] = b2[i] / CUT, b2[i] %= CUT;
	
	int len = n+m-1;
	vector<ll> ans = FFT::conv(a, b);
	for (ll &num: ans) num %= MOD;
	// // for (ll i: ans) cerr << i << ' '; cerr << "'\n'";
	ans.resize(len, 0);
	
	ans = ans + (FFT::conv(a, b2) + FFT::conv(a2, b)) * CUT;
	// // for (ll i: ans) cerr << i << ' '; cerr << "'\n'";
	ans = ans + (FFT::conv(a2, b2)) * (CUT * CUT);
	// // for (ll i: ans) cerr << i << ' '; cerr << "'\n'";
	// ans = ans + (FFT::conv(a2, b3) + FFT::conv(a3, b2)) * (CUT * CUT * CUT);
	// // for (ll i: ans) cerr << i << ' '; cerr << "'\n'";
	// ans = ans + FFT::conv(a3, b3) * (CUT * CUT * CUT * CUT % MOD);
	// // for (ll i: ans) cerr << i << ' '; cerr << "'\n'";

	while (int(ans.size()) < len) ans.push_back(0);
	// for (ll i: ans) cerr << i << ' '; cerr << "'\n'";
	FOR(int, i, 0, len-1) cout << ans[i] << " \n"[i == len-1];
}