#define MAX 500005
#include "../../tex/contents/strings/suffixarray.hpp"
using namespace Suffix;

int main() {
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	cin >> s; s += '$'; len = s.length();
	build();
	FOR(int, i, 1, len - 1) cout << sa[i] << ' ';
	cout << '\n';
}
