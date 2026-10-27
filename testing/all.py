import os
import platform
import re
import shutil
import subprocess
import sys
import time
import urllib.request
import json

sys.stdout.reconfigure(line_buffering=True)  # CI pipes stdout: keep our lines in order with the tests' output

TESTING_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(TESTING_DIR)
OJ_TESTS_DIR = os.path.join(TESTING_DIR, ".oj-tests")


def load_env(path=os.path.join(ROOT, ".env")):
	# KEY=value lines; variables already set (e.g. CI secrets) win
	if not os.path.exists(path):
		return
	for line in open(path):
		line = line.strip()
		if line and not line.startswith("#") and "=" in line:
			key, value = line.split("=", 1)
			os.environ.setdefault(key.strip(), value.strip().strip("\"'"))


load_env()

# COEF scales every time limit (e.g. COEF=2 on a slower CI runner); set it in the env or .env
COEF = float(os.environ.get("COEF") or 1)


def find_cxx():
	for cxx in [os.environ.get("CXX"), "g++-16", "g++-15", "g++-14", "g++"]:
		if cxx and shutil.which(cxx):
			return cxx
	print("No C++ compiler found")
	exit(1)


def oj_command():
	if shutil.which("oj"):
		return ["oj"]
	return [sys.executable, "-m", "onlinejudge_command.main"]


def download_yukicoder(url, test_dir):
	# oj only knows cookie login for yukicoder; the API takes the token
	no = re.search(r"/problems/no/(\d+)", url).group(1)
	api = "https://yukicoder.me/api/v1/problems/no/" + no + "/file/"

	def get(path):
		request = urllib.request.Request(api + path, headers={"Authorization": "Bearer " + os.environ["YUKICODER_TOKEN"]})
		return urllib.request.urlopen(request, timeout=60).read()

	os.makedirs(test_dir, exist_ok=True)
	for name in json.loads(get("in")):
		base = os.path.splitext(name)[0]
		for kind in ("in", "out"):
			with open(os.path.join(test_dir, base + "." + kind), "wb") as f:
				f.write(get(kind + "/" + name))


def run(command):
	if subprocess.call(command, cwd=TESTING_DIR) != 0:
		print("FAILED")
		exit(1)


class TestCase:
	"""Library Checker problem, generated from the submodule by test.sh."""

	def __init__(self, source_code, testing_folder, time_limit: float = 1):
		self.source_code = source_code
		self.testing_folder = testing_folder
		self.time_limit = time_limit * COEF

	def process(self):
		env = dict(os.environ, CXX=find_cxx())
		version = problem_version(self.testing_folder)
		stamp = os.path.join(ROOT, SUBMODULE, self.testing_folder, "in", ".version")
		if os.path.exists(stamp) and open(stamp).read() == version:
			mark_fresh(self.testing_folder)
		command = ["bash", "test.sh", self.source_code, self.testing_folder, str(self.time_limit)]
		if subprocess.call(command, cwd=TESTING_DIR, env=env) != 0:
			print("FAILED")
			exit(1)
		open(stamp, "w").write(version)


# generate.py regenerates a problem whose in/out files are older than any of its sources. After a checkout
# (CI) every source is new, so cached data always looked stale. Instead: in/.version holds a hash of the sources
# (same files as generate.py's list_depending_files); if it still matches, touch in/out so generate.py skips them.
def problem_version(folder):
	import hashlib
	lc = os.path.join(ROOT, SUBMODULE)
	base = os.path.join(lc, folder)
	paths = [os.path.join(lc, "generate.py")]
	for top in (base, os.path.join(lc, "common")):
		for d, dirs, files in os.walk(top):
			if d == base:
				dirs[:] = [x for x in dirs if x not in ("in", "out")]
			paths += [os.path.join(d, f) for f in files if os.path.splitext(f)[1] and f != "params.h"]
	digests = sorted(hashlib.sha256(open(f, "rb").read()).digest() for f in paths)
	return hashlib.sha256(b"".join(digests)).hexdigest()


def mark_fresh(folder):
	for sub in ("in", "out"):
		d = os.path.join(ROOT, SUBMODULE, folder, sub)
		for f in os.listdir(d) if os.path.isdir(d) else []:
			os.utime(os.path.join(d, f))


