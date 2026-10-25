#include "../../tex/contents/geometry/circles.hpp"
typedef Point<ld> P;
bool lt(P a, P b){ return abs(a.x - b.x) > 1e-9 ? a.x < b.x : a.y < b.y; }

int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	cout << fixed << setprecision(10);
	ld v[6]; for (ld &t: v) cin >> t;
	vector<P> s;
	for (int k: {1, -1}) for (auto &t: tangents(P(v[0], v[1]), v[2], P(v[3], v[4]), k * v[5])) s.push_back(t.fi);
	sort(s.begin(), s.end(), lt);
	for (P p: s) cout << p.x << ' ' << p.y << '\n';
}
