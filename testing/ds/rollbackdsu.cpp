#include "../../tex/contents/ds/rollbackdsu.hpp"
int main(){
	int n, q; read(n, q);
	vector<int> t(q), u(q), v(q); vector<vector<int>> ch(q + 1); // node k+1 = version k, node 0 = G_{-1}
	FOR(int, i, 0, q - 1){ int k; read(t[i], k, u[i], v[i]); ch[k + 1].push_back(i); }
	RollbackDSU uf(n); vector<int> ans(q, -1);
	auto dfs = [&](auto self, int x) -> void {
		for (int i: ch[x]){
			if (t[i]){ ans[i] = uf.find(u[i]) == uf.find(v[i]); continue; }
			int tm = uf.time(); uf.join(u[i], v[i]); self(self, i + 1); uf.rollback(tm);
		}
	};
	dfs(dfs, 0);
	string out; FOR(int, i, 0, q - 1) if (t[i]) out += char('0' + ans[i]), out += '\n';
	fputs(out.c_str(), stdout);
}
