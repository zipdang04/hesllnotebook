#include "../../tex/contents/graph/lowlink.hpp"
int main(){
	int n, m; read(n, m);
	Lowlink L(n);
	while (m--){ int u, v; read(u, v); L.addEdge(u, v); }
	L.run();
	string s = to_string(L.bcc.size()) + "\n";
	for (auto &b: L.bcc){
		s += to_string(b.size());
		for (int v: b) s += " " + to_string(v);
		s += "\n";
	}
	fputs(s.c_str(), stdout);
}
