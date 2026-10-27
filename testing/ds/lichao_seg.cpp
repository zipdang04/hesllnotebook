#include "../../tex/contents/ds/lichao.hpp"
int main(){
	int n, q; read(n, q);
	vector<array<ll, 5>> a(n + q); vector<ll> xs;
	for (int i = 0; i < n + q; i++){
		ll t = 0; if (i >= n) read(t);
		a[i][0] = t;
		if (t == 0) read(a[i][1], a[i][2], a[i][3], a[i][4]); else read(a[i][1]), xs.push_back(a[i][1]);
	}
	LiChao lc(xs); string out;
	for (auto& v: a){
		if (v[0] == 0) lc.add(v[3], v[4], v[1], v[2]);
		else { ll r = lc.query(v[1]); out += r == LLONG_MAX ? "INFINITY" : to_string(r); out += '\n'; }
	}
	fputs(out.c_str(), stdout);
}
