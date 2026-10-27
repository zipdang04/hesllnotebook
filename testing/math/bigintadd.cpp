#include "../../tex/contents/math/bigint.hpp"

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int t; cin >> t;
	while (t--){ bigint a, b; cin >> a >> b; cout << a + b << '\n'; }
}
