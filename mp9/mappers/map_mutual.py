# map function:
def map(filename):
  dictionary = {}
  neighbors = []
  with open(filename, "r", encoding="utf-8") as f:
    for line in f:
        node = line[0]
        for i in range(3, len(line)):
            if line[i] != ",":
                next_node = line[i]
                #print(next_node, i)
                neighbors.append(next_node)

        for n in neighbors:
          if node < n:
            key = "("+ node + " " + n + ")"
          else:
            key = "("+ n + " " + node + ")"
          dictionary[key] = neighbors
        neighbors = []

  return dictionary
