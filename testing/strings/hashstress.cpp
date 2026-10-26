#include "../../tex/contents/strings/strhash.hpp"
int main(){
	mt19937 rng(1);
	for (int seed = 0; seed < 3000; seed++){
		int n = seed < 20 ? seed % 2 + 1 : rng() % 40 + 1; if (seed % 100 == 99) n = 2000;
		int k = rng() % 3 + 1; string s;
		for (int i = 0; i < n; i++) s += char('a' + rng() % k);
		if (seed % 7 == 0) s += string(s.rbegin(), s.rend()); // palindromes
		n = s.size(); HashString h(s);
		for (int t = 0; t < 300; t++){
			int l = rng() % n + 1, r = rng() % n + 1, l2 = rng() % n + 1, r2 = rng() % n + 1;
			if (l > r) swap(l, r);
			if (l2 > r2) swap(l2, r2);
			string x = s.substr(l-1, r-l+1), y = s.substr(l2-1, r2-l2+1);
			if ((h.getF(l, r) == h.getF(l2, r2)) != (x == y)){ cout << "F FAIL " << s << ' ' << l << ' ' << r << ' ' << l2 << ' ' << r2 << endl; return 1; }
			if ((h.getF(l, r) == h.getB(l, r)) != (x == string(x.rbegin(), x.rend()))){ cout << "B FAIL " << s << ' ' << l << ' ' << r << endl; return 1; }
		}
	}
	int n = 1000005; string s(n, 'a'); // old limit was 1e6
	for (int i = 0; i < n; i++) s[i] = 'a' + (i * 7 + i / 3) % 3;
	string t = s + "b" + string(s.rbegin(), s.rend()); n = t.size(); HashString h(t);
	if (h.getF(1, n) != h.getB(1, n)){ cout << "big pal FAIL" << endl; return 1; }
	if (h.getF(1, n-1) == h.getB(1, n-1)){ cout << "big nonpal FAIL" << endl; return 1; }
	cout << "OK" << endl;
}
