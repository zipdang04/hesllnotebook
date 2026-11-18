#include "../../tex/contents/graph/hld.hpp"
#include <cassert>

struct Fenwick{
	vector<ll> bit;
	Fenwick(int n): bit(n + 1){}
	void add(int u, ll x){
		for(; u < sz(bit); u += u & -u)
			bit[u] += x;
	}
	ll get(int u){
		ll res = 0;
		for(; u; u -= u & -u)
			res += bit[u];
		return res;
	}
};
int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, q; cin >> n >> q;
	vector<ll> a(n + 1);
	FOR(u, 1, n)
		cin >> a[u];
	vector<vector<int>> g(n + 1);
	FOR(u, 2, n){
		int p; cin >> p; p++;
		g[u].push_back(p); g[p].push_back(u);
	}
	HLD hld(g); Fenwick bit(n);
	FOR(u, 1, n)
		bit.add(hld.pos[u], a[u]);
	assert(hld.sub[1] == n && hld.timer == n);
	while(q--){
		int type, u; cin >> type >> u; u++;
		if(type == 0){
			ll x; cin >> x;
			bit.add(hld.pos[u], x);
		}else{
			int l = hld.pos[u], r = l + hld.sub[u] - 1;
			cout << bit.get(r) - bit.get(l - 1) << '\n';
		}
	}
}
