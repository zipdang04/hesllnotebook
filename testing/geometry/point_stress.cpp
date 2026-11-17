#include "../../tex/contents/geometry/point.hpp"
#include <cassert>

ll crossRaw(PL a, PL b){
	return a.x * b.y - a.y * b.x;
}

bool bruteInter(PL a, PL b, PL c, PL d){
	PL u{b.x - a.x, b.y - a.y};
	PL v{d.x - c.x, d.y - c.y};
	PL w{c.x - a.x, c.y - a.y};
	ll den = crossRaw(u, v);
	ll s = crossRaw(w, v), t = crossRaw(w, u);
	if(den < 0) den = -den, s = -s, t = -t;
	if(den) return 0 <= s && s <= den
		&& 0 <= t && t <= den;
	if(s || t) return false;
	if(!u.x && !u.y && !v.x && !v.y)
		return a.x == c.x && a.y == c.y;
	bool useX = max(abs(u.x), abs(v.x))
		>= max(abs(u.y), abs(v.y));
	ll lo = useX ? min(a.x, b.x) : min(a.y, b.y);
	ll hi = useX ? max(a.x, b.x) : max(a.y, b.y);
	ll otherLo = useX ? min(c.x, d.x) : min(c.y, d.y);
	ll otherHi = useX ? max(c.x, d.x) : max(c.y, d.y);
	return max(lo, otherLo) <= min(hi, otherHi);
}

void checkLine(PL a, PL b, PL p){
	if(a == b) return;
	PD x = PD(a), y = PD(b), z = PD(p), u = y - x;
	double len = hypot(u.x, u.y);
	// 6000 exceeds every point separation in either regime.
	PD ext = u * (6000 / len);
	double want = orient(a, b, p)
		* segDist(x - ext, y + ext, z);
	assert(abs(lineDist(a, b, p) - want) <= 1e-9);
	assert(abs(lineDist(x, y, z) - want) <= 1e-9);
	PD foot = lineProj(x, y, z);
	assert(abs((foot - x).cross(u)) / len <= 1e-9);
	assert(abs((z - foot).dot(u)) / len <= 1e-9);
	PD mirror = lineProj(x, y, z, true);
	assert(((mirror + z) / 2 - foot).dist() <= 1e-9);
}

void checkAngle(PL a, PL b, PL c){
	bool ab = angLess(a, b), ba = angLess(b, a);
	bool bc = angLess(b, c), cb = angLess(c, b);
	bool ac = angLess(a, c), ca = angLess(c, a);
	assert(!angLess(a, a));
	assert(!(ab && ba));
	if(ab && bc) assert(ac);
	if(!ab && !ba && !bc && !cb) assert(!ac && !ca);
	assert(!angLess(a, a * 2));
	assert(!angLess(a * 2, a));
	ld alpha = atan2l((ld)a.y, (ld)a.x);
	ld beta = atan2l((ld)b.y, (ld)b.x);
	if(abs(alpha - beta) > 1e-9) assert(ab == (alpha < beta));
}

int main(){
	mt19937 rng(712367);
	double pi = acos((double)-1);
	uniform_real_distribution<double> angle(-pi, pi);
	for(int bound: {5, 1000}){
		uniform_int_distribution<int> coord(-bound, bound);
		FOR(it, 0, 99999){
			PL a{coord(rng), coord(rng)};
			PL b{coord(rng), coord(rng)};
			PL c{coord(rng), coord(rng)};
			PL d{coord(rng), coord(rng)};
			if(it % 8 == 0) b = a;
			if(it % 8 == 1) d = c;
			if(it % 8 == 2) c = b;
			if(it % 8 == 3){
				b.y = a.y;
				c.y = a.y;
				d.y = a.y;
			}
			PD x = PD(a), y = PD(b), z = PD(c), w = PD(d);
			ll side = crossRaw(b - a, c - a);
			int turn = (side > 0) - (side < 0);
			assert(orient(a, b, c) == turn);
			assert(orient(x, y, z) == turn);
			bool on = onSegment(a, b, c);
			assert(on == bruteInter(a, b, c, c));
			assert(onSegment(x, y, z) == on);
			bool hit = bruteInter(a, b, c, d);
			assert(segInter(a, b, c, d) == hit);
			assert(segInter(x, y, z, w) == hit);
			if(!(a == b) && !(c == d)){
				auto res = lineInter(x, y, z, w);
				ll den = crossRaw(b - a, d - c);
				int status = den ? 1 : (side ? 0 : -1);
				assert(res.fi == status);
				if(status != 1) assert(res.se == PD{});
				if(hit && status == 1){
					assert(segDist(x, y, res.se) <= 1e-9);
					assert(segDist(z, w, res.se) <= 1e-9);
				}
			}
			checkLine(a, b, c);
			assert(abs(segDist(x, x, z)
				- hypot(z.x - x.x, z.y - x.y)) <= 1e-12);
			if(a.x || a.y){
				assert(abs(x.unit().dist() - 1) <= 1e-12);
				PD rotated = x.rotate(angle(rng));
				assert(abs(rotated.dist() - x.dist()) <= 1e-12);
				assert((x.rotate(pi / 2) - x.perp()).dist()
					<= 1e-12);
			}
			checkAngle(a, b, c);
		}
	}
}
