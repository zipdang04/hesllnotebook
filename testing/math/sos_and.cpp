#include "../../tex/contents/math/sos.hpp"
#include "../../tex/contents/math/modint.hpp"

// superset sums, pointwise product, superset inverse
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n;
	cin >> n;
	vector<MintN> a(1 << n), b(1 << n);
	for(auto &x: a) cin >> x;
	for(auto &x: b) cin >> x;
	auto add = [](MintN &x, MintN y){ x += y; };
	sos(a, add, true), sos(b, add, true);
	FOR(i, 0, (1 << n) - 1) a[i] *= b[i];
	sos(a, [](MintN &x, MintN y){ x -= y; }, true);
	FOR(i, 0, (1 << n) - 1)
		cout << a[i] << " \n"[i == (1 << n) - 1];
}
