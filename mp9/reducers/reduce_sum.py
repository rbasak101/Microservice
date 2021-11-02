from collections import Counter
# reduce function:
def reduce(left, right):
  # combined = left | right
  # return combined
  A = Counter(left)
  B = Counter(right)
  return dict(A+B)
  # combined = left.update(right)
  # return combined
  #return {}
