import sys
import inspect
import os
import re
import json
import functools
from typing import Any, List
from typing import Dict

from .evaluators import *


def prYellow(str):
    print("\033[93m {}\033[00m".format(str))


class TestTarget:
    def __init__(self, target: str):
        self.__parse_target(target)
        self.__compile_patterns()

    def __parse_target(self, target: str):
        tgt = target.split(":")

        self.__module = ".*"
        self.__category = ".*"
        self.__test = ".*"

        if not target:
            return

        self.__module = tgt[0]

        if len(tgt) > 1:
            self.__category = tgt[1]

        if len(tgt) > 2:
            self.__test = tgt[2]

    def __compile_patterns(self):
        self.__module_pattern = re.compile(self.__module)
        self.__category_pattern = re.compile(self.__category)
        self.__test_pattern = re.compile(self.__test)

    def __search(self, pattern: re.Pattern, input: str) -> bool:
        if pattern.search(input):
            return True
        return False

    def module_matches(self, module: str) -> bool:
        return self.__search(self.__module_pattern, module)

    def category_matches(self, category: str) -> bool:
        return self.__search(self.__category_pattern, category)

    def test_matches(self, test: str) -> bool:
        return self.__search(self.__test_pattern, test)

    def __repr__(self):
        return f"{self.__module}:{self.__category}:{self.__test}"


@functools.total_ordering
class Test:
    def __init__(
        self, module: str, category: str, name: str, type: str, evaluator: str
    ):
        self.module = module
        self.category = category
        self.name = name
        self.type = type
        self.evaluator = evaluator

    def __key(self):
        return (self.module, self.category, self.name)

    def __hash__(self):
        return hash(self.__key())

    def __eq__(self, other):
        if isinstance(other, Test):
            return self.__key() == other.__key()
        return NotImplemented

    def __lt__(self, other):
        if isinstance(other, Test):
            return self.__key() < other.__key()
        return NotImplemented

    def __repr__(self):
        return f"{self.module}:{self.category}:{self.name}"


