#!/bin/bash
set -eu
python3 - <<'PY'
import os
from pathlib import Path
import subprocess
import tempfile

header = Path('../tex/contents/ds/lazyseg.hpp').resolve()
source = header.read_text()
source = source[source.index('struct LazySeg{'):]
begin = source.index('\t// ---- EDIT:')
end = source.index('\t// ---- end of EDIT ----')
# Change only EDIT; exercise the entry's tree operations.
source = source[:begin] + '''
	struct Node{ string sum; int len = 0; };
	struct Tag{};
	Node combine(Node x, Node y){
		return {x.sum + y.sum, x.len + y.len};
	}
	void apply(int, Tag){}
''' + source[end:]
source = source.replace('LazySeg', 'OrderSeg')
source = f'#include "{header}"\n#include <cassert>\n' + source
source += '''
int main(){
	string a = "abcdefghi";
	vector<OrderSeg::Node> nodes;
	for(char c: a) nodes.push_back({string(1, c), 1});
	OrderSeg seg(nodes);
	FOR(l, 0, 8)
		FOR(r, l, 8)
			assert(seg.get(l, r).sum == a.substr(l, r - l + 1));
	cout << "OK: non-commutative combine" << '\\n';
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
