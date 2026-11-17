#include "../../tex/contents/flow/dinic.hpp"

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int V, E; cin >> V >> E;
	Dinic d(V);
	FOR(i, 0, E - 1){
		int u, v; ll c; cin >> u >> v >> c;
		d.addEdge(u, v, c);
	}
	cout << d.maxFlow(0, V - 1) << '\n';
}
