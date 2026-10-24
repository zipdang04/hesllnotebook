#pragma once
#include "../contest/template.hpp"

// SPFA with SLF/LLL deque heuristics. The caller supplies the edges:
// adj(u, relax) must call relax(v, w, id) for every usable edge u -> v.
// Returns true if a negative cycle is reachable from s; with detect, dist = -oo on/after it.
struct SPFA {
	static constexpr ll oo = LLONG_MAX / 4;
	vector<ll> dist; vector<int> par;              // par[v] = id of the edge into v, -1 if none

	template<class Adj> bool run(int n, int s, Adj adj, bool detect = false) {
		dist.assign(n, oo); par.assign(n, -1);
		vector<int> len(n), cnt(n); vector<char> inq(n); vector<int> bad;
		const int LIM = sqrt(n);
		deque<int> q{s}; dist[s] = 0; inq[s] = 1;
		__int128 qSum = 0;                         // sum of dist over queued vertices
		while (!q.empty()) {
			while (dist[q.front()] > dist[q.back()]) q.push_back(q.front()), q.pop_front();
			int u = q.front(); q.pop_front(); inq[u] = 0; qSum -= dist[u];
			adj(u, [&](int v, ll w, int id) {
				ll nd = dist[u] + w;
				if (nd >= dist[v]) return;
				qSum += inq[v] ? nd - dist[v] : nd;
				dist[v] = nd; par[v] = id; len[v] = len[u] + 1;
				if (len[v] >= n) { bad.push_back(v); if (!inq[v]) qSum -= nd; return; }   // >= n edges: negative cycle
				if (inq[v]) return;
				inq[v] = 1;
				if (++cnt[v] > LIM || (__int128)nd * (ll)q.size() > qSum) q.push_back(v);
				else if (q.empty() || nd <= dist[q.front()]) q.push_front(v);
				else q.push_back(v);
			});
			if (!bad.empty() && !detect) return true;
		}
		if (bad.empty()) return false;
		for (int v: bad) dist[v] = -oo, q.push_back(v);
		while (!q.empty()) {
			int u = q.front(); q.pop_front();
			adj(u, [&](int v, ll, int) { if (dist[v] != -oo) dist[v] = -oo, q.push_back(v); });
		}
		return true;
	}
};

inline void _example() {
	int n = 4, s = 0;
	vector<vector<array<ll, 3>>> g(n);             // g[u] = {v, w, id}; shuffle each g[u] once
	SPFA sp;
	sp.run(n, s, [&](int u, auto relax) { for (auto [v, w, id]: g[u]) relax(v, w, id); });
}
