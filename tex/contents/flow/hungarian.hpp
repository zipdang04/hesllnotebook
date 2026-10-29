#pragma once
#include "../contest/template.hpp"

// min-cost assignment of every row to a distinct column, n <= m (scipy LSAP / kactl)
struct Hungarian{
	int n, m; vector<vector<ll>> c;   // fill c[i][j] = cost of row i -> column j
	Hungarian(int n, int m): n(n), m(m), c(n, vector<ll>(m)){}
	pair<ll, vector<int>> run(){      // {min cost, match}, row i -> column match[i]
		vector<ll> dist(m), pot(m); vector<int> match(n), rev(m, -1), cols(m), prv(m);
		ll d = 0, nd, cost = 0; int col, s, r;
		REV(int, i, n - 1, 0){
			FOR(int, j, 0, m - 1) dist[j] = c[i][j], cols[j] = j, prv[j] = i;
			for (s = 0;;){   // Dijkstra over columns, cols[0..s) settled
				FOR(int, j, s, m - 1){
					col = cols[j]; nd = dist[col] - pot[col];
					if (j == s || d > nd) d = nd, swap(cols[s], cols[j]);
				}
				if ((r = rev[col = cols[s++]]) == -1) break;   // free column reached
				FOR(int, j, 0, m - 1) if (dist[j] > (nd = c[r][j] - c[r][col] + dist[col])) dist[j] = nd, prv[j] = r;
			}
			cost += dist[col];
			while (s--) pot[cols[s]] = dist[cols[s]] - d;
			for (; r != i; swap(col, match[r])) r = rev[col] = prv[col];   // augment
		}
		return {cost, match};
	}
};
