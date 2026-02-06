import re

from .evaluator import AbstractEvaluator

class ThreadDecreasingPriorityEvaluator(AbstractEvaluator):
    def evaluate(self, result, args):
        if '[ERROR]' in result or '[CRITICAL]' in result:
            return 0, '[ERROR] or [CRITICAL] found in result!'

        pattern = re.compile(r'Thread \[.*\] with priority (\d+)')

        captured_data = []
        for line in result.split('\n'):
            m = pattern.search(line)
            if m:
                captured_data.append(int(m.group(1)))
        
        sorted_data = captured_data.copy()
        sorted_data.sort(reverse=True)

        if captured_data == sorted_data:
            return 10, '' 

        return 0, 'Priorities should appear in decreasing order!'

class ThreadRoundRobinEvaluator(AbstractEvaluator):
    def evaluate(self, result, args):
        if '[ERROR]' in result or '[CRITICAL]' in result:
            return 0, '[ERROR] or [CRITICAL] found in result!'

        prio_pattern = re.compile(r'Thread (0x[0-9a-fA-F]+) with priority (\d+)')
        count_pattern = re.compile(r'Will create (\d+) threads for running test')

        captured_data = []
        no_of_threads = 0

        for line in result.split('\n'):
            m = prio_pattern.search(line)
            if m:
                captured_data.append((int(m.group(1), base=16), int(m.group(2))))
            else:
                m = count_pattern.search(line)
                if m:
                    no_of_threads = int(m.group(1))
        
        th_idx = 0
        sequence = []
        thread_times = {}
        msg = 'Errors:\n'
        failure = False

        for tid, prio in captured_data:
            if not tid in thread_times:
                thread_times[tid] = 0
            thread_times[tid] = thread_times[tid] + 1
            if th_idx < no_of_threads:
                sequence.append(tid)
            else:
                if sequence[th_idx % no_of_threads] != tid:
                    msg += f'          Next thread should have been with TID {hex(sequence[th_idx % no_of_threads])}, instead thread with TID {hex(tid)} executed!\n' 
                    failure = True
                    break
            th_idx = th_idx + 1

        if not failure:
            if len(thread_times.keys()) != no_of_threads:
                msg += f'          Only {no_of_threads} threads should have been scheduled, but {len(thread_times.keys())} have actually run!\n' 
                failure = True

        if failure:
            return 0, msg
        return 10, ''
