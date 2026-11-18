#include "../../tex/contents/strings/suffixautomaton.hpp"
#include <cassert>

int main(){
	SAM sam;
	assert(sz(sam.t) == 1 && sam.t[0].link == -1);
	for(char c: string("abb")) sam.add(c);
	int b = sam.t[0].nxt['b' - SAM::FIRST];
	int bb = sam.t[b].nxt['b' - SAM::FIRST];
	assert(sz(sam.t) == 5 && b == 4 && bb == 3);
	assert(sam.t[b].len == 1 && sam.t[b].pos == 1);
	assert(sam.t[bb].len == 3 && sam.t[bb].pos == 2);
	assert(sam.last == 3);
	sam = SAM();
	assert(sz(sam.t) == 1 && sam.last == 0);
	string s; cin >> s;
	for(char c: s) sam.add(c);
	ll ans = 0;
	FOR(v, 1, sz(sam.t) - 1)
		ans += sam.t[v].len - sam.t[sam.t[v].link].len;
	cout << ans << '\n';
}
