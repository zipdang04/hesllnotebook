#!/usr/bin/env bash
set -euo pipefail
scratch=$(mktemp -d)
trap 'rm -rf "$scratch"' EXIT
mkdir -p "$scratch/tex/contents/contest" \
	"$scratch/tex/contents/geometry" \
	"$scratch/testing/geometry"
cp ../tex/contents/contest/template.hpp \
	"$scratch/tex/contents/contest/template.hpp"
for path in tex/contents/geometry/point.hpp \
		testing/geometry/point_stress.cpp; do
	perl -pe 's/\bdouble\b/long double/g' "../$path" \
		> "$scratch/$path"
done
"$CXX" -std=c++17 -O2 -Wall -Wextra -Wshadow \
	"$scratch/testing/geometry/point_stress.cpp" \
	-o "$scratch/stress"
"$scratch/stress"
