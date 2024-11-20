#include "../../../tex/contents/math/numbertheory/millerrabin.hpp"
main()
{
	ios_base::sync_with_stdio(0); cin.tie(0);
	int q; read(q);
	FOR(int, _, 1, q) {
		ll num; read(num);
		cout << (MillerRabin::checkPrime(num) ? "Yes\n" : "No\n");
	}
}