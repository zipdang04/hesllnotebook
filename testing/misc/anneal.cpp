#include "../../tex/contents/misc/anneal.hpp"
#include <cassert>

int main(){
	int hits = 0;
	FOR(s, 1, 50){
		mt19937 gen(s);
		vector<pll> pts(8);
		for(auto &[x, y]: pts){
			x = gen() % 1000; y = gen() % 1000;
		}
		double dist[8][8];
		FOR(i, 0, 7)
			FOR(j, 0, 7)
				dist[i][j] = hypot(pts[i].fi - pts[j].fi,
					pts[i].se - pts[j].se);
		vi p(8); iota(all(p), 0);
		auto length = [&](){
			double res = 0;
			FOR(i, 0, 7)
				res += dist[p[i]][p[(i+1)%8]];
			return res;
		};
		double brute = 1e100;
		do
			minimize(brute, length());
		while(next_permutation(p.begin() + 1, p.end()));
		double cur = length(), best = cur, delta = 0;
		int a = 0, b = 0;
		auto propose = [&](){
			a = gen() % 8; b = gen() % 8;
			if(a > b)
				swap(a, b);
			delta = 0;
			if(a == b || (a == 0 && b == 7))
				return delta;
			int u = p[(a+7)%8], v = p[(b+1)%8];
			delta = dist[u][p[b]] + dist[p[a]][v]
				- dist[u][p[a]] - dist[p[b]][v];
			return delta;
		};
		auto commit = [&](){
			reverse(p.begin() + a, p.begin() + b + 1);
			cur += delta; minimize(best, cur);
		};
		auto start = chrono::steady_clock::now();
		anneal(0.02, 100, 0.01, propose, commit);
		chrono::duration<double> elapsed =
			chrono::steady_clock::now() - start;
		assert(elapsed.count() >= 0.02);
		assert(elapsed.count() <= 0.02 + 0.05);
		hits += abs(best - brute) < 1e-6;
	}
	// Statistical check: the draft got 50/50 every time.
	assert(hits >= 48);
	cout << hits << "/50 optima reached" << '\n';
}
