#include "../../tex/contents/graph/centroid.hpp"
#include <cassert>

mt19937 rng(12345);
int rnd(int a, int b){ return a + rng() % (b - a + 1); }

// the note's DFS: enter only v with level[v] > level[c]
void reach(int u, int p, int lv, const Centroid &cd,
		const vector<vector<int>> &g, vector<char> &in){
	in[u] = 1;
	for(int v: g[u])
		if(v != p && cd.level[v] > lv)
			reach(v, u, lv, cd, g, in);
}

void tree(int u, int p, const vector<vector<int>> &g,
		vector<int> &id, int t){
	id[u] = t;
	for(int v: g[u]) if(v != p) tree(v, u, g, id, t);
}

void check(const vector<vector<int>> &g){
	int n = sz(g) - 1;
	Centroid cd(g);
	vector<int> id(n + 1), tsz(n + 2), tops(n + 2);
	int t = 0;
	FOR(u, 1, n) if(!id[u]) tree(u, 0, g, id, ++t);
	FOR(u, 1, n) tsz[id[u]]++;
	vector<int> cnt(n + 1);
	vector<vector<char>> in(n + 1, vector<char>(n + 1));
	FOR(c, 1, n){
		reach(c, 0, cd.level[c], cd, g, in[c]);
		int x = c, steps = 0;
		while(x && steps <= n) x = cd.par[x], steps++;
		assert(x == 0 && steps <= n);
		if(!cd.par[c]){
			tops[id[c]]++;
			assert(cd.level[c] == 0);
		}else{
			assert(id[cd.par[c]] == id[c]);
			assert(cd.level[c] == cd.level[cd.par[c]] + 1);
		}
		assert(cd.level[c] <= log2(tsz[id[c]]));
	}
	FOR(u, 1, t) assert(tops[u] == 1);
	FOR(c, 1, n)
		FOR(v, 1, n){
			bool anc = false;
			for(int x = v; x; x = cd.par[x]) anc |= x == c;
			assert(anc == (bool)in[c][v]);
			cnt[c] += in[c][v];
		}
	// a child's part is at most half its parent's
	FOR(c, 1, n) if(cd.par[c])
		assert(cnt[c] <= cnt[cd.par[c]] / 2);
}

void big(bool star){
	int n = 200000;
	vector<vector<int>> g(n + 1);
	FOR(i, 2, n){
		int p = star ? 1 : i - 1;
		g[i].push_back(p), g[p].push_back(i);
	}
	Centroid cd(g);
	assert(*max_element(all(cd.level)) <= 17);
}

int main(){
	FOR(it, 1, 4000){
		int n = rnd(0, 30), shape = rnd(0, 3);
		vector<int> perm(n + 1);
		iota(all(perm), 0);
		shuffle(perm.begin() + 1, perm.end(), rng);
		vector<vector<int>> g(n + 1);
		FOR(i, 2, n){
			int p = rnd(1, i - 1);
			if(shape == 1) p = i - 1;
			if(shape == 2) p = 1;
			if(shape == 3) p = i <= n / 2 ? i - 1 : rnd(1, n / 2);
			if(rnd(1, 6) == 1) continue;
			g[perm[i]].push_back(perm[p]);
			g[perm[p]].push_back(perm[i]);
		}
		check(g);
	}
	big(false);
	big(true);
}
