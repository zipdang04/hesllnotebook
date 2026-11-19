#include "../../tex/contents/interactive/hamming.hpp"
#include <cassert>
int main(){
	assert(hammingEncode(0) == 0);
	assert(hammingEncode(1) == 15);
	ll mask = (1LL << 57) - 1;
	vector<ll> payloads = {0, 1, mask,
		0x0AAAAAAAAAAAAAAALL & mask,
		0x0555555555555555LL & mask};
	FOR(b, 0, 56)
		payloads.push_back(1LL << b);
	mt19937_64 rng(7123);
	FOR(i, 1, 20)
		payloads.push_back(rng() & mask);
	for(ll x: payloads){
		auto c = hammingEncode(x);
		assert(__builtin_parityll(c) == 0);
		assert(hammingDecode(c) == x);
		FOR(i, 0, 63)
			assert(hammingDecode(c ^ (1ULL << i)) == x);
		FOR(i, 0, 63)
			FOR(j, i + 1, 63)
				assert(hammingDecode(c ^ (1ULL << i)
					^ (1ULL << j)) == -1);
	}
	auto w = hammingEncode(5);
	assert(hammingDecode(w ^ 14) == (5 ^ 1));
	return 0;
}
