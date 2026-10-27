#include "../../tex/contents/geometry/circles.hpp"
typedef Point<ld> P;
bool lt(P a, P b){ return abs(a.x - b.x) > 1e-9 ? a.x < b.x : a.y < b.y; }

int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	cout << fixed << setprecision(10);
	ld px, py, x, y, r; cin >> px >> py >> x >> y >> r;
	vector<P> s;
	for (auto &t: tangents(P(x, y), r, P(px, py), (ld)0)) s.push_back(t.fi);
	sort(s.begin(), s.end(), lt);
	for (P p: s) cout << p.x << ' ' << p.y << '\n';
}
