#pragma once
#include "../contest/template.hpp"

// Min-cost assignment (Hungarian, shortest augmenting
// paths): rows 1..n to distinct columns 1..m, n <= m.
// O(n^2 m). No recursion.
pair<ll, vector<int>> hungarian(
		const vector<vector<ll>> &a){
	int i = sz(a), m = sz(a[0]) - 1, c = 0, s, r = 0;
	vector<ll> dist(m + 1), pot(m + 1);
	vector<int> match(i), rev(m + 1), cols(m + 1),
		prev(m + 1);
	ll d = 0, nd, cost = 0;
	while(--i){
		FOR(j, 1, m)
			dist[j] = a[i][j], cols[j] = j, prev[j] = i;
		for(s = 1;;){
			FOR(j, s, m){
				c = cols[j], nd = dist[c] - pot[c];
				if(j == s || d > nd) d = nd, swap(cols[s], cols[j]);
			}
			r = rev[c = cols[s++]];
			if(!r) break;
			FOR(j, s, m){
				int k = cols[j];
				nd = a[r][k] - a[r][c] + dist[c];
				if(minimize(dist[k], nd)) prev[k] = r;
			}
		}
		cost += dist[c];
		while(--s)
			pot[cols[s]] = dist[cols[s]] - d;
		for(; r != i; swap(c, match[r]))
			r = rev[c] = prev[c];
	}
	return {cost, match};
}
