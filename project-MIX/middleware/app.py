from flask import Flask, render_template, request, jsonify
import requests
import datetime
import time #for testing speed to check cache works
import os
import reverse_geocoder as rg
app = Flask(__name__)

# Route for "/" (frontend):
@app.route('/')
def index():
  return render_template("index.html")

def order_hierarchy(file): # orders url in ascending hierarchy order
  class Pair:
    def __init__(self, line, hierarchy):
      self.hierarchy = hierarchy
      self.line = line
  
  array = []
  with open(file, "r") as f:
    for line in f:
      link, input, output, hierarchy, parameters = line.split("|")
      pair = Pair(line, hierarchy)
      array.append(pair)
  f.close()

  array.sort(key=lambda x: x.hierarchy)
  for element in array:
    print(element.hierarchy, element.line)
  f = open(file, "w")
  f.truncate(0)
  for element in array:
    f.write(element.line)
  f.close()
  array.clear()

class Node: # IM representation 
  def __init__(self, url):
    if type(url) != dict:
      r = requests.get(url)
      self.all_data = r.json()
      print(self.all_data)
    else:
      self.all_data = url # url in this case refers to the cached output that is a dictionary
  
  def print_nodeIMID(self):
    print(self.all_data["IMID"])
  
  def getIMID(self):
    return self.all_data["IMID"]
  
  def getInput(self):
    return self.all_data["input"]
  
  def getOutput(self):
    return self.all_data["output"]
  

class Graph: # mainly used for adding and removing
  adj_dict = {} # representation of adj list

  def checkSubset(self, small):
    if small in self.adj_dict:
      return True
    return False
  
  def add_node(self, node):
    #print("inside add_node")
    node.print_nodeIMID()
    if self.checkSubset(node) == False:
      if node.all_data["input"] == "GPS":
        self.adj_dict[node] = []
        #print("added root")
        for vertex in self.adj_dict.keys(): # possibly add another root and another lower node could utilize its output
          if vertex.all_data["input"] == node.all_data["output"]:
            self.adj_dict[vertex].append(node)
      else:
        flag = False
        for vertex in list(self.adj_dict.keys()):
          if vertex.all_data["output"] == node.all_data["input"] or node.all_data["input"] in vertex.all_data["output"]:
            #print("for lower IM")
            #print(vertex.getIMID(), node.getIMID())
            flag = True
            self.adj_dict[node] = [vertex]

        if flag == False:
          print("Can not add because input and output do not match")
        else:
          print("Added")
    else:
      print("Already exists")
    print("add_node() finished \n")

  def remove_node(self, node): #Removes that node and all others that are dependent on it. Experience this feature if another button was added in frontend for removing
    print("entered remove function")
    removed = self.adj_dict.pop(node, "Node (IM) does not exist")
    print("removed key is")
    node.print_nodeIMID()
    for key, value in list(self.adj_dict.items()):
      if len(value) == 1 and value[0].getIMID() == node.getIMID():
        print("found another node to be deleted")
        self.remove_node(key)
  
  def print_graph(self):
    print("start of graph printing")
    #print(self.adj_dict.items())
    for key, value in self.adj_dict.items():
      print(key.getIMID(), end=":")
      for IM in value:
        print(IM.getIMID())  
      print(" ", end = "\n")  
    
print("Reordering links based on hierarchy") #only once!
order_hierarchy("url.txt")

cache = {} #key = node, value = [time, data]
graph = Graph()
# Route for "/MIX" (middleware):
@app.route('/MIX', methods=["POST"])
def POST_weather():
  print("Start of MIX")
  t0 = time.time()
  frontend = {}
  graph.adj_dict.clear()
  graph.print_graph()
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

  url_file = open("url.txt", "r")
  num_lines = sum(1 for line in open('url.txt'))
  for i in range(num_lines):
    print("i is", i)
    line = url_file.readline()
    line.rstrip()
    print(line)
    link, input, output, hierarchy, parameters = line.split("|") 
    if input == "GPS":
      url = link + str(lat)+ "/" + str(long)
      #print("Inside input == GPS", url)
      if url not in cache:
        print("Line 133", url)
        root = Node(url)
        graph.add_node(root)
        frontend = {**root.all_data, **frontend}
        cache[url] = [datetime.datetime.now(), root.all_data]
        continue
      elif (url in cache) and (cache[url][0] + datetime.timedelta(hours=2) < datetime.datetime.now()):
        print("deleting url from cache because its old", url)
        del cache[url]
        continue
      else:
        print("Using cached value!")
        frontend = {**cache[url][1], **frontend}
        nodeX = Node(cache[url][1])
        print(frontend)
        print("Added cached node in the tree")
        graph.add_node(nodeX)
        continue

    for key in graph.adj_dict: # Find node with right output and input match
      if input in key.getOutput():
        url = link + key.all_data[input]
        break

    print("Printing url", url)
    if url not in cache:
      nodeX = Node(url)
      nodeX.print_nodeIMID()
      graph.add_node(nodeX)
      frontend = {**nodeX.all_data, **frontend}
      cache[url] = [datetime.datetime.now(), nodeX.all_data]
    elif (url in cache) and (cache[url][0] + datetime.timedelta(hours=2) < datetime.datetime.now()):
      print("deleting url from cache because its old", url)
      del cache[url]
    else:
      print("Using cached value (second check)!")
      frontend = {**cache[url][1], **frontend}
      print(frontend)

    if not line or i >= num_lines:
      break

  url_file.close()

  del frontend["owner"], frontend["input"], frontend["output"], frontend["IMID"]
  print("printing final", frontend)

  t1 = time.time()
  print("time differential, check to see if caching works", t1-t0)
  return jsonify(frontend), 200

