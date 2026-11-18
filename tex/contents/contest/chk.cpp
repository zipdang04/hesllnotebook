// ./chk in out ans; exit 0 = OK, 1 = WA
#include <bits/stdc++.h>
using namespace std;
int main(int, char **argv){
	ifstream in(argv[1]), out(argv[2]), ans(argv[3]);
	long long x, y;
	ans >> y;
	if(!(out >> x)) return 1; // missing output
	string extra;
	if(out >> extra) return 1; // too much output
	return x == y ? 0 : 1;
}
