from flask import Flask, jsonify, request
import pycountry
import requests
app = Flask(__name__)

register_json = {
  "port" : "5109", 
  "ip": "http://127.0.0.1",

  "name": "Country's alpha3", 
  "creator": "Ron", 
  "tile": "Alpha-3 Code", 

  "dependencies": [
      {
          "port" : "5049", 
          "ip": "http://127.0.0.1",
          "dependencies": []
      }
  ]
}
content = {"content-type": "application/json"}
link = "http://127.0.0.1:5000/microservice"

requests.put(link, json = register_json)



# input: country name
# output: alpha
#@app.route('/<country>/') # returns the raw json format
@app.route('/')
#def POST_country_to_alpha3(country):
def POST_country_to_alpha3():
    print("Entered second IM")
    data = request.json
    print(data)
    country = data["country"]
    for finding_country in pycountry.countries:
        #print(finding_country.numeric, finding_country.name, finding_country.alpha_3)
        if str(finding_country.name) == str(country):
            alpha_3 = finding_country.alpha_3
            answer = jsonify({"alpha_3": alpha_3})
            answer.headers["Cache-Control"] = "max-age=350"
            return answer, 200


    answer = jsonify({"alpha_3": "N/A"})
    answer.headers["Cache-Control"] = "max-age=350"
    return answer, 200  # for some reason alpha does not exist for this country bc of API or invalid input
  
#answer.headers["Cache-Control"] = "max-age=350"