def oj_dir(url):
	return os.path.join(OJ_TESTS_DIR, re.sub(r"\W+", "_", url.split("://", 1)[-1]).strip("_"))


class OjTest:
	"""AOJ, yukicoder, or any judge online-judge-tools can download from; checked with `oj test`.
	Test data is cached in testing/.oj-tests/. yukicoder needs YUKICODER_TOKEN (.env or env)."""

	def __init__(self, source_code, url, time_limit: float = 1, error=None):
		self.source_code = source_code
		self.url = url
		self.time_limit = time_limit * COEF
		self.error = error

	def process(self):
		if "yukicoder.me" in self.url and not os.environ.get("YUKICODER_TOKEN"):
			print("SKIPPED (no YUKICODER_TOKEN)")
			return
		test_dir = oj_dir(self.url)
		if not os.path.isdir(test_dir) or not os.listdir(test_dir):
			if "yukicoder.me" in self.url:
				download_yukicoder(self.url, test_dir)
			else:
				run(oj_command() + ["download", "--system", "--silent", self.url, "-d", test_dir])
		compile_command = [find_cxx(), self.source_code + ".cpp", "-O2", "-std=c++17", "-o", self.source_code]
		if platform.system() == "Darwin":
			compile_command.append("-Wl,-stack_size,0x10000000")
		run(compile_command)
		test_command = oj_command() + ["test", "--silent", "-c", "./" + self.source_code,
									   "-d", test_dir, "-t", str(self.time_limit)]
		if self.error is not None:
			test_command += ["-e", str(self.error)]
		run(test_command)


class StressTest:
	"""Self-checking program: loops over random cases, compares with a brute force, exits 0 when all agree."""

	def __init__(self, source_code, time_limit: float = 10):
		self.source_code = source_code
		self.time_limit = time_limit * COEF

	def process(self):
		compile_command = [find_cxx(), self.source_code + ".cpp", "-O2", "-std=c++17", "-o", self.source_code]
		if platform.system() == "Darwin":
			compile_command.append("-Wl,-stack_size,0x10000000")
		run(compile_command)
		try:
			code = subprocess.call(["./" + self.source_code], cwd=TESTING_DIR, timeout=self.time_limit)
		except subprocess.TimeoutExpired:
			print("TLE")
			exit(1)
		if code != 0:
			print("FAILED")
			exit(1)
		print("[SUCCESS] stress test passed")


class ShellTest:
	"""Shell self-test (testing/<sec>/<name>.sh), run with bash from testing/ with CXX set; exit 0 = pass."""

	def __init__(self, source_code, time_limit: float = 300, uses=()):
		self.source_code = source_code
		self.time_limit = time_limit * COEF
		self.uses = uses  # repo files the script runs (for --changed)

	def process(self):
		env = dict(os.environ, CXX=find_cxx())
		try:
			code = subprocess.call(["bash", self.source_code + ".sh"], cwd=TESTING_DIR, env=env, timeout=self.time_limit)
		except subprocess.TimeoutExpired:
			print("TLE")
			exit(1)
		if code != 0:
			print("FAILED")
			exit(1)
		print("[SUCCESS] shell test passed")


