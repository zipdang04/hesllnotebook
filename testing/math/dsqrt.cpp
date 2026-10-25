#include "../../tex/contents/math/dlog.hpp"

int main(){
	int t; read(t);
	string out;
	while (t--){
		ll y, p; read(y, p);
		out += to_string(dsqrt(y, p)) + '\n';
	}
	fputs(out.c_str(), stdout);
}