class Tester:
    def __init__(
        self,
        json_config: str,
        targets: List[str],
        resources: str,
        module_path: str,
        log_file: str,
        timeout: int | None,
    ):
        self.__targets = []
        self.__resources = resources
        self.__module_path = module_path
        self.__log_file = log_file
        self.__max_score_per_test = 10.0
        self.__timeout = timeout
        self.timeout = 0
        self.__init_targets(targets)
        self.__parse_config(json_config)

    def __init_targets(self, targets: List[str]):
        for target in targets:
            self.__targets.append(TestTarget(target))

    def __select_tests(self, tests: Dict[str, Dict], target: TestTarget):
        modules = []

        for mod in tests.keys():
            if target.module_matches(mod):
                modules.append(mod)

        if not modules:
            return

        for mod in modules:
            categories = tests[mod]
            for cat, tsts in categories.items():
                if target.category_matches(cat):
                    for tst in tsts:
                        if target.test_matches(tst["name"]):
                            self.__tests.add(
                                Test(
                                    mod, cat, tst["name"], tst["type"], tst["evaluator"]
                                )
                            )

    def __expand_params(self, cmd: str, params: Dict[str, Any], test=False) -> str:
        ret = cmd
        for param in re.findall("@@([_a-zA-Z][_a-zA-Z0-9]*)@@", cmd):
            if param != "TEST" or test:
                val = params[param] if param in params else ""
                ret = ret.replace(f"@@{param}@@", str(val))
        return ret

    def __parse_test_types(self, types: List[Dict], params: Dict[str, Any]):
        for type in types:
            name = type["name"]
            cmd = type["command"]
            self.__types[name] = self.__expand_params(cmd, params)

    def __parse_config(self, json_config: str):
        f = open(json_config, "r")
        config = json.load(f)
        f.close()

        self.__tests: set[Test] = set()
        for target in self.__targets:
            self.__select_tests(config["modules"], target)

        if not self.__tests:
            return

        self.__types = {}
        self.__parse_test_types(config["config"]["types"], config["config"]["params"])

        self.__module_header = config["config"]["module_header"]
        self.__module_trailer = config["config"]["module_trailer"]

        if "timeout" in config["config"]:
            if self.__timeout is None:
                self.timeout = int(config["config"]["timeout"])
            else:
                self.timeout = min(self.__timeout, int(config["config"]["timeout"]))

    def __generate_test_module(self):
        f = open(self.__module_path, "w")
        f.truncate(0)

        for cmd in self.__module_header:
            f.write(cmd + "\n")

        cmds = []

        for test in self.__tests:
            name = test.name
            cmd = self.__types[test.type]
            cmd = self.__expand_params(cmd, {"TEST": name}, True)
            cmds.append(cmd + "\n")

        cmds.sort()
        for cmd in cmds:
            f.write(cmd)

        for cmd in self.__module_trailer:
            f.write(cmd + "\n")

        f.close()

    def generate_tests_module(self) -> str:
        if not self.__tests:
            return "There are no matching tests to run"

        self.__generate_test_module()

        return ""

    @classmethod
    def __lazy_reader(clazz, file):
        while True:
            line = file.readline()
            if not line:
                break
            yield line

    def __extract_test_logs(self) -> Dict[str, str]:
        f = open(self.__log_file, "r")
        start = re.compile("\\[TEST\\]Test \\[(.+)\\] START!\n")
        end = re.compile("\\[TEST\\]Test \\[(.+)\\] END!\n")
        curr_test = ""
        curr_log = ""
        ret = {}

        for line in Tester.__lazy_reader(f):
            if not curr_test:
                start_match = start.fullmatch(line)
                if start_match:
                    curr_test = start_match.group(1)
                    # curr_log += line
            else:
                end_match = end.fullmatch(line)
                if not end_match:
                    curr_log += line
                elif end_match.group(1) == curr_test:
                    # curr_log += line
                    ret[curr_test] = curr_log.strip()
                    curr_test = ""
                    curr_log = ""

        f.close()
        return ret

    def __dump_results(self, results: List[tuple[Test, float, str]]) -> str:
        curr_module = ""
        curr_category = ""
        sum = 0.0

        ret = ""
        for test, score, msg in results:
            if curr_module != test.module:
                ret += f"{test.module}:\n"
                curr_module = test.module
            if curr_category != test.category:
                ret += f"  {test.category}:\n"
                curr_category = test.category
            # We could map linearly but we do not know both ranges
            score = (
                self.__max_score_per_test
                if score >= self.__max_score_per_test
                else score
            )
            ret += f"    {test.name} {score}\n"
            if msg:
                ret += f"    NOTE: {msg}\n"
            sum += score

        ret += f"Total: {sum}/{len(results) * self.__max_score_per_test}"

        return ret

    def evaluate_results(self) -> str:
        evals = {}
        for name, clazz in inspect.getmembers(sys.modules[__name__], inspect.isclass):
            if "Evaluator" in name and not "Abstract" in name:
                name = name.replace("Evaluator", "")
                if not issubclass(clazz, AbstractEvaluator):
                    prYellow(
                        f"Evaluator {name} does not implement required interface. Skipping."
                    )
                    continue
                evals[name] = clazz()

        test_logs = self.__extract_test_logs()

        results = []
        for test in self.__tests:
            eval = test.evaluator.split(":")
            if eval[0] not in evals:
                prYellow(f"Evaluator not found for test {test}. Skipping.")
                continue
            if test.name not in test_logs:
                prYellow(f"Test {test} not found in HAL9000 log file. Skipping.")
                continue
            args = [test, os.path.abspath(self.__resources)]
            if len(eval) > 1:
                t_args = eval[1].split(",")
                args.append(list(map(lambda x: x.strip(), t_args)))
            eval_inst = evals[eval[0]]
            score, msg = eval_inst.evaluate(test_logs[test.name], args)
            results.append((test, score, msg))

        results.sort()

        return self.__dump_results(results)