all_source_codes = {
	"contest": {
		"scripts": ShellTest("contest/scripts", 300, uses=["tex/contents/contest/stress.sh", "tex/contents/contest/hash.sh",
			"tex/contents/contest/gen.cpp", "tex/contents/contest/checker.cpp", "tex/entryhash.sh"]),
		"readtest": TestCase("contest/readtest", "sample/many_aplusb", 5),
		"readstress": StressTest("contest/readstress", 10),
		"dbg": StressTest("contest/dbg", 10),
	},
	"flow": {
		"stoerwagner": StressTest("flow/stoerwagner", 10),
		"gomoryhu": StressTest("flow/gomoryhu", 10),
		"dinitz": TestCase("flow/dinitz", "graph/bipartitematching", 1.5),
		"hopcroftkarp": TestCase("flow/hopcroftkarp", "graph/bipartitematching", 1),
		"mcmf_aoj": OjTest("flow/mcmf_aoj", "https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_B", 1),
		"mcmfstress": StressTest("flow/mcmfstress", 10),
		"costscaling": TestCase("flow/costscaling", "graph/min_cost_b_flow", 10),
		"costscaling_aoj": OjTest("flow/costscaling_aoj", "https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_B", 1),
		"hungarian": TestCase("flow/hungarian", "graph/assignment", 2),
		"hungarian_stress": StressTest("flow/hungarian_stress", 10),
		"blossom": TestCase("flow/blossom", "graph/general_matching", 5),
		"blossom_stress": StressTest("flow/blossom_stress", 10),
		"edgecases": StressTest("flow/edgecases", 10),
	},
	"graph": {
		"maxclique": TestCase("graph/maxclique", "graph/maximum_independent_set", 5),
		"centroid": TestCase("graph/centroid", "tree/frequency_table_of_tree_distance", 5),
		"centroid_stress": StressTest("graph/centroid_stress", 10),
		"spfa": TestCase("graph/spfa", "graph/shortest_path", 1),
		"spfa_aoj": OjTest("graph/spfa_aoj", "https://onlinejudge.u-aizu.ac.jp/problems/GRL_1_B", 1),
		"spfadetect": StressTest("graph/spfadetect", 10),
		"old_spfa": TestCase("graph/old_spfa", "graph/shortest_path", 1),
		"lowlink": TestCase("graph/lowlink", "graph/biconnected_components", 2),
		"lowlink_2ecc": TestCase("graph/lowlink_2ecc", "graph/two_edge_connected_components", 2),
		"lowlink_stress": StressTest("graph/lowlink_stress", 10),
		"scc": TestCase("graph/scc", "graph/scc", 2),
		"2sat": TestCase("graph/2sat", "other/two_sat", 2),
		"manhattanmst": TestCase("graph/manhattanmst", "geo/manhattanmst", 5),
		"euler_directed": TestCase("graph/euler_directed", "graph/eulerian_trail_directed", 5),
		"euler_undirected": TestCase("graph/euler_undirected", "graph/eulerian_trail_undirected", 5),
		"euler_stress": StressTest("graph/euler_stress", 10),
		"hld": TestCase("graph/hld", "tree/vertex_add_path_sum", 5),
		"hld_lca": TestCase("graph/hld_lca", "tree/lca", 2),
		"hld_subtree": TestCase("graph/hld_subtree", "tree/vertex_add_subtree_sum", 2),
		"hld_stress": StressTest("graph/hld_stress", 10),
		"dominator": TestCase("graph/dominator", "graph/dominatortree", 5),
		"dominator_stress": StressTest("graph/dominator_stress", 10),
		"dmst": TestCase("graph/dmst", "graph/directedmst", 5),
		"dmst_stress": StressTest("graph/dmst_stress", 10),
		"virtualtree": OjTest("graph/virtualtree", "https://yukicoder.me/problems/no/901", 3),
		"virtualtree_stress": StressTest("graph/virtualtree_stress", 10),
	},
	"ds": {
		"lazyseg": TestCase("ds/lazyseg", "data_structure/range_affine_range_sum", 5),
		"lazyseg_stress": StressTest("ds/lazyseg_stress", 10),
		"lct": TestCase("ds/lct", "tree/dynamic_tree_vertex_add_path_sum", 5),
		"lct_stress": StressTest("ds/lct_stress", 10),
		"mo": TestCase("ds/mo", "data_structure/static_range_inversions_query", 5),
		"pbds": TestCase("ds/pbds", "data_structure/ordered_set", 10),
		"pbds_hash": TestCase("ds/pbds_hash", "data_structure/associative_array", 5),
		"ps_seg": TestCase("ds/ps_seg", "data_structure/rectangle_sum", 5),
		"psegstress": StressTest("ds/psegstress", 10),
		"lichao_seg": TestCase("ds/lichao_seg", "data_structure/segment_add_get_min", 5),
		"lichao_line": TestCase("ds/lichao_line", "data_structure/line_add_get_min", 5),
		"rollbackdsu": TestCase("ds/rollbackdsu", "data_structure/persistent_unionfind", 5),
		"rollbackdsu_stress": StressTest("ds/rollbackdsu_stress", 10),
		"treap": TestCase("ds/treap", "data_structure/dynamic_sequence_range_affine_range_sum", 10),
		"treap_stress": StressTest("ds/treap_stress", 10),
	},
	"strings": {
		"hash": TestCase("strings/hash", "string/enumerate_palindromes"),
		"hashstress": StressTest("strings/hashstress", 10),
		"suffixarray": TestCase("strings/suffixarray", "string/suffixarray", 5),
		"numsubstr": TestCase("strings/numsubstr", "string/number_of_substrings", 5),
		"sacyclic": StressTest("strings/sacyclic", 10),
		"ahocorasick": OjTest("strings/ahocorasick", "https://yukicoder.me/problems/no/430", 2),
		"zfunc": TestCase("strings/zfunc", "string/zalgorithm", 5),
		"lyndon": TestCase("strings/lyndon", "string/lyndon_factorization", 5),
		"minrot": StressTest("strings/minrot", 10),
		"sam": TestCase("strings/sam", "string/number_of_substrings", 5),
		"sam_stress": StressTest("strings/sam_stress", 10),
		"eertree": TestCase("strings/eertree", "string/eertree", 5),
		"eertreestress": StressTest("strings/eertreestress", 10),
	},
	"geometry": {
		"cgl1a": OjTest("geometry/cgl1a", "https://onlinejudge.u-aizu.ac.jp/problems/CGL_1_A", 1, error=1e-8),
		"cgl1c": OjTest("geometry/cgl1c", "https://onlinejudge.u-aizu.ac.jp/problems/CGL_1_C", 1),
		"cgl2b": OjTest("geometry/cgl2b", "https://onlinejudge.u-aizu.ac.jp/problems/CGL_2_B", 1),
		"cgl2c": OjTest("geometry/cgl2c", "https://onlinejudge.u-aizu.ac.jp/problems/CGL_2_C", 1, error=1e-8),
		"cgl2d": OjTest("geometry/cgl2d", "https://onlinejudge.u-aizu.ac.jp/problems/CGL_2_D", 1, error=1e-8),
		"segstress": StressTest("geometry/segstress", 10),
		"convexhull": TestCase("geometry/convexhull", "geo/static_convex_hull", 2),
		"furthestpair": TestCase("geometry/furthestpair", "geo/furthest_pair", 2),
		"inhull": StressTest("geometry/inhull", 10),
		"cgl7c": OjTest("geometry/cgl7c", "https://onlinejudge.u-aizu.ac.jp/problems/CGL_7_C", 1, error=1e-6),
		"cgl7d": OjTest("geometry/cgl7d", "https://onlinejudge.u-aizu.ac.jp/problems/CGL_7_D", 1, error=1e-6),
		"cgl7e": OjTest("geometry/cgl7e", "https://onlinejudge.u-aizu.ac.jp/problems/CGL_7_E", 1, error=1e-6),
		"cgl7f": OjTest("geometry/cgl7f", "https://onlinejudge.u-aizu.ac.jp/problems/CGL_7_F", 1, error=1e-6),
		"cgl7g": OjTest("geometry/cgl7g", "https://onlinejudge.u-aizu.ac.jp/problems/CGL_7_G", 1, error=1e-6),
		"mecstress": StressTest("geometry/mecstress", 10),
		"cgl3a": OjTest("geometry/cgl3a", "https://onlinejudge.u-aizu.ac.jp/problems/CGL_3_A", 1, error=1e-6),
		"cgl3c": OjTest("geometry/cgl3c", "https://onlinejudge.u-aizu.ac.jp/problems/CGL_3_C", 1),
		"cgl4c": OjTest("geometry/cgl4c", "https://onlinejudge.u-aizu.ac.jp/problems/CGL_4_C", 1, error=1e-5),
		"polystress": StressTest("geometry/polystress", 10),
		"halfplane": StressTest("geometry/halfplane", 10),
		"hull3dstress": StressTest("geometry/hull3dstress", 10),
		"closestpair": TestCase("geometry/closestpair", "geo/closest_pair", 5),
		"kdtreestress": StressTest("geometry/kdtreestress", 10),
	},
	"math": {
		"modint": TestCase(
			"math/combinatorics",
			"enumerative_combinatorics/binomial_coefficient_prime_mod",
		),
		"bigint": TestCase(
			"math/bigint", "big_integer/multiplication_of_big_integers", 2
		),
		"bigintstress": StressTest("math/bigintstress", 10),
		"bigintadd": TestCase("math/bigintadd", "big_integer/addition_of_big_integers", 2),
		"bigintdivstress": StressTest("math/bigintdivstress", 10),
		"sieve": TestCase("math/sieve", "number_theory/enumerate_primes", 5),
		"sieve_stress": StressTest("math/sieve_stress", 10),
		"millerrabin": TestCase("math/millerrabin", "number_theory/primality_test", 2),
		"pollardrho": TestCase("math/pollardrho", "number_theory/factorize", 2),
		"extgcd": OjTest("math/extgcd", "https://onlinejudge.u-aizu.ac.jp/problems/NTL_1_E", 1),
		"crt": OjTest("math/crt", "https://yukicoder.me/problems/no/186", 2),
		"floorsum": TestCase("math/floorsum", "number_theory/sum_of_floor_of_linear", 5),
		"dlog": TestCase("math/dlog", "number_theory/discrete_logarithm_mod", 2),
		"dsqrt": TestCase("math/dsqrt", "number_theory/sqrt_mod", 3),
		"dlogstress": StressTest("math/dlogstress", 10),
		"simpson": StressTest("math/simpson", 10),
		"numtheorystress": StressTest("math/numtheorystress", 10),
		"gauss": TestCase("math/gauss", "linear_algebra/system_of_linear_equations", 0.5),
		"gaussdet": TestCase("math/gaussdet", "linear_algebra/matrix_det", 2),
		"gaussinv": TestCase("math/gaussinv", "linear_algebra/inverse_matrix", 2),
		"gaussrank": TestCase("math/gaussrank", "linear_algebra/matrix_rank", 2),
		"gaussxor": TestCase("math/gaussxor", "linear_algebra/system_of_linear_equations_mod_2", 2),
		"simplex": StressTest("math/simplex", 10),
		"fft": TestCase("math/fft", "convolution/convolution_mod_1000000007", 3),
		"fftstress": StressTest("math/fftstress", 10),
		"ntt": TestCase("math/ntt", "convolution/convolution_mod"),
		"fwht": TestCase("math/fwht", "convolution/bitwise_xor_convolution"),
		"fwhtand": TestCase("math/fwhtand", "convolution/bitwise_and_convolution"),
		"fwhtstress": StressTest("math/fwhtstress", 10),
		"lagrangestress": StressTest("math/lagrangestress", 10),
		"fpsinv": TestCase("math/fpsinv", "polynomial/inv_of_formal_power_series", 5),
		"fpslog": TestCase("math/fpslog", "polynomial/log_of_formal_power_series", 5),
		"fpsexp": TestCase("math/fpsexp", "polynomial/exp_of_formal_power_series", 10),
		"fpsdiv": TestCase("math/fpsdiv", "polynomial/division_of_polynomials", 5),
		"fpsstress": StressTest("math/fpsstress", 10),
		"bm": TestCase("math/bm", "other/find_linear_recurrence", 5),
		"bmkth": TestCase("math/bmkth", "other/kth_term_of_linearly_recurrent_sequence", 10),
		"bmstress": StressTest("math/bmstress", 10),
		"includeall": StressTest("math/includeall", 10),
		"includeall2": StressTest("math/includeall2", 10),
	},
	"interactive": {
		"hammingstress": StressTest("interactive/hammingstress", 10),
		"ahu": TestCase("interactive/ahu", "tree/rooted_tree_isomorphism_classification", 5),
		"ahustress": StressTest("interactive/ahustress", 10),
		"rankunrankstress": StressTest("interactive/rankunrankstress", 10),
	},
	"misc": {
		"ternary": StressTest("misc/ternary", 10),
		"dpoptobst": OjTest("misc/dpoptobst", "https://onlinejudge.u-aizu.ac.jp/problems/ALDS1_10_D", 1, error=1e-4),
		"dpoptstress": StressTest("misc/dpoptstress", 10),
	},
}

