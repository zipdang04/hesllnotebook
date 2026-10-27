#include "../../tex/contents/graph/lowlink.hpp"
int main(){
	int n, m; read(n, m);
	Lowlink L(n);
	while (m--){ int u, v; read(u, v); L.addEdge(u, v); }
	L.run();
	vector<vector<int>> c(L.k2);
	EFOR(int, i, 0, n) c[L.two[i]].push_back(i);
	string s = to_string(L.k2) + "\n";
	for (auto &b: c){
		s += to_string(b.size());
		for (int v: b) s += " " + to_string(v);
		s += "\n";
	}
	fputs(s.c_str(), stdout);
}
