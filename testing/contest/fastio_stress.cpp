#include "../../tex/contents/contest/fastio.hpp"
#include <cassert>

#include <climits>
#include <unistd.h>

void openInput(const string &s){
	FILE *file = fopen("fastio_stress.tmp", "wb");
	assert(file);
	assert((int)fwrite(s.data(), 1, sz(s), file) == sz(s));
	assert(fclose(file) == 0);
	assert(freopen("fastio_stress.tmp", "r", stdin));
}

// The values written are the oracle.
void checkReads(const vector<ll> &want){
	int i = 0;
	ll x;
	while(read(x)){
		assert(i < sz(want));
		assert(x == want[i]);
		i++;
	}
	assert(i == sz(want));
	assert(readChar() == -1);
}

int main(){
	// POSIX read(fd, buf, n) still compiles next to ours
	int fd[2], z = pipe(fd);
	char posix[8];
	size_t cnt = sizeof posix;
	assert(z == 0 && write(fd[1], "X", 1) == 1);
	assert(read(fd[0], posix, cnt) == 1 && posix[0] == 'X');
	openInput("");
	checkReads({});
	openInput(" \t\r\n\n   \t\r\n");
	checkReads({});
	openInput("0");
	checkReads({0});
	openInput("-");
	checkReads({});
	openInput("5 -");
	checkReads({5});
	openInput("-7");
	checkReads({-7});
	openInput(to_string(LLONG_MAX) + "\t"
		+ to_string(-LLONG_MAX));
	checkReads({LLONG_MAX, -LLONG_MAX});
	openInput(" \t\r\n\n0   -1\t2\r\n\n-3 \n");
	checkReads({0, -1, 2, -3});
	// Split a sign or digits across the 64 KB boundary.
	FOR(pad, 65530, 65536){
		openInput(string(pad, ' ') + to_string(-LLONG_MAX));
		checkReads({-LLONG_MAX});
	}
	openInput(to_string(INT_MIN) + "\t"
		+ to_string(LLONG_MAX) + "\r\n"
		+ to_string(INT_MAX) + "\n\n-17");
	int a = 0, c = 0;
	ll b = 0;
	assert(read(a, b, c));
	assert(a == INT_MIN && b == LLONG_MAX && c == INT_MAX);
	checkReads({-17});
	b = 456; c = 789;
	assert(!read(a, b, c));
	assert(b == 456 && c == 789);
	checkReads({});
	openInput("17");
	assert(!read(a, b, c));
	assert(a == 17 && c == 789);
	checkReads({});
	string raw = "A\t9\r\n";
	raw += '\0';
	openInput(raw);
	for(char ch: raw) assert(readChar() == ch);
	checkReads({});
	string bytes;
	FOR(i, 128, 255) bytes += char(i);
	openInput(bytes);
	FOR(i, 128, 255) assert(readChar() == i);
	checkReads({});
	openInput(bytes + "42" + bytes + "-7");
	checkReads({42, -7});
	mt19937_64 rng(712367);
	vector<string> sep{" ", "\t", "\r\n", "\n\n",
		" \t\r\n  ", ",", ";abc;", "+"};
	FOR(trial, 1, 500){
		int n = (int)(rng() % 300001);
		if(trial == 1) n = 300000;
		vector<ll> want(n);
		string s = sep[rng() % sz(sep)];
		s.reserve(24 * n + 32);
		FOR(i, 0, n - 1){
			ll x = (ll)(rng() >> 1);
			if(rng() & 1) x = -x;
			want[i] = x;
			if(i) s += sep[rng() % sz(sep)];
			s += to_string(x);
		}
		if(rng() & 1) s += sep[rng() % sz(sep)];
		openInput(s);
		checkReads(want);
	}
	assert(fclose(stdin) == 0);
	assert(remove("fastio_stress.tmp") == 0);
}
