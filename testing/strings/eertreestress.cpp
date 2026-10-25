#include "../../tex/contents/strings/eertree.hpp"
int main(){
	mt19937 rng(1);
	for (int seed = 0; seed < 20000; seed++){
		int n = rng() % 30 + 1, k = rng() % 3 + 1;
		string s; for (int i = 0; i < n; i++) s += char('a' + rng() % k);
		Eertree<3> e; vector<string> str{"",""};
		for (int i = 0; i < n; i++){
			int sz = e.t.size(), u = e.add(s[i]);
			string best; for (int l = 1; l <= i+1; l++){ string x = s.substr(i+1-l, l); if (x == string(x.rbegin(), x.rend())) best = x; }
			if ((int)e.t.size() > sz) str.push_back(s.substr(i + 1 - e.t[u].len, e.t[u].len));
			if (str[u] != best){ cout << "suffix FAIL " << s << endl; return 1; }
		}
		e.calc();
		map<string,int> cnt;
		for (int i = 0; i < n; i++) for (int l = 1; i + l <= n; l++){ string x = s.substr(i, l); if (x == string(x.rbegin(), x.rend())) cnt[x]++; }
		if (cnt.size() != e.t.size() - 2){ cout << "count FAIL " << s << endl; return 1; }
		for (int u = 2; u < (int)e.t.size(); u++) if (cnt[str[u]] != e.t[u].cnt || (int)str[u].size() != e.t[u].len){ cout << "occ FAIL " << s << endl; return 1; }
	}
	cout << "OK" << endl;
}
