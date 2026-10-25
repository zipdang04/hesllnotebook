#include "../../tex/contents/math/bm.hpp"

int main(){
	ios_base::sync_with_stdio(0); cin.tie(0);
	int n; cin >> n;
	FPS::poly a(n); for (auto &x: a) cin >> x;
	FPS::poly c = berlekampMassey(a);
	cout << c.size() << '\n';
	for (auto x: c) cout << x << ' ';
	cout << '\n';
}
