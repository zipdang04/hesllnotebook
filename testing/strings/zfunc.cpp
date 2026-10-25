#include "../../tex/contents/strings/zfunc.hpp"

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	string s; cin >> s;
	for (int x: zFunc(s)) cout << x << ' ';
	cout << '\n';
}
