from collections import Counter
# reduce function:
def reduce(left, right):

  A = Counter(left)
  B = Counter(right)
  
  return A+B