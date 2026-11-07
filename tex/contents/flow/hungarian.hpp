#pragma once
#include "../contest/template.hpp"
struct Hungarian{
	int n, m;
	vector<vector<ll>> c;
	Hungarian(int n, int m): n(n), m(m),
		c(n, vector<ll>(m)){}
	pair<ll, vector<int>> run(){
		vector<ll> dist(m), pot(m);
		vector<int> match(n), rev(m, -1);
		vector<int> cols(m), prv(m);
		ll cost = 0;
		REV(int, i, n - 1, 0){
			FOR(int, j, 0, m - 1){
				dist[j] = c[i][j];
				cols[j] = j; prv[j] = i;
			}
			int s = 0, col = -1, r = -1;
			ll d = 0;
			// Dijkstra over columns; cols[0..s) are settled
			while (true){
				FOR(int, j, s, m - 1){
					ll nd = dist[cols[j]] - pot[cols[j]];
					if (j == s || d > nd){
						d = nd; swap(cols[s], cols[j]);
					}
				}
				col = cols[s++];
				r = rev[col];
				if (r == -1) break;
				FOR(int, j, 0, m - 1){
					ll nd = c[r][j] - c[r][col] + dist[col];
					if (dist[j] > nd){
						dist[j] = nd; prv[j] = r;
					}
				}
			}
			cost += dist[col];
			FOR(int, k, 0, s - 1)
				pot[cols[k]] = dist[cols[k]] - d;
			// augment along prv back to row i
			while (r != i){
				r = prv[col]; rev[col] = r;
				swap(col, match[r]);
			}
		}
		return {cost, match};
	}
};
