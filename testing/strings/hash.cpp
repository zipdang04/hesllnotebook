#include "../../tex/contents/string/strhash.hpp"


int n;
HashString str;

int check(int pos) {
    int maxLen = min(2*n-2 - pos, pos);
    int lo = 1, hi = maxLen, ans = 0;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        int L = pos - mid, R = pos + mid;
        L = (L + L % 2) / 2 + 1, R = (R - R % 2) / 2 + 1;
        if (str.getF(L, R) == str.getB(L, R))
            ans = mid, lo = mid + 1;
        else
            hi = mid - 1;
    }
    int L = pos - ans, R = pos + ans;
    L = (L + L % 2) / 2 + 1, R = (R - R % 2) / 2 + 1;
    return R-L+1;
}

int main() {
    string s; cin >> s;
    n = s.size(); str = HashString(s);
    FOR(int, i, 0, 2*n-2) {
        cout << check(i) << " \n"[i == 2*n-2];
    }
}
