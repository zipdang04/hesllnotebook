#include "../../tex/contents/interactive/hamming.hpp"
#include <cassert>

unsigned long long checkEncode(ll x){
	auto word = hammingEncode(x);
	// Each parity row is checked separately.
	FOR(k, 0, 5){
		int parity = 0;
		FOR(i, 0, 63) if((i >> k) & 1)
			parity ^= (word >> i) & 1ULL;
		assert(parity == 0);
	}
	assert(__builtin_popcountll(word) % 2 == 0);
	ll data = 0;
	int bit = 0;
	FOR(i, 1, 63) if(i & (i - 1)){
		if((word >> i) & 1ULL) data |= 1LL << bit;
		bit++;
	}
	assert(bit == 57 && data == x);
	assert(hammingDecode(word) == x);
	return word;
}

unsigned long long randomMask(int weight, mt19937_64 &rng){
	unsigned long long mask = 0;
	while(__builtin_popcountll(mask) < weight)
		mask |= 1ULL << (rng() % 64);
	return mask;
}

ll brute(unsigned long long word,
		const vector<unsigned long long> &code){
	ll answer = -1;
	FOR(a, 0, sz(code) - 1)
		if(__builtin_popcountll(word ^ code[a]) <= 1){
			assert(answer == -1);
			answer = a;
		}
	return answer;
}

int main(){
	mt19937_64 rng(723);
	assert(checkEncode(0) == 0);
	assert(checkEncode(1) == 15);
	ll limit = (1LL << 57) - 1;
	checkEncode(limit);
	FOR(b, 0, 56) checkEncode(1LL << b);
	vector<unsigned long long> code(1 << 12);
	FOR(a, 0, sz(code) - 1) code[a] = checkEncode(a);
	int dist = 64;
	FOR(a, 0, sz(code) - 1)
		FOR(b, 0, a - 1){
			int d = __builtin_popcountll(code[a] ^ code[b]);
			dist = min(dist, d);
		}
	assert(dist == 4);
	FOR(trial, 1, 10000){
		ll a = rng() % sz(code);
		int weight = rng() % 3;
		auto mask = randomMask(weight, rng);
		auto word = code[a] ^ mask;
		ll want = brute(word, code);
		assert(want == (weight < 2 ? a : -1));
		assert(hammingDecode(word) == want);
	}
	FOR(trial, 1, 20000){
		ll a = rng() & limit, b = rng() & limit;
		auto ca = checkEncode(a), cb = checkEncode(b);
		assert(hammingEncode(a ^ b) == (ca ^ cb));
		FOR(weight, 0, 2){
			auto word = ca ^ randomMask(weight, rng);
			assert(hammingDecode(word)
				== (weight < 2 ? a : -1));
		}
	}
	return 0;
}
