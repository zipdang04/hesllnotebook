#include "../../../tex/contents/math/ds/modint.hpp"

#define MAX 10000000
modint fact[MAX + 1], ifac[MAX + 1];

main()
{
	ios_base::sync_with_stdio(0); cin.tie(0);
	int t; read(t, modint::MOD);
	const int LIMIT = min(1ll*MAX, modint::MOD - 1);
	fact[0] = 1; FOR(int, i, 1, LIMIT) fact[i] = fact[i - 1] * i;
	ifac[LIMIT] = 1 / fact[LIMIT];
	REV(int, i, LIMIT - 1, 0) ifac[i] = ifac[i + 1] * (i + 1);

	FOR(int, _, 1, t) {
		int n, k; read(n, k);
		if (n < k) {cout << "0\n"; continue;}
		modint ans = 1;
		while (n > 0 and k > 0) {
			int dN = modint::mod(n), dK = modint::mod(k);
			ans *= fact[dN] * ifac[dK] * ifac[dN - dK];
			n /= modint::MOD, k /= modint::MOD;
		}
		cout << ans << '\n';
	}
}
