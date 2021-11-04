import re
from collections import Counter
#from itertools import islice, izip
# map function:
def map(filename):
    words = re.findall('\w+', open(filename).read().lower())
    #print(Counter(zip(words,words[1:])))
    return Counter(zip(words,words[1:])) #stackoverflow