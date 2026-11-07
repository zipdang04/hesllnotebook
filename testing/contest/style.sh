#!/bin/bash
# Code-layout rules for restyled notebook headers (FORMAT.md, "Code conventions").
set -u
cd "$(dirname "$0")/.." || exit 1
DIR=${STYLE_DIR:-../tex/contents}
# Restyled files; a section's files are added here when its restyle merges.
FILES=${STYLE_FILES:-"flow/dinitzwscaling.hpp"}

fail(){ echo "FAIL: $*"; exit 1; }
ok(){ echo "ok: $*"; }

# A ";" right after "}" (end of struct) is not counted.
# Limit: strings/chars are blanked by regex, so a '"' inside a char
# literal or a multi-line raw string can fool the // and ; checks.
BAD=0; N=0
for f in $FILES; do
	N=$((N+1))
	[ -f "$DIR/$f" ] || fail "missing $f"
	OUT=$(expand -t2 "$DIR/$f" | awk -v F="$f" '
		!body && /^#(pragma|include)/ { next }
		{ body = 1 }
		length > 58 { print F ":" NR ": wider than 58 columns" }
		{ s = $0
		  gsub(/"([^"\\]|\\.)*"|'"'"'([^'"'"'\\]|\\.)*'"'"'/, "\"\"", s)
		  if (match(s, /\/\//)) {
			  if (substr(s, 1, RSTART - 1) ~ /[^ ]/)
				  print F ":" NR ": trailing // comment"
			  s = substr(s, 1, RSTART - 1)
		  }
		  d = 0; k = 0
		  for (i = 1; i <= length(s); i++) {
			  c = substr(s, i, 1)
			  p = i > 1 ? substr(s, i - 1, 1) : ""
			  if (c == "(") d++
			  else if (c == ")") d--
			  else if (c == ";" && d <= 0 && p != "}") k++
		  }
		  if (k > 2) print F ":" NR ": " k " statements on one line" }')
	[ -n "$OUT" ] && { echo "$OUT"; BAD=1; }
done
[ "$BAD" -eq 0 ] || exit 1
ok "style ($N files)"
