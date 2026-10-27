#define LOCAL
#define STRESS 1
#include "../../tex/contents/contest/template.hpp"
int main(){
	const char *f = "/tmp/hesl_dbg_stderr.txt";
	assert(freopen(f, "w", stderr));
	int x = 3; string s = "hi";
	dbg(x, s); dbg(x + 1);
	fflush(stderr);
	ifstream in(f); stringstream ss; ss << in.rdbuf();
	string want = "[x, s] = 3 hi\n[x + 1] = 4\n";
	static_assert(STRESS == 1, "");
	const int B = STRESS ? 2 : 300; assert(B == 2);
	if (ss.str() != want){ fprintf(stdout, "dbg output mismatch: [%s]\n", ss.str().c_str()); return 1; }
	remove(f);
}
