#include "../../tex/contents/strings/suffixarray.hpp"

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	string s; cin >> s;
	SA r(s);
	for (int x: r.sa) cout << x << ' ';
	cout << '\n';
}
