from flask import Flask, request, jsonify
import reverse_geocode 
app = Flask(__name__)
import requests


register_json = {
  "port" : "5049", 
  "ip": "http://172.22.150.7",

  "name": "Reverse Geocode", 
  "creator": "Ron Basak", 
  "tile": "Geographic location", 

  "dependencies": []
}
content = {"content-type": "application/json"}
link = "http://cs240-adm-01.cs.illinois.edu:5000/microservice"

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

  google = "https://maps.googleapis.com/maps/api/geocode/json?latlng=" + str(lat) +"," + str(long) + "&key=AIzaSyBzCeFbZYSp4ks07Vu3rHb-S5BrvDyNKMg"
  r = requests.get(google)
  print(google)
  google_data = r.json()
  city = ""
  country = ""
  for i in google_data["results"][0]["address_components"]:
    if "locality" in i["types"]:
      city = i["long_name"]
      print(i["long_name"])
    if "country" in i["types"]:
      country = i["long_name"]
      print(i["long_name"])
  #print(google_data["results"])

  print("city", geographic_dictionary["city"],
        "country", geographic_dictionary["country"])
  print("city", city,
        "country", country)
  if city == "" or country == "":
    answer = jsonify({ "city": geographic_dictionary["city"],
                    "country": geographic_dictionary["country"]}) # Using PyCountry API
  else:
    answer = jsonify({ "city": city,
                       "country": country}) # Using Google API if not in water
  
  answer.headers["Cache-Control"] = "max-age=350"

  return answer, 200
