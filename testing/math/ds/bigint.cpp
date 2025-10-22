#include "../../../tex/contents/math/ds/bigint.hpp"

int main() {
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int t; cin >> t;
	FOR(int, _, 1, t) {
		bigint a, b; cin >> a >> b;
		cout << (a * b) << '\n';
	}
}