#include "../../tex/contents/math/bm.hpp"

int main(){
	ios_base::sync_with_stdio(0); cin.tie(0);
	int d; ll k; cin >> d >> k;
	FPS::poly a(d), c(d);
	for (auto &x: a) cin >> x;
	for (auto &x: c) cin >> x;
	cout << kthTerm(a, c, k) << '\n';
}
