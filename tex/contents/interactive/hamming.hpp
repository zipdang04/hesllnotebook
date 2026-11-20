#pragma once
#include "../contest/template.hpp"

// Hamming (64,57): correct 1 flip, detect 2 flips. Bit 0:
// overall parity; bits 1, 2, 4, .., 32: check bits; payload
// bits fill 3, 5, 6, 7, 9, .. lowest first. O(64).
unsigned long long hammingEncode(ll data){
	unsigned long long w = 0;
	int s = 0;
	FOR(i, 1, 63) if(i & (i - 1)){
		if(data & 1) w |= 1ULL << i, s ^= i;
		data >>= 1;
	}
	// check bit 2^k makes bit k of the XOR of positions 0
	FOR(k, 0, 5) if(testBit(s, k))
		w |= 1ULL << (1 << k);
	return w | __builtin_parityll(w);
}
ll hammingDecode(unsigned long long word){
	int s = 0;
	FOR(i, 1, 63) if(testBit(word, i))
		s ^= i;
	// s != 0 with even parity: two flips
	if(s && !__builtin_parityll(word)) return -1;
	if(s) word ^= 1ULL << s;
	ll data = 0;
	REV(i, 63, 1) if(i & (i - 1))
		data = data << 1 | testBit(word, i);
	return data;
}
