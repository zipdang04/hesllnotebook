#include "../../tex/contents/ds/lichao.hpp"
int main(){
	int n, q; read(n, q);
	vector<array<ll, 3>> a(n + q); vector<ll> xs;
	for (int i = 0; i < n + q; i++){
		ll t = 0; if (i >= n) read(t);
		a[i][0] = t;
		if (t == 0) read(a[i][1], a[i][2]); else read(a[i][1]), xs.push_back(a[i][1]);
	}
	LiChao lc(xs); string out;
	for (auto& v: a){
		if (v[0] == 0) lc.add(v[1], v[2]);
		else out += to_string(lc.query(v[1])) + '\n';
	}
	fputs(out.c_str(), stdout);
}
