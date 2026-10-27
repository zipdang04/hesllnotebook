"""GitHub Actions helpers around all.py's test table (they never run a test):
  python3 testing/ci.py plan [BASE]   cache keys + whether Library Checker data is needed -> $GITHUB_OUTPUT
  python3 testing/ci.py prune         drop cached data of problems / URLs no longer tested (before saving)"""
import hashlib
import os
import shutil
import sys

import all as suite


def tested():
	tests = [test for _, test in suite.all_tests()]
	return ({t.testing_folder for t in tests if isinstance(t, suite.TestCase)},
			{t.url for t in tests if isinstance(t, suite.OjTest)})


def plan(base):
	# lc key: submodule commit + tested problems; oj key: tested URLs. Upstream edits to a problem are caught by
	# all.py's in/.version stamps, so they do not need a new key.
	key = lambda items: hashlib.sha256("\n".join(sorted(items)).encode()).hexdigest()[:16]
	folders, urls = tested()
	commit = (suite.git("rev-parse", "HEAD:" + suite.SUBMODULE) or "").strip()
	chosen = suite.select(base) if base else None  # PR: the tests --changed will run
	need_lc = chosen is None or any(isinstance(t, suite.TestCase) for n, t in suite.all_tests() if n in chosen)
	out = {"lc_key": "lc-" + key(folders | {commit}), "oj_key": "oj-" + key(urls), "need_lc": str(need_lc).lower()}
	print(out)
	with open(os.environ.get("GITHUB_OUTPUT", os.devnull), "a") as f:
		f.writelines(f"{k}={v}\n" for k, v in out.items())


def prune():
	folders, urls = tested()
	oj = {suite.oj_dir(url) for url in urls}
	lc = os.path.join(suite.ROOT, suite.SUBMODULE)
	for d, dirs, _ in os.walk(lc):
		if "in" in dirs and os.path.relpath(d, lc) not in folders:
			for sub in ("in", "out"):
				shutil.rmtree(os.path.join(d, sub), ignore_errors=True)
	for d in os.listdir(suite.OJ_TESTS_DIR) if os.path.isdir(suite.OJ_TESTS_DIR) else []:
		if os.path.join(suite.OJ_TESTS_DIR, d) not in oj:
			shutil.rmtree(os.path.join(suite.OJ_TESTS_DIR, d))


if __name__ == "__main__":
	if sys.argv[1:2] == ["plan"]:
		plan(sys.argv[2] if len(sys.argv) > 2 else None)
	elif sys.argv[1:2] == ["prune"]:
		prune()
	else:
		print(__doc__)
		exit(1)
