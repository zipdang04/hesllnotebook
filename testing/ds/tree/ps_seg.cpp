#include "../../../tex/contents/ds/tree/ps_seg.hpp"

template<> constexpr ll Persistent::SegTree<ll>::DEFAULT_VALUE = 0;
template<> inline ll Persistent::SegTree<ll>::process(ll a, ll b) {
	return a+b;
}
Persistent::SegTree<ll> seg;

#define MAX 200'005
int coorX[MAX], coorY[MAX], lenX, lenY;
int n, q;
int x[MAX], y[MAX], w[MAX];

void compress() {
	copy(x+1, x+1+n, coorX+1);
	copy(y+1, y+1+n, coorY+1);
	sort(coorX + 1, coorX + 1 + n);
	sort(coorY + 1, coorY + 1 + n);
	lenX = unique(coorX + 1, coorX + 1 + n) - coorX - 1;
	lenY = unique(coorY + 1, coorY + 1 + n) - coorY - 1;

	FOR(int, i, 1, n) x[i] = lower_bound(coorX + 1, coorX + 1 + lenX, x[i]) - coorX;
	FOR(int, i, 1, n) y[i] = lower_bound(coorY + 1, coorY + 1 + lenY, y[i]) - coorY;
}

vector<pair<int, int>> upd[MAX];
ll arr[MAX];

ll get(int x, int y) {
	if (x == 0 or y == 0) return 0;
	return seg.get(x, 1, y);
}
ll get(int x1, int y1, int x2, int y2) {
	x2 = lower_bound(coorX + 1, coorX + 1 + lenX, x2) - coorX - 1;
	y2 = lower_bound(coorY + 1, coorY + 1 + lenY, y2) - coorY - 1;
	x1 = lower_bound(coorX + 1, coorX + 1 + lenX, x1) - coorX - 1;
	y1 = lower_bound(coorY + 1, coorY + 1 + lenY, y1) - coorY - 1;
	return get(x2, y2) - get(x2, y1) - get(x1, y2) + get(x1, y1);
}

int main()
{
	ios_base::sync_with_stdio(0); cin.tie(0);
	cin >> n >> q;
	FOR(int, i, 1, n) cin >> x[i] >> y[i] >> w[i];
	compress();
	FOR(int, i, 1, n)
		upd[x[i]].push_back({y[i], w[i]});

	Persistent::Range::init(lenY);
	seg.buildTree(arr);
	FOR(int, i, 1, lenY)
		assert(seg.get(0, i, i) == 0);
	FOR(int, x, 1, lenX) {
		seg.addTree(x-1);
		for (auto [y, w]: upd[x]) {
			arr[y] += w;
			seg.update(x, y, arr[y]);
		}
	}

	FOR(int, _, 1, q) {
		int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
		cout << get(x1, y1, x2, y2) << '\n';
	}
}
