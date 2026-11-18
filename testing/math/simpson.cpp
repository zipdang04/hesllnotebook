#include "../../tex/contents/math/simpson.hpp"
#include <cassert>

int main(){
	const double PI = acos(-1);
	assert(simpson(2, 2, [](double x){ return x; }) == 0);
	assert(abs(simpson(0, 1, [](double x){
		return x * x * x - 2 * x + 1;
	}) - 0.25) < 1e-6);
	assert(abs(simpson(1, 0, [](double x){
		return x * x;
	}) + 1.0 / 3) < 1e-6);
	assert(abs(simpson(PI, 0, [](double x){
		return sin(x);
	}) + 2) < 1e-6);
	assert(abs(simpson(0, PI, [](double x){
		return sin(x);
	}) - 2) < 1e-6);
	assert(abs(simpson(0, 1, [](double x){
		return exp(x);
	}) - (exp(1) - 1)) < 1e-6);
	assert(abs(simpson(-1, 1, [](double x){
		return sqrt(max(0.0, 1 - x * x));
	}) - PI / 2) < 1e-6);
	assert(abs(simpson(-1, 1, [](double x){
		double h = sqrt(max(0.0, 1 - x * x));
		return simpson(-h, h, [&](double y){
			return 1.0 + 0 * x + 0 * y;
		});
	}) - PI) < 1e-6);
	assert(abs(simpson(-1, 1, [](double x){
		return abs(x);
	}) - 1) < 1e-6);
	assert(abs(simpson(0, 1e6, [](double x){
		return x * 1e-6;
	}) - 5e5) < 1e-6);
	return 0;
}
