#
# Count the length of words in the input files.  Deliminate on all non-word characters.
#
# Example: "data/taylor/*.txt"
#  4: 140
#  3: 123
#  2: 112
#  1: 80

import sys
from MapReduce import MapReduce

import mappers.map_words_length as map_words_length
import reducers.reduce_words_length as reduce_words_length


if __name__ == '__main__':
  mr = MapReduce(map_words_length.map, reduce_words_length.reduce)
  mr( sys.argv[1:] )
