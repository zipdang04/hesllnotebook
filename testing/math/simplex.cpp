#include "../../tex/contents/math/simplex.hpp"
#include <cassert>

int main(){
	Simplex p({{1, -1}, {-1, 1}, {-1, -2}},
		{1, 1, -4}, {-1, -1});
	assert(abs(p.val + 7.0 / 3) < 1e-9);
	assert(sz(p.x) == 2);
	assert(abs(p.x[0] - 2.0 / 3) < 1e-9);
	assert(abs(p.x[1] - 5.0 / 3) < 1e-9);
	assert(p.m == 3 && p.n == 2);
	assert(sz(p.N) == 3 && sz(p.B) == 3);
	assert(sz(p.D) == 5 && sz(p.D[0]) == 4);
	assert(p.EPS == 1e-8);
	assert(p.ltj({0, 0, 0}, 0, -1));
	assert(p.ltj({0, 0, 0}, 0, 1) == (p.N[0] < p.N[1]));
	Simplex big({{1}}, {1e9}, {1e9});
	assert(big.val == 1e18 && big.val != INFINITY);
	assert(big.val != -INFINITY && sz(big.x) == 1);
	Simplex u({}, {}, {1});
	assert(u.val == INFINITY && sz(u.x) == 1);
	assert(u.x[0] >= -1e-9);
	u = Simplex({}, {}, {-1});
	assert(abs(u.val) < 1e-9);
	assert(u.x == vector<double>{0});
	u = Simplex({{1}}, {-1}, {1});
	assert(u.val == -INFINITY && u.x.empty());
	u = Simplex({{1, -1}}, {1}, {1, 1});
	assert(u.val == INFINITY && sz(u.x) == 2);
	assert(u.x[0] - u.x[1] <= 1 + 1e-9);
	assert(u.x[0] >= -1e-9 && u.x[1] >= -1e-9);
	Simplex q({{1, 1}, {-1, -1}, {1, 0}},
		{3, -3, 1}, {-1, -2});
	assert(abs(q.val + 5) < 1e-9);
	assert(sz(q.x) == 2);
	assert(abs(q.x[0] - 1) < 1e-9);
	assert(abs(q.x[1] - 2) < 1e-9);
	u = Simplex({{0.25, -8, -1, 9}, {0.5, -12, -0.5, 3},
		{0, 0, 1, 0}}, {0, 0, 1}, {0.75, -20, 0.5, -6});
	assert(abs(u.val - 1.25) < 1e-9);
	u = Simplex({{1, 1}, {1, -1}, {-1, 1}, {1, 0}},
		{0, 0, 0, 0}, {1, 1});
	assert(abs(u.val) < 1e-9);
	p = Simplex({{1}}, {3}, {2});
	assert(abs(p.val - 6) < 1e-9);
	assert(sz(p.x) == 1 && abs(p.x[0] - 3) < 1e-9);
	assert(abs(q.val + 5) < 1e-9);
	assert(abs(q.x[0] - 1) < 1e-9);
	assert(abs(q.x[1] - 2) < 1e-9);
	return 0;
}
