#include "../../tex/contents/strings/lyndon.hpp"
#include <cassert>

int main(){
	assert(duval("").empty());
	assert((duval("a") == vector<int>{0}));
	assert((duval("aaa") == vector<int>{0, 1, 2}));
	assert((duval("abab") == vector<int>{0, 2}));
	assert((duval("ba") == vector<int>{0, 1}));
	string s;
	cin >> s;
	for(int i: duval(s)) cout << i << ' ';
	cout << sz(s) << '\n';
}