# ---------- all.py --changed BASE [section [test]]: only the tests the diff BASE...HEAD can affect ----------
# test -> its .cpp/.sh + every header it #includes (transitively) + ShellTest.uses; anything unknown -> all tests.
NO_TESTS = ("docs/", ".scrap/", ".agents/", ".claude/", ".github/ISSUE", "tex/main.pdf", "tex/customlexers/",
			"GEMINI.md", ".mcp.json", ".gitignore", "README")
NO_TESTS_EXT = (".tex", ".cls", ".sty", ".md")  # notebook text: the PDF job checks it
# Makefile, .github/: how tests are run; a break shows up in any test, so the smoke set suffices
# smoke set (test.sh, Makefile, .github/, all.py runner code): one of each kind; Library Checker ones cover a tight time limit
# + recursion (dinitz, gauss), heavy code (ntt, fpsexp). Pushes to main still run everything.
SMOKE = {".contest.readtest", ".flow.dinitz", ".math.gauss", ".math.ntt", ".math.fpsexp",
		 ".math.extgcd", ".strings.hashstress", ".contest.scripts"}
SUBMODULE = "library-checker-problems"


def git(*args, cwd=ROOT):
	r = subprocess.run(["git", *args], cwd=cwd, capture_output=True, text=True)
	return r.stdout if r.returncode == 0 else None


