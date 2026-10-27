#include "../../tex/contents/math/fps.hpp"

int main(){
	ios_base::sync_with_stdio(0); cin.tie(0);
	int n; cin >> n;
	FPS::poly a(n); for (auto &x: a) cin >> x;
	for (auto x: FPS::exp(a, n)) cout << x << ' ';
	cout << '\n';
}
