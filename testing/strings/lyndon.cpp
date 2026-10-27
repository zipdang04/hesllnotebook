#include "../../tex/contents/strings/lyndon.hpp"

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	string s; cin >> s;
	for (int x: duval(s)) cout << x << ' ';
	cout << '\n';
}
