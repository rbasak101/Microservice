#
# Calculate a list of mutual friends for pairs of users.
# (Use a list as the value component of the reduce result.)
#
# Example: "data/graph0/*.txt"
#   (B C): ['D']
#   (B D): ['C']
#   (C D): ['B']
#

import sys
from MapReduce import MapReduce

import mappers.map_mutual as map_mutual
import reducers.reduce_mutual as reduce_mutual


if __name__ == '__main__':
  mr = MapReduce(map_mutual.map, reduce_mutual.reduce)
  mr( sys.argv[1:] )