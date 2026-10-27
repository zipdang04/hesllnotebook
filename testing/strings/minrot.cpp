#include "../../tex/contents/strings/lyndon.hpp"

mt19937 rng(1);
bool check(const string &s){
	int n = s.size(), r = minRotation(s);
	string best = s;
	EFOR(int, i, 0, n) best = min(best, s.substr(i) + s.substr(0, i));
	if (r < 0 || r >= n || s.substr(r) + s.substr(0, r) != best){
		printf("FAIL s=%s r=%d\n", s.c_str(), r); return false;
	}
	return true;
}
int main(){
	for (string s: {"a", "aaaa", "abab", "ba", "abcabc", "bab"}) if (!check(s)) return 1;
	EFOR(int, it, 0, 200000){
		int n = it % 10 == 0 ? rng() % 200 + 1 : rng() % 12 + 1, c = rng() % 3 + 1;
		string s(n, 'a');
		for (char &ch: s) ch += rng() % c;
		if (!check(s)) return 1;
	}
	puts("OK");
}
