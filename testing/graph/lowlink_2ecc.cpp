#include "../../tex/contents/graph/lowlink.hpp"

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, m; cin >> n >> m;
	vector<vector<pair<int, int>>> g(n + 1);
	FOR(id, 1, m){
		int a, b; cin >> a >> b; a++; b++;
		g[a].push_back({b, id});
		g[b].push_back({a, id});
	}
	LowLink low(g, m);
	vector<bool> seen(n + 1);
	vector<vector<int>> comp;
	FOR(s, 1, n){
		if(seen[s]) continue;
		vector<int> q = {s}; seen[s] = true;
		for(int i = 0; i < (int)q.size(); i++)
			for(auto [v, id]: g[q[i]])
				if(!low.isBridge[id] && !seen[v]){
					seen[v] = true; q.push_back(v);
				}
		comp.push_back(q);
	}
	cout << comp.size() << "\n";
	for(auto &c: comp){
		cout << c.size();
		for(int u: c) cout << " " << u - 1;
		cout << "\n";
	}
}
