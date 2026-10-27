#include "../../tex/contents/math/simpson.hpp"

int main(){
	// 1. polynomial on [-1, 3]
	{
		auto f = [](double x){ return x * x * x - 2 * x * x + x - 5; };
		auto F = [](double x){ return x * x * x * x / 4.0 - 2.0 * x * x * x / 3.0 + x * x / 2.0 - 5.0 * x; };
		double exact = F(3.0) - F(-1.0);
		double got = simpson(f, -1.0, 3.0);
		if (abs(got - exact) > 1e-9) {
			printf("Check 1 failed: got %.12f expected %.12f\n", got, exact);
			return 1;
		}
	}

	// 2. sin on [0, pi]
	{
		double pi = acos(-1.0);
		double got = simpson([](double x){ return sin(x); }, 0.0, pi);
		if (abs(got - 2.0) > 1e-8) {
			printf("Check 2 failed: got %.12f expected 2.0\n", got);
			return 1;
		}
	}

	// 3. exp on [0, 5]
	{
		double exact = exp(5.0) - 1.0;
		double got = simpson([](double x){ return exp(x); }, 0.0, 5.0);
		if (abs(got - exact) > 1e-8) {
			printf("Check 3 failed: got %.12f expected %.12f\n", got, exact);
			return 1;
		}
	}

	// 4. sqrt endpoint singularity on [-1, 1]
	{
		double pi = acos(-1.0);
		double exact = pi / 2.0;
		double got = simpson([](double x){ return sqrt(max(0.0, 1.0 - x * x)); }, -1.0, 1.0, 1e-9);
		if (abs(got - exact) > 1e-6) {
			printf("Check 4 failed: got %.12f expected %.12f\n", got, exact);
			return 1;
		}
	}

	// 5. 2D nested integral
	{
		double exact = 1.0 + sin(2.0) - (sin(3.0) - sin(1.0));
		auto inner = [](double x){
			return simpson([x](double y){ return x * y + sin(x + y); }, 0.0, 1.0, 1e-9);
		};
		double got = simpson(inner, 0.0, 2.0, 1e-9);
		if (abs(got - exact) > 1e-6) {
			printf("Check 5 failed: got %.12f expected %.12f\n", got, exact);
			return 1;
		}
	}

	// 6. disk indicator nested integral
	{
		double pi = acos(-1.0);
		auto inner = [](double x){
			return simpson([x](double y){ return (double)(x * x + y * y < 1.0); }, -1.0, 1.0, 1e-6);
		};
		double got = simpson(inner, -1.0, 1.0, 1e-6);
		if (abs(got - pi) > 2e-2) {
			printf("Check 6 failed: got %.12f expected %.12f\n", got, pi);
			return 1;
		}
	}

	// 7. narrow spike miss / wider spike caught
	{
		double got1 = simpson([](double x){ return exp(-(x - 0.3) * (x - 0.3) * 1e10); }, 0.0, 1.0);
		if (got1 >= 1e-6) {
			printf("Check 7a failed: got %.12f expected < 1e-6\n", got1);
			return 1;
		}
		double pi = acos(-1.0);
		double exact2 = sqrt(pi) / 100.0;
		double got2 = simpson([](double x){ return exp(-(x - 0.3) * (x - 0.3) * 1e4); }, 0.0, 1.0);
		if (abs(got2 - exact2) > 1e-8) {
			printf("Check 7b failed: got %.12f expected %.12f\n", got2, exact2);
			return 1;
		}
	}

	// 8. a > b and a == b
	{
		double got1 = simpson([](double x){ return x * x; }, 1.0, 0.0);
		if (abs(got1 - (-1.0 / 3.0)) > 1e-9) {
			printf("Check 8a failed: got %.12f expected %.12f\n", got1, -1.0 / 3.0);
			return 1;
		}
		double got2 = simpson([](double x){ return x * x; }, 2.0, 2.0);
		if (got2 != 0.0) {
			printf("Check 8b failed: got %.12f expected 0.0\n", got2);
			return 1;
		}
	}

	// 9. simpsonN cubic exact
	{
		double got = simpsonN([](double x){ return x * x * x; }, 0.0, 2.0, 2);
		if (abs(got - 4.0) > 1e-12) {
			printf("Check 9 failed: got %.12f expected 4.0\n", got);
			return 1;
		}
	}

	// 10. simpsonN exp
	{
		double exact = exp(5.0) - 1.0;
		double got = simpsonN([](double x){ return exp(x); }, 0.0, 5.0, 1000);
		if (abs(got - exact) > 1e-6) {
			printf("Check 10 failed: got %.12f expected %.12f\n", got, exact);
			return 1;
		}
	}

	// kink inside piece
	{
		double exact = 0.26690;
		double got = simpson([](double x){ return abs(x - 0.37); }, 0.0, 1.0);
		if (abs(got - exact) > 1e-7) {
			printf("Kink check failed: got %.12f expected %.12f\n", got, exact);
			return 1;
		}
	}

	puts("ok");
	return 0;
}
