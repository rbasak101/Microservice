import re
from collections import Counter
# map function:
def map(filename):
    words = re.findall('\w+', open(filename).read().lower())
    return Counter(zip(words,words[1:])) #stackoverflow