def includes(path, seen):
	# repo-relative path -> itself and every quoted #include under it
	if path in seen:
		return seen
	seen.add(path)  # kept even if missing: a test including a deleted header must run (and fail)
	if not os.path.isfile(os.path.join(ROOT, path)):
		return seen
	for line in open(os.path.join(ROOT, path), errors="ignore"):
		m = re.match(r'\s*#\s*include\s*"([^"]+)"', line)
		if m:
			includes(os.path.normpath(os.path.join(os.path.dirname(path), m.group(1))), seen)
	return seen


def all_tests(node=None, prefix=""):
	node = all_source_codes if node is None else node
	if isinstance(node, dict):
		return [t for key in node for t in all_tests(node[key], prefix + DELIMITER + key)]
	return [(prefix, node)]


def dependencies(test):
	if isinstance(test, ShellTest):
		return {"testing/" + test.source_code + ".sh", *test.uses}
	return includes("testing/" + test.source_code + ".cpp", set())


def changed_entries(base):
	# (keys of all_source_codes added or changed since base, whether code outside the table changed)
	import ast
	old = git("show", base + ":testing/all.py")  # base = the merge base
	if old is None:
		return None, True
	def split(src):
		tree = ast.parse(src)
		table = {}
		for stmt in tree.body:
			if isinstance(stmt, ast.Assign) and getattr(stmt.targets[0], "id", "") == "all_source_codes":
				for sec, entries in zip(stmt.value.keys, stmt.value.values):
					for key, value in zip(entries.keys, entries.values):
						table[DELIMITER + sec.value + DELIMITER + key.value] = ast.dump(value)
				stmt.value = ast.Constant(None)
		return table, ast.dump(tree)
	(old_table, old_rest), (new_table, new_rest) = split(old), split(open(__file__).read())
	return {key for key, value in new_table.items() if old_table.get(key) != value}, old_rest != new_rest


