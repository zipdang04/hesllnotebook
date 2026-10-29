#include "template.hpp"
// ./checker in out ans: exit 0 = OK, 1 = WA (why on
// stderr). Example: out = any vertex of max degree
int main(int argc, char **argv){
	ifstream in(argv[1]), out(argv[2]), ans(argv[3]);
	auto wa = [](string s){ cerr << s << '\n'; exit(1); };
	int n, u, v, x; in >> n; vector<int> d(n + 1);
	FOR(int, i, 1, n - 1) in >> u >> v, d[u]++, d[v]++;
	if (!(out >> x) || x < 1 || x > n) wa("bad vertex");
	int mx = *max_element(d.begin(), d.end());
	if (d[x] != mx) wa("not max");
	string s; if (out >> s) wa("extra output");
}
