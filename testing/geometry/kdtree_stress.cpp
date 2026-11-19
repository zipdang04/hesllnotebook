#include "../../tex/contents/geometry/closestpair.hpp"
#include "../../tex/contents/geometry/kdtree.hpp"
#include <chrono>

ll distance2(PL a, PL b){
	ll dx = a.x - b.x, dy = a.y - b.y;
	return dx * dx + dy * dy;
}

void fail(const char *what, const vector<PL> &v,
		PL p, PL q, ll got, ll want){
	cerr << what << '\n';
	cerr << "first/query: " << p.x << ' ' << p.y << '\n';
	cerr << "second/result: " << q.x << ' ' << q.y << '\n';
	cerr << "got " << got << ", want " << want << '\n';
	cerr << sz(v) << '\n';
	for(PL a: v) cerr << a.x << ' ' << a.y << '\n';
	exit(1);
}

ll bruteClosest(const vector<PL> &v){
	ll best = LLONG_MAX;
	FOR(i, 0, sz(v) - 1)
		FOR(j, 0, i - 1)
			best = min(best, distance2(v[i], v[j]));
	return best;
}

ll checkPair(const vector<PL> &v, pair<PL, PL> res){
	auto [a, b] = res;
	int ca = count(all(v), a), cb = count(all(v), b);
	ll d = distance2(a, b);
	if(!ca || !cb || (a == b && ca < 2)
			|| (a - b).dist2() != d)
		fail("closestPair result", v, a, b,
			(a - b).dist2(), d);
	return d;
}

ll checkClosest(const vector<PL> &v){
	ll want = bruteClosest(v);
	auto res = closestPair(v);
	ll got = checkPair(v, res);
	if(got != want)
		fail("closestPair distance", v, res.fi, res.se,
			got, want);
	return want;
}

void checkNearest(const vector<PL> &v, PL p,
		pair<ll, PL> res){
	ll want = LLONG_MAX;
	bool found = false;
	for(PL q: v){
		want = min(want, distance2(p, q));
		if(q == res.se) found = true;
	}
	if(res.fi != want || !found
			|| distance2(p, res.se) != res.fi
			|| (p - res.se).dist2() != res.fi)
		fail("nearest", v, p, res.se, res.fi, want);
}

int main(){
	mt19937_64 rng(712367);
	const ll limit = 1000000000;
	vector<PL> edge{{-limit, -limit}};
	PL corner{limit, limit};
	KdTree one(edge);
	checkNearest(edge, corner, one.nearest(corner));
	edge.push_back(corner);
	checkClosest(edge);
	for(ll bound: {3LL, 1000LL, limit}){
		uniform_int_distribution<ll> coord(-bound, bound);
		FOR(it, 0, 2999){
			int n = 1 + (int)(rng() % 60);
			vector<PL> v(n);
			PL c{coord(rng), coord(rng)};
			for(PL &p: v){
				p = {coord(rng), coord(rng)};
				if(it % 5 == 1) p.x = c.x;
				if(it % 5 == 2) p.y = c.y;
				if(it % 5 == 3) p = c;
			}
			if(it % 5 == 4 && n > 1) v.back() = v[0];
			if(n >= 2) checkClosest(v);
			KdTree kd(v);
			FOR(j, 0, 19){
				PL p{coord(rng), coord(rng)};
				if(j % 3 == 0) p = v[rng() % n];
				if(j % 3 == 2){
					p.x = (j / 3 & 1) ? limit : -limit;
					p.y = (j / 3 & 2) ? limit : -limit;
				}
				checkNearest(v, p, kd.nearest(p));
			}
		}
	}
	const int n = 200000;
	uniform_int_distribution<ll> coord(-limit, limit);
	vector<PL> v(n), queries(n);
	for(PL &p: v) p = {coord(rng), coord(rng)};
	for(PL &p: queries) p = {coord(rng), coord(rng)};
	vector<pair<ll, PL>> answers(n);
	auto start = chrono::steady_clock::now();
	KdTree kd(v);
	FOR(i, 0, n - 1) answers[i] = kd.nearest(queries[i]);
	chrono::duration<double> elapsed
		= chrono::steady_clock::now() - start;
	cerr << "kd build + queries: " << elapsed.count()
		<< " s" << '\n';
	FOR(i, 0, n - 1){
		ll want = distance2(queries[i], answers[i].se);
		if(answers[i].fi != want)
			fail("nearest distance", v, queries[i],
				answers[i].se, answers[i].fi, want);
	}
	FOR(i, 0, 999){
		int j = i * (n / 1000);
		checkNearest(v, queries[j], answers[j]);
	}
	start = chrono::steady_clock::now();
	auto res = closestPair(v);
	elapsed = chrono::steady_clock::now() - start;
	cerr << "closestPair: " << elapsed.count() << " s"
		<< '\n';
	ll got = checkPair(v, res);
	vector<PL> prefix(v.begin(), v.begin() + 3000);
	ll want = checkClosest(prefix);
	if(got > want)
		fail("closestPair exceeds prefix minimum", v,
			res.fi, res.se, got, want);
	return 0;
}
