#include "../../tex/contents/strings/suffixarray.hpp"

int main(){
	mt19937 rng(12345);
	vector<string> cases = {"a", "aaaa", "abab", "ba", "abcabc", "bab"};
	FOR(int, it, 0, 199999){
		int n = it % 100 == 99 ? rng() % 200 + 1 : rng() % 12 + 1, m = rng() % 3 + 1;
		string s(n, 'a');
		for (char &ch: s) ch += rng() % m;
		cases.push_back(s);
	}
	for (string &s: cases) for (int cyc = 0; cyc < 2; cyc++){
		int n = s.size();
		vector<string> v(n);
		FOR(int, i, 0, n - 1) v[i] = cyc ? s.substr(i) + s.substr(0, i) : s.substr(i);
		vector<int> p(n); iota(p.begin(), p.end(), 0);
		stable_sort(p.begin(), p.end(), [&](int a, int b){ return v[a] < v[b]; });
		SA r(s, cyc);
		bool ok = (int)r.sa.size() == n && (int)r.rk.size() == n && (int)r.lcp.size() == n && r.lcp[0] == 0;
		bool distinct = true;
		FOR(int, i, 1, n - 1) distinct &= v[p[i]] != v[p[i - 1]];
		if (ok) FOR(int, i, 0, n - 1){
			ok &= r.rk[r.sa[i]] == i && v[r.sa[i]] == v[p[i]];
			if (!cyc) ok &= r.sa[i] == p[i];
			if (i && (!cyc || distinct)){
				int l = 0; const string &a = v[r.sa[i - 1]], &b = v[r.sa[i]];
				while (l < n && a[l] == b[l]) l++;
				ok &= r.lcp[i] == l;
			}
		}
		if (!ok){ cout << "FAIL " << s << " cyc=" << cyc << '\n'; return 1; }
	}
	cout << "OK\n";
}
