from flask import Flask, render_template, request, jsonify
import requests
import os
import reverse_geocoder as rg
app = Flask(__name__)

# Route for "/" (frontend):
@app.route('/')
def index():
  return render_template("index.html")

print("Running here")

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

  server_url = os.getenv("IM1_url") 
  r = requests.get(f'{server_url}/{lat}/{long}')
  data = r.json()
  print(data)  
  return jsonify({"input": "GPS",
                  "output": "City and Country",
                  "city": data["city"],
                  "country": data["country"]}), 200