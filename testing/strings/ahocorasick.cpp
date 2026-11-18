#include "../../tex/contents/strings/ahocorasick.hpp"
#include <cassert>

int main(){
	Aho a; a.add("ab"); a.add("b"); a.build();
	Aho b; b.add("a"); b.build();
	assert(a.countMatch("abab") == 4);
	assert(b.countMatch("abab") == 2);
	Aho d; int e1 = d.add("a"), e2 = d.add("a");
	assert(e1 == e2);
	d.build();
	assert(d.t[e1].cnt == 2);
	a = Aho();
	assert(sz(a.t) == 1 && a.order.empty());
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	string s; int m; cin >> s >> m;
	for(char &c: s) c = c - 'A' + 'a';
	Aho aho;
	FOR(i, 1, m){
		string p; cin >> p;
		for(char &c: p) c = c - 'A' + 'a';
		aho.add(p);
	}
	aho.build();
	cout << aho.countMatch(s) << '\n';
}
