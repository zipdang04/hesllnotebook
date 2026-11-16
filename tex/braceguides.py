#!/usr/bin/env python3
# braceguides.py SRC DST: display copy of a C++ file with block braces replaced by indent guides.
# Drops a `}` line (or the `}` of `} else ...`) when its `{` ends the opener line; that `{` goes too.
# Kept as braces: `};` `});` `}while(..);`, one-line `{ .. }`, brace initialisers, `{` not at line end.
# Each line that needs guides gets `@\G{cols}{ticks}@` after its leading tabs (minted escapeinside=@@):
# cols = tab columns of the guides through this line, ticks = guides whose block ends on this line.
import os, re, sys

def braces(lines):
    """(line, col, ch) of every brace outside strings, chars and comments."""
    out, block = [], False
    for i, s in enumerate(lines):
        j = 0
        while j < len(s):
            if block:
                k = s.find('*/', j)
                if k < 0: break
                block, j = False, k + 2
                continue
            c = s[j]
            if s.startswith('//', j): break
            if s.startswith('/*', j): block, j = True, j + 2; continue
            if c in '"\'':
                j += 1
                while j < len(s) and s[j] != c: j += 2 if s[j] == '\\' else 1
            elif c in '{}': out.append((i, j, c))
            j += 1
    return out

def convert(lines):
    lines = [s.rstrip('\n') for s in lines]
    stack, blocks = [], []          # blocks: (open line, close line, close col, tab column)
    for i, j, c in braces(lines):
        if c == '{': stack.append((i, j)); continue
        oi, oj = stack.pop()
        rest = lines[i][j + 1:].strip()
        if lines[oi][oj + 1:].strip() == '' and lines[i][:j].strip() == '' and oi < i \
                and (rest == '' or re.match(r'else\b', rest)
                or (re.match(r'while\b', rest) and re.match(r'\s*do\s*\{', lines[oi]))):
            tab = len(lines[i]) - len(lines[i].lstrip('\t'))
            if all(s.startswith('\t' * (tab + 1)) for s in lines[oi + 1:i] if s.strip()):  # body indented deeper
                blocks.append((oi, i, j, tab))
    assert not stack, 'unbalanced braces'
    assert all(b[3] < 10 for b in blocks), 'one digit per guide column'
    out = list(lines)
    drop = set()
    for oi, ci, cj, tab in blocks:
        out[oi] = re.sub(r'\s*\{\s*$', '', out[oi])
        rest = lines[ci][cj + 1:].strip()
        if rest: out[ci] = '\t' * tab + rest   # `} else ...` keeps `else ...` (its own `{` is handled by its block)
        else: drop.add(ci)
    cols = [[] for _ in lines]; ticks = [[] for _ in lines]
    for oi, ci, cj, tab in blocks:
        last = max(k for k in range(oi, ci) if k not in drop)   # last printed body line: tick at its bottom
        for k in range(oi + 1, ci): cols[k].append(tab)
        ticks[last].append(tab)
    res = []
    for k, s in enumerate(out):
        if k in drop: continue
        if cols[k]:
            ind = len(s) - len(s.lstrip('\t'))
            g = ''.join(map(str, sorted(cols[k]))); t = ''.join(map(str, sorted(ticks[k])))
            s = s[:ind] + '@\\G{%s}{%s}@' % (g, t) + s[ind:]
        res.append(s)
    return res

if __name__ == '__main__':
    src = open(sys.argv[1]).readlines()
    # template.hpp is typed first, under the most pressure: printed as written
    res = [l.rstrip('\n') for l in src] if os.path.basename(sys.argv[1]) == 'template.hpp' else convert(src)
    os.makedirs(os.path.dirname(sys.argv[2]) or '.', exist_ok=True)
    open(sys.argv[2], 'w').write('\n'.join(res) + '\n')
