from .evaluator import AbstractEvaluator

class PassEvaluator(AbstractEvaluator):
    def evaluate(self, result, args):
        if '[ERROR]' in result or '[CRITICAL]' in result:
            return 0, '[ERROR] or [CRITICAL] found in result!'
        if '[PASS]' in result:
            return 10, '' 
        return 0, '[PASS] not fonud in result!'
