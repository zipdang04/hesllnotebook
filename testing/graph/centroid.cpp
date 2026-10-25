#include "../../tex/contents/graph/centroid.hpp"
#include "../../tex/contents/math/fft.hpp"

vector<ll> ans;
void _add(vector<ll> a, int sgn){ // ans += sgn * (a * a)
	vector<ll> b;
	if (a.size() <= 64){
		b.assign(2 * a.size() - 1, 0);
		EFOR(int, i, 0, (int)a.size()) EFOR(int, j, 0, (int)a.size()) b[i + j] += a[i] * a[j];
	} else {
		vector<double> d(a.begin(), a.end()), r = conv(d, d);
		b.resize(r.size()); EFOR(int, i, 0, (int)r.size()) b[i] = llround(r[i]);
	}
	EFOR(int, i, 0, (int)min(b.size(), ans.size())) ans[i] += sgn * b[i];
}
int main(){
	int n; read(n);
	Centroid cd(n);
	EFOR(int, i, 0, n - 1){ int a, b; read(a, b); cd.addEdge(a, b); }
	ans.assign(n, 0);
	auto cnt = [&](int r, int p){
		vector<ll> a;
		for (int u: cd.bfs(r, p)){ if (cd.dep[u] >= (int)a.size()) a.resize(cd.dep[u] + 1); a[cd.dep[u]]++; }
		return a;
	};
	cd.run([&](int c){
		_add(cnt(c, -1), 1);
		for (int v: cd.g[c]) if (!cd.rem[v]) _add(cnt(v, c), -1);
	});
	string out;
	EFOR(int, i, 1, n) out += to_string(ans[i] / 2) + (i + 1 < n ? " " : "");
	puts(out.c_str());
}
