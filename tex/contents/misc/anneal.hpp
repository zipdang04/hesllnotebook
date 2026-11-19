#pragma once
#include "../contest/template.hpp"

// Simulated annealing (minimise) for sec seconds; geometric
// cooling t0 -> t1. Keep state and best in the lambdas.
// propose(): pick a random move, keep it, return its cost
// change (double). commit(): apply the kept move.
// Call: anneal(1.8, 1e3, 1e-3, [&]{ .. }, [&]{ .. });
template<class Propose, class Commit>
void anneal(double sec, double t0, double t1,
		Propose propose, Commit commit){
	static mt19937_64 rng(CURRENT_TIMESTAMP());
	uniform_real_distribution<double> unif(0, 1);
	auto start = chrono::steady_clock::now();
	double t = t0;
	for(ll it = 0;; it++){
		// the clock is slow: read it every 256 moves
		if(it % 256 == 0){
			chrono::duration<double> el =
				chrono::steady_clock::now() - start;
			if(el.count() >= sec)
				return;
			t = t0 * pow(t1 / t0, el.count() / sec);
		}
		double delta = propose();
		if(delta <= 0 || unif(rng) < exp(-delta / t))
			commit();
	}
}