def changed_problems(base):
	# Library Checker folders changed by a submodule bump; None = shared files changed (or unknown)
	diff = git("diff", base, "--", SUBMODULE) or ""
	commits = re.findall(r"^[-+]Subproject commit (\w+)", diff, re.M)
	if len(commits) != 2:
		return None
	sub = os.path.join(ROOT, SUBMODULE)
	git("fetch", "-q", "--depth=1", "origin", commits[0], cwd=sub)  # shallow CI checkouts lack the old commit
	files = git("diff", "--name-only", commits[0], commits[1], cwd=sub)
	if files is None:
		return None
	problems = set()
	for f in files.split():
		parts = f.split("/")
		if len(parts) < 3 or parts[0] == "common":
			return None
		problems.add(parts[0] + "/" + parts[1])
	return problems


def select(base):
	# set of test names to run, or None for all
	# committed + uncommitted + untracked changes since the merge base (CI: the PR's changes)
	merge_base = (git("merge-base", base, "HEAD") or "").strip()
	files = git("diff", "--name-only", merge_base) if merge_base else None
	if files is None:
		print("--changed: cannot diff against", base, "-> running everything")
		return None
	files += git("ls-files", "--others", "--exclude-standard", "--", "testing", "tex") or ""
	tests = all_tests()
	deps = {name: dependencies(test) for name, test in tests}
	chosen = set()
	for f in files.split():
		if f.startswith(NO_TESTS) or (f.endswith(NO_TESTS_EXT) and not f.startswith("testing/")):
			continue
		if f == "Makefile" or f.startswith(".github/"):
			print("--changed:", f, "-> smoke set")
			chosen |= SMOKE
		elif f == "testing/all.py":
			keys, runner = changed_entries(merge_base)
			if runner:
				print("--changed: testing/all.py (runner code) -> smoke set")
				chosen |= SMOKE
			chosen |= keys or set()
		elif f == "testing/test.sh":
			chosen |= SMOKE
		elif f == SUBMODULE:
			problems = changed_problems(merge_base)
			chosen |= {name for name, test in tests if isinstance(test, TestCase)
					   and (problems is None or test.testing_folder in problems)}
		else:
			users = {name for name in deps if f in deps[name]}
			gone = not os.path.exists(os.path.join(ROOT, f))
			if not users and not gone and not f.startswith("tex/contents/"):
				print("--changed:", f, "is not used by any test -> everything"); return None
			chosen |= users  # an untested notebook file selects nothing
	print("--changed:", len(chosen), "test(s):", " ".join(sorted(chosen)) or "none")
	return chosen


