import os
import sys


class TestCase:
    source_code = ""
    testing_folder = ""
    time_limit = 1

    def __init__(self, source_code, testing_folder, time_limit: float = 1):
        self.source_code = source_code
        self.testing_folder = testing_folder
        self.time_limit = time_limit

    def process(self):
        command = "./test.sh {} {} {}".format(
            self.source_code, self.testing_folder, self.time_limit
        )
        if os.system(command) != 0:
            print("FAILED")
            exit(1)


all_source_codes = {
    "math": {
        "ds": {
            "modint": TestCase(
                "math/ds/combinatorics",
                "enumerative_combinatorics/binomial_coefficient_prime_mod",
            ),
            "bigint": TestCase(
                "math/ds/combinatorics", "big_integer/multiplication_of_big_integers", 2
            ),
        },
        "numbertheory": {
            "millerrabin": TestCase(
                "math/numbertheory/millerrabin", "number_theory/primality_test", 2
            ),
            "pollardrho": TestCase(
                "math/numbertheory/pollardrho", "number_theory/factorize", 2
            ),
            "gauss": TestCase(
                "math/numbertheory/gauss",
                "number_theory/system_of_linear_equations",
                0.5,
            ),
        },
        "numerical": {
            "fft": TestCase(
                "math/numerical/fft", "convolution/convolution_mod_1000000007", 3
            ),
            "ntt": TestCase("math/numerical/ntt", "convolution/convolution_mod"),
        },
    },
    "string": {"hash": TestCase("string/enumerate_palindromes", "string/hash")},
    "graph": {
        "shortestpath": TestCase("graph/shortest_path", "graph/shortestpath/spfa", 1)
    },
}

DELIMITER = "."
name = DELIMITER + DELIMITER.join(sys.argv[1:])


def testingProcess(node, prefix=""):
    if isinstance(node, TestCase):
        if name == "." or name == prefix:
            print(">> ", prefix)
            node.process()
    elif isinstance(node, dict):
        for key in node.keys():
            testingProcess(node[key], prefix + DELIMITER + key)
    else:
        for testcase in node:
            testingProcess(testcase, prefix)


testingProcess(all_source_codes)
# for test in all_source_codes["math"]["ds"]["modint"]:
# 	test.process()
