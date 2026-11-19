#include "../../tex/contents/graph/dominator.hpp"
#include <cassert>

vector<int> reach(const vector<vector<int>> &g,
		int root, int banned){
	vector<int> seen(sz(g)), q{root};
	seen[root] = 1;
	for(int i = 0; i < sz(q); i++){
		for(int v: g[q[i]]){
			if(v == banned || seen[v]) continue;
			seen[v] = 1;
			q.push_back(v);
		}
	}
	return seen;
}

int main(){
	mt19937 rng(12345);
	FOR(trial, 1, 20000){
		int n = rng() % 8 + 1, m = rng() % 17;
		int root = rng() % n + 1;
		vector<vector<int>> g(n + 1);
		FOR(i, 1, m){
			int u = rng() % n + 1, v = rng() % n + 1;
			g[u].push_back(v);
		}
		vector<vector<int>> seen(n + 1);
		seen[0] = reach(g, root, 0);
		FOR(x, 1, n) if(x != root)
			seen[x] = reach(g, root, x);
		vector<int> want(n + 1);
		want[root] = root;
		FOR(v, 1, n){
			if(v == root || !seen[0][v]) continue;
			int best = -1;
			FOR(x, 1, n){
				if(x == v || !seen[0][x]) continue;
				if(x != root && seen[x][v]) continue;
				int count = 0;
				FOR(y, 1, n){
					if(y == x || !seen[0][y]) continue;
					if(y == root || !seen[y][x]) count++;
				}
				if(count > best){
					best = count;
					want[v] = x;
				}
			}
		}
		Dominator dt(g, root);
		assert(dt.idom == want);
	}
	FOR(trial, 1, 10){
		int n = 2000;
		vector<int> path(n);
		iota(all(path), 1);
		shuffle(all(path), rng);
		vector<vector<int>> g(n + 1);
		FOR(i, 1, n - 1){
			g[path[i - 1]].push_back(path[i]);
			g[path[i]].push_back(path[rng() % i]);
		}
		Dominator dt(g, path[0]);
	}
}