DELIMITER = "."
timings = []  # (test, seconds, passed), for the report


def report(top=15):
	# slowest tests and the total; on GitHub Actions also a table in the run's summary page
	if not timings:
		return
	total = sum(t for _, t, _ in timings)
	slow = sorted(timings, key=lambda x: -x[1])[:top]
	failed = [name for name, _, ok in timings if not ok]
	print(f"\n{len(timings)} test(s) in {total:.0f}s; slowest:")
	for name, t, ok in slow:
		print(f"  {t:7.1f}s  {name}" + ("" if ok else "  FAILED"))
	path = os.environ.get("GITHUB_STEP_SUMMARY")
	if path:
		with open(path, "a") as f:
			f.write(f"### Tests: {len(timings)} in {total:.0f}s" + (f", failed: {', '.join(failed)}" if failed else "") + "\n\n")
			f.write("| test | time | |\n|---|---:|---|\n")
			f.writelines(f"| `{name}` | {t:.1f}s | {'' if ok else 'FAILED'} |\n" for name, t, ok in slow)


def testingProcess(node, name, selected, prefix=""):
	if isinstance(node, (TestCase, OjTest, StressTest, ShellTest)):
		# "all.py" runs everything, "all.py math" a section, "all.py math ntt" one test
		if (name == "." or prefix == name or prefix.startswith(name + DELIMITER)) and (selected is None or prefix in selected):
			print(">> ", prefix)
			start, ok = time.time(), False
			try:
				node.process()
				ok = True
			finally:  # also when process() exits on a failure
				seconds = time.time() - start
				timings.append((prefix, seconds, ok))
				print("<< ", prefix, f"{seconds:.1f}s", "" if ok else "FAILED")
	elif isinstance(node, dict):
		for key in node.keys():
			testingProcess(node[key], name, selected, prefix + DELIMITER + key)
	else:
		for testcase in node:
			testingProcess(testcase, name, selected, prefix)


if __name__ == "__main__":  # ci.py imports the table without running anything
	args = sys.argv[1:]
	selected = None
	if args[:1] == ["--changed"]:
		selected = select(args[1])
		args = args[2:]
	try:
		testingProcess(all_source_codes, DELIMITER + DELIMITER.join(args), selected)
	finally:
		report()
