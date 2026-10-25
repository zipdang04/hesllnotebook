#pragma once
#include "../contest/template.hpp"
// integral of f over [a, b], absolute error ~ eps (adaptive); f: double -> double
template<class F> double simpson(F f, double a, double b, double eps = 1e-9){
	auto S = [&](double l, double r, double fl, double fm, double fr){ return (fl + 4 * fm + fr) * (r - l) / 6; };
	auto rec = [&](auto &&self, double l, double r, double fl, double fm, double fr, double s, double e) -> double{
		double m = (l + r) / 2, lm = (l + m) / 2, mr = (m + r) / 2, flm = f(lm), fmr = f(mr);
		double s1 = S(l, m, fl, flm, fm), s2 = S(m, r, fm, fmr, fr), t = s1 + s2;
		if (abs(t - s) <= 15 * e || abs(r - l) < 1e-12) return t + (t - s) / 15;
		return self(self, l, m, fl, flm, fm, s1, e / 2) + self(self, m, r, fm, fmr, fr, s2, e / 2);
	};
	const int P = 16; double h = (b - a) / P, res = 0;  // 16 initial pieces: avoids false early stops
	EFOR(int, i, 0, P){
		double l = a + i * h, r = l + h, fl = f(l), fm = f((l + r) / 2), fr = f(r);
		res += rec(rec, l, r, fl, fm, fr, S(l, r, fl, fm, fr), eps / P);
	}
	return res;
}
// fixed-n composite Simpson (n even), error O(h^4 f'''')
template<class F> double simpsonN(F f, double a, double b, int n = 1000){
	double h = (b - a) / n, v = f(a) + f(b);
	EFOR(int, i, 1, n) v += f(a + i * h) * (i & 1 ? 4 : 2);
	return v * h / 3;
}
