from flask import Flask, jsonify
import reverse_geocode 
app = Flask(__name__)


@app.route('/<x>/<y>/') # returns the raw json format
def POST_geographic_location(x, y):
  print(x, type(x)) # x and y are strings
  try:
    lat = float(x)
    long = float(y)
  except:
    return "Error: Missing other coordinate", 400
  
  coordinates =(lat, long),   # API has a bug: Always expects a comma in the list of coordinates: Comma at end is needed!
  geographic_list = reverse_geocode.search(coordinates) # a list of dictionary for each coordinate
  geographic_dictionary = geographic_list[0]
  owner = "Ron"
  return jsonify({"input": "GPS",
                  "output": "City and Country",
                  "city": geographic_dictionary["city"],
                  "owner": owner,
                  "country": geographic_dictionary["country"]}), 200
