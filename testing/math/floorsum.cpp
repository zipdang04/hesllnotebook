#include "../../tex/contents/math/floorsum.hpp"

int main(){
	int t; read(t);
	string out;
	while (t--){
		ll n, m, a, b; read(n, m, a, b);
		out += to_string(floorSum(n, m, a, b)) + '\n';
	}
	fputs(out.c_str(), stdout);
}
