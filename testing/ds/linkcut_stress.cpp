#include "../../tex/contents/ds/linkcut.hpp"
#include <cassert>

vector<int> parents(vector<set<int>> &adj, int root){
	vector<int> par(sz(adj), -1), q = {root};
	par[root] = 0;
	for(int i = 0; i < sz(q); i++)
		for(int v: adj[q[i]]) if(par[v] == -1){
			par[v] = q[i];
			q.push_back(v);
		}
	return par;
}

void checkNull(LinkCut &lct){
	auto x = lct.t[0];
	assert(x.val == 0 && x.sum == 0 && x.size == 0);
	assert(x.par == 0 && x.ch[0] == 0 && x.ch[1] == 0);
	assert(x.rev == 0 && x.tag.add == 0);
}

int main(){
	mt19937 rng(123456789);
	FOR(round, 1, 2000){
		int n = rng() % 12 + 1, root = 0;
		LinkCut lct(n + 1);
		vector<set<int>> adj(n + 1);
		vector<ll> val(n + 1);
		FOR(step, 1, 200){
			int u = rng() % n + 1, v = rng() % n + 1;
			int op = rng() % 8, x = (int)(rng() % 21) - 10;
			auto par = parents(adj, u);
			bool same = par[v] != -1;
			if(op == 0 && !same){
				lct.link(u, v);
				adj[u].insert(v); adj[v].insert(u);
				root = 0;
			}else if(op == 1 && adj[u].count(v)){
				lct.cut(u, v);
				adj[u].erase(v); adj[v].erase(u);
				root = 0;
			}else if(op == 2){
				lct.update(u, (ll)x); val[u] = x;
			}else if(op == 3 && same){
				lct.update(u, v, {x});
				for(int w = v; w; w = par[w]) val[w] += x;
				root = 0;
			}else if(op == 4 && same){
				ll sum = 0;
				int size = 0;
				for(int w = v; w; w = par[w]){
					sum += val[w]; size++;
				}
				auto got = lct.get(u, v);
				assert(got.sum == sum && got.size == size);
				root = 0;
			}else if(op == 5){
				assert(lct.connected(u, v) == same);
			}else if(op == 6){
				lct.makeRoot(u); root = u;
			}else if(op == 7 && same && root){
				auto p = parents(adj, root);
				if(p[u] == -1) continue;
				set<int> ancestors;
				for(int w = u; w; w = p[w])
					ancestors.insert(w);
				int w = v;
				while(!ancestors.count(w)) w = p[w];
				assert(lct.lca(u, v) == w);
			}
		}
		FOR(u, 1, n) assert(lct.get(u, u).val == val[u]);
		checkNull(lct);
	}
	FOR(round, 1, 4){
		int n = 2000;
		LinkCut lct(n + 1);
		vector<ll> val(n + 1), pref(n + 1);
		FOR(u, 2, n) lct.link(u - 1, u);
		FOR(step, 1, 1000){
			int u = rng() % n + 1, v = rng() % n + 1;
			if(step % 10 <= 1) u = 1, v = n;
			int l = min(u, v), r = max(u, v);
			if(step % 2){
				int x = (int)(rng() % 21) - 10;
				lct.update(u, v, {x});
				FOR(i, l, r) val[i] += x;
			}else{
				FOR(i, 1, n) pref[i] = pref[i - 1] + val[i];
				auto got = lct.get(u, v);
				assert(got.sum == pref[r] - pref[l - 1]);
				assert(got.size == r - l + 1);
			}
		}
		FOR(u, 1, n) assert(lct.get(u, u).val == val[u]);
		checkNull(lct);
	}
}
