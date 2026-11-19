#include "../../tex/contents/contest/template.hpp"
#include <cassert>

int main(){
	int x = 3; string s = "ab";
	vector<vector<int>> v{{1, 2}, {3}};
	pair<int, string> p{4, "x"};
	int a[3] = {7, 8, 9};
	vector<bool> b{1, 0};
	map<int, vector<int>> m{{1, {2}}};
	char ca[2] = {'A', 'B'}, cz[5] = "xy"; // ca has no NUL
	int dbgArg = 5;
#ifdef LOCAL
	stringstream ss;
	auto old = cerr.rdbuf(ss.rdbuf());
	auto got = [&]{
		string r = ss.str();
		ss.str("");
		return r;
	};
	dbg(x); string r1 = got();
	dbg(v); string r2 = got();
	dbg(p); string r3 = got();
	dbg(a); string r4 = got();
	dbg(s); string r5 = got();
	dbg(b); string r6 = got();
	dbg(m); string r7 = got();
	dbg(x, s); string r8 = got();
	dbg(ca); string r9 = got();
	dbg(cz, "lit"); string r10 = got();
	dbg(dbgArg); string r11 = got();
	dbg(vector<int>{}, string{});
	assert(got() == "[vector<int>{}, string{}]: {} \n");
	cerr.rdbuf(old);
	assert(r1 == "[x]: 3\n");
	assert(r2 == "[v]: {{1, 2}, {3}}\n");
	assert(r3 == "[p]: (4, x)\n");
	assert(r4 == "[a]: {7, 8, 9}\n");
	assert(r5 == "[s]: ab\n");
	assert(r6 == "[b]: {1, 0}\n");
	assert(r7 == "[m]: {(1, {2})}\n");
	assert(r8 == "[x, s]: 3 ab\n");
	assert(r9 == "[ca]: AB\n");
	assert(r10 == "[cz, \"lit\"]: xy lit\n");
	assert(r11 == "[dbgArg]: 5\n");
#else
	int calls = 0;
	auto f = [&]{ return ++calls; };
	dbg(f());
	(void)f;
	assert(calls == 0);
	assert(x == 3 && s == "ab" && sz(v) == 2 && p.fi == 4);
	assert(a[0] == 7 && sz(b) == 2 && sz(m) == 1);
	assert(ca[1] == 'B' && cz[1] == 'y' && dbgArg == 5);
#endif
	int c = 0;
	FOR(i, 0, 2) c += i;
	REV(i, 2, 0) c += i;
	assert(c == 6);
	ll big = oo; pll q{1, 2};
	assert(big == (ll)1e18 && q.se == 2 && q.fi == 1);
	pii w{1, 2}; assert(w.fi + w.se == 3);
	vi u{3, 1, 2}; sort(all(u));
	assert(sz(u) == 3 && u[0] == 1);
	int mx = 1, mn = 5;
	assert(maximize(mx, 4) && minimize(mn, 2));
	assert(mx == 4 && mn == 2);
	assert(!maximize(mx, 4) && !minimize(mn, 3));
	assert(mx == 4 && mn == 2);
	assert(cntBit(7) == 3 && log2(8) == 3);
	assert(testBit(5, 2) && !testBit(5, 1));
	assert(flipBit(5, 0) == 4);
	PQMin<int> pq; pq.push(3); pq.push(1);
	assert(pq.top() == 1);
	PQMax<int> pm; pm.push(3); pm.push(1);
	assert(pm.top() == 3);
	vll vl{1}; assert(vl[0] == 1);
	ld d = 1; assert(d == 1);
}
