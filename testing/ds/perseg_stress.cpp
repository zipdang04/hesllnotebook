#include "../../tex/contents/ds/perseg.hpp"
#include <cassert>

int main(){
	mt19937_64 rng(123456789);
	FOR(n, 1, 20){
		int limit = 1, leaves = 1;
		while(leaves < n){
			leaves *= 2;
			limit++;
		}
		FOR(round, 1, 30){
			vector<PerSeg::Node> a(n);
			vector<vector<ll>> ver(1, vector<ll>(n));
			FOR(i, 0, n - 1){
				ver[0][i] = (int)(rng() % 2001) - 1000;
				a[i] = {ver[0][i]};
			}
			PerSeg s(a);
			FOR(step, 1, 100){
				int v = rng() % sz(ver);
				int p = rng() % n;
				ll x = (int)(rng() % 2001) - 1000;
				int before = sz(s.node);
				int w = s.update(v, p, {x});
				assert(w == sz(ver));
				assert(sz(s.node) > before);
				assert(sz(s.node) - before <= limit);
				ver.push_back(ver[v]);
				ver.back()[p] = x;
				int first = rng() % sz(ver);
				int last = first;
				if(step % 50 == 0){
					first = 0;
					last = sz(ver) - 1;
				}
				before = sz(s.node);
				FOR(u, first, last)
					FOR(l, 0, n){
						ll sum = 0;
						FOR(r, l - 1, n - 1){
							if(r >= l) sum += ver[u][r];
							assert(s.get(u, l, r).sum == sum);
						}
					}
				assert(sz(s.node) == before);
			}
		}
	}
}
