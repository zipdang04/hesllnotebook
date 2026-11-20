#pragma once
#include "../contest/template.hpp"

// Shortest paths from s, any weights: SPFA with SLF + LLL.
// 1..n, g[u] = {v, w}. No O(VE) bound.
struct SPFA{
	vector<ll> dist; vector<int> par; bool negCycle = false;
	SPFA(const vector<vector<pair<int, ll>>> &g, int s):
			dist(g.size(), oo), par(g.size()){
		int n = sz(g) - 1;
		vector<int> len(sz(g)), relaxCnt(sz(g)), bad;
		vector<char> inq(sz(g)); const int LIM = sqrt(n);
		deque<int> q{s}; dist[s] = 0; inq[s] = 1;
		__int128 qSum = 0; // queued sum can exceed ll
		while(!q.empty()){
			while(dist[q.front()] > dist[q.back()])
				q.push_back(q.front()), q.pop_front();
			int u = q.front(); q.pop_front(); inq[u] = 0;
			ll du = dist[u]; int lu = len[u]; qSum -= du;
			if(lu >= n) continue; // in bad already
			for(auto [v, w]: g[u]){
				ll nd = du + w; if(nd >= dist[v]) continue;
				if(inq[v]) qSum += nd - dist[v];
				dist[v] = nd; par[v] = u; len[v] = lu + 1;
				if(len[v] >= n) bad.push_back(v);
				if(inq[v] || len[v] >= n) continue;
				if(++relaxCnt[v] > LIM
					|| (__int128)nd * sz(q) > qSum) q.push_back(v);
				else if(q.empty() || nd <= dist[q.front()])
					q.push_front(v);
				else q.push_back(v);
				qSum += nd; inq[v] = 1;
			}
		}
		negCycle = !bad.empty();
		for(int v: bad) if(dist[v] != -oo)
			dist[v] = -oo, q.push_back(v);
		while(!q.empty()){
			int u = q.front(); q.pop_front();
			for(auto [v, w]: g[u]) if(dist[v] != -oo)
				dist[v] = -oo, q.push_back(v);
		}
	}
};
