#include "../../tex/contents/graph/lowlink.hpp"
#include <cassert>

struct Brute{
	int count = 0;
	vector<int> comp;
	Brute(const vector<vector<pair<int, int>>> &g,
		int bannedVertex = 0, int bannedEdge = -1)
		: comp(g.size()){
		FOR(root, 1, (int)g.size() - 1){
			if(root == bannedVertex || comp[root]) continue;
			vector<int> q{root}; comp[root] = ++count;
			for(int i = 0; i < sz(q); i++)
				for(auto [v, id]: g[q[i]]){
					if(v == bannedVertex || id == bannedEdge
						|| comp[v]) continue;
					comp[v] = count; q.push_back(v);
				}
		}
	}
};

int main(){
	mt19937 rng(12345);
	FOR(trial, 1, 20000){
		int n = rng()%7+1, m = rng()%11;
		vector<vector<pair<int, int>>> g(n+1);
		vector<int> eu(m+1), ev(m+1);
		FOR(id, 1, m){
			eu[id] = rng()%n+1; ev[id] = rng()%n+1;
			g[eu[id]].push_back({ev[id], id});
			g[ev[id]].push_back({eu[id], id});
		}
		LowLink low(g, m);
		assert((int)low.bcc.size() == m+1);
		assert((int)low.isBridge.size() == m+1);
		assert((int)low.isCut.size() == n+1);
		assert(!low.isCut[0] && !low.isBridge[0]);
		assert(low.bcc[0] == 0);
		int base = Brute(g).count;
		vector<vector<int>> without(n+1);
		FOR(u, 1, n){
			Brute removed(g, u);
			assert(low.isCut[u] == (removed.count > base));
			without[u] = move(removed.comp);
		}
		vector<bool> used(low.bccCnt+1);
		FOR(id, 1, m){
			assert(low.isBridge[id] ==
				(Brute(g, 0, id).count > base));
			if(eu[id] == ev[id]) assert(low.bcc[id] == 0);
			else{
				assert(1 <= low.bcc[id]);
				assert(low.bcc[id] <= low.bccCnt);
				used[low.bcc[id]] = true;
			}
		}
		FOR(c, 1, low.bccCnt) assert(used[c]);
		FOR(a, 1, m) FOR(b, a+1, m){
			if(eu[a] == ev[a] || eu[b] == ev[b]) continue;
			bool same = true;
			FOR(x, 1, n){
				int u = eu[a] == x ? ev[a] : eu[a];
				int v = eu[b] == x ? ev[b] : eu[b];
				if(without[x][u] != without[x][v]) same = false;
			}
			assert((low.bcc[a] == low.bcc[b]) == same);
		}
	}
	// Edge id 0, unused ids, a loop stored once, skip g[0].
	vector<vector<pair<int, int>>> g(4);
	g[0].push_back({100, 100});
	g[1] = {{2, 0}, {1, 2}}; g[2] = {{1, 0}};
	LowLink low(g, 3);
	assert(low.bccCnt == 1 && low.bcc[0] == 1);
	assert(low.isBridge[0] && !low.isBridge[2]);
	assert(low.bcc[1] == 0 && low.bcc[2] == 0);
	assert(low.bcc[3] == 0 && !low.isBridge[3]);
	FOR(u, 0, 3) assert(!low.isCut[u]);
	return 0;
}
