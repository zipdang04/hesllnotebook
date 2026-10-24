#include "../../tex/contents/strings/ahocorasick.hpp"

// yukicoder 430: total occurrences of M patterns (uppercase) in S
int main() {
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	string s; int m; cin >> s >> m;
	AhoCorasick<26, 'A'> aho;
	FOR(int, _, 1, m) {
		string p; cin >> p;
		aho.add(p);
	}
	aho.build();
	cout << aho.countMatch(s) << '\n';
}
