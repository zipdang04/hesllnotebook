#include "../../tex/contents/contest/fastio.hpp"
#include <cassert>
int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	int t; read(t);
	FOR(i, 1, t){
		ll a, b; assert(read(a, b));
		cout << a + b << '\n';
	}
	ll x; assert(!read(x)); assert(readChar() == -1);
}
