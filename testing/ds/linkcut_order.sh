#!/bin/bash
set -eu
python3 - <<'PY'
import os
from pathlib import Path
import subprocess
import tempfile

header = Path('../tex/contents/ds/linkcut.hpp').resolve()
source = header.read_text()
source = source[source.index('struct LinkCut{'):]
begin = source.index('\t// ---- EDIT:')
end = source.index('\t// ---- end of EDIT ----')
source = source[:begin] + '''
	struct Tag{};
	struct Node{
		char val = 0; string sum, rsum; Tag tag;
		int size = 0, par = 0, ch[2] = {0, 0}; bool rev = 0;
	};
	void pull(int u){
		Node &x = t[u], &l = t[x.ch[0]], &r = t[x.ch[1]];
		x.size = l.size + 1 + r.size;
		x.sum = l.sum + x.val + r.sum;
		x.rsum = r.rsum + x.val + l.rsum;
	}
	void apply(int, Tag){}
''' + source[end:]
source = source.replace('t[u].rev ^= 1;',
                        't[u].rev ^= 1; swap(t[u].sum, t[u].rsum);')
source = source.replace('LinkCut', 'OrderLinkCut')
source = f'#include "{header}"\n#include <cassert>\n' + source
source += '''
int main(){
	OrderLinkCut a(5);
	FOR(u, 1, 4) a.update(u, 'a' + u - 1);
	FOR(u, 1, 3) a.link(u, u + 1);
	assert(a.get(1, 4).sum == "abcd");
	assert(a.get(4, 1).sum == "dcba");
	a.makeRoot(2);
	assert(a.get(2, 4).sum == "bcd");
	assert(a.get(4, 2).sum == "dcb");
	cout << "OK: ordered paths and flip" << '\\n';
}
'''
with tempfile.TemporaryDirectory() as tmp:
    cpp = Path(tmp) / 'order.cpp'
    exe = Path(tmp) / 'order'
    cpp.write_text(source)
    subprocess.run([os.environ['CXX'], '-std=c++17', '-O2',
                    '-Wall', '-Wextra', '-Wshadow',
                    str(cpp), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
PY
