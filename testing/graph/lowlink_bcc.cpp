#include "../../tex/contents/graph/lowlink.hpp"
#include <cassert>

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, m; cin >> n >> m;
	vector<vector<pair<int, int>>> g(n + 1);
	vector<int> eu(m + 1), ev(m + 1);
	FOR(id, 1, m){
		cin >> eu[id] >> ev[id]; eu[id]++; ev[id]++;
		g[eu[id]].push_back({ev[id], id});
		g[ev[id]].push_back({eu[id], id});
	}
	LowLink low(g, m);
	vector<vector<int>> group(low.bccCnt + 1);
	vector<int> cnt(low.bccCnt + 1);
	FOR(id, 1, m){
		int c = low.bcc[id];
		assert(1 <= c && c <= low.bccCnt);
		group[c].push_back(eu[id]);
		group[c].push_back(ev[id]);
		cnt[c]++;
	}
	vector<int> inBcc(n + 1);
	FOR(c, 1, low.bccCnt){
		sort(group[c].begin(), group[c].end());
		group[c].erase(unique(group[c].begin(),
			group[c].end()), group[c].end());
		for(int u: group[c]) inBcc[u]++;
	}
	int iso = 0;
	FOR(u, 1, n) iso += g[u].empty();
	FOR(u, 1, n) assert(low.isCut[u] == (inBcc[u] >= 2));
	FOR(id, 1, m)
		assert(low.isBridge[id] == (cnt[low.bcc[id]] == 1));
	cout << low.bccCnt + iso << "\n";
	FOR(c, 1, low.bccCnt){
		cout << group[c].size();
		for(int u: group[c]) cout << " " << u - 1;
		cout << "\n";
	}
	FOR(u, 1, n) if(g[u].empty())
		cout << "1 " << u - 1 << "\n";
}
