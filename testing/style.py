#!/usr/bin/env python3
# Mechanical checks of docs/STYLE.md §12 for entries (.hpp) and tests (.cpp).
# python3 testing/style.py [--compile] FILE...   (no FILE: every .hpp under tex/contents)
# Prints one line per problem and exits 1 if there is any.
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
WIDTH = 60
FORBIDDEN = re.compile(r"\b(EFOR|EREV|namespace|class|private|inline|init|memset|function|define|typedef)\b")
SPACING = [
	(re.compile(r"\b(if|for|while|switch) \("), "no space after a keyword: if("),
	(re.compile(r"\}\s+else|else\s+\{"), "}else{"),
	(re.compile(r"\)\s+\{"), "no space before an opening brace: ){"),
	(re.compile(r"\bfor\([^;]*? : "), "range for: v: g"),
	(re.compile(r'"\\n"|\bendl\b'), "line break is '\\n'"),
]


def code_part(line):
	# drop a // comment and string literals, so words in them are not flagged
	line = re.sub(r'"(\\.|[^"\\])*"', '""', line)
	return line.split("//", 1)[0]


def check(path):
	problems = []
	lines = open(path, encoding="utf-8").read().split("\n")
	text_ends_with_newline = lines[-1] == ""
	if text_ends_with_newline:
		lines.pop()
	else:
		problems.append((len(lines), "no newline at the end of the file"))
	is_entry = path.endswith(".hpp") and os.path.basename(path) != "template.hpp"
	if is_entry:
		if lines[:1] != ["#pragma once"] or len(lines) < 4 or not re.fullmatch(r'#include "[^"]+"', lines[1]) or lines[2] != "":
			problems.append((1, "header must be: #pragma once / #include \"..\" / blank line"))
		elif lines[3].strip() == "":
			problems.append((4, "blank line after line 3"))
	for i, line in enumerate(lines, 1):
		width = len(line.expandtabs(2))
		if width > WIDTH:
			problems.append((i, f"{width} columns > {WIDTH}"))
		if re.match(r"\t* +\S", line):
			problems.append((i, "indent with tabs"))
		if line != line.rstrip():
			problems.append((i, "trailing blank"))
		code = code_part(line)
		if is_entry:
			word = FORBIDDEN.search(re.sub(r"template<[^<>]*(<[^<>]*>[^<>]*)*>|<class ", "", code))
			if word:
				problems.append((i, f"forbidden word '{word.group(1)}' (STYLE §4)"))
			if "/*" in code and "*/" not in code:
				problems.append((i, "multi-line /* */ comment: use // (STYLE §1.4, brace guides)"))
			if re.search(r"\{\s*//", line):
				problems.append((i, "comment after an opening {: put it on its own line (STYLE §1.4)"))
		for pattern, rule in SPACING:
			if pattern.search(code):
				problems.append((i, f"spacing: {rule}"))
	return problems


def compile_check(path):
	problems = []
	cxx = os.environ.get("CXX") or next((c for c in ["g++-16", "g++-15", "g++-14", "g++"] if subprocess.run(["which", c], capture_output=True).returncode == 0), "g++")
	for std in ["c++17", "gnu++20"]:
		src = f'#include "{os.path.abspath(path)}"\nint main(){{}}\n'
		r = subprocess.run([cxx, f"-std={std}", "-Wall", "-Wextra", "-Wshadow", "-Wno-psabi", "-fsyntax-only", "-x", "c++", "-"], input=src, capture_output=True, text=True)
		if r.returncode or r.stderr.strip():
			first = (r.stderr.strip().splitlines() or ["error"])[0]
			problems.append((0, f"-std={std}: {first}"))
	return problems


if __name__ == "__main__":
	args = sys.argv[1:]
	do_compile = "--compile" in args
	files = [a for a in args if a != "--compile"]
	if not files:
		base = os.path.join(ROOT, "tex", "contents")
		files = sorted(os.path.join(d, f) for d, _, fs in os.walk(base) for f in fs if f.endswith(".hpp"))
	bad = False
	for f in files:
		problems = check(f)
		if do_compile and f.endswith(".hpp"):
			problems += compile_check(f)
		for line, message in problems:
			print(f"{os.path.relpath(f, ROOT)}:{line}: {message}")
			bad = True
	sys.exit(1 if bad else 0)
