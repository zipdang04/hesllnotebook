#include "../../tex/contents/math/eratos_factorable.cpp"

int main() {
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	int n, a, b; cin >> n >> a >> b;
	Eratos_Factorable::sieve(n);
	int pi = Eratos_Factorable::cntPrime;
	vector<int> out;
	for (ll i = b; i < pi; i += a)
		out.push_back(Eratos_Factorable::primes[i + 1]);
	cout << pi << ' ' << out.size() << '\n';
	FOR(size_t, i, 0, out.size() - 1) cout << out[i] << ' ';
	cout << '\n';
}
