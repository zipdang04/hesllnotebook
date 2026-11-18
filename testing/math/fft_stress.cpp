#include "../../tex/contents/math/fft.hpp"

mt19937_64 rng(712367);

ll rnd(ll n){ return rng() % n; }

ll value(ll mod, int kind){
	ll cut = sqrt(mod), h = cut / 2;
	ll digit = h - min(h, rnd(4));
	ll x = digit * cut + digit;
	if(kind == 0) return rnd(mod);
	if(kind == 1) return 0;
	if(kind == 2) return mod - 1 - rnd(min(mod, 4LL));
	if(kind == 3) return x % mod;
	if(kind == 4) return (mod - x % mod) % mod;
	ll base = 1 << 15;
	ll up = mod / base;
	return ((up - 1 - rnd(min(up, 1000LL))) * base
		+ base - 1 - rnd(1000)) % mod;
}

ll eval(const vector<ll> &a, ll r, ll mod){
	ll x = 0;
	REV(i, sz(a) - 1, 0)
		x = ((__int128)x * r + a[i]) % mod;
	return x;
}

bool identity(const vector<ll> &a, const vector<ll> &b,
		const vector<ll> &c, ll mod){
	if(sz(c) != sz(a) + sz(b) - 1) return false;
	for(ll x: c) if(x < 0 || x >= mod) return false;
	FOR(t, 0, 2){
		ll r = rnd(mod);
		ll want = (__int128)eval(a, r, mod)
			* eval(b, r, mod) % mod;
		if(eval(c, r, mod) != want) return false;
	}
	return true;
}

int main(){
	vector<ll> mods = {1, 2, 7, 998244353,
		1000000007, 1LL << 30};
	FOR(t, 0, 1999){
		int limit = t < 100 ? 4 : 300;
		int n = 1 + rnd(limit), m = 1 + rnd(limit);
		vector<double> a(n), b(m);
		for(double &x: a) x = rnd(100001);
		for(double &x: b) x = rnd(100001);
		if(t % 2){
			for(double &x: a) if(rnd(2)) x = -x;
			for(double &x: b) if(rnd(2)) x = -x;
		}
		vector<ll> want(n + m - 1);
		FOR(i, 0, n - 1) FOR(j, 0, m - 1)
			want[i + j] += (ll)a[i] * (ll)b[j];
		auto got = conv(a, b);
		if(sz(got) != sz(want)){
			cerr << "conv size " << t << '\n';
			return 1;
		}
		FOR(i, 0, n + m - 2) if(llround(got[i]) != want[i]){
			cerr << "conv " << t << " " << i << '\n';
			return 1;
		}
		for(ll mod: mods){
			vector<ll> x(n), y(m);
			for(ll &v: x) v = value(mod, rnd(5));
			for(ll &v: y) v = value(mod, rnd(5));
			vector<__int128> sum(n + m - 1);
			FOR(i, 0, n - 1) FOR(j, 0, m - 1)
				sum[i + j] += (__int128)x[i] * y[j];
			auto c = convMod(x, y, mod);
			if(sz(c) != sz(sum)){
				cerr << "mod size " << t << " " << mod << '\n';
				return 1;
			}
			FOR(i, 0, n + m - 2) if(c[i] != sum[i] % mod){
				cerr << "mod " << t << " " << mod
					<< " " << i << '\n';
				return 1;
			}
		}
	}
	for(ll mod: {1000000007LL, 1LL << 30})
		FOR(kind, 0, 4){
			int n = 1 << 19;
			vector<ll> a(n), b(n);
			int k = kind == 0 ? 0 : kind + 1;
			for(ll &x: a) x = value(mod, k);
			for(ll &x: b) x = value(mod, k);
			auto c = convMod(a, b, mod);
			if(!identity(a, b, c, mod)){
				cerr << "limit " << mod << " " << kind << '\n';
				return 1;
			}
		}
	ll mod = 1LL << 30;
	vector<ll> a((1 << 19) + 1), b(1 << 19);
	for(ll &x: a) x = value(mod, 3);
	for(ll &x: b) x = value(mod, 3);
	if(!identity(a, b, convMod(a, b, mod), mod)){
		cerr << "exact limit\n";
		return 1;
	}
	FOR(t, 0, 1){
		int n = 1 << (t ? 17 : 16);
		vector<double> x(n), y(n);
		for(double &v: x)
			v = t ? rnd(2) : 19800 + rnd(201);
		if(t) x[1] = 100000;
		for(double &v: y) v = 19800 + rnd(201);
		if(t) y = x;
		auto c = conv(x, y);
		vector<ll> u(all(x)), v(all(y));
		vector<ll> w;
		for(double z: c) w.push_back(llround(z) % 1000000007);
		if(!identity(u, v, w, 1000000007)){
			cerr << "conv limit " << t << '\n';
			return 1;
		}
	}
}
