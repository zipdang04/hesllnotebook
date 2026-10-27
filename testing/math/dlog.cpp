#include "../../tex/contents/math/dlog.hpp"

int main(){
	int t; read(t);
	string out;
	while (t--){
		ll x, y, m; read(x, y, m);
		out += to_string(dlog(x, y, m)) + '\n';
	}
	fputs(out.c_str(), stdout);
}
