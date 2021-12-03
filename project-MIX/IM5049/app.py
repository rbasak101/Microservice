from flask import Flask, request, jsonify
import reverse_geocode 
app = Flask(__name__)
import requests


register_json = {
  "port" : "5049", 
  "ip": "http://127.0.0.1",

  "name": "Reverse Geocode", 
  "creator": "Ron", 
  "tile": "Geographic location", 

  "dependencies": []
}
content = {"content-type": "application/json"}
link = "http://127.0.0.1:5000/microservice"

requests.put(link, json = register_json)

@app.route('/')
def POST_geographic_location():
  # print(x, type(x)) # x and y are strings
  # try:
  #   lat = float(x)
  #   long = float(y)
  # except:
  #   return "Error: Missing other coordinate", 400

  print("Entered IM")
  data = request.json
  print(data)
  lat = data["latitude"]
  long = data["longitude"]
  
  coordinates =(lat, long),   # API has a bug: Always expects a comma in the list of coordinates: Comma at end is needed!
  geographic_list = reverse_geocode.search(coordinates) # a list of dictionary for each coordinate
  geographic_dictionary = geographic_list[0]

  print("city", geographic_dictionary["city"],
        "country", geographic_dictionary["country"])

  answer = jsonify({ "city": geographic_dictionary["city"],
                   "country": geographic_dictionary["country"]})
  
  answer.headers["Cache-Control"] = "max-age=350"

  return answer, 200
  
  # return jsonify({"input": "GPS",
  #                 "output": "city,country",
  #                 "owner": owner,
  #                 #"url": "http://127.0.0.1:5049/<x>/<y>/",
  #                 "IMID": "IM5049",
  #                 "city": geographic_dictionary["city"],
  #                 "country": geographic_dictionary["country"]}), 200
