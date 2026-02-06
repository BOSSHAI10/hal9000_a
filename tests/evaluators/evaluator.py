from abc import ABC, abstractmethod
from typing import List

class AbstractEvaluator(ABC):
    @abstractmethod
    def evaluate(self, result: str, args: List) -> tuple[float, str]:
        pass 
