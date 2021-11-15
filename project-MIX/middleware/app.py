from flask import Flask, render_template, request, jsonify
import requests
import os
import reverse_geocoder as rg
app = Flask(__name__)

class Node: # Each node represents an IM
  def __init__(self, input, output, IMID, url, data):
    self.input = input
    self.output = output
    self.IMID = IMID
    self.url = url
    self.data = data # actual data 
  
  def print_node(self):
    print(self.IMID)

class Node2:
  def __init__(self, url):
    r = requests.get(url)
    self.data = r.json() # actual data 
  
  def print_node(self):
    print(self.IMID)



class Graph:
  adj_dict = {} # representation of adj list
  
  def add_node(self, node):
    print("add_node() called", node.IMID)
    if node.IMID not in self.adj_dict:
      if node.input == "GPS":
        self.adj_dict[node] = []
        print("added root")

        for vertex in self.adj_dict.keys(): # possibly add another root and another lower node could utilize its output
          if vertex.input == node.output:
            self.adj_dict[vertex].append(node)
      else:
        flag = False
        
        for vertex in list(self.adj_dict.keys()):
          if vertex.output == node.input:
            print("for lower IM")
            print(vertex.IMID, node.IMID)
            flag = True
            self.adj_dict[node] = [vertex]

        if flag == False:
          print("Can not add because input and output do not match")
        else:
          print("Added")
    else:
      print("Already exists")
    print("add_node() finished \n")

  def remove_node(self, node): # takes in IMID instead of full node --> fails because key takes in node not IMID
    print("entered function")
    removed = self.adj_dict.pop(node, "Node (IM) does not exist")
    print("removed key is", node.IMID)
    for key, value in list(self.adj_dict.items()):
      if len(value) == 1 and value[0].IMID == node.IMID:
        print("found another node to be deleted")
        self.remove_node(key)

  
  def print_graph(self):
    print("start")
    print(self.adj_dict.items())
    for key, value in self.adj_dict.items():
      print(key.IMID, end=":")
      for IM in value:
        print(IM.IMID)  
      print(" ", end = "\n")  



print("Testing Graph Structure")
IM5049 = Node("GPS", "City&Country", "IM5049", "http://127.0.0.1:5049/<x>/<y>/", "insert JSON response")
IM5040 = Node("City&Country", "Music", "IM5040", "http://127.0.0.1:5040/<city>/", "insert JSON response")
IM1 = Node("Nope", "Music", "IM5040", "http://127.0.0.1:5001/<stuff>/", "insert JSON response")
IM5030 = Node("Music", "Artist", "IM5030", "http://127.0.0.1:5030/<jazz>/", "insert JSON response")

graph = Graph()
graph.add_node(IM5049)
graph.add_node(IM5040)
graph.add_node(IM5030)
graph.add_node(IM1)
graph.print_graph()

print("Removing")
IM5048 = Node("Music", "Artist", "IM5048", "http://127.0.0.1:5048/", "insert JSON response")
graph.remove_node(IM5040)
graph.print_graph()
print("Executed")

# Reading url text and creating node objects
url_file = open("url.txt", "r")
num_lines = sum(1 for line in open('url.txt'))
for i in range(num_lines):
  line = url_file.readline()
  space_index = line.find(" ")
  url = line[0:space_index]
  nodeX = Node2(url)
  print(nodeX.data)
  if not line:
    break
url_file.close()

# Route for "/" (frontend):
@app.route('/')
def index():
  return render_template("index.html")


# Route for "/MIX" (middleware):
@app.route('/MIX', methods=["POST"])
def POST_weather():
  coordinates = request.form["location"]
  print(coordinates, type(coordinates))

  if "(" in coordinates or ")" in coordinates: 
    coordinates = coordinates.replace("(","").replace(")","")

  try:
    lat, long = coordinates.split(",")
    print(lat, long)
  except: 
    return "Error : Missing Comma or more than two numbers entered", 400

  try:
    lat = float(lat)
    long = float(long)
  except:
    return "Error: Missing other coordinate", 400

  server_url = os.getenv("IM5049_url") 
  r = requests.get(f'{server_url}/{lat}/{long}')
  data = r.json()
  print(data)  

  graph.print_graph()
  for key in graph.adj_dict.keys():
    print(key.url)

  a,b,c = server_url.split(":")
  IMID = c[0: len(c)-1]
  return jsonify({"input": data["input"],
                  "output": data["output"],
                  "IMID": IMID,
                  "city": data["city"],
                  "country": data["country"]}), 200







# # Route for "/MIX" (middleware):
# #@app.route('/MIX', methods=["POST"])
# def POST_graph_traversal(): 
#   coordinates = request.form["location"]
#   print(coordinates, type(coordinates))

#   if "(" in coordinates or ")" in coordinates: 
#     coordinates = coordinates.replace("(","").replace(")","")
#   try:
#     lat, long = coordinates.split(",")
#     print(lat, long)
#   except: 
#     return "Error : Missing Comma or more than two numbers entered", 400

#   try:
#     lat = float(lat)
#     long = float(long)
#   except:
#     return "Error: Missing other coordinate", 400

#   server_url = os.getenv("IM5049_url") 
#   r = requests.get(f'{server_url}/{lat}/{long}')
#   data = r.json()
#   print(data)  

#   a,b,c = server_url.split(":")
#   IMID = c[0: len(c)-1]
#   return jsonify({"input": "GPS",
#                   "output": "City&Country",
#                   "IMID": IMID,
#                   "city": data["city"],
#                   "country": data["country"]}), 200