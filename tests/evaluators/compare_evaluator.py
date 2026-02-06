import os

from .evaluator import AbstractEvaluator

class CompareEvaluator(AbstractEvaluator):
    def evaluate(self, result, args):
        test = args[0] 
        res = args[1]
        
        if '[ERROR]' in result or '[CRITICAL]' in result:
            return 0, '[ERROR] or [CRITICAL] found in result'

        check_file = os.path.join(res, test.module, test.category, f'{test.name}.test')
        if not os.path.exists(check_file):
            return 0, 'Reference file not found!'
        
        f = open(check_file, 'r')
        reference = f.read().strip()
        f.close() 

        if result == reference:
            return 10, ''

        return 0, 'Difference between expected output and actual output!'
