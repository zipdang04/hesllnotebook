#include "../../tex/contents/graph/centroid.hpp"
#include <cassert>
#include "../../tex/contents/math/fft.hpp"

const Centroid *pc;
const vector<vector<int>> *pg;
vector<double> cnt;
void collect(int u, int p, int d, int lv){
	if(sz(cnt) <= d) cnt.resize(d + 1);
	cnt[d]++;
	for(int v: (*pg)[u]) if(v != p && pc->level[v] > lv)
		collect(v, u, d + 1, lv);
}
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	Centroid empty{vector<vector<int>>(1)};
	{
		vector<vector<int>> g(4);
		g[0] = {1}; g[1] = {2}; g[2] = {1};
		Centroid cd(g);
		FOR(u, 1, 3) assert(cd.taken[u] == 1);
		assert(cd.par[3] == 0 && cd.level[3] == 0);
		int t = cd.par[1] == 0 ? 1 : 2, o = 3 - t;
		assert(cd.par[t] == 0 && cd.par[o] == t);
		assert(cd.level[t] == 0 && cd.level[o] == 1);
	}
	{
		vector<vector<int>> g(6);
		FOR(u, 1, 4){
			g[u].push_back(u + 1); g[u + 1].push_back(u);
		}
		Centroid cd(g);
		assert(cd.par[3] == 0 && cd.level[3] == 0);
		for(int a: {1, 4}){
			int b = a + 1, t = cd.par[a] == 3 ? a : b;
			int o = a + b - t;
			assert(cd.par[t] == 3 && cd.level[t] == 1);
			assert(cd.par[o] == t && cd.level[o] == 2);
		}
		Centroid cd2(vector<vector<int>>(1));
		cd2 = Centroid(g);
		assert(cd2.par == cd.par);
	}
	int n; cin >> n;
	vector<vector<int>> g(n + 1);
	FOR(i, 1, n - 1){
		int u, v; cin >> u >> v; u++; v++;
		g[u].push_back(v); g[v].push_back(u);
	}
	Centroid cd(g);
	pc = &cd; pg = &g;
	vector<ll> ans(n + 1);
	FOR(c, 1, n){
		assert(cd.level[c] <= log2(n));
		int p = cd.par[c];
		assert(cd.level[c] == (p ? cd.level[p] + 1 : 0));
		cnt.assign(1, 0);
		collect(c, 0, 0, cd.level[c]);
		auto a = conv(cnt, cnt);
		FOR(d, 1, sz(a) - 1) ans[d] += llround(a[d]);
		for(int v: g[c]) if(cd.level[v] > cd.level[c]){
			cnt.assign(1, 0);
			collect(v, c, 1, cd.level[c]);
			auto b = conv(cnt, cnt);
			FOR(d, 1, sz(b) - 1) ans[d] -= llround(b[d]);
		}
	}
	FOR(d, 1, n - 1)
		cout << ans[d] / 2 << " \n"[d == n - 1];
	if(n == 1) cout << '\n';
}
