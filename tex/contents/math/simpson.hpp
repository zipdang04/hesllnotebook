#pragma once
#include "../contest/template.hpp"

// Adaptive Simpson of f: double(double), error ~eps.
// Call: simpson(0, 1, [&](double x){ return x * x; });
// A double integral: call simpson inside f.
template<class Func> double simpsonRec(Func &f, double a,
		double b, double eps, double whole){
	double c = (a + b) / 2;
	double l = (f(a) + 4 * f((a + c) / 2) + f(c))
		* (c - a) / 6;
	double r = (f(c) + 4 * f((c + b) / 2) + f(b))
		* (b - c) / 6;
	double t = l + r;
	// Absolute width also stops on reversed intervals.
	if(abs(t - whole) <= 15 * eps || abs(b - a) < 1e-10)
		return t + (t - whole) / 15;
	return simpsonRec(f, a, c, eps / 2, l)
		+ simpsonRec(f, c, b, eps / 2, r);
}
template<class Func> double simpson(double a, double b,
		Func f, double eps = 1e-9){
	double whole = (f(a) + 4 * f((a + b) / 2) + f(b))
		* (b - a) / 6;
	return simpsonRec(f, a, b, eps, whole);
}
