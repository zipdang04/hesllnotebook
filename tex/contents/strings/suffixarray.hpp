#include "../contest/template.hpp"

namespace Suffix {
typedef pair<int, int> pii;
typedef pair<pii, int> ppiii;

string s;
int len;
int sa[MAX], rk[MAX];
int lcp[MAX];

void build(){
	{
		pii v[len];
		FOR(int, i, 0, len - 1) v[i] = {s[i], i};
		sort(v, v + len);
		FOR(int, i, 0, len - 1) sa[i] = v[i].second;
		rk[sa[0]] = 0;
		FOR(int, i, 1, len - 1)
			rk[sa[i]] = rk[sa[i - 1]] + (v[i].first != v[i - 1].first);
	}
	int k = 0;
	while ((1 << k) < len){
		vector<ppiii> radix[len];
		FOR(int, i, 0, len - 1){
			int pos = sa[i];
			int prev = (pos + len - (1 << k)) % len;
			int rPrev = rk[prev], rPos = rk[pos];
			radix[rPrev].push_back({{rPrev, rPos}, prev});
		}
		ppiii v[len]; int cnt = 0;
		FOR(int, i, 0, len - 1){
			for (ppiii curr: radix[i]){
				v[cnt++] = curr;
			}
		}
		FOR(int, i, 0, len - 1) sa[i] = v[i].second;
		rk[sa[0]] = 0;
		FOR(int, i, 1, len - 1)
			rk[sa[i]] = rk[sa[i - 1]] + (v[i].first != v[i - 1].first);
		k++;
	}
}

void buildLCP(){
	int preCal = 0;
	FOR(int, i, 0, len - 2){
		int posInSA = rk[i];
		int curr = i, prev = sa[posInSA - 1];
		int ans = preCal;
		FOR(int, j, preCal, len - 1){
			char cPrev = s[(prev + j) % len], cPos = s[(curr + j) % len];
			if (cPrev != cPos) break;
			ans++;
		}
		lcp[posInSA] = ans;
		preCal = max(ans - 1, 0);
	}
}

}
