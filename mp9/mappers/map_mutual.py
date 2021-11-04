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
    #print(dictionary)
  return dictionary
    # array = []
    # f = open(filename, "r")
    # for line in f.readlines():
    #   line = line.strip()
    #   src, dst = line.split("->")
    #   dst_list = dst.split(",")
    #   for d in dst_list:
    #     if src < d:
    #       array.append("("+ src + " " + d + ")")
    #     else:
    #       array.append("("+ d + " "+ src + ")")
    # f.close()
    # print(array)
    # return array
