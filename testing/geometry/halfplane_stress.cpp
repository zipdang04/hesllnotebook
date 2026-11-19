#include "../../tex/contents/geometry/halfplane.hpp"

template<class T>
ld area(const vector<Point<T>> &p){
	ld res = 0;
	FOR(i, 0, sz(p) - 1){
		auto a = p[i], b = p[(i+1)%sz(p)];
		res += (ld)a.x * b.y - (ld)a.y * b.x;
	}
	return res / 2;
}

ld brute(const vector<pair<PD, PD>> &hs,
		vector<Point<ld>> p){
	for(auto [a, b]: hs){
		Point<ld> u = (Point<ld>)a;
		Point<ld> d = (Point<ld>)b - u;
		vector<Point<ld>> q;
		FOR(i, 0, sz(p) - 1){
			auto s = p[i], t = p[(i+1)%sz(p)];
			ld x = d.cross(s - u), y = d.cross(t - u);
			if(x >= 0) q.push_back(s);
			if((x < 0 && y > 0) || (x > 0 && y < 0))
				q.push_back(s + (t - s) * (x / (x - y)));
		}
		p = move(q);
	}
	return abs(area(p));
}

int main(){
	mt19937 rng(712367);
	auto draw = [&](int lo, int hi){
		return uniform_int_distribution<int>(lo, hi)(rng);
	};
	int radii[] = {3, 20, 1000};
	FOR(it, 0, 999999){
		int r = radii[draw(0, 2)], n = draw(1, 12);
		vector<pair<PD, PD>> hs;
		FOR(i, 0, n - 1){
			if(i && draw(0, 4) == 0){
				auto h = hs[draw(0, i - 1)];
				hs.push_back(h);
				continue;
			}
			bool diagonal = draw(0, 2) == 0;
			int sign = draw(0, 1) ? 1 : -1;
			// tenths are not exact in binary
			int k = draw(0, 1) ? 10 : 1;
			auto coord = [&](){
				return (double)draw(-r * k, r * k) / k;
			};
			PD a, b;
			do{
				a = {coord(), coord()};
				b = {coord(), coord()};
				if(diagonal){
					a.y = sign * a.x;
					b.y = sign * b.x;
				}
			}while(a == b);
			hs.push_back({a, b});
		}
		bool bounded = it % 3 == 0;
		ld bound = bounded ? r + 1 : 1e6L;
		vector<Point<ld>> p{{bound, bound}, {-bound, bound},
			{-bound, -bound}, {bound, -bound}};
		if(bounded)
			FOR(i, 0, 3){
				PD a = (PD)p[i], b = (PD)p[(i+1)%4];
				int pos = draw(0, sz(hs));
				hs.insert(hs.begin() + pos, {a, b});
			}
		ld a = brute(hs, p), b = area(halfPlaneInter(hs));
		ld eps = bounded ? 1e-7L * max(1.L, a)
			: 1e-6L * a + 1e3L;
		bool ok = abs(a - b) <= eps;
		if(bounded || a > 1e3L) ok = ok && b >= -1e-6L;
		if(ok) continue;
		cerr << setprecision(20) << "case " << it
			<< ", bounded " << bounded << '\n';
		cerr << "brute " << a << ", got " << b << '\n';
		cerr << sz(hs) << '\n';
		for(auto [s, t]: hs)
			cerr << s.x << ' ' << s.y << ' '
				<< t.x << ' ' << t.y << '\n';
		return 1;
	}
	return 0;
}
