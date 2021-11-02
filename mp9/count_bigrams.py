#
# Count bigrams in the input files.  Deliminate on all non-word characters and count as lower-case.
#
# Example: "data/taylor/*.txt"
#   (it s): 11
#   (i m): 10
#   (got a): 9
#   (m insane): 7
#   (we re): 6
#   (a long): 6
#   (long list): 6
#   (list of): 6
#   (of ex): 6
#   (ex lovers): 6
#   (lovers they): 6
#   (they ll): 6
#   (ll tell): 6
#   (tell you): 6
#   (you i): 6
#   (s gonna): 6
#   (love the): 6
#

import sys
from MapReduce import MapReduce

import mappers.map_bigrams as map_bigrams
import reducers.reduce_bigrams as reduce_bigrams


if __name__ == '__main__':
  mr = MapReduce(map_bigrams.map, reduce_bigrams.reduce)
  mr( sys.argv[1:] )
