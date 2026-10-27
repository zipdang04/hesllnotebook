#include "../../tex/contents/flow/costscaling.hpp"
#include "../../tex/contents/flow/dinitzwscaling.hpp"
#include "../../tex/contents/ds/lazyseg.hpp"
#include "../../tex/contents/graph/euler.hpp"
#include "../../tex/contents/graph/hld.hpp"

int main() {
	// 1. costscaling solve() twice gives equal results
	{
		MinCostCirculation mc(3);
		mc.addEdge(0, 1, 0, 5, -2);
		mc.addEdge(1, 2, 0, 5, -3);
		mc.addEdge(2, 0, 0, 3, 1);
		auto res1 = mc.solve();
		ll f0_1 = mc.flowOf(0);
		auto res2 = mc.solve();
		ll f0_2 = mc.flowOf(0);
		assert(res1.first == true && res2.first == true);
		assert(res1.second == res2.second);
		assert(f0_1 == f0_2);

		mt19937 rng(12345);
		for (int it = 0; it < 50; it++) {
			int n = rng() % 5 + 2, m = rng() % 8;
			MinCostCirculation mc1(n);
			for (int i = 0; i < m; i++) {
				int u = rng() % n, v = rng() % n;
				ll cap = rng() % 10 + 1, cost = (ll)(rng() % 21) - 10;
				mc1.addEdge(u, v, 0, cap, cost);
			}
			auto a = mc1.solve();
			vector<ll> flows_a(m);
			for (int i = 0; i < m; i++) flows_a[i] = mc1.flowOf(i);

			auto b = mc1.solve();
			vector<ll> flows_b(m);
			for (int i = 0; i < m; i++) flows_b[i] = mc1.flowOf(i);

			assert(a.first == b.first);
			assert(a.second == b.second);
			assert(flows_a == flows_b);
		}
	}

	// 2. Dinic s == t returns 0
	{
		Flow f;
		f.init(2, 0, 0);
		f.addEdge(0, 1, 5);
		assert(f.calculate() == 0);

		Flow f2;
		f2.init(5, 3, 3);
		f2.addEdge(3, 1, 10);
		f2.addEdge(1, 3, 10);
		f2.addEdge(3, 3, 10);
		assert(f2.calculate() == 0);
	}

	// 3. LazySeg on an empty vector constructs
	{
		LazySeg<_exampleAddMin> s(vector<ll>{});
	}

	// 4. Euler with n == 0
	{
		Euler e;
		e.init(0, false);
		vector<int> vs, es;
		bool ok = e.solve(vs, es);
		assert(ok && vs.empty() && es.empty());

		e.init(0, true);
		ok = e.solve(vs, es);
		assert(ok && vs.empty() && es.empty());
	}

	// 5. HLD build on a forest does not crash
	{
		HLD h(3);
		h.addEdge(0, 1);
		h.build(0); // vertex 2 is disconnected

		HLD h2(10);
		h2.addEdge(1, 2);
		h2.addEdge(2, 3);
		h2.addEdge(4, 5);
		h2.addEdge(6, 7);
		h2.addEdge(7, 8);
		h2.addEdge(8, 9);
		for (int r = 0; r < 10; r++) {
			h2.build(r);
		}

		// Random forests (acyclic)
		mt19937 rng(54321);
		for (int it = 0; it < 100; it++) {
			int n = rng() % 20 + 2;
			HLD hf(n);
			vector<int> p(n);
			iota(p.begin(), p.end(), 0);
			auto find = [&](auto self, int x) -> int {
				return p[x] == x ? x : p[x] = self(self, p[x]);
			};
			int m = rng() % n;
			for (int i = 0; i < m; i++) {
				int u = rng() % n, v = rng() % n;
				int pu = find(find, u), pv = find(find, v);
				if (pu != pv) {
					p[pu] = pv;
					hf.addEdge(u, v);
				}
			}
			int r = rng() % n;
			hf.build(r);
		}
	}

	puts("all edge cases passed");
	return 0;
}
