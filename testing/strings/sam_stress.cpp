#include "../../tex/contents/strings/sam.hpp"
int main(){
	mt19937 rng(1);
	FOR(int, seed, 0, 19999){
		int A = rng() % 3 + 1, n = rng() % (seed % 10 == 0 ? 60 : 12) + 1;
		string s; FOR(int, i, 0, n - 1) s += char('a' + rng() % A);
		SuffixAutomaton<3> sam;
		set<string> all;
		FOR(int, i, 0, n - 1){
			sam.extend(s[i]);
			FOR(int, j, 0, i) all.insert(s.substr(j, i - j + 1));
			if ((ll)all.size() != sam.distinct()){ printf("distinct %s %d\n", s.c_str(), i); return 1; }
		}
		if (n >= 2 && (int)sam.t.size() > 2 * n - 1){ puts("states"); return 1; }
		SuffixAutomaton<3> sam2(s);
		if (sam2.distinct() != (ll)all.size()) { puts("ctor"); return 1; }
		sam.countEndpos();
		auto chk = [&](const string &p){
			int occ = 0; for (int i = 0; i + (int)p.size() <= n; i++) occ += s.compare(i, p.size(), p) == 0;
			int u = sam.find(p);
			if ((occ == 0) != (u < 0) || (u > 0 && sam.t[u].cnt != occ)){ printf("occ %s %s\n", s.c_str(), p.c_str()); return false; }
			return true;
		};
		for (auto &p: all) if (!chk(p)) return 1;
		FOR(int, k, 0, 19){ string p; int m = rng() % 8 + 1; FOR(int, i, 0, m - 1) p += char('a' + rng() % 3); if (!chk(p)) return 1; }
	}
	SuffixAutomaton<26> big(string(500000, 'z'));
	if (big.distinct() != 500000){ puts("big"); return 1; }
	puts("OK");
}
