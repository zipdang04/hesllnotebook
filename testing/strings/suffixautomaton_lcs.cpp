#include "../../tex/contents/strings/suffixautomaton.hpp"
#include <cassert>

int main(){
	string s, t; cin >> s >> t;
	SAM sam;
	for(char c: s) sam.add(c);
	int v = 0, len = 0, best = 0, end = 0, node = 0;
	FOR(e, 0, sz(t) - 1){
		int x = t[e] - SAM::FIRST;
		while(v != 0 && !sam.t[v].nxt[x]){
			v = sam.t[v].link;
			len = sam.t[v].len;
		}
		if(sam.t[v].nxt[x]){
			v = sam.t[v].nxt[x]; len++;
		}
		if(len > best){
			best = len; end = e; node = v;
		}
	}
	if(best == 0){ cout << "0 0 0 0" << '\n'; return 0; }
	int a = sam.t[node].pos - best + 1;
	cout << a << ' ' << a + best << ' '
		<< end - best + 1 << ' ' << end + 1 << '\n';
}
