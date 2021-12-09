from flask import Flask, jsonify, request
import pycountry
from countryinfo import CountryInfo
import requests
app = Flask(__name__)


register_json = {
  "port" : "5169", 
  "ip": "http://172.22.150.7",

  "name": "Country's Population", 
  "creator": "Ron Basak", 
  "tile": "Population", 

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




# input: country
# output: population
@app.route('/') # returns the raw json format
def POST_country_to_population():
    data = request.json
    print(data)
    country = data["country"]
    nation = CountryInfo(country) #CountryInfo API has some flaws: Ex: Vatican City
    try:
        if country == "Vatican City":
            pop = 825
            answer = jsonify({"country population": pop})
            answer.headers["Cache-Control"] = "max-age=350"
            return answer, 200
        
        data = nation.info()
        pop = data["population"]
        pop = "{:,}".format(pop)
        answer = jsonify({"country population": pop})
        answer.headers["Cache-Control"] = "max-age=350"
        return answer, 200

    except:
        answer = jsonify({"country population": "N/A"})
        answer.headers["Cache-Control"] = "max-age=350"
        return answer, 400


   
  
