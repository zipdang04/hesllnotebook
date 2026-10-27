#include "../../tex/contents/strings/sam.hpp"

int main(){
	ios_base::sync_with_stdio(false); cin.tie(nullptr);
	string s; cin >> s;
	SuffixAutomaton<> sam(s);
	cout << sam.distinct() << '\n';
}
