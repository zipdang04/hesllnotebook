#include "../../tex/contents/ds/rollbackdsu.hpp"
int main(){
	mt19937 rng(1);
	FOR(int, seed, 1, 3000){
		int n = rng() % 10 + 1;
		RollbackDSU uf(n); vector<int> lab(n);
		iota(lab.begin(), lab.end(), 0);
		vector<pair<int, vector<int>>> snap;
		auto fail = [&](){ printf("FAIL seed %d\n", seed); exit(1); };
		EFOR(int, op, 0, 200){
			int r = rng() % 10;
			if (r < 5){
				int a = rng() % n, b = rng() % n;
				if (uf.join(a, b) != (lab[a] != lab[b])) fail();
				int la = lab[a], lb = lab[b];
				for (int &x: lab) if (x == lb) x = la;
			} else if (r < 7) snap.push_back({uf.time(), lab});
			else if (r < 9 && !snap.empty()){
				int k = rng() % snap.size();
				uf.rollback(snap[k].fi); lab = snap[k].se;
				snap.resize(k + (rng() & 1));
			}
			EFOR(int, x, 0, n){
				int c = 0;
				EFOR(int, y, 0, n){
					c += lab[y] == lab[x];
					if ((uf.find(x) == uf.find(y)) != (lab[x] == lab[y])) fail();
				}
				if (uf.size(x) != c) fail();
			}
		}
	}
	puts("OK");
}
