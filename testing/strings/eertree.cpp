#include "../../tex/contents/strings/eertree.hpp"
#include <cassert>

int main(){
	Eertree et;
	assert(sz(et.t) == 2);
	assert(et.t[0].len == -1 && et.t[1].len == 0);
	assert(et.t[0].link == 0 && et.t[1].link == 0);
	assert(et.t[0].dep == 0 && et.t[1].dep == 0);
	assert(et.add('a') == 2);
	assert(et.t[2].len == 1 && et.t[2].link == 1);
	assert(et.t[2].dep == 1);
	assert(et.add('b') == 3 && et.add('a') == 4);
	assert(et.t[4].len == 3 && et.t[4].link == 2);
	assert(et.t[4].dep == 2);
	for(int v = sz(et.t) - 1; v >= 2; v--)
		et.t[et.t[v].link].cnt += et.t[v].cnt;
	assert(et.t[2].cnt == 2);
	et = Eertree();
	assert(sz(et.t) == 2 && et.s.empty() && et.last == 1);
	for(char c: string("aaa")) et.add(c);
	assert(et.t[2].cnt == 1 && et.t[3].cnt == 1);
	assert(et.t[4].cnt == 1);
	assert(et.t[0].nxt['a' - et.FIRST] == 2);
	assert(et.t[1].nxt['a' - et.FIRST] == 3);
	assert(et.t[2].nxt['a' - et.FIRST] == 4);
	et = Eertree();
	string s;
	cin >> s;
	vector<int> ret;
	for(char c: s) ret.push_back(et.add(c));
	vector<int> par(sz(et.t));
	FOR(u, 0, sz(et.t) - 1)
		FOR(c, 0, et.K - 1) if(et.t[u].nxt[c])
			par[et.t[u].nxt[c]] = u;
	cout << sz(et.t) - 2 << '\n';
	FOR(v, 2, sz(et.t) - 1)
		cout << par[v] - 1 << ' ' << et.t[v].link - 1 << '\n';
	for(int v: ret) cout << v - 1 << ' ';
	cout << '\n';
}
