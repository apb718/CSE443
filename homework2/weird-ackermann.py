# Import the lru_cache decorator for memoization
from functools import lru_cache

@lru_cache(maxsize=None)  # Cache an unlimited number of results
def ackermann(m, n):
    """
    Ackermann function with memoization.
    
    Args:
    m: The first parameter of the Ackermann function (non-negative integer).
    n: The second parameter of the Ackermann function (non-negative integer).
    
    Returns:
    The result of the Ackermann function for given m and n.
    """
    if m == 0:
        return n + 1
    elif m > 0 and n == 0:
        return ackermann(m - 1, 1)
    elif m > 0 and n > 0:
        return ackermann(m - 1, ackermann(m, n - 1))

# Example usage:
print(ackermann(3, 4))  # This will compute Ackermann(3, 4)