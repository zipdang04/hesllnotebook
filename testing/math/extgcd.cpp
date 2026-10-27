#include "../../tex/contents/math/crt.hpp"

int main(){
	ll a, b, x, y; cin >> a >> b;
	extGcd(a, b, x, y);
	cout << x << " " << y << "\n";
}
