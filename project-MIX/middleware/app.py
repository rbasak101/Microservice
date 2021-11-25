from flask import Flask, render_template, request, jsonify
import requests
import os
import reverse_geocoder as rg
app = Flask(__name__)

class Node:
  def __init__(self, url):
    r = requests.get(url)
    self.all_data = r.json()
    print(self.all_data)
  
  def print_nodeIMID(self):
    print(self.all_data["IMID"])
  

# Route for "/" (frontend):
@app.route('/')
def index():
  return render_template("index.html")



# Route for "/MIX" (middleware):
@app.route('/MIX', methods=["POST"])
def POST_weather():
  frontend = {}
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

  frontend = {**data, **frontend}

  a,b,c = server_url.split(":")
  IMID = c[0: len(c)-1]


  root = Node(f'{server_url}/{lat}/{long}')
  #print("root", root.all_data["country"])

  url_file = open("url.txt", "r")
  num_lines = sum(1 for line in open('url.txt'))
  for i in range(num_lines):
    print("i is", i)
    line = url_file.readline()
    line.rstrip()
    print(line, type(line))

    if i >= 1:
      link, input, output, hierarchy, parameters = line.split("|") 
      print("Printing", root.all_data) 
      print("input is", input)
      if input in root.all_data["output"]:
        url = link + root.all_data[input]
        print(url)
      nodeX = Node(url)
      nodeX.print_nodeIMID()
      frontend = {**nodeX.all_data, **frontend}
    print("\n")
    print("i is currently", i, num_lines)
    if not line or i >= num_lines:
      break
  url_file.close()
  print("j")
  del frontend["owner"], frontend["input"], frontend["output"], frontend["IMID"]
  print("printing final", frontend)

  return jsonify(frontend), 200
  # return jsonify({"input": data["input"],
  #                 "output": data["output"],
  #                 "IMID": IMID,
  #                 "city": data["city"],
  #                 "country": data["country"]}), 200

