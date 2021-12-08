from flask import Flask, jsonify, request
#from app import app
#from flask_mobility import Mobility
import pycountry
import requests
app = Flask(__name__)

register_json = {
  "port" : "5109", 
  "ip": "http://172.22.150.7",

  "name": "Country's alpha3 and flag", 
  "creator": "Ron Basak", 
  "tile": "Alpha-3 Code + flag", 

  "dependencies": [
      {
          "port" : "5049", 
          "ip": "http://172.22.150.7",
          "dependencies": []
      }
  ]
}
content = {"content-type": "application/json"}
link = "http://cs240-adm-01.cs.illinois.edu:5000/microservice"

requests.put(link, json = register_json)



# input: country name
# output: alpha + flag
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
            restAPI = "https://restcountries.com/v2/alpha/" + str(alpha_3)
            #print("restAPI:", restAPI)
            r = requests.get(restAPI)
            restAPI_data = r.json()
            #print(restAPI_data)
            flag_link = restAPI_data["flag"]
            #print(flag_link)
            answer = jsonify({"alpha_3": alpha_3, "flag": flag_link})
            answer.headers["Cache-Control"] = "max-age=350"
            return answer, 200

    answer = jsonify({"alpha_3": "N/A", "flag": "N/A"})
    answer.headers["Cache-Control"] = "max-age=350"
    return answer, 200  # for some reason alpha does not exist for this country bc of API or invalid input
  
#answer.headers["Cache-Control"] = "max-age=